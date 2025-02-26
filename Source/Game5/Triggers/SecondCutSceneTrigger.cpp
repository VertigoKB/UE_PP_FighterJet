// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondCutSceneTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/F15Pawn.h"
#include "../UI/PlayerHUD.h"
#include "../UI/BlackWidget.h"
#include "../Enemy/EnemySu33Pawn.h"
#include "../Enemy/EnemyPlaneController.h"
#include "../Enemy/EnemyPositionUpdater.h"
#include "../CutScene/BattleFieldLevelSequenceActor.h"
#include "../CutScene/CameraActors/BattleFieldCineCamera.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"

// Sets default values
ASecondCutSceneTrigger::ASecondCutSceneTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;



	bIsSpatiallyLoaded = false;
	
}

// Called when the game starts or when spawned
void ASecondCutSceneTrigger::BeginPlay()
{
	Super::BeginPlay();
	SettingActors();
}

void ASecondCutSceneTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

// Called every frame
void ASecondCutSceneTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASecondCutSceneTrigger::SettingActors()
{
	FTimerHandle InitTimer;
	GetWorldTimerManager().SetTimer(InitTimer, FTimerDelegate::CreateLambda([&]() {
		bInitFlag = CachAndInitialize(); 
		
		if (bInitFlag)
		{
			PlayerPawn->PlayerState = EPlayerState::CutScene;
			PlayerPawn->ThrustParam = PlayerPawn->MaxThrustSpeed;

			PlayerPawn->RequestRetractWheel();

			EnemyController->State = EEnemyState::None;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CachAndInitialize Failed &ASecondCutSceneTrigger::BeginPlay"));
		}
		PlayerHud->GeneratedBlackWidget->PlayFadeIn();
		SceneAPlayerSequence();
		}), 0.05, false);
}

bool ASecondCutSceneTrigger::CachAndInitialize()
{ 
	APawn* GottenPlayerFromWorld = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (GottenPlayerFromWorld)
		PlayerPawn = Cast<AF15Pawn>(GottenPlayerFromWorld);
	if (!PlayerPawn)
		return false;

	APlayerController* TheController = Cast<APlayerController>(PlayerPawn->GetController());
	if (TheController)
		FirstController = TheController;
	if (!FirstController)
		return false;

	AHUD* WorldHud = FirstController->GetHUD();
	if (WorldHud)
		PlayerHud = Cast<APlayerHUD>(WorldHud);
	if (!PlayerHud)
		return false;

	TArray<AActor*> EnemyPawnArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySu33Pawn::StaticClass(), EnemyPawnArray);
	if (EnemyPawnArray.Num() > 0)
	{
		AActor* EnemyActor = EnemyPawnArray[0];
		EnemyPawn = Cast<AEnemySu33Pawn>(EnemyActor);
	}
	if (!EnemyPawn)
		return false;

	AController* RawEnemyController = EnemyPawn->GetController();
	if (RawEnemyController)
		EnemyController = Cast<AEnemyPlaneController>(RawEnemyController);
	if (!EnemyController)
		return false;

	UActorComponent* GottenComp = EnemyPawn->GetComponentByClass(UEnemyPositionUpdater::StaticClass());
	EnemyPositionUpdater = Cast<UEnemyPositionUpdater>(GottenComp);
	if (!EnemyPositionUpdater)
		return false;

	TArray<AActor*> CameraFinder;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleFieldCineCamera::StaticClass(), CameraFinder);
	if (CameraFinder.Num() > 0)
		SequenceCamera = CameraFinder[0];
	if (!SequenceCamera)
		return false;

	TArray<AActor*> SequenceFinder;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleFieldLevelSequenceActor::StaticClass(), SequenceFinder);
	if (SequenceFinder.Num() > 0)
	{
		ABattleFieldLevelSequenceActor* MySequenceActor = Cast<ABattleFieldLevelSequenceActor>(SequenceFinder[0]);
		if (MySequenceActor)
			SequenceExecuter = MySequenceActor;
	}
	if (!SequenceExecuter)
		return false;

	return true;
}

void ASecondCutSceneTrigger::SceneAPlayerSequence()
{
	FirstController->SetViewTarget(SequenceCamera);
	ULevelSequencePlayer* MyLevelSequencePlayer = SequenceExecuter->GetSequencePlayer();
	ULevelSequence* MyLevelSequence = SequenceExecuter->LevelSequenceAsset;
	FFrameRate FrameRate = MyLevelSequence->GetMovieScene()->GetTickResolution();
	FFrameTime TotalDuration = MyLevelSequence->GetMovieScene()->GetPlaybackRange().Size<FFrameTime>();

	float DurationSeconds = TotalDuration / FrameRate;

	FTimerHandle ViewChanger;
	GetWorldTimerManager().SetTimer(ViewChanger, FTimerDelegate::CreateLambda([&]() {
		FirstController->SetViewTargetWithBlend(PlayerPawn, 1.f);
		GameStart();
		}), (DurationSeconds - 0.05f), false);
}

void ASecondCutSceneTrigger::GameStart()
{
	PlayerPawn->PlayerState = EPlayerState::None;
	PlayerPawn->ThrustSpeed = PlayerPawn->MaxThrustSpeed;
	PlayerPawn->CurrentSpeed = PlayerPawn->MaxThrustSpeed;

	EnemyController->State = EEnemyState::Stabilize;
	EnemyPositionUpdater->ThrustSpeed = EnemyPositionUpdater->MaxThrustSpeed;
	EnemyPositionUpdater->CurrentSpeed = EnemyPositionUpdater->MaxThrustSpeed;
}
