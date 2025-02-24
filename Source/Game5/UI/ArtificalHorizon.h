// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArtificalHorizon.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API UArtificalHorizon : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	bool CachAndInitialize();

protected:
	UPROPERTY()
	TObjectPtr<class APlayerHUD> OwnerHud;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UMaterialInstance> ParentMaterial;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> HorizonImage;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> HorizonMaterial;

protected:
	UPROPERTY()
	float PitchOffset = 0.f;
	UPROPERTY()
	float CurrentPitch = 0.f;
	UPROPERTY()
	float CurrentRoll = 0.f;

};