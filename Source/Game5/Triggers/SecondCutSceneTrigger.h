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
	void SettingActors();
	UFUNCTION()
	bool CachAndInitialize();

	bool bInitFlag = false;
	
protected:
	UFUNCTION()
	void SceneAPlayerSequence();
	UFUNCTION()
	void GameStart();
protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> PlayerPawn;
	UPROPERTY()
	TObjectPtr<class APlayerHUD> PlayerHud;
	UPROPERTY()
	TObjectPtr<class APlayerController> FirstController;

	UPROPERTY()
	TObjectPtr<class AEnemySu33Pawn> EnemyPawn;
	UPROPERTY()
	TObjectPtr<class AEnemyPlaneController> EnemyController;
	UPROPERTY()
	TObjectPtr<class UEnemyPositionUpdater> EnemyPositionUpdater;

	UPROPERTY()
	TObjectPtr<class AActor> SequenceCamera;
	UPROPERTY()
	TObjectPtr<class ABattleFieldLevelSequenceActor> SequenceExecuter;

	
};
