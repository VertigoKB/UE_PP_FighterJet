// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSMComponent.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Stabilize = 0,
	Maneuver,
	Death
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API UEnemyFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	UFUNCTION()
	bool Initialize();

protected:
	UFUNCTION()
	void ExecuteStabilizeState();
	UFUNCTION()
	void ExecuteManeuverState();

protected:
	UPROPERTY()
	bool bRoolingDone = false;
	UPROPERTY()
	bool bPullUpDone = false;
	UPROPERTY()
	bool bImmelmannTurnDone = false;
	UPROPERTY()
	bool bStabilizeDone = false;

	UPROPERTY(EditDefaultsOnly)
	float MaxManeuverTime = 3.f;

	EEnemyState State;

protected:
	UPROPERTY()
	TObjectPtr<class AEnemySu33Pawn> CompOwner;
	UPROPERTY()
	TObjectPtr<class UWorld> World;
	UPROPERTY()
	TObjectPtr<class AActor> Target;
	UPROPERTY()
	TObjectPtr<class UEnemyPositionUpdater> PositionUpdater;

private:
	inline void InitFalseDelegateBoolean() {
		bRoolingDone = false;
		bPullUpDone = false;
		bImmelmannTurnDone = false;
		bStabilizeDone = false;
	}


};
