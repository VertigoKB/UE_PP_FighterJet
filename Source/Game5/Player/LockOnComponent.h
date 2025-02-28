// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FLocked)
DECLARE_MULTICAST_DELEGATE(FLostSignal)
DECLARE_DELEGATE(FOnLocking)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	bool CachAndInit();

protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> PlayerPawn;
	UPROPERTY()
	TObjectPtr<class AUserController> UserController;

protected:
	float LockOnElappsedTime = 0.f;
	float LockOnInterval = 2.5f;

	float ReleaseLockRatio = 0.6f;

	bool bLockOnable = false;
	
public:
	UPROPERTY()
	bool bLaunchable = false;

public:
	FLocked OnLocked;
	FLostSignal OnLostSignal;
	FOnLocking OnLocking;
		
};
