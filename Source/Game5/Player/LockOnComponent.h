// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"


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
	TObjectPtr<class AUserController> UserController;

protected:
	float LockOnElappsedTime = 0.f;
	float LockOnInterval = 1.5f;

	float ReleaseLockRatio = 0.5f;

	bool bLockOnable = false;
	
public:
	UPROPERTY()
	bool bLaunchable = false;
		
};
