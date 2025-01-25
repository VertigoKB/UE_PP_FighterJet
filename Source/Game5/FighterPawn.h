// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Default

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/BoxComponent.h"

#include "PlaneCharacter.generated.h"

#include "FighterPawn.generated.h"

UCLASS()
class GAME5_API AFighterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFighterPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PrimitiveComponent")
	class UPrimitiveComponent* PrimitiveRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* FighterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FighterMovement")
	class UCharacterMovementComponent* FighterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
	UBoxComponent* FrontRandingGearBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
	UBoxComponent* RearLeftRandingGearBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxComponent")
	UBoxComponent* RearRightRandingGearBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void OnConstruction(const FTransform& Transform) override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	//Inlined Function

	inline void BoxCollisionInit(UBoxComponent* ParamCollision) {
		ParamCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ParamCollision->SetSimulatePhysics(true);
		ParamCollision->SetCollisionObjectType(ECC_Pawn);
		ParamCollision->SetCollisionResponseToAllChannels(ECR_Block);
	};
};

