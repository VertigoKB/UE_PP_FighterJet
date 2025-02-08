// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "../Interfaces/DataSavior.h"

#include "F15Pawn.generated.h"

UCLASS()
class GAME5_API AF15Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AF15Pawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:	//Missile
	UFUNCTION(BlueprintCallable)
	FTransform GetMissileSocketLoaction(const uint8 SocketNum);

	UFUNCTION(BlueprintCallable)
	void InitLoadMissile(UObjectPoolSystem* Pool);

	UFUNCTION(BlueprintCallable)
	void MissileLoadProcess(UObjectPoolSystem* Pool, uint8 Num);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> LoadedMissiles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> IsMissileEmpty;

	static constexpr int8 MaxLoadableMissile = 4;

protected:	//Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
	class UBoxComponent* BoxRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* ModelMesh;

protected:	//Input, Control Surfaces Animation
	UFUNCTION()
	void ThrottleInput(const FInputActionValue& Value);
	UFUNCTION()
	void YawInput(const FInputActionValue& Value);
	UFUNCTION()
	void PitchInput(const FInputActionValue& Value);
	UFUNCTION()
	void RollInput(const FInputActionValue& Value);
	UFUNCTION()
	void LaunchInput(const FInputActionValue& Value);

	UFUNCTION()
	void UpdatePosition(float DeltaSeconds);
	UFUNCTION()
	void UpdateYaw(float DeltaSeconds, float Yaw);
	UFUNCTION()
	void UpdatePitch(float DeltaSeconds, float Pitch);
	UFUNCTION()
	void UpdateRoll(float DeltaSeconds, float Roll);
	UFUNCTION()
	void MissileAction();

	UFUNCTION()
	void RotateAnimation(float DeltaSeconds);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* AirCraftInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputDataAsset* InputAction;

	//Constants
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constants")
	float MaxThrustSpeed = 10000.f;
	UPROPERTY()
	float MinThrustToNotFall = 4000.f;	//AirCraft will Fall Down if its speed less than this
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constants")
	float ThrustMultiplier = 2500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constants")
	float Gravity = 981.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constants")
	float Drag = 0.25f;
	//Constants - Control surfaces Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants",
		meta = (ToolTip = "Default = 1. The maximum extent angle multiplier which the control surfaces can be deployed."))
	float CtrlSurfacesRatio = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants",
		meta = (ToolTip = "Default = 1. The speed at which the control surfaces recover to their original position."))
	float CtrlSurfacesRecoveryFactor = 1.f;

public:
	//DynamicVariables	//UHoldingDataComp need to access
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicVariables")
	float ThrustSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicVariables")
	float CurrentSpeed = 0.f;

protected:
	//DynamicVariables
	UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	float AppliedGravity = 0.f;
	//DynamicVariables - Rotation
	UPROPERTY()
	float TargetYaw = 0.f;
	UPROPERTY()
	float CurrentYaw = 0.f;
	UPROPERTY()
	float TargetPitch = 0.f;
	UPROPERTY()
	float CurrentPitch = 0.f;
	UPROPERTY()
	float TargetRoll = 0.f;
	UPROPERTY()
	float CurrentRoll = 0.f;
	//DynamicVariables - Control Surfaces
	UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	float RudderScale = 0.f;			//Yaw
	UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	float FlapsScale = 0.f;				//Pitch, Front
	UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	float StabilizersScale = 0.f;		//Pitch, Rear
	UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	float AileronScale = 0.f;			//Roll


private: //Debug
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Debug")
	float DebugValueA = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Debug")
	float DebugValueB = 0.f;
};
