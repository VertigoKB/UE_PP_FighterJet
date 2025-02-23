// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "BlackWidget.h"
#include "PilotAimHelper.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (BlackWidget)
	{
		GeneratedBlackWidget = CreateWidget<UBlackWidget>(GetWorld(), BlackWidget);
		if (GeneratedBlackWidget)
			GeneratedBlackWidget->AddToViewport();
	}

	if (AimHelper)
	{
		GeneratedAimHelper = CreateWidget<UPilotAimHelper>(GetWorld(), AimHelper);
		if (GeneratedAimHelper)
			GeneratedAimHelper->AddToViewport();
	}
}
