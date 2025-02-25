// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "../Interfaces/DataSavior.h"

#include "F15Pawn.generated.h"

DECLARE_DELEGATE_FourParams(FHUDDelegate, float, float, float, float)
DECLARE_DELEGATE(FViewChanger)

UENUM()
enum class ECameraType : uint8
{
	Main = 0,
	Cockpit,
	CutScene
};
UENUM()
enum class EPlayerState : uint8
{
	None = 0,
	CutScene
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CockpitCamera;

	FHUDDelegate OnReceiveHudValue;
	FViewChanger OnViewChange;

	FTimerHandle HudValueExecuter;

	EPlayerState PlayerState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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

	UPROPERTY()
	TObjectPtr<class UFighterRotateComponent> InputRotateComponent;

protected:	//Input, Control Surfaces Animation
	UFUNCTION()
	void ThrottleInput(const FInputActionValue& Value);
	UFUNCTION()
	void YawInput(const FInputActionValue& Value);
	UFUNCTION()
	void YawRelease(const FInputActionValue& Value);
	UFUNCTION()
	void PitchInput(const FInputActionValue& Value);
	UFUNCTION()
	void PitchRelease(const FInputActionValue& Value);
	UFUNCTION()
	void RollInput(const FInputActionValue& Value);
	UFUNCTION()
	void RollRelease(const FInputActionValue& Value);
	
	UFUNCTION()
	void LaunchInput(const FInputActionValue& Value);
	UFUNCTION()
	void ChangeViewInput(const FInputActionValue& Value);

	UFUNCTION()
	void UpdatePosition(float DeltaSeconds);
	UFUNCTION()
	void MissileAction();

	UFUNCTION()
	void RotateAnimation(float DeltaSeconds);

	UFUNCTION()
	void RequestActiveCamera();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* AirCraftInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputDataAsset* InputAction;

protected:
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
	//Constants
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Constants")
	float MaxThrustSpeed = 15000.f;
	//DynamicVariables	//UHoldingDataComp need to access
	UPROPERTY(BlueprintReadWrite, Category = "DynamicVariables")
	float ThrustSpeed = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "DynamicVariables")
	float ThrustParam = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "DynamicVariables")
	float CurrentSpeed = 0.f;

protected:
	//DynamicVariables
	UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	float AppliedGravity = 0.f;

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
	float DebugValueA = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Debug")
	float DebugValueB = 0.f;

};
