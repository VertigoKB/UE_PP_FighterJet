// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FighterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API AFighterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFighterGameMode();

	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	UFUNCTION()
	void FindCarrierCamera(AController* NewPlayer);

	UPROPERTY()
	TSubclassOf<AActor> BPfighter;
	UPROPERTY()
	TSubclassOf<AActor> BPspawnpointCarrier;

	UPROPERTY()
	TSubclassOf<AActor> BPCameraCarrierA;
	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	TObjectPtr<class APlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<AActor> CameraCarrierA;

protected:

};
