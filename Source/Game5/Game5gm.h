// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Game5gm.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API AGame5gm : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGame5gm();

	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	UPROPERTY()
	TSubclassOf<AActor> BPfighter;
	UPROPERTY()
	TSubclassOf<AActor> BPspawnpointCarrier;
	UPROPERTY()
	APawn* PlayerPawn;

};
