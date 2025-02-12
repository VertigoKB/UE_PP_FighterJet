// Fill out your copyright notice in the Description page of Project Settings.


#include "CutSceneSubCamera.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "../Player/F15Pawn.h"


ACutSceneSubCamera::ACutSceneSubCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsSpatiallyLoaded = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComp);
	
}

void ACutSceneSubCamera::BeginPlay()
{
	Super::BeginPlay();

	AActor* TempPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	Player = Cast<AF15Pawn>(TempPlayer);
	SetOwner(Player);
}

void ACutSceneSubCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Player)
	{
		PlayerLocation = Player->GetActorLocation();
		SetActorLocationAndRotation(FVector(PlayerLocation.X + 545.f, PlayerLocation.Y + 635.f, PlayerLocation.Z),FRotator(0.f, -120.f, 0.f).Quaternion());
	}
}
