// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CutSceneSubCamera.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API ACutSceneSubCamera : public AActor
{
	GENERATED_BODY()

	ACutSceneSubCamera();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<class USceneComponent> RootComp;
	UPROPERTY()
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<class AF15Pawn> Player;

	UPROPERTY()
	FVector PlayerLocation;
	
};
