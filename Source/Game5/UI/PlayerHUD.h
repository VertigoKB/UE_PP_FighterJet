// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
class UBlackWidget;
UCLASS()
class GAME5_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	UFUNCTION()
	void AsyncValue(float Thrust, float Altitude, float Pitch, float Roll);
	UFUNCTION()
	void ChangeVisiblity();

	UFUNCTION()
	bool CachEnemy();

	UFUNCTION()
	void EnemyScreenPositionUpdater(float X, float Y);

	UFUNCTION(BlueprintImplementableEvent)
	void SetLockboxVisibility(bool bVisible);

	UFUNCTION(BlueprintCallable)
	void GameEndFadeOut();
	UFUNCTION(BlueprintCallable)
	void GameEndText();

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BlackWidget;
	UPROPERTY(BlueprintReadWrite)
	UBlackWidget* GeneratedBlackWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> AimHelper;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UPilotAimHelper> GeneratedAimHelper;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> Horizon;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UArtificalHorizon> GeneratedHorizon;

	UPROPERTY()
	float ThrustValue = 0.f;
	UPROPERTY()
	float AltitudeValue = 0.f;
	UPROPERTY()
	float PitchValue = 0.f;
	UPROPERTY()
	float RollValue = 0.f;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EnemyX = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EnemyY = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bLockable = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInLockOnRange = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bLocked = false;
protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> OwnerPlayer;

	UPROPERTY()
	TObjectPtr<class AUserController> MyController;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> EnemyFinder;
};
