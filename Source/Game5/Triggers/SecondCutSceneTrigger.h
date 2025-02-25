// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecondCutSceneTrigger.generated.h"

UCLASS()
class GAME5_API ASecondCutSceneTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecondCutSceneTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
	void SceneATakeOffByTimer();
	UFUNCTION()
	void SceneBPitchUp();
	UFUNCTION()
	void MoveToNextLevel();
protected:
	UFUNCTION()
	void FindPlayer();
	UFUNCTION()
	bool CachAndInitialize();

	bool bInitFlag = false;
protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> PlayerPawn;
	UPROPERTY()
	TObjectPtr<class UFighterRotateComponent> PlayerRotateComp;
	UPROPERTY()
	TObjectPtr<class APlayerHUD> PlayerHud;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UWorld> NextLevel;
protected:
	UPROPERTY()
	TObjectPtr<class UBoxComponent> PassingbyCollision;

	

	
	bool bSceneTrigger = true;

	bool bPitchUpTrigger = false;

	UPROPERTY()
	FVector WaittingLocation;

protected:
	UPROPERTY(EditDefaultsOnly)
	float SceneATakeOffDelay = 4.f;
	UPROPERTY(EditDefaultsOnly)
	float TakeOffSpeedRatio = 0.6f;

	UPROPERTY(EditDefaultsOnly)
	float SceneBPitchUpDelay = 1.2f;

protected:
	FTimerHandle SceneBTimer;
	FTimerHandle SceneBOpenNextLevel;
	

	
};
