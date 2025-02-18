// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPositionUpdater.h"

#include "Kismet/KismetMathLibrary.h"

#include "EnemySu33Pawn.h"

// Sets default values for this component's properties
UEnemyPositionUpdater::UEnemyPositionUpdater()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UEnemyPositionUpdater::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (!Initialize())
	{
		SetComponentTickEnabled(false);
		return;
	}

}

// Called every frame
void UEnemyPositionUpdater::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePosition(DeltaTime);

	if (CompOwner->bPitchUp)
		UpdatePitch(DeltaTime, 1.f);
	if (CompOwner->bPitchDown)
		UpdatePitch(DeltaTime, -1.f);

	if (CompOwner->bRollLeft)
		UpdateRoll(DeltaTime, -1.f);
	if (CompOwner->bRollRight)
		UpdateRoll(DeltaTime, 1.f);

	if (CompOwner->bYawLeft)
		UpdateYaw(DeltaTime, -1.f);
	if (CompOwner->bYawRight)
		UpdateYaw(DeltaTime, 1.f);

	if (bTryStablize)
		ExecuteStabilize();
}

bool UEnemyPositionUpdater::Initialize()
{
	CompOwner = Cast<AEnemySu33Pawn>(GetOwner());
	if (!CompOwner)
		return false;
	
	World = GetWorld();
	if (!World)
		return false;

	return true;
}

//Update PawnTransform
void UEnemyPositionUpdater::UpdatePosition(float DeltaSeconds)
{
	if (ThrustSpeed < CurrentSpeed)
		CurrentSpeed = FMath::FInterpTo(CurrentSpeed, ThrustSpeed, DeltaSeconds, Drag);
	else
		CurrentSpeed = ThrustSpeed;

	FVector NewPosition = (CurrentSpeed * DeltaSeconds) * CompOwner->GetActorForwardVector();

	AppliedGravity = UKismetMathLibrary::MapRangeClamped(CurrentSpeed, 0.f, MinThrustToNotFall, Gravity, 0.f);

	float FallingScale = NewPosition.Z - (AppliedGravity * DeltaSeconds * 1.5f);
	NewPosition = FVector(NewPosition.X, NewPosition.Y, FallingScale);

	CompOwner->AddActorWorldOffset(NewPosition, true, nullptr, ETeleportType::TeleportPhysics);
}

void UEnemyPositionUpdater::UpdateYaw(float DeltaSeconds, float Yaw)
{
	TargetYaw = Yaw;
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaSeconds, 10.f);

	FRotator NewRotation = FRotator(0.f, (CurrentYaw * DeltaSeconds * 80.f), 0.f);
	CompOwner->AddActorLocalRotation(NewRotation, true);
}

void UEnemyPositionUpdater::UpdatePitch(float DeltaSeconds, float Pitch)
{
	TargetPitch = Pitch;
	CurrentPitch = FMath::FInterpTo(CurrentPitch, TargetPitch, DeltaSeconds, 10.f);

	FRotator NewRotation = FRotator((CurrentPitch * DeltaSeconds * 80.f), 0.f, 0.f);
	CompOwner->AddActorLocalRotation(NewRotation, true);
}

void UEnemyPositionUpdater::UpdateRoll(float DeltaSeconds, float Roll)
{
	TargetRoll = Roll;
	CurrentRoll = FMath::FInterpTo(CurrentRoll, TargetRoll, DeltaSeconds, 10.f);

	FRotator NewRotation = FRotator(0.f, 0.f, (CurrentRoll * DeltaSeconds * 80.f));
	CompOwner->AddActorLocalRotation(NewRotation, true);
}

void UEnemyPositionUpdater::TryRolling(FName RollDirection)
{
	if (RollDirection == FName(TEXT("Right")))
		CompOwner->bRollRight = true;
	else if (RollDirection == FName(TEXT("Left")))
		CompOwner->bRollLeft = true;

	FTimerHandle RollingTimer;
	World->GetTimerManager().SetTimer(RollingTimer, FTimerDelegate::CreateLambda([this]() {
		CompOwner->bRollRight = false;
		CompOwner->bRollLeft = false;
		RollingDone.ExecuteIfBound();
		}), 1.1f, false);
}

void UEnemyPositionUpdater::ExecuteStabilize()
{
	FRotator CurrentRotation = CompOwner->GetActorRotation();

	bool bRollLeftFlag = false;
	bool bRollRightFlag = false;
	bool bPitchDownFlag = false;
	bool bPitchUpFlag = false;


	if (CurrentRotation.Roll >= 0.1f)
	{
		CompOwner->bRollLeft = true;
		bRollLeftFlag = false;
	}
	else
	{
		CompOwner->bRollLeft = false;
		bRollLeftFlag = true;
	}

	if (CurrentRotation.Roll <= -0.1f)
	{
		CompOwner->bRollRight = true;
		bRollRightFlag = false;
	}
	else
	{
		CompOwner->bRollRight = false;
		bRollRightFlag = true;
	}

	if (CurrentRotation.Pitch >= 0.1f)
	{
		CompOwner->bPitchDown = true;
		bPitchDownFlag = false;
	}
	else
	{
		CompOwner->bPitchDown = false;
		bPitchDownFlag = true;
	}

	if (CurrentRotation.Pitch <= -0.1f)
	{
		CompOwner->bPitchUp = true;
		bPitchUpFlag = false;
	}
	else
	{
		CompOwner->bPitchUp = false;
		bPitchUpFlag = true;
	}

	if (bTryStablize == true &&
		bRollLeftFlag == true &&
		bRollRightFlag == true &&
		bPitchDownFlag == true &&
		bPitchUpFlag == true)
	{
		bTryStablize = false;
		StabilizeDone.ExecuteIfBound();
	}
}

void UEnemyPositionUpdater::TryImmelmannTurn()
{
	CompOwner->bPitchUp = true;

	FTimerHandle CobraTimer;
	World->GetTimerManager().SetTimer(CobraTimer, FTimerDelegate::CreateLambda([this]() {
		CompOwner->bPitchUp = false;
		ImmelmannTurnDone.ExecuteIfBound();
		}), 2.24f, false);
}

void UEnemyPositionUpdater::TryPullUp()
{
	CompOwner->bPitchUp = true;

	FTimerHandle PullUpTimer;
	World->GetTimerManager().SetTimer(PullUpTimer, FTimerDelegate::CreateLambda([this]() {
		CompOwner->bPitchUp = false;
		PullUpDone.ExecuteIfBound();
		}), 1.12f, false);
}
