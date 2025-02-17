// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseLogicComponent.h"
#include "EnemySu33Pawn.h"
#include "EnemyPositionUpdater.h"

// Sets default values for this component's properties
UChaseLogicComponent::UChaseLogicComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChaseLogicComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Initialize())
	{
		SetComponentTickEnabled(false);
		return;
	}
}


// Called every frame
void UChaseLogicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UChaseLogicComponent::Initialize()
{
	World = GetWorld();
	if (!World)
		return false;

	CompOwner = Cast<AEnemySu33Pawn>(GetOwner());
	if (!CompOwner)
		return false;

	PositionUpdater = CompOwner->GetComponentByClass<UEnemyPositionUpdater>();
	if (!PositionUpdater)
		return false;

	return true;
}

void UChaseLogicComponent::AdjustPitch()
{
	if (CompOwner->Decision.bIsAbove)
	{
		CompOwner->bPitchUp = true;
		CompOwner->bPitchDown = false;
	}
	else
	{
		CompOwner->bPitchUp = false;
		CompOwner->bPitchDown = true;
	}
}

void UChaseLogicComponent::AdjustYaw()
{
	if (CompOwner->Decision.bIsInFront)
	{
		if (CompOwner->Decision.bIsInSight)
		{
			CompOwner->bYawRight = CompOwner->Decision.bIsRight;
			CompOwner->bYawLeft = !CompOwner->Decision.bIsRight;
		}
	}
	else
	{
		CompOwner->bYawRight = false;
		CompOwner->bYawLeft = false;
	}
}

void UChaseLogicComponent::RollManeuver(float DeltaTime)
{
	static bool bRollingRight = true;
	static float LastRollTime = 0.f;
	static float RollCoolDown = 1.2f;
	
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastRollTime > RollCoolDown)
	{
		PositionUpdater->RequestRoll(bRollingRight);
		bRollingRight = !bRollingRight;
		LastRollTime = CurrentTime;
	}
}

