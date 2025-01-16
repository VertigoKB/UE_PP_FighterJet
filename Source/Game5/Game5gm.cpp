// Fill out your copyright notice in the Description page of Project Settings.


#include "Game5gm.h"

//Engine
#include "Kismet/GameplayStatics.h"

AGame5gm::AGame5gm()
{
	ConstructorHelpers::FClassFinder<APlayerController> BPusercontroller(TEXT("/Game/Blueprints/Player/BP_UserController.BP_UserController_C"));
	if (BPusercontroller.Succeeded())
		PlayerControllerClass = BPusercontroller.Class;


	ConstructorHelpers::FClassFinder<AActor> FIGHTERJET(TEXT("/Game/Blueprints/Player/BP_PlayerVehicle.BP_PlayerVehicle_C"));
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BPspawnpointCarrier, FoundSpawnPoints);

	if (FoundSpawnPoints.Num() > 0)
		SpawnLocation = FoundSpawnPoints[0]->GetActorLocation();

	FRotator SpawnRotation = FRotator(0, 0, 0);

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AActor* SpawningActor;
	if (BPfighter != nullptr)
	{
		SpawningActor = GetWorld()->SpawnActor<AActor>(BPfighter, SpawnLocation, SpawnRotation, SpawnParam);
		PlayerPawn = Cast<APawn>(SpawningActor);
		NewPlayer->Possess(PlayerPawn);
	//	FindPlayerStart()->GetTransform();
	}
}
