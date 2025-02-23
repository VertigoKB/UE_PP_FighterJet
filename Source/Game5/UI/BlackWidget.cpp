// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackWidget.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void UBlackWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FadeInEndTime = FadeIn->GetEndTime();
	NormalizedDuration = FadeInEndTime / FadeDuration;
	
}

void UBlackWidget::PlayFadeIn()
{
	PlayAnimation(FadeIn, 0.f, 1, EUMGSequencePlayMode::Forward, NormalizedDuration, false);
}

void UBlackWidget::PlayFadeOut()
{
	PlayAnimation(FadeIn, 0.f, 1, EUMGSequencePlayMode::Reverse, NormalizedDuration, false);
}
