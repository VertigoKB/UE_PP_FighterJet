// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UserController.generated.h"

UCLASS()
class GAME5_API AUserController : public APlayerController//, public IThrottle
{
	GENERATED_BODY()

public:
	AUserController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
};
