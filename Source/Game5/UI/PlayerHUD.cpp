// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "BlackWidget.h"
#include "PilotAimHelper.h"
#include "../Player/F15Pawn.h"

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

	TObjectPtr<APlayerController> GetOwnerController = Cast<APlayerController>(GetOwner());
	if (GetOwnerController)
	{
		OwnerPlayer = Cast<AF15Pawn>(GetOwnerController->GetPawn());
		if (OwnerPlayer)
			OwnerPlayer->OnReceiveHudValue.BindUObject(this, &APlayerHUD::AsyncValue);
	}
	
}

void APlayerHUD::AsyncValue(float Thrust, float Altitude)
{
	ThrustValue = Thrust;
	AltitudeValue = Altitude;
}
