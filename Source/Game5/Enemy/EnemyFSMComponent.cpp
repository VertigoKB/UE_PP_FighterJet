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

	FTimerHandle Temp;
	World->GetTimerManager().SetTimer(Temp, FTimerDelegate::CreateLambda([this]() {
		IsTooClose();
		}), 10.f, true);

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
	OnceCNode.Reset();						//DoOnce 리셋

	TargetDone = nullptr;

	InitFalseDelegateBoolean();				//델리게이트 신호 초기화
	CompOwner->InitFalseManeuverBoolean();	//조종간에서 손 떼

	if (CompOwner->Health > 0.f)			//죽지 않았다면 다음 행동 개시
		State = EEnemyState::Maneuver;
}

void UEnemyFSMComponent::OnStabilizeOnce()
{
	PositionUpdater->bTryStablize = true;
}
void UEnemyFSMComponent::OnStabilizeTick()
{
	if (bStabilizeDone)
		State = EEnemyState::Idle;
}

void UEnemyFSMComponent::OnManeuverOnce()
{
	//IsTooClose();
}
void UEnemyFSMComponent::OnManeuverTick()
{
	ReceiveDelegateCall(TargetDone);
}

void UEnemyFSMComponent::OnAttackOnce()
{
}
void UEnemyFSMComponent::OnAttackTick()
{
}


void UEnemyFSMComponent::OnDeath()
{
}


void UEnemyFSMComponent::ReceiveDelegateCall(bool* ReceiveTarget)
{
	if (!ReceiveTarget)
		return;

	FPlayerRelativePosition RelativeState = CompOwner->Decision;

	if (*ReceiveTarget)
		OnceANode.Execute([&]() { 

		CompOwner->bPitchUp = true;
		World->GetTimerManager().SetTimer(ManeuverTimer, FTimerDelegate::CreateLambda([this]() {
			State = EEnemyState::Stabilize;
			}), MaxManeuverTime, false);

			});


	if (RelativeState.bIsInFront == true)
	{
		World->GetTimerManager().ClearTimer(ManeuverTimer);
		State = EEnemyState::Stabilize;
	}

	
}

void UEnemyFSMComponent::IsTooClose()
{
	FPlayerRelativePosition RelativeState = CompOwner->Decision;

	if (RelativeState.bIsTooClose == true)		
	{	//Player Too Close
		
		PositionUpdater->TryRolling(FName(TEXT("Left")));
		TargetDone = &bRollingDone;	// Point TargetDone will prevent return in ReceiveDelegateCall(). The Function is tick action.
	}
	else
	{	//Player Not Close

	}
}
