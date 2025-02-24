// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WidgetVisiblityController.generated.h"

/**
 이 클래스는 DEPRECATED 되었으나 필요할 경우 활용할 것
 */
UCLASS()
class GAME5_API UWidgetVisiblityController : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	UFUNCTION()
	void RequestChangeVisibility();

protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> CachedPlayer;
};
