// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFinder.h"
#include "EnemySu33Pawn.h"

// Sets default values for this component's properties
UPlayerFinder::UPlayerFinder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerFinder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	bComponentFlag = Initialize();
}


// Called every frame
void UPlayerFinder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPlayerFinder::Initialize()
{
	World = GetWorld();
	if (!World)
		return false;

	CompOwner = Cast<AEnemySu33Pawn>(GetOwner());
	if (!CompOwner)
		return false;

	Target = World->GetFirstPlayerController()->GetPawn();
	if (!Target)
		return false;

	return true;
}

