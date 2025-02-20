// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyComponentInterface.h"
#include "../CommonData/PlayerRelativePosition.h"
#include "EnemySu33Pawn.generated.h"

UCLASS()
class GAME5_API AEnemySu33Pawn : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	AEnemySu33Pawn();

public:
	UFUNCTION()
	void InitFalseManeuverBoolean();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPitchUp = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPitchDown = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRollLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRollRight = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bYawLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bYawRight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerRelativePosition Decision;

public:
	UPROPERTY(EditDefaultsOnly)
	float Health = 100.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshComponent")
	TObjectPtr<class USkeletalMeshComponent> EnemyMesh;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UEnemyPositionUpdater> PositionUpdater;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPlayerFinder> PlayerFinder;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<class UEnemyFSMComponent> FSMComponent;

protected:
	UPROPERTY()
	TObjectPtr<class AF15Pawn> Player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
