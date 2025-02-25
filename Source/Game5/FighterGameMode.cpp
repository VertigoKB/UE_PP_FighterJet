// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterGameMode.h"

//Engine
#include "Kismet/GameplayStatics.h"
#include "Player/UserController.h"
#include "CutScene/CameraActors/CameraCarrierA.h"

AFighterGameMode::AFighterGameMode()
{
	ConstructorHelpers::FClassFinder<APlayerController> BPusercontroller(TEXT("/Game/Blueprints/Player/BP_UserController.BP_UserController_C"));
	if (BPusercontroller.Succeeded())
		PlayerControllerClass = BPusercontroller.Class;


	ConstructorHelpers::FClassFinder<AActor> FIGHTERJET(TEXT("/Game/Blueprints/Player/BP_VehicleF15.BP_VehicleF15_C"));
	if (FIGHTERJET.Succeeded())
		BPfighter = FIGHTERJET.Class;

	ConstructorHelpers::FClassFinder<AActor> SPAWNPOINT_CARRIER(TEXT("/Game/Blueprints/Player/BP_SpawnPoint_Carrier.BP_SpawnPoint_Carrier_C"));
	if (SPAWNPOINT_CARRIER.Succeeded())
		BPspawnpointCarrier = SPAWNPOINT_CARRIER.Class;

	ConstructorHelpers::FClassFinder<AActor> CAMERACARRIER(TEXT("/Game/Blueprints/CutScene/CameraActors/BP_CameraCarrierA.BP_CameraCarrierA_C"));
	if (CAMERACARRIER.Succeeded())
		BPCameraCarrierA = CAMERACARRIER.Class;

	DefaultPawnClass = nullptr;
}

void AFighterGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	FVector SpawnLocation = FVector::ZeroVector;
	TArray<AActor*> FoundSpawnPoints;
	FRotator SpawnRotation = FRotator(0, 0, 0);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BPspawnpointCarrier, FoundSpawnPoints);

	if (FoundSpawnPoints.Num() > 0) {
		SpawnLocation = FoundSpawnPoints[0]->GetActorLocation();
		SpawnRotation = FoundSpawnPoints[0]->GetActorRotation();
	}


	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AActor* SpawningActor;
	if (BPfighter != nullptr)
	{
		SpawningActor = GetWorld()->SpawnActor<AActor>(BPfighter, SpawnLocation, SpawnRotation, SpawnParam);
		PlayerPawn = Cast<APawn>(SpawningActor);
		NewPlayer->Possess(PlayerPawn);
	}

	FindCarrierCamera(NewPlayer);
}

void AFighterGameMode::FindCarrierCamera(AController* NewPlayer)
{
	FTimerHandle CameraFinder;
	GetWorldTimerManager().SetTimer(CameraFinder, FTimerDelegate::CreateLambda([&]() {
		TArray<AActor*> CameraRefArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), BPCameraCarrierA, CameraRefArray);

		if (CameraRefArray.Num() > 0)
			CameraCarrierA = CameraRefArray[0];

		if (CameraCarrierA != nullptr)
		{
			APlayerController* MyController = Cast<APlayerController>(PlayerPawn->GetController());
			MyController->SetViewTarget(CameraCarrierA);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CameraCarrierA is null &AFighterGameMode::OnPostLogin"));
		}
		}), 0.5f, false);

}