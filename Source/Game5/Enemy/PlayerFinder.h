// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerFinder.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API UPlayerFinder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFinder();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle RelativePositionCheckTimer;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	bool Initialize();


protected:
	UPROPERTY()
	TObjectPtr<class AEnemySu33Pawn> CompOwner;

	UPROPERTY()
	TObjectPtr<class AEnemyPlaneController> AIController;
	
	UPROPERTY()
	TObjectPtr<class UWorld> World;

	UPROPERTY()
	TObjectPtr<AActor> Target;

protected:
	UFUNCTION()
	struct FPlayerRelativePosition GetPlayerPositionRelativeToCompOwner();

	UPROPERTY()
	float ViewAngle = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceToTarget = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float EffectiveRange = 15000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TooCloseRange = 2500.f;
		
};
