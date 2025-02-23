// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySu33Pawn.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "../Player/F15Pawn.h"
#include "../Player/FighterRotateComponent.h"
#include "EnemyPositionUpdater.h"
#include "PlayerFinder.h"
#include "EnemyPlaneController.h"


// Sets default values
AEnemySu33Pawn::AEnemySu33Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComp"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(1000.f, 100.f, 100.f));
	/*
	DummyCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsCube"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
		DummyCube->SetStaticMesh(CubeAsset.Object);
	DummyCube->SetupAttachment(Root);*/

	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	EnemyMesh->SetupAttachment(Root);
	EnemyMesh->SetRelativeLocation(FVector(0.f, 0.f, -245.f));

	PositionUpdater = CreateDefaultSubobject<UEnemyPositionUpdater>(TEXT("PositionUpdater"));
	PlayerFinder = CreateDefaultSubobject<UPlayerFinder>(TEXT("PlayerFinder"));
	//FSMComponent = CreateDefaultSubobject<UEnemyFSMComponent>(TEXT("ChaseComponent"));

	AIControllerClass = AEnemyPlaneController::StaticClass();
	bIsSpatiallyLoaded = false;
}

void AEnemySu33Pawn::InitFalseManeuverBoolean()
{
	bPitchUp = false;
	bPitchDown = false;
	bRollLeft = false;
	bRollRight = false;
	bYawLeft = false;
	bYawRight = false;
}

// Called when the game starts or when spawned
void AEnemySu33Pawn::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AF15Pawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	
}

// Called every frame
void AEnemySu33Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemySu33Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

