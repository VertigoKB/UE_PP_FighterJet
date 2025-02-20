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
	SightConfig->SightRadius = LockOnRange;
	SightConfig->LoseSightRadius = LockOnRange;
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
		DistanceToTarget = MyPawn->GetDistanceTo(Target);
		}), 0.1f, true);

}

void AEnemyPlaneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyPawn->Health <= 0.f)
		State = EEnemyState::Death;

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
	}case EEnemyState::Stabilize:
	{
		OnceStabilizeNode.Execute([&]() { OnStabilizeOnce(); });
		OnStabilizeTick();
		break;
	}	
	case EEnemyState::Search:
	{
		OnceSearchNode.Execute([&]() { OnSearchOnce(); });
		OnSearchTick();
		break;
	}
	case EEnemyState::Attack:
	{
		OnceAttackNode.Execute([&]() { OnAttackOnce(); });
		OnAttackTick();
		break;
	}
	}

	//if (PosState.bIsInRange == true &&
	//	PosState.bIsInSight == true &&
	//	PosState.bIsTooClose != true) {
	//	State = EEnemyState::Attack;
	//}
	

}

void AEnemyPlaneController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
 	if (Stimulus.WasSuccessfullySensed() &&
		Actor->Tags.Num() > 0)
	{
		if (Actor->Tags[0] == FName(TEXT("Player")))
			bLockable = true;
		else
			bLockable = false;
	}
	else
		bLockable = false;
}

bool AEnemyPlaneController::Initialize()
{

	MyPawn = Cast<AEnemySu33Pawn>(GetPawn());
	if (!MyPawn)
		return false;

	if (!Target)
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
	OnceSearchNode.Reset();

	OnceANode.Reset();
	OnceBNode.Reset();
	OnceCNode.Reset();						//Reset all DoOnce

	FloatConditionA = 0.f;
	FloatConditionB = 0.f;
	CompOperator = NAME_None;
	DelegateDone = nullptr;

	//InitFalseDelegateBoolean();				//Reset all delegate receiver
	bRollingDone = false;
	bPullUpDone = false;
	bImmelmannTurnDone = false;
	bStabilizeDone = false;
	MyPawn->InitFalseManeuverBoolean();	//Reset ai joystick

	if (bLockable &&
		!PosState.bIsTooClose)
		State = EEnemyState::Attack;
	else
		State = EEnemyState::Maneuver;
}

void AEnemyPlaneController::OnStabilizeOnce()
{
	PositionUpdater->bTryStablize = true;
}
void AEnemyPlaneController::OnStabilizeTick()
{
	if (bStabilizeDone)
		State = EEnemyState::Idle;
}

void AEnemyPlaneController::OnManeuverOnce()
{
	if (!bManeuverStateForImmelmann)
	{
		IsTooFar();
		return;
	}
	else
	{ 
		PositionUpdater->TryImmelmannTurn();
		DelegateDone = &bImmelmannTurnDone;
		UE_LOG(LogTemp, Warning, TEXT("Search Immelmann"))
	}

}
void AEnemyPlaneController::OnManeuverTick()
{
	ReceiveDelegateCall(DelegateDone);
	CompareFloat(FloatConditionA, FloatConditionB, CompOperator);
}

void AEnemyPlaneController::OnAttackOnce()
{
	FTimerHandle Temp;
	GetWorldTimerManager().SetTimer(Temp, FTimerDelegate::CreateLambda([&]() {
		State = EEnemyState::Stabilize;
		}), 5.f, false);
}
void AEnemyPlaneController::OnAttackTick()
{

}

void AEnemyPlaneController::OnSearchOnce()
{
}
void AEnemyPlaneController::OnSearchTick()
{
	if (PosState.bIsInFront)
	{
		if (PosState.bIsAbove)
		{
			MyPawn->bPitchUp = true;
			MyPawn->bPitchDown = false;
		}
		else
		{
			MyPawn->bPitchUp = false;
			MyPawn->bPitchDown = true;
		}

		if (PosState.bIsRight)
		{
			MyPawn->bYawRight = true;
			MyPawn->bYawLeft = false;
		}
		else
		{
			MyPawn->bYawRight = false;
			MyPawn->bYawLeft = true;
		}
	}
	else
	{
		bManeuverStateForImmelmann = true;
		State = EEnemyState::Stabilize;
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

		if (bLockable)
		{
			GetWorld()->GetTimerManager().ClearTimer(ManeuverTimer);
			State = EEnemyState::Attack;
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
	
	if (ReceiveTarget == &bImmelmannTurnDone)
	{
		if (*ReceiveTarget)
		{
			bManeuverStateForImmelmann = false;
			State = EEnemyState::Stabilize;
		}
	}

	if (bLockable)
	{
		GetWorld()->GetTimerManager().ClearTimer(ManeuverTimer);
		State = EEnemyState::Attack;
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

void AEnemyPlaneController::IsTooFar()
{
	if (DistanceToTarget > LockOnRange)
	{	//Player too far
		State = EEnemyState::Search;
		UE_LOG(LogTemp, Warning, TEXT("Too Far!"))
	}
	else
	{	//Player in range
		IsTooClose();
	}
}

void AEnemyPlaneController::IsTooClose()
{
	if (PosState.bIsTooClose == true)
	{	//Player Too Close
		RequestRolling(FName(TEXT("Left")));
		UE_LOG(LogTemp, Warning, TEXT("Too Close"))
	}
	else //Player Not Close
		IsAbove();
}

void AEnemyPlaneController::IsAbove()
{
	if (PosState.bIsAbove == true)
	{	//Player position in above 
		if (bLockable)
		{	//But player in sight
			State = EEnemyState::Attack;
			UE_LOG(LogTemp, Warning, TEXT("Not Close / Above / Lockable!"))
		}
		else
		{	//But player not in sight
			PositionUpdater->TryPullUp();
			DelegateDone = &bPullUpDone;
			UE_LOG(LogTemp, Warning, TEXT("Not Close / Above / I Cant Lock!"))
		}
	}
	else //Player position in downside
		IsFront();
}

void AEnemyPlaneController::IsFront()
{
	if (PosState.bIsInFront == true)
	{	//Player position in front
		if (bLockable)
		{	//But player in sight.
			State = EEnemyState::Attack;
			UE_LOG(LogTemp, Warning, TEXT("Not Close / Not Above / Front / Lockable!"))
		}
		else
		{	//But player not in sight.
			if (PosState.bIsRight == true)
			{	//And player position in Right.
				RequestRolling(FName(TEXT("Right")));
				UE_LOG(LogTemp, Warning, TEXT("Not Close / Not Above / Front / Cant Lock / Right "))
			}
			else
			{	//And player position in Left.
				RequestRolling(FName(TEXT("Left")));
				UE_LOG(LogTemp, Warning, TEXT("Not Close / Not Above / Front / Cant Lock / Left "))
			}
		}
	}
	else
	{	//Player position in rear
		if (DistanceToTarget > LockOnRange)
		{	//And Player too far.
			PositionUpdater->TryImmelmannTurn();
			DelegateDone = &bImmelmannTurnDone;
			UE_LOG(LogTemp, Warning, TEXT("Not Close / Not Above / Back / Too far "))
		}
		else
		{	//And Player position in sight range.
			if (PosState.bIsRight == true)
			{	//Also player in right
				RequestRolling(FName(TEXT("Right")));
				UE_LOG(LogTemp, Warning, TEXT("Not Close / Not Above / Back / InRange / Right "))
			}
			else
			{	//Also player in left
				RequestRolling(FName(TEXT("Left")));
				UE_LOG(LogTemp, Warning, TEXT("Not Close / Not Above / Back / InRange / Left "))
			}
		}
	}
}

void AEnemyPlaneController::RequestRolling(FName Direction)
{
	if (Direction == FName(TEXT("Left")))
		PositionUpdater->TryRolling(Direction);
	else if (Direction == FName(TEXT("Right")))
		PositionUpdater->TryRolling(Direction);
	else
		return;

	DelegateDone = &bRollingDone;
}

void AEnemyPlaneController::InitFalseDelegateBoolean() 
{
	bRollingDone = false;
	bPullUpDone = false;
	bImmelmannTurnDone = false;
	bStabilizeDone = false;
}