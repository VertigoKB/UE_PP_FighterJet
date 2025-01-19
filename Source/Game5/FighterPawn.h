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

//Custom
#include "Interfaces/Throttle.h"


#include "FighterPawn.generated.h"


class UPawnMovementComponent;
UCLASS()
class GAME5_API AFighterPawn : public APawn, public IThrottle
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFighterPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Value")
	USkeletalMeshComponent* FighterMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Value")
	UBoxComponent* HitBox;


	//Custom Interface
	virtual void Test() override;
	virtual float GetThrottle() const override { return PawnThrottle; }
	virtual void SetThrottle(float Value) override { PawnThrottle = Value; }

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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PawnThrottle = 0.f;

	FTimerHandle TimerAfterBunner;

};
