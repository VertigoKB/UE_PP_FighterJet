// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Default
#include "CoreMinimal.h"

//Component
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
//Engine
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"



#include "FighterPawn.generated.h"


class UPawnMovementComponent;
UCLASS()
class GAME5_API AFighterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFighterPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Value")
	float JetMaxSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Value")
	USkeletalMeshComponent* FighterMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Value")
	UBoxComponent* HitBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* FighterMovementComponent;


	
	//virtual UPawnMovementComponent* GetMovementComponent() const override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
