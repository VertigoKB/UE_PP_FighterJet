// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondCutSceneTrigger.h"
#include "Components/BoxComponent.h"
#include "../Player/F15Pawn.h"
#include "../Player/FighterRotateComponent.h"

// Sets default values
ASecondCutSceneTrigger::ASecondCutSceneTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PassingbyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PssingbyCollision"));
	PassingbyCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

// Called when the game starts or when spawned
void ASecondCutSceneTrigger::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	FindPlayer();
	SceneATakeOffByTimer();
}

// Called every frame
void ASecondCutSceneTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn->ThrustParam < PlayerPawn->MaxThrustSpeed)
		PlayerPawn->ThrustParam += 100.f;
	
	if (bPitchUpTrigger)
		PlayerRotateComp->PitchValue = 1.f;
	else
		PlayerRotateComp->PitchValue = 0.f;
	
}

void ASecondCutSceneTrigger::SceneATakeOffByTimer()
{
	FTimerHandle TakeOffTimer;
	GetWorldTimerManager().SetTimer(TakeOffTimer, FTimerDelegate::CreateLambda([&]() {

		PlayerPawn->ThrustSpeed = PlayerPawn->MaxThrustSpeed * TakeOffSpeedRatio;
		PlayerPawn->CurrentSpeed = PlayerPawn->MaxThrustSpeed * TakeOffSpeedRatio;
		GetWorldTimerManager().SetTimer(SceneBTimer, this, &ASecondCutSceneTrigger::SceneBPitchUp, SceneBPitchUpDelay, false);

		}), SceneATakeOffDelay, false);
}

void ASecondCutSceneTrigger::SceneBPitchUp()
{
	bPitchUpTrigger = true;
	FTimerHandle PitchEnd;
	GetWorldTimerManager().SetTimer(PitchEnd, FTimerDelegate::CreateLambda([&]() {
		bPitchUpTrigger = false;
		}), 0.3f, false);
}

void ASecondCutSceneTrigger::FindPlayer()
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
		}), 0.05, false);

}

bool ASecondCutSceneTrigger::CachAndInitialize()
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

	
	return true;
}