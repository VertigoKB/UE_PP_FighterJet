// Fill out your copyright notice in the Description page of Project Settings.

#include "UserController.h"
#include "TimerManager.h"
#include "../Player/F15Pawn.h"
#include "../UI/PlayerHUD.h"
#include "../UI/PilotAimHelper.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "../Enemy/EnemySu33Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"

AUserController::AUserController()
{
	PrimaryActorTick.bCanEverTick = true; 
	PrimaryActorTick.SetTickFunctionEnable(false);
	bShowMouseCursor = false;

}

void AUserController::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DelayInitTimer;
	GetWorldTimerManager().SetTimer(DelayInitTimer, FTimerDelegate::CreateLambda([&]() {

		bInitFlag = CachAndInit();
		if (!bInitFlag)
			return;
		else
			SetActorTickEnabled(true);

		int32 ScreenX, ScreenY;
		GetViewportSize(ScreenX, ScreenY);

		if (!(FMath::IsNearlyEqual(ScreenX, BaseX, (10.E-4))))
			ViewRatio = ScreenX / BaseX;
		else
			ViewRatio = 1.f;

		LockOnCenter = FVector2D(ScreenX * 0.5f, ScreenY * 0.5f);

		}), 0.05f, false);
}

void AUserController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void AUserController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPilotView)
		SearchAndBroadcast();
}

void AUserController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->Viewport->ViewportResizedEvent.RemoveAll(this);
}

bool AUserController::CachAndInit()
{
	PlayerPawn = Cast<AF15Pawn>(GetPawn());
	if (!PlayerPawn)
		return false;

	PlayerHud = Cast<APlayerHUD>(GetHUD());
	if (!PlayerHud)
		return false;

	PilotAimWidget = PlayerHud->GeneratedAimHelper;
	if (!PilotAimWidget)
		return false;

	//MissileRangeSizeBox = PilotAimWidget->MissileSizeBox;
	//if (!MissileRangeSizeBox)
	//	return false;

	TArray<AActor*> TempEnemyFinder;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySu33Pawn::StaticClass(), TempEnemyFinder);
	if (TempEnemyFinder.Num() > 0)
	{
		EnemyFinder.Empty();
		for (AActor* Actor : TempEnemyFinder)
		{
			EnemyFinder.Add(Actor);
		}
		PlayerPawn->FoundEnemy = EnemyFinder;
	}
	else
		return false;

	PlayerPawn->OnViewChange.AddLambda([&]() {	bPilotView = !bPilotView; });

	return true;
}

void AUserController::SearchAndBroadcast()
{
	if (EnemyFinder.IsEmpty())
		return;

	int32 EnemyCount = EnemyFinder.Num();

	LockOnRadius = (PilotAimWidget->ImageMissileCircle->GetDesiredSize().X * ViewRatio) * 0.4f;

	for (int32 i = 0; i < EnemyCount; i++)
	{
		FVector EnemyLocationInArray = EnemyFinder[i]->GetActorLocation();
		FVector2D EnemyScreenPosition = FVector2D(0.f, 0.f);

		bool ConditionA = ProjectWorldLocationToScreen(EnemyLocationInArray, EnemyScreenPosition);

		float DistanceToCenter = FVector2D::Distance(LockOnCenter, EnemyScreenPosition);
		bool ConditionB = DistanceToCenter <= LockOnRadius;
		
		if (ConditionA && ConditionB)
		{
			
			DoLockOnOnce.Execute([&]() {
				OnLockOnable.Broadcast();
				DoEnemyLostOnce.Reset();
				});
		}
		else
		{
			DoEnemyLostOnce.Execute([&]() {
				OnEnemyLost.Broadcast();
				DoLockOnOnce.Reset();
				});
		}
	}

}