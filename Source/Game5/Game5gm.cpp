// Fill out your copyright notice in the Description page of Project Settings.


#include "Game5gm.h"

//Engine
#include "Kismet/GameplayStatics.h"
#include "UserController.h"
#include "FighterPawn.h"

AGame5gm::AGame5gm()
{
	ConstructorHelpers::FClassFinder<APlayerController> BPusercontroller(TEXT("/Game/Blueprints/Player/BP_UserController.BP_UserController_C"));
	if (BPusercontroller.Succeeded())
		PlayerControllerClass = BPusercontroller.Class;


	ConstructorHelpers::FClassFinder<AActor> FIGHTERJET(TEXT("/Game/Blueprints/Player/BP_UserVehicle.BP_UserVehicle_C"));
	if (FIGHTERJET.Succeeded())
		BPfighter = FIGHTERJET.Class;

	ConstructorHelpers::FClassFinder<AActor> SPAWNPOINT_CARRIER(TEXT("/Game/Blueprints/Player/BP_SpawnPoint_Carrier.BP_SpawnPoint_Carrier_C"));
	if (SPAWNPOINT_CARRIER.Succeeded())
		BPspawnpointCarrier = SPAWNPOINT_CARRIER.Class;

	DefaultPawnClass = nullptr;
}

void AGame5gm::OnPostLogin(AController* NewPlayer)
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
		//AFighterPawn* tempPawn = Cast<AFighterPawn>(PlayerPawn);
		NewPlayer->Possess(PlayerPawn);
		//AUserController* TempCtrl = Cast<AUserController>(NewPlayer);
		//TempCtrl->SetViewTargetWithBlend(Cast<AActor>(tempPawn->Camera), 1.f, EViewTargetBlendFunction::VTBlend_Linear);
	//	FindPlayerStart()->GetTransform();
	}
}
