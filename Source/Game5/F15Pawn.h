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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "ture"), Category = "RootComponent")
	class USceneComponent* SceneRoot;
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
	void UpdatePosition(float DeltaSeconds);
	UFUNCTION()
	void UpdateYaw(float DeltaSeconds, float Yaw);
	UFUNCTION()
	void UpdatePitch(float DeltaSeconds, float Pitch);
	UFUNCTION()
	void UpdateRoll(float DeltaSeconds, float Roll);

	//Constants
	UPROPERTY()
	float MaxThrustSpeed = 10000.f;
	UPROPERTY()
	float MinThrustToNotFall = 4000.f;	//AirCraft will Fall Down if its speed less than this
	UPROPERTY()
	float ThrustMultiplier = 2500.f;
	UPROPERTY()
	float Gravity = 981.f;
	UPROPERTY()
	float Drag = 0.25f;
	//Constants - Control Surfaces
	UPROPERTY()
	float MaxFlapPitch = 10.f;
	UPROPERTY()
	float MaxElevatorPitch = 25.f;
	UPROPERTY()
	float MaxRudderYaw = 45.f;
	UPROPERTY()
	float MaxAileronPitch = 45.f;

	//DynamicVariables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DynamicVariables")
	float ThrustSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DynamicVariables")
	float CurrentSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DynamicVariables")
	float AppliedGravity = 0.f;
	//DynamicVariables - Rotation
	UPROPERTY()
	float TargetYaw = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DynamicVariables")
	float CurrentYaw = 0.f;
	UPROPERTY()
	float TargetPitch = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DynamicVariables")
	float CurrentPitch = 0.f;
	UPROPERTY()
	float TargetRoll = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DynamicVariables")
	float CurrentRoll = 0.f;
};
