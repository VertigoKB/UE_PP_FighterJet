// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterPawn.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFighterPawn::AFighterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimitiveRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	SetRootComponent(PrimitiveRoot);

	//BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//SetRootComponent(BoxCollision);
	//BoxCollision->SetBoxExtent(FVector(950.f, 70.f, 60.f));
	//BoxCollisionInit(BoxCollision);

	FrontRandingGearBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontRandingGearBox"));
	FrontRandingGearBox->SetupAttachment(RootComponent);
	FrontRandingGearBox->SetBoxExtent(FVector(27.f, 10.f, 54.f));
	FrontRandingGearBox->SetRelativeLocation(FVector(300.f, 0.f, -117.f));
	BoxCollisionInit(FrontRandingGearBox);

	RearRightRandingGearBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RearRightRandingGearBox"));
	RearRightRandingGearBox->SetupAttachment(RootComponent);
	RearRightRandingGearBox->SetBoxExtent(FVector(45.f, 14.f, 56.f));
	RearRightRandingGearBox->SetRelativeLocation(FVector(-237.f, 141.f, -117.f));
	BoxCollisionInit(RearRightRandingGearBox);

	RearLeftRandingGearBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RearLeftRandingGearBox"));
	RearLeftRandingGearBox->SetupAttachment(RootComponent);
	RearLeftRandingGearBox->SetBoxExtent(FVector(45.f, 14.f, 56.f));
	RearLeftRandingGearBox->SetRelativeLocation(FVector(-237.f, -141.f, -117.f));
	BoxCollisionInit(RearLeftRandingGearBox);

	FighterMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("FighterMovement"));

	//FighterMesh->SetupAttachment(RootComponent);
	//FighterMesh->SetRelativeLocation(FVector(-20.f, 0.f, -170.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeLocation(FVector(-1600.f, 0.f, 570.f));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 100.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AFighterPawn::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Capsule is Root: %s"), (FighterMesh() == GetRootComponent()) ? TEXT("True") : TEXT("False"));
	//UE_LOG(LogTemp, Warning, TEXT("FighterMesh Physics: %s"), FighterMesh->IsSimulatingPhysics() ? TEXT("True") : TEXT("False"));
	//UE_LOG(LogTemp, Warning, TEXT("BoxCollision Physics: %s"), BoxCollision->IsSimulatingPhysics() ? TEXT("True") : TEXT("False"));
}

void AFighterPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFighterPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
// Called every frame
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * 100000.f;
	FighterMovement->AddImpulse(Force);
}


//RegacyCode
/*
void AFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AUserController* OwnerController = GetController<AUserController>();
	float CtrlerThrottle = OwnerController->GetThrottle();
	float ScalelizeThrottle = CtrlerThrottle * (1 / MaxThrottle);
	AddMovementInput(GetActorForwardVector(), ScalelizeThrottle);
}


*/