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
	bComponentFlag = Initialize();
}

// Called every frame
void UEnemyPositionUpdater::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bComponentFlag)
	{
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
	}
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

void UEnemyPositionUpdater::RequestRoll(bool bIsRollRight)
{
	if (bIsRollRight)
		CompOwner->bRollRight = true;
	else
		CompOwner->bRollLeft = true;

	FTimerHandle RollingTimer;
	World->GetTimerManager().SetTimer(RollingTimer, FTimerDelegate::CreateLambda([this]() {
		CompOwner->bRollRight = false;
		CompOwner->bRollLeft = false;
		}), 1.1f, false);
}

void UEnemyPositionUpdater::RequestRollStabilize()
{
	FRotator CurrentRotation = CompOwner->GetActorRotation();
	float RollAngle = FMath::Abs(CurrentRotation.Roll);

	if (RollAngle > 1.f)
		CompOwner->bRollLeft = true;
	else
		CompOwner->bRollLeft = false;

	if (RollAngle < -1.f)
		CompOwner->bRollRight = true;
	else
		CompOwner->bRollRight = false;
}
