// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyPositionUpdater.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API UEnemyPositionUpdater : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyPositionUpdater();

	UPROPERTY()
	bool bComponentFlag = false;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	bool Initialize();

	UPROPERTY()
	TObjectPtr<class AEnemySu33Pawn> CompOwner;
	UPROPERTY()
	TObjectPtr<class UWorld> World;

protected:
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
	UPROPERTY( )
	float Drag = 0.25f;

	//DynamicVariables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicVariables")
	float ThrustSpeed = 9500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DynamicVariables")
	float CurrentSpeed = 9500.f;
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
	//DynamicVariables - Control Surfaces - DEPRECATED
	//UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	//float RudderScale = 0.f;			//Yaw
	//UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	//float FlapsScale = 0.f;				//Pitch, Front
	//UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	//float StabilizersScale = 0.f;		//Pitch, Rear
	//UPROPERTY(BlueprintReadOnly, Category = "DynamicVariables")
	//float AileronScale = 0.f;			//Roll

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
