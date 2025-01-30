// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "ture"), Category = "BoxComponent")
	class UBoxComponent* BoxRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "ture"), Category = "SkeletalMesh")
	class USkeletalMeshComponent* ModelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Input")
	class UInputMappingContext* AirCraftInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Input")
	class UInputDataAsset* InputAction;

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
	void RotateAnimation(float DeltaSeconds);

	//Constants
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Constants")
	float MaxThrustSpeed = 10000.f;
	UPROPERTY()
	float MinThrustToNotFall = 4000.f;	//AirCraft will Fall Down if its speed less than this
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Constants")
	float ThrustMultiplier = 2500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Constants")
	float Gravity = 981.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Constants")
	float Drag = 0.25f;
	//Constants - Control surfaces Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants",
		meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "Default = 1. The maximum extent angle multiplier which the control surfaces can be deployed."))
	float CtrlSurfacesRatio = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants",
		meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "Default = 1. The speed at which the control surfaces recover to their original position."))
	float CtrlSurfacesRecoveryFactor = 1.f;

	//DynamicVariables
	UPROPERTY(Category = "DynamicVariables")
	float ThrustSpeed = 0.f;
	UPROPERTY(Category = "DynamicVariables")
	float CurrentSpeed = 0.f;
	UPROPERTY(Category = "DynamicVariables")
	float AppliedGravity = 0.f;
	//DynamicVariables - Rotation
	UPROPERTY(Category = "DynamicVariables")
	float TargetYaw = 0.f;
	UPROPERTY(Category = "DynamicVariables")
	float CurrentYaw = 0.f;
	UPROPERTY(Category = "DynamicVariables")
	float TargetPitch = 0.f;
	UPROPERTY(Category = "DynamicVariables")
	float CurrentPitch = 0.f;
	UPROPERTY(Category = "DynamicVariables")
	float TargetRoll = 0.f;
	UPROPERTY(Category = "DynamicVariables")
	float CurrentRoll = 0.f;
	//DynamicVariables - Control Surfaces
	UPROPERTY(Category = "DynamicVariables")
	float RudderScale = 0.f;			//Yaw
	UPROPERTY(Category = "DynamicVariables")
	float FlapsScale = 0.f;				//Pitch, Front
	UPROPERTY(Category = "DynamicVariables")
	float StabilizersScale = 0.f;		//Pitch, Rear
	UPROPERTY(Category = "DynamicVariables")
	float AileronScale = 0.f;			//Roll


	//Debug
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Debug")
	float DebugValueA = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Debug")
	float DebugValueB = 0.f;
};
