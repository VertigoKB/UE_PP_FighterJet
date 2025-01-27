// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ActorStatus.h"
#include "PoolingObject.generated.h"

UCLASS()
class GAME5_API APoolingObject : public AActor, public IActorStatus
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnActivate();
	UFUNCTION(BlueprintNativeEvent)
	void OnDeactivate();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
