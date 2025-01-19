// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

//Engine
#include "Kismet/GameplayStatics.h"

//EnhancedInput
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"

//Custom
#include "InputDataAsset.h"
#include "Interfaces/Throttle.h"

#include "UserController.generated.h"

/**
 * 
 */

UCLASS()
class GAME5_API AUserController : public APlayerController, public IThrottle
{
	GENERATED_BODY()

public:
	AUserController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void Test() override;
	virtual float GetThrottle() const override { return CtrlerThrottle; }
	virtual void SetThrottle(float Value) override { CtrlerThrottle = Value; }

	//meta = (AllowPrivateAccess = "true")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* Game5InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputDataAsset* InputActions;

	UPROPERTY(BlueprintReadOnly, Category = "Throttle")
	float CtrlerThrottle	= 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Nose")
	float NoseYaw			= 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Nose")
	float NosePitch			= 0.f; 
	UPROPERTY(BlueprintReadOnly, Category = "Nose")
	float NoseRoll			= 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Value")
	bool OnGround = true;
	

private:

	UPROPERTY()
	APawn* AllocatedPawn;


	UFUNCTION()
	void ThrottleCtrl(const FInputActionValue& Value);
	UFUNCTION()
	void YawCtrl(const FInputActionValue& Value);
	UFUNCTION()
	void PitchCtrl(const FInputActionValue& Value);
	UFUNCTION()
	void RollCtrl(const FInputActionValue& Value);

	UFUNCTION()
	float RotatorClear(float Axis);
	
	UFUNCTION()
	void AfterBunnerSpeed();
};
