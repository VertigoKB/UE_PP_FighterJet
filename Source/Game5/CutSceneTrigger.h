// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutSceneTrigger.generated.h"

UCLASS()
class GAME5_API ACutSceneTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACutSceneTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	class USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera = nullptr;

	UPROPERTY()
	class AF15Pawn* Player = nullptr;
	UPROPERTY()
	class AUserController* PlayerCtrler = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly)
	FVector StageEntry = FVector(-50000.f, 0.f, 50000.f);
	UPROPERTY(EditDefaultsOnly)
	FRotator CameraRotation = FRotator(0.f, -180.f, 0.f);
	UPROPERTY(EditDefaultsOnly)
	float EntryDistance = -5000.f;

protected:
	UFUNCTION()
	void CameraBlend();
	
	UPROPERTY(EditDefaultsOnly)
	float StartBlendDelay = 5.f;
	UPROPERTY(EditDefaultsOnly)
	float BlendTime = 5.f;

	FTimerHandle CameraTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
