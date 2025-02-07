// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWidget.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void UBlackWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	NormalizedDuration = FadeIn->GetEndTime() / FadeDuration;
	
}

void UBlackWidget::PlayFadeEffect(bool bIsFadeIn)
{
	if (bIsFadeIn)
		PlayAnimation(FadeIn, 0.f, 1, EUMGSequencePlayMode::Forward, NormalizedDuration, false);
	else
		PlayAnimation(FadeIn, 0.f, 1, EUMGSequencePlayMode::Reverse, NormalizedDuration, false);
}