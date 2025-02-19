// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSMComponent.h"
#include "EnemySu33Pawn.h"
#include "EnemyPositionUpdater.h"

// Sets default values for this component's properties
UEnemyFSMComponent::UEnemyFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	State = EEnemyState::Stabilize;
	// ...
}


// Called when the game starts
void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Initialize())
	{
		SetComponentTickEnabled(false);
		return;
	}
	//FTimerHandle Temp;
	//World->GetTimerManager().SetTimer(Temp, FTimerDelegate::CreateLambda([this]() {
	//	IsTooClose();
	//	}), 5.f, true);

	World->GetTimerManager().SetTimer(PosStateUpdater, FTimerDelegate::CreateLambda([this]() {
		PosState = CompOwner->Decision;
		}), 0.1f, true);

}


// Called every frame
void UEnemyFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case EEnemyState::Idle:
	{
		OnIdleTick();
		break;
	}
	case EEnemyState::Maneuver:
	{
		OnceManeuverNode.Execute([&]() { OnManeuverOnce(); });
		OnManeuverTick();
		break;
	}
	case EEnemyState::Attack:
	{
		OnceAttackNode.Execute([&]() { OnAttackOnce(); });
		OnAttackTick();
		break;
	}
	case EEnemyState::Stabilize:
	{
		OnceStabilizeNode.Execute([&]() { OnStabilizeOnce(); });
		OnStabilizeTick();
		break;
	}
	}

	//if (PosState.bIsInRange == true &&
	//	PosState.bIsInSight == true &&
	//	PosState.bIsTooClose != true) {
	//	State = EEnemyState::Attack;
	//}


	if (CompOwner->Health <= 0.f)
		State = EEnemyState::Death;
}

bool UEnemyFSMComponent::Initialize()
{
	World = GetWorld();
	if (!World)
		return false;

	CompOwner = Cast<AEnemySu33Pawn>(GetOwner());
	if (!CompOwner)
		return false;

	Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Target)
		return false;

	PositionUpdater = CompOwner->GetComponentByClass<UEnemyPositionUpdater>();
	if (!PositionUpdater)
		return false;

	PositionUpdater->RollingDone.BindLambda([this]() {
		bRollingDone = true;
		});
	PositionUpdater->PullUpDone.BindLambda([this]() {
		bPullUpDone = true;
		});
	PositionUpdater->ImmelmannTurnDone.BindLambda([this]() {
		bImmelmannTurnDone = true;
		});
	PositionUpdater->StabilizeDone.BindLambda([this]() {
		bStabilizeDone = true;
		});


	return true;
}

void UEnemyFSMComponent::OnIdleTick()
{
	OnceManeuverNode.Reset();						
	OnceAttackNode.Reset();							
	OnceStabilizeNode.Reset();				

	OnceANode.Reset();
	OnceBNode.Reset();
	OnceCNode.Reset();						//Reset all DoOnce

	FloatConditionA = 0.f;
	FloatConditionB = 0.f;
	CompOperator = NAME_None;
	DelegateDone = nullptr;

	InitFalseDelegateBoolean();				//Reset all delegate receiver
	CompOwner->InitFalseManeuverBoolean();	//Reset ai joystick

	if (CompOwner->Health > 0.f)			//Execute next state if not die
		State = EEnemyState::Maneuver;
}

void UEnemyFSMComponent::OnStabilizeOnce()
{
	PositionUpdater->bTryStablize = true;
}
void UEnemyFSMComponent::OnStabilizeTick()
{
	if (CompOwner->Health > 0.f)
	{
		if (bStabilizeDone)
			State = EEnemyState::Idle;
	}
}

void UEnemyFSMComponent::OnManeuverOnce()
{
	//IsTooClose();
}
void UEnemyFSMComponent::OnManeuverTick()
{
	if (CompOwner->Health > 0.f)
	{
		ReceiveDelegateCall(DelegateDone);
		CompareFloat(FloatConditionA, FloatConditionB, CompOperator);
	}
}

void UEnemyFSMComponent::OnAttackOnce()
{
}
void UEnemyFSMComponent::OnAttackTick()
{
	if (CompOwner->Health > 0.f)
	{

	}
}


void UEnemyFSMComponent::OnDeath()
{
}


void UEnemyFSMComponent::ReceiveDelegateCall(bool* ReceiveTarget)
{
	if (!ReceiveTarget)
		return;

	if (ReceiveTarget == &bRollingDone)
	{
		if (*ReceiveTarget)
			OnceANode.Execute([&]() {
			CompOwner->bPitchUp = true;
			StopManeuveringWhenLimit();
				});

		if (PosState.bIsInFront == true)
		{
			World->GetTimerManager().ClearTimer(ManeuverTimer);
			State = EEnemyState::Stabilize;
		}
	}
	
	if (ReceiveTarget == &bPullUpDone)
	{
		float CurrentAltitude = CompOwner->GetActorLocation().Z;
		float TargetAltitude = Target->GetActorLocation().Z;

		OnceANode.Execute([&]() {
			StopManeuveringWhenLimit();
			});

		FloatConditionA = CurrentAltitude;
		FloatConditionB = TargetAltitude;
		CompOperator = FName(TEXT(">="));
	}
}

void UEnemyFSMComponent::CompareFloat(float condA, float condB, FName CompOp)
{
	if (CompOp == NAME_None)
		return;

	if ( CompOp == FName(TEXT(">=")) &&
		(condA >= condB))
	{
		World->GetTimerManager().ClearTimer(ManeuverTimer);
		State = EEnemyState::Stabilize;
	}
}

void UEnemyFSMComponent::StopManeuveringWhenLimit()
{
	World->GetTimerManager().SetTimer(ManeuverTimer, FTimerDelegate::CreateLambda([this]() {
		State = EEnemyState::Stabilize;
		}), MaxManeuverTime, false);
}

void UEnemyFSMComponent::IsTooClose()
{
	if (PosState.bIsTooClose == true)		
	{	//Player Too Close
		
		PositionUpdater->TryRolling(FName(TEXT("Left")));
		DelegateDone = &bRollingDone;	// Point TargetDone will prevent return in ReceiveDelegateCall(). The Function is tick action.
	}
	else //Player Not Close
		IsAbove();
}

void UEnemyFSMComponent::IsAbove()
{
	if (PosState.bIsAbove == true)
	{	//Player position in above
		if (PosState.bIsAboveInsight == true)
		{	//But player in sight

		}
		else
		{	//But player not in sight
			PositionUpdater->TryPullUp();
			DelegateDone = &bPullUpDone;
		}
	}
	else
	{	//Player position in downside

	}
}
