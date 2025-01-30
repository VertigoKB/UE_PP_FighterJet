// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "InputAction.h"

#include "InputDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* Throttle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* NoseYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* NosePitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* NoseRoll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* LaunchProjectile;
};
