// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPlaneController.h"
#include "EnemySu33Pawn.h"
#include "EnemyPositionUpdater.h"

#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyPlaneController::AEnemyPlaneController()
{
	PrimaryActorTick.bCanEverTick = true;

	Intelligence = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Inteligence"));
	SetPerceptionComponent(*Intelligence);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 15000.f;
	SightConfig->LoseSightRadius = 15000.f;
	SightConfig->PeripheralVisionAngleDegrees = 15.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	Intelligence->ConfigureSense(*SightConfig);
	Intelligence->SetDominantSense(SightConfig->GetSenseImplementation());

	

	State = EEnemyState::Stabilize;
}

void AEnemyPlaneController::BeginPlay()
{
	Super::BeginPlay();

	if (!Initialize())
	{
		SetActorTickEnabled(false);
		return;
	}
	//FTimerHandle Temp;
	//GetWorld()->GetTimerManager().SetTimer(Temp, FTimerDelegate::CreateLambda([this]() {
	//	IsTooClose();
	//	}), 5.f, true);

	Intelligence->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyPlaneController::OnTargetPerceptionUpdated);

	GetWorld()->GetTimerManager().SetTimer(PosStateUpdater, FTimerDelegate::CreateLambda([this]() {
		PosState = MyPawn->Decision;
		}), 0.1f, true);

}

void AEnemyPlaneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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


	if (MyPawn->Health <= 0.f)
		State = EEnemyState::Death;
}

void AEnemyPlaneController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	bDebugBoolean = Stimulus.WasSuccessfullySensed();

	if (Actor->Tags.Num() > 0)
	{
		MyDebugName = Actor->Tags[0];
	}
}

bool AEnemyPlaneController::Initialize()
{

	MyPawn = Cast<AEnemySu33Pawn>(GetPawn());
	if (!MyPawn)
		return false;

	Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Target)
		return false;

	PositionUpdater = MyPawn->GetComponentByClass<UEnemyPositionUpdater>();
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

void AEnemyPlaneController::OnIdleTick()
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
	MyPawn->InitFalseManeuverBoolean();	//Reset ai joystick

	if (MyPawn->Health > 0.f)			//Execute next state if not die
		State = EEnemyState::Maneuver;
}

void AEnemyPlaneController::OnStabilizeOnce()
{
	PositionUpdater->bTryStablize = true;
}
void AEnemyPlaneController::OnStabilizeTick()
{
	if (MyPawn->Health > 0.f)
	{
		if (bStabilizeDone)
			State = EEnemyState::Idle;
	}
}

void AEnemyPlaneController::OnManeuverOnce()
{
	//IsTooClose();
}
void AEnemyPlaneController::OnManeuverTick()
{
	if (MyPawn->Health > 0.f)
	{
		ReceiveDelegateCall(DelegateDone);
		CompareFloat(FloatConditionA, FloatConditionB, CompOperator);
	}
}

void AEnemyPlaneController::OnAttackOnce()
{
}
void AEnemyPlaneController::OnAttackTick()
{
	if (MyPawn->Health > 0.f)
	{

	}
}

void AEnemyPlaneController::OnDeath()
{
}

void AEnemyPlaneController::ReceiveDelegateCall(bool* ReceiveTarget)
{
	if (!ReceiveTarget)
		return;

	if (ReceiveTarget == &bRollingDone)
	{
		if (*ReceiveTarget)
			OnceANode.Execute([&]() {
			MyPawn->bPitchUp = true;
			StopManeuveringWhenLimit();
				});

		if (PosState.bIsInFront == true)
		{
			GetWorld()->GetTimerManager().ClearTimer(ManeuverTimer);
			State = EEnemyState::Stabilize;
		}
	}

	if (ReceiveTarget == &bPullUpDone)
	{
		float CurrentAltitude = MyPawn->GetActorLocation().Z;
		float TargetAltitude = Target->GetActorLocation().Z;

		OnceANode.Execute([&]() {
			StopManeuveringWhenLimit();
			});

		FloatConditionA = CurrentAltitude;
		FloatConditionB = TargetAltitude;
		CompOperator = FName(TEXT(">="));
	}
}

void AEnemyPlaneController::CompareFloat(float condA, float condB, FName CompOp)
{
	if (CompOp == NAME_None)
		return;

	if (CompOp == FName(TEXT(">=")) &&
		(condA >= condB))
	{
		GetWorld()->GetTimerManager().ClearTimer(ManeuverTimer);
		State = EEnemyState::Stabilize;
	}
}

void AEnemyPlaneController::StopManeuveringWhenLimit()
{
	GetWorld()->GetTimerManager().SetTimer(ManeuverTimer, FTimerDelegate::CreateLambda([this]() {
		State = EEnemyState::Stabilize;
		}), MaxManeuverTime, false);
}

void AEnemyPlaneController::IsTooClose()
{
	if (PosState.bIsTooClose == true)
	{	//Player Too Close

		PositionUpdater->TryRolling(FName(TEXT("Left")));
		DelegateDone = &bRollingDone;	// Point TargetDone will prevent return in ReceiveDelegateCall(). The Function is tick action.
	}
	else //Player Not Close
		IsAbove();
}

void AEnemyPlaneController::IsAbove()
{
	if (PosState.bIsAbove == true)
	{	//Player position in above
		if (true)
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
