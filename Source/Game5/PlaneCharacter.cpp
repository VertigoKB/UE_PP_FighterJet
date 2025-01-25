// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlaneCharacter::APlaneCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetBoxExtent(FVector(950.f, 70.f, 60.f));
	BoxCollisionInit(BoxCollision);
	
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




	GetArrowComponent()->SetupAttachment(BoxCollision);

	if (GetCharacterMovement())
	{
		MovementComponent = GetCharacterMovement();
		//MovementComponent->UpdatedComponent = BoxCollision;
		//MovementComponent->NavAgentProps = FNavAgentProperties();
		//MovementComponent->NavAgentProps.AgentRadius = BoxCollision->GetScaledBoxExtent().X;
		//MovementComponent->NavAgentProps.AgentHeight = BoxCollision->GetScaledBoxExtent().Z;
		//MovementComponent->MovementMode = MOVE_Walking;
		//MovementComponent->MaxFlySpeed = 800.f;
		//MovementComponent->AirControl = 1.f;
	}

	FighterMesh = GetMesh();
	FighterMesh->SetupAttachment(RootComponent);
	FighterMesh->SetRelativeLocation(FVector(-20.f, 0.f, -170.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeLocation(FVector(-1600.f, 0.f, 570.f));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 100.f);
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->CameraRotationLagSpeed = 5.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APlaneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//UE_LOG(LogTemp, Warning, TEXT("Capsule is Root: %s"), (FighterMesh() == GetRootComponent()) ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Warning, TEXT("FighterMesh Physics: %s"), FighterMesh->IsSimulatingPhysics() ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Warning, TEXT("BoxCollision Physics: %s"), BoxCollision->IsSimulatingPhysics() ? TEXT("True") : TEXT("False"));
}

void APlaneCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlaneCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
// Called every frame
void APlaneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Forward = GetActorForwardVector();
	FVector Force = Forward * 1000.f;
	//MovementComponent->AddForce(Force);
	//AddMovementInput(Forward, 100.f);
	GetCharacterMovement()->AddForce(Force);
}

// Called to bind functionality to input
void APlaneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

