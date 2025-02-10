// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlackWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API UBlackWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void PlayFadeEffect(bool bIsFadeIn);

	UPROPERTY(meta = (BindWidget))
	class UImage* BlackImage;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeIn;
	UPROPERTY()
	float FadeInEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeDuration = 1.f;
	UPROPERTY()
	float NormalizedDuration = 1.f;


};
