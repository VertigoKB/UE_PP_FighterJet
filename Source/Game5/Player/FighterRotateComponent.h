// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FighterRotateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API UFighterRotateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFighterRotateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	bool Initialize();

public:		// These function have to call by tick
	UFUNCTION()
	void Rolling(float DeltaTime);	
	UFUNCTION()
	void Pitching(float DeltaTime);
	UFUNCTION()
	void Yawing(float DeltaTime);

public:	//(RotateAxis)Value have to initialize by input value. (1.f or -1.f)
	UPROPERTY()
	float RollValue = 0.f;
	UPROPERTY()
	float CurrentRollSpeed = 1.f;

	UPROPERTY()
	float PitchValue = 0.f;
	UPROPERTY()
	float CurrentPitchSpeed = 1.f;

	UPROPERTY()
	float YawValue = 0.f;
	UPROPERTY()
	float CurrentYawSpeed = 1.f;

protected:
	UPROPERTY(EditDefaultsOnly)
	float MaxRollSpeed = 80.f;
	UPROPERTY(EditDefaultsOnly)
	float RollAcceleration = 2.5f;
	UPROPERTY(EditDefaultsOnly)
	float RollDeceleration = 3.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxPitchSpeed = 80.f;
	UPROPERTY(EditDefaultsOnly)
	float PitchAcceleration = 2.5f;
	UPROPERTY(EditDefaultsOnly)
	float PitchDeceleration = 3.f;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxYawSpeed = 80.f;
	UPROPERTY(EditDefaultsOnly)
	float YawAcceleration = 2.5f;
	UPROPERTY(EditDefaultsOnly)
	float YawDeceleration = 3.f;

protected:
	UPROPERTY()
	TObjectPtr<AActor> MyPawn;

private:
	float ConditionEpsilon = 0.001f;
};
