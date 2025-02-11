// Fill out your copyright notice in the Description page of Project Settings.


#include "CutSceneTrigger.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Player/F15Pawn.h"
#include "Player/UserController.h"

// Sets default values
ACutSceneTrigger::ACutSceneTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Root);

	bIsSpatiallyLoaded = false;
}

// Called when the game starts or when spawned
void ACutSceneTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AF15Pawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerCtrler = Cast<AUserController>(Player->GetController());

	
	if (Player)
	{
		FTimerHandle Temp;
		GetWorldTimerManager().SetTimer(Temp, FTimerDelegate::CreateLambda([this]() {
			Player->ThrustSpeed = 10000.f;
			Player->CurrentSpeed = 10000.f;
			FVector PlayerEntryLocation = FVector(StageEntry.X - EntryDistance, StageEntry.Y, StageEntry.Z);
			Player->SetActorLocation(PlayerEntryLocation);
			PlayerCtrler->SetViewTarget(this);
			}), 0.05, false);
	}

	GetWorldTimerManager().SetTimer(CameraTimer, this, &ACutSceneTrigger::CameraBlend, StartBlendDelay, false);
}

void ACutSceneTrigger::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SetActorLocationAndRotation(StageEntry, CameraRotation.Quaternion());
}

void ACutSceneTrigger::CameraBlend()
{
	Player->CameraChange();
	PlayerCtrler->SetViewTargetWithBlend(Player, BlendTime);
}

// Called every frame
void ACutSceneTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

