// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterPawn.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UFloatingPawnMovement;
// Sets default values
AFighterPawn::AFighterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FighterMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	FighterMovementComponent->MaxSpeed = JetMaxSpeed;
	FighterMovementComponent->Acceleration = 5000.f;

	FighterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	FighterMesh->SetupAttachment(RootComponent);
	FighterMesh->SetSimulatePhysics(true);

	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (FighterMesh != nullptr &&
		FighterMesh->SkeletalMesh != nullptr)
	{
		HitBox->SetupAttachment(FighterMesh);
		HitBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
		HitBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECR_Block);
	}
}

// Called when the game starts or when spawned
void AFighterPawn::BeginPlay()
{
	Super::BeginPlay();
}
/*
UPawnMovementComponent* AFighterPawn::GetMovementComponent() const
{
	return FighterMovementComponent;
}
*/
// Called every frame
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

