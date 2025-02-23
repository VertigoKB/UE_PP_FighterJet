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

protected:
	UFUNCTION()
	void AsyncValue(float Thrust, float Altitude);

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BlackWidget;
	UPROPERTY(BlueprintReadWrite)
	UBlackWidget* GeneratedBlackWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> AimHelper;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UPilotAimHelper> GeneratedAimHelper;

	UPROPERTY()
	float ThrustValue = 0.f;
	UPROPERTY()
	float AltitudeValue = 0.f;
protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> OwnerPlayer;
};
