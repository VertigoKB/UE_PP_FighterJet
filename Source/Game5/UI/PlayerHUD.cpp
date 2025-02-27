// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "BlackWidget.h"
#include "PilotAimHelper.h"
#include "ArtificalHorizon.h"
#include "../Player/F15Pawn.h"
#include "../Enemy/EnemySu33Pawn.h"

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
		{
			GeneratedAimHelper->AddToViewport();
			GeneratedAimHelper->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (Horizon)
	{
		GeneratedHorizon = CreateWidget<UArtificalHorizon>(GetWorld(), Horizon);
		if (GeneratedHorizon)
		{
			GeneratedHorizon->AddToViewport();
			GeneratedHorizon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	TObjectPtr<APlayerController> GetOwnerController = Cast<APlayerController>(GetOwner());
	if (GetOwnerController)
	{
		OwnerPlayer = Cast<AF15Pawn>(GetOwnerController->GetPawn());
		if (OwnerPlayer)
		{
			OwnerPlayer->OnReceiveHudValue.BindUObject(this, &APlayerHUD::AsyncValue);
			OwnerPlayer->OnViewChange.AddUObject(this, &APlayerHUD::ChangeVisiblity);
		}
	}

	CachEnemy();
	
}

void APlayerHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OwnerPlayer->OnReceiveHudValue.Unbind();
	OwnerPlayer->OnViewChange.RemoveAll(this);
}

void APlayerHUD::AsyncValue(float Thrust, float Altitude, float Pitch, float Roll)
{
	ThrustValue = Thrust;
	AltitudeValue = Altitude;
	PitchValue = Pitch;
	RollValue = Roll;
}

void APlayerHUD::ChangeVisiblity()
{
	if (GeneratedAimHelper->GetVisibility() == ESlateVisibility::Collapsed)
	{
		GeneratedAimHelper->SetVisibility(ESlateVisibility::Visible);
		GeneratedHorizon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GeneratedAimHelper->SetVisibility(ESlateVisibility::Collapsed);
		GeneratedHorizon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool APlayerHUD::CachEnemy()
{
	TArray<AActor*> TempEnemyFinder;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySu33Pawn::StaticClass(), TempEnemyFinder);
	
	if (TempEnemyFinder.Num() > 0)
	{
		EnemyFinder.Empty();
		for (AActor* Actor : TempEnemyFinder)
		{
			EnemyFinder.Add(Actor);
		}
	}
	else
		return false;

	return true;

}
