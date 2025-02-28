// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../CommonData/TDoOnce.h"
#include "UserController.generated.h"

DECLARE_DELEGATE_TwoParams(FSendEnemyPos, float, float)
DECLARE_DELEGATE(FInLockOnRange)
DECLARE_DELEGATE(FNotLockOnRange)
DECLARE_MULTICAST_DELEGATE(FOnLockOnable)
DECLARE_MULTICAST_DELEGATE(FOnEnemyLost)

UCLASS()
class GAME5_API AUserController : public APlayerController//, public IThrottle
{
	GENERATED_BODY()

public:
	AUserController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	const TArray<TObjectPtr<AActor>>& GetEnemyFinder() const { return EnemyFinder; }

protected:
	UFUNCTION()
	bool CachAndInit();

	bool bInitFlag = false;

private:
	UFUNCTION()		//Idiot function with hardcode
	void SearchAndBroadcast();

protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> PlayerPawn;
	UPROPERTY()
	TObjectPtr<class APlayerHUD> PlayerHud;

	UPROPERTY()
	TObjectPtr<class UPilotAimHelper> PilotAimWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> EnemyFinder;
	
protected:
	UPROPERTY()
	bool bPilotView = false;

protected:
	UPROPERTY(EditAnywhere)
	float LockOnRadius = 250;
	UPROPERTY()
	FVector2D LockOnCenter;

	float BaseX = 1920.f;
	float BaseY = 1080.f;

	float ViewRatio = 0.f;

public:
	FOnLockOnable OnLockOnable;
	FOnEnemyLost OnEnemyLost;
	FInLockOnRange InLockOnRange;
	FSendEnemyPos SendEnemyPos;
	FNotLockOnRange NotLockOnRange;

protected:
	TDoOnce DoLockOnOnce;
	TDoOnce DoEnemyLostOnce;


};
