// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstCutSceneTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "../Player/F15Pawn.h"
#include "../Player/FighterRotateComponent.h"
#include "../UI/PlayerHUD.h"
#include "../UI/BlackWidget.h"

// Sets default values
AFirstCutSceneTrigger::AFirstCutSceneTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PassingbyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PssingbyCollision"));
	PassingbyCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


	bIsSpatiallyLoaded = false;
	
}

// Called when the game starts or when spawned
void AFirstCutSceneTrigger::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	//Set player's view target is processed in FighterGameMode::OnPostLogin		(Bad structure)
	FindPlayer();
}

void AFirstCutSceneTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SceneBTimer);
	GetWorldTimerManager().ClearTimer(SceneBOpenNextLevel);
	SceneBTimer.Invalidate();
	SceneBOpenNextLevel.Invalidate();
}

// Called every frame
void AFirstCutSceneTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn->ThrustParam < PlayerPawn->MaxThrustSpeed)
		PlayerPawn->ThrustParam += 100.f;
	
	if (bPitchUpTrigger)
		PlayerRotateComp->PitchValue = 1.f;
	else
		PlayerRotateComp->PitchValue = 0.f;
	
}

void AFirstCutSceneTrigger::SceneATakeOffByTimer()
{
	FTimerHandle TakeOffTimer;
	GetWorldTimerManager().SetTimer(TakeOffTimer, FTimerDelegate::CreateLambda([&]() {

		PlayerPawn->ThrustSpeed = PlayerPawn->MaxThrustSpeed * TakeOffSpeedRatio;
		PlayerPawn->CurrentSpeed = PlayerPawn->MaxThrustSpeed * TakeOffSpeedRatio;
		GetWorldTimerManager().SetTimer(SceneBTimer, this, &AFirstCutSceneTrigger::SceneBPitchUp, SceneBPitchUpDelay, false);

		}), SceneATakeOffDelay, false);
}

void AFirstCutSceneTrigger::SceneBPitchUp()
{
	bPitchUpTrigger = true;
	FTimerHandle PitchEnd;
	GetWorldTimerManager().SetTimer(PitchEnd, FTimerDelegate::CreateLambda([&]() {

		bPitchUpTrigger = false;
		
		GetWorldTimerManager().SetTimer(SceneBOpenNextLevel, this, &AFirstCutSceneTrigger::MoveToNextLevel, 4.f, false);

		}), 0.3f, false);
}

void AFirstCutSceneTrigger::MoveToNextLevel()
{
	float FadeOutEndTime = PlayerHud->GeneratedBlackWidget->FadeInEndTime;
	PlayerHud->GeneratedBlackWidget->PlayFadeOut();
	FTimerHandle FadeOutTimer;
	GetWorldTimerManager().SetTimer(FadeOutTimer, FTimerDelegate::CreateLambda([&]() {
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("BattleField")));
		}), FadeOutEndTime, false);
}

void AFirstCutSceneTrigger::FindPlayer()
{
	FTimerHandle InitTimer;
	GetWorldTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([&]() {
		bInitFlag = CachAndInitialize(); 
		
		if (bInitFlag)
		{
			bSceneTrigger = true;
			WaittingLocation = PlayerPawn->GetActorLocation();
			PlayerPawn->PlayerState = EPlayerState::CutScene;
			SetActorTickEnabled(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CachAndInitialize Failed &ASecondCutSceneTrigger::BeginPlay"));
		}

		SceneATakeOffByTimer();
		PlayerHud->GeneratedBlackWidget->PlayFadeIn();
		}), 0.05, false);

}

bool AFirstCutSceneTrigger::CachAndInitialize()
{
	APawn* GottenPlayerFromWorld = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (GottenPlayerFromWorld)
		PlayerPawn = Cast<AF15Pawn>(GottenPlayerFromWorld);
	if (!PlayerPawn)
		return false;

	UActorComponent* Comp = PlayerPawn->GetComponentByClass(UFighterRotateComponent::StaticClass());
	PlayerRotateComp = Cast<UFighterRotateComponent>(Comp);
	if (!PlayerRotateComp)
		return false;

	APlayerController* TheController = Cast<APlayerController>(PlayerPawn->GetController());
	if (TheController)
	{
		AHUD* WorldHud = TheController->GetHUD();
		if (WorldHud)
			PlayerHud = Cast<APlayerHUD>(WorldHud);
	}
	if (!PlayerHud)
		return false;

	//if (!NextLevel)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("NextLevel is null &ASecondCutSceneTrigger::CachAndInitialize"));
	//	return false;
	//}

	return true;
}