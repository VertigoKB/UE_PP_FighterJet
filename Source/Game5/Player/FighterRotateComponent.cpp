// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterRotateComponent.h"

// Sets default values for this component's properties
UFighterRotateComponent::UFighterRotateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFighterRotateComponent::BeginPlay()
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
void UFighterRotateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FMath::Abs(RollValue) > ConditionEpsilon)
		Rolling(DeltaTime);
	if (FMath::Abs(PitchValue) > ConditionEpsilon)
		Pitching(DeltaTime);
	if (FMath::Abs(YawValue) > ConditionEpsilon)
		Yawing(DeltaTime);
}

bool UFighterRotateComponent::Initialize()
{
	MyPawn = GetOwner();
	if (!MyPawn)
		return false;

	return true;
}

void UFighterRotateComponent::Rolling(float DeltaTime)
{
	float TargetSpeed = RollValue * MaxRollSpeed;
	if (FMath::Abs(RollValue) > 0.01f)
		CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetSpeed, DeltaTime, RollAcceleration);
	else
		CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, 0.f, DeltaTime, RollDeceleration);

	FRotator RollRotation = FRotator(0.f, 0.f, (CurrentRollSpeed * DeltaTime));
	MyPawn->AddActorLocalRotation(RollRotation, true);
}

void UFighterRotateComponent::Pitching(float DeltaTime)
{
	float TargetSpeed = PitchValue * MaxPitchSpeed;
	if (FMath::Abs(PitchValue) > 0.01f)
		CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetSpeed, DeltaTime, PitchAcceleration);
	else
		CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, 0.f, DeltaTime, PitchDeceleration);

	FRotator PitchRotation = FRotator((CurrentPitchSpeed * DeltaTime), 0.f, 0.f);
	MyPawn->AddActorLocalRotation(PitchRotation, true);
}

void UFighterRotateComponent::Yawing(float DeltaTime)
{
	float TargetSpeed = YawValue * MaxYawSpeed;
	if (FMath::Abs(YawValue) > 0.01f)
		CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetSpeed, DeltaTime, YawAcceleration);
	else
		CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, 0.f, DeltaTime, YawDeceleration);

	FRotator YawRotation = FRotator(0.f, (CurrentYawSpeed * DeltaTime), 0.f);
	MyPawn->AddActorLocalRotation(YawRotation, true);
}

