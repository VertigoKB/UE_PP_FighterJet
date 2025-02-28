// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnComponent.h"
#include "UserController.h"
#include "F15Pawn.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(false);

	// ...
}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (!CachAndInit())
	{
		SetComponentTickEnabled(false);
		return;
	}
	SetComponentTickEnabled(true);

	
}


// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bLockOnable)
	{
		if (LockOnElappsedTime < LockOnInterval)
			LockOnElappsedTime += DeltaTime;
	}
	else
	{
		if (LockOnElappsedTime > 0.f)
			LockOnElappsedTime -= DeltaTime * ReleaseLockRatio;
	}

	if (LockOnElappsedTime >= LockOnInterval)
	{
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, TEXT("Launchable"));
		bLaunchable = true;
		OnLocked.ExecuteIfBound();
	}
	else
	{
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, TEXT("Cant Launc"));
		bLaunchable = false;
		OnLostSignal.ExecuteIfBound();
	}
}

bool ULockOnComponent::CachAndInit()
{
	PlayerPawn = Cast<AF15Pawn>(GetOwner());
	if (!PlayerPawn)
		return false;

	UserController = Cast<AUserController>(PlayerPawn->GetController());
	if (!UserController)
		return false;
	
	UserController->OnLockOnable.AddLambda([&]() { bLockOnable = true; });
	UserController->OnEnemyLost.AddLambda([&]() { bLockOnable = false; });
	PlayerPawn->OnViewChange.AddLambda([&]() { bLockOnable = false; });

	return true;
}

