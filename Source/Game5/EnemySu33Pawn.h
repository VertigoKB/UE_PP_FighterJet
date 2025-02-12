// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemySu33Pawn.generated.h"

UCLASS()
class GAME5_API AEnemySu33Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemySu33Pawn();
protected:
	UPROPERTY()
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> DummyCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshComponent")
	TObjectPtr<class USkeletalMeshComponent> EnemyMesh;

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
