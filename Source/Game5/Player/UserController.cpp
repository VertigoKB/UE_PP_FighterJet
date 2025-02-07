// Fill out your copyright notice in the Description page of Project Settings.

#include "UserController.h"
#include "TimerManager.h"

AUserController::AUserController()
{
	PrimaryActorTick.bCanEverTick = true; 
	bShowMouseCursor = false;
}

void AUserController::BeginPlay()
{
	Super::BeginPlay();
}

void AUserController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void AUserController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}