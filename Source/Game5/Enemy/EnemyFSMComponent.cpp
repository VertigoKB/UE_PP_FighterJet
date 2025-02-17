// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSMComponent.h"
#include "EnemySu33Pawn.h"
#include "EnemyPositionUpdater.h"

// Sets default values for this component's properties
UEnemyFSMComponent::UEnemyFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Initialize())
	{
		SetComponentTickEnabled(false);
		return;
	}

	//FTimerHandle Temp;
	//World->GetTimerManager().SetTimer(Temp, FTimerDelegate::CreateLambda([this]() {
	//	PositionUpdater->bTryStablize = true;
	//	}), 5.f, true);
}


// Called every frame
void UEnemyFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



}

bool UEnemyFSMComponent::Initialize()
{
	World = GetWorld();
	if (!World)
		return false;

	CompOwner = Cast<AEnemySu33Pawn>(GetOwner());
	if (!CompOwner)
		return false;

	Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Target)
		return false;

	PositionUpdater = CompOwner->GetComponentByClass<UEnemyPositionUpdater>();
	if (!PositionUpdater)
		return false;

	PositionUpdater->RollingDone.BindLambda([this]() {
		bRoolingDone = true;
		});
	PositionUpdater->PullUpDone.BindLambda([this]() {
		bPullUpDone = true;
		});
	PositionUpdater->ImmelmannTurnDone.BindLambda([this]() {
		bImmelmannTurnDone = true;
		});
	PositionUpdater->StabilizeDone.BindLambda([this]() {
		bStabilizeDone = true;
		UE_LOG(LogTemp, Warning, TEXT("Lambda Delegate Called"));
		});

	return true;
}

void UEnemyFSMComponent::ExecuteStabilizeState()
{
	State = EEnemyState::Stabilize;

	PositionUpdater->bTryStablize = true;
	if (bStabilizeDone)
	{
		InitFalseDelegateBoolean();
		CompOwner->InitFalseManeuverBoolean();
	}

}

void UEnemyFSMComponent::ExecuteManeuverState()
{
	State = EEnemyState::Maneuver;
}
