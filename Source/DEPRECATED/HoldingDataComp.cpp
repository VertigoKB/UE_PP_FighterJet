// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldingDataComp.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "F15Pawn.h"
#include "../GameInstance/FighterGameInst.h"
#include "../RefOnly/GameChangerTarget.h"
#include "../UI/PlayerHUD.h"
#include "../UI/BlackWidget.h"

// Sets default values for this component's properties
UHoldingDataComp::UHoldingDataComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TargetClass = AGameChangerTarget::StaticClass();
	// ...

	TargetArray.SetNum(1);
}


// Called when the game starts
void UHoldingDataComp::BeginPlay()
{
	Super::BeginPlay();

	if (!Initialize())
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed To Initialize Component : UHoldingDataComp::BeginPlay")); }
		bFlag = false;
	}
	
	if (bFlag && (WorldGameInst->MoveCounter == 0))
		FindTargetWhenInit();

	if (WorldGameInst->MoveCounter == 0)
		SetUpdaterTimerWhenCountZero();
	else
	{	//It may be overridden even when the function is executed due to external factors such as the process of initializing the world.

		//DEPRECATED: This timer is disabled. Player's data will be initialized by ACutSceneTrigger
		//FTimerHandle Temp;
		//TheWorld->GetTimerManager().SetTimer(Temp, this, &UHoldingDataComp::RequestDataFromGameInst, 0.05f, false);
	}

	Hud->GeneratedBlackWidget->PlayFadeEffect(true);

	if (WorldGameInst->MoveCounter == 1)
	{
		//Player->RequestActiveCamera(false);
	}
}


// Called every frame
void UHoldingDataComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UHoldingDataComp::SetMoveCounterFromGameInst()
{
	MoveCounter = WorldGameInst->MoveCounter; 
}

int32 UHoldingDataComp::GetMoveCounterFromComp()
{
	return MoveCounter;
}

void UHoldingDataComp::RequestSetData()
{
	if (!WorldGameInst)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Faild To Load GameInstance : UHoldingDataComp::RequestSetData")); }
		return;
	}

	WorldGameInst->ThrustSpeed = Player->ThrustSpeed;
	WorldGameInst->CurrentSpeed = Player->CurrentSpeed;
	WorldGameInst->PlayerZ = Player->GetActorLocation().Z;
	WorldGameInst->PlayerRotator = Player->GetActorRotation();
}

void UHoldingDataComp::RequestDataFromGameInst()
{
	if (!WorldGameInst)
	{
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Faild To Load GameInstance : UHoldingDataComp::RequestDataFromGameInst")); }
		return;
	}

	Player->ThrustSpeed = WorldGameInst->ThrustSpeed;
	Player->CurrentSpeed = WorldGameInst->CurrentSpeed;
	
	FRotator Rotation = WorldGameInst->PlayerRotator;
	FVector Location = Player->GetActorLocation();
	Location.Z = WorldGameInst->PlayerZ;
	Player->SetActorLocationAndRotation(Location, Rotation.Quaternion());
}

bool UHoldingDataComp::Initialize()
{
	TheWorld = GetWorld();
	if (!TheWorld)
		return false;

	WorldGameInst = TheWorld->GetGameInstance<UFighterGameInst>();
	if (!WorldGameInst)
		return false;

	Player = Cast<AF15Pawn>(GetOwner());
	if (!Player)
		return false;

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	Hud = Cast<APlayerHUD>(Controller->GetHUD());
	if (!Hud)
		return false;


	return true;
}

bool UHoldingDataComp::FindTargetWhenInit()
{
	UGameplayStatics::GetAllActorsOfClass(TheWorld, TargetClass, TargetArray);
	if (TargetArray.Num() == 0)
	{
		TheWorld->GetTimerManager().SetTimer(TargetFinder, FTimerDelegate::CreateLambda([this]() {
			if (TargetArray.Num() == 0)
			{
				UGameplayStatics::GetAllActorsOfClass(TheWorld, TargetClass, TargetArray);
				if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed To Find Target : UHoldingDataComp::FindTargetWhenInit")); }
			}
			else
			{
				TheWorld->GetTimerManager().ClearTimer(TargetFinder);
				Target = Cast<AGameChangerTarget>(TargetArray[0]);
			}
			}), 0.5f, true);
	}
	else
		Target = Cast<AGameChangerTarget>(TargetArray[0]);

	return Target != nullptr;
}

void UHoldingDataComp::SetUpdaterTimerWhenCountZero()
{
	TheWorld->GetTimerManager().SetTimer(DistanceCheckUpdater, FTimerDelegate::CreateLambda([this]() {
		CurrentDistanceToTarget = Player->GetDistanceTo(Target);
		if (CurrentDistanceToTarget > MinDistanceToMoveLevel)
		{
			TheWorld->GetTimerManager().ClearTimer(DistanceCheckUpdater);
			RequestSetData();
			WorldGameInst->RequestMoveCountIncrement();
			Hud->GeneratedBlackWidget->PlayFadeEffect(false);

			TheWorld->GetTimerManager().ClearTimer(Player->HudValueExecuter);
			Player->HudValueExecuter.Invalidate();
			
			FTimerHandle WaitFadeAnimEnd;
			float EndTime = Hud->GeneratedBlackWidget->FadeInEndTime;
			TheWorld->GetTimerManager().SetTimer(WaitFadeAnimEnd, FTimerDelegate::CreateLambda([this]() {
				UGameplayStatics::OpenLevelBySoftObjectPtr(TheWorld, BattleField);
				}), EndTime, false);
		}

		}), 0.03, true);
}

