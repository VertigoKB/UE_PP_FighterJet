// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "BlackWidget.h"
#include "PilotAimHelper.h"
#include "ArtificalHorizon.h"
#include "../Player/F15Pawn.h"
#include "../Enemy/EnemySu33Pawn.h"
#include "../Player/UserController.h"
#include "../Player/LockOnComponent.h"

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

			MyController = Cast<AUserController>(OwnerPlayer->GetController());
		}
	}
	if (MyController)
	{
		MyController->OnLockOnable.AddLambda([&]() { bLockable = true; });
		MyController->OnEnemyLost.AddLambda([&]() { bLockable = false; });
		MyController->InLockOnRange.BindLambda([&]() { bInLockOnRange = true; });
		MyController->NotLockOnRange.BindLambda([&]() { bInLockOnRange = false; });
		MyController->SendEnemyPos.BindUObject(this, &APlayerHUD::EnemyScreenPositionUpdater);
	}
	CachEnemy();

	if (OwnerPlayer)
	{
		UActorComponent* RawLockComp = OwnerPlayer->GetComponentByClass(ULockOnComponent::StaticClass());
		if (RawLockComp)
		{
			ULockOnComponent* LockComp = Cast<ULockOnComponent>(RawLockComp);
			LockComp->OnLocked.BindLambda([&]() { bLocked = true; });
			LockComp->OnLostSignal.BindLambda([&]() { bLocked = false; });
		}
	}
	
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
		SetLockboxVisibility(true);
	}
	else
	{
		GeneratedAimHelper->SetVisibility(ESlateVisibility::Collapsed);
		GeneratedHorizon->SetVisibility(ESlateVisibility::Collapsed);
		SetLockboxVisibility(false);
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

void APlayerHUD::EnemyScreenPositionUpdater(float X, float Y)
{
	EnemyX = X;
	EnemyY = Y;
}

void APlayerHUD::GameEndFadeOut()
{
	GeneratedBlackWidget->PlayFadeOut();
}

void APlayerHUD::GameEndText()
{
	GeneratedBlackWidget->SetEndMentVisibility(true);
}
