// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FighterGameInst.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API UFighterGameInst : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RequestMoveCountIncrement();

	UPROPERTY(BlueprintReadWrite)
	float ThrustSpeed = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float CurrentSpeed = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float PlayerZ = 0.f;
	UPROPERTY(BlueprintReadWrite)
	FRotator PlayerRotator = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite)
	int32 MoveCounter = 0;

};
