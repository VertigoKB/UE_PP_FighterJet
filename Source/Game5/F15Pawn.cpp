// Fill out your copyright notice in the Description page of Project Settings.


#include "F15Pawn.h"
//Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//Components
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
//Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

//EnhancedInput
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"



//Custom
#include "InputDataAsset.h"

// Sets default values
AF15Pawn::AF15Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	SetRootComponent(BoxRoot);
	BoxRoot->SetBoxExtent(FVector(950.f, 70.f, 60.f));
	BoxRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxRoot->SetCollisionObjectType(ECC_Pawn);
	BoxRoot->SetCollisionResponseToAllChannels(ECR_Block);

	ModelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	ModelMesh->SetupAttachment(BoxRoot);

	//InputMappingContext
	const ConstructorHelpers::FObjectFinder<UInputMappingContext> FindMapping(TEXT("/Game/Blueprints/Input/IMC_Game5InputMapping.IMC_Game5InputMapping"));
	if (FindMapping.Succeeded())
		AirCraftInputMapping = FindMapping.Object;
	const ConstructorHelpers::FObjectFinder<UInputDataAsset> FindInputAction(TEXT("/Game/Blueprints/Input/InputDataBind.InputDataBind"));
	if (FindInputAction.Succeeded())
		InputAction = FindInputAction.Object;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeLocation(FVector(-1600.f, 0.f, 570.f));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 100.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AF15Pawn::BeginPlay()
{
	Super::BeginPlay();

	ThrustSpeed = MinThrustToNotFall;
	CurrentSpeed = MinThrustToNotFall;
}

// Called every frame
void AF15Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePosition(DeltaTime);

	if (RudderScale > 0.f)
		RudderScale -= DeltaTime;
	if (FlapsScale > 0.f)
		FlapsScale -= DeltaTime;
	if (AileronScale > 0.f)
		AileronScale -= DeltaTime;


	if (RudderScale < 0.f)
		RudderScale += DeltaTime;
	if (FlapsScale < 0.f)
		FlapsScale += DeltaTime;
	if (AileronScale < 0.f)
		AileronScale += DeltaTime;
}

// Called to bind functionality to input
void AF15Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstLocalPlayerFromController());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(AirCraftInputMapping, 0);
	UEnhancedInputComponent* AircraftInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (AircraftInput)
	{
		AircraftInput->BindAction(InputAction->Throttle, ETriggerEvent::Triggered, this, &AF15Pawn::ThrottleInput);
		AircraftInput->BindAction(InputAction->NoseYaw, ETriggerEvent::Triggered, this, &AF15Pawn::YawInput);
		AircraftInput->BindAction(InputAction->NosePitch, ETriggerEvent::Triggered, this, &AF15Pawn::PitchInput);
		AircraftInput->BindAction(InputAction->NoseRoll, ETriggerEvent::Triggered, this, &AF15Pawn::RollInput);
	}

}
void AF15Pawn::ThrottleInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	float CalcPosition = (Input * (GetWorld()->GetDeltaSeconds()) * ThrustMultiplier) + ThrustSpeed;

	ThrustSpeed = FMath::Clamp(CalcPosition, 0.f, MaxThrustSpeed);
}
void AF15Pawn::YawInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	UpdateYaw(GetWorld()->GetDeltaSeconds(), Input);
}
void AF15Pawn::PitchInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	UpdatePitch(GetWorld()->GetDeltaSeconds(), Input);
}
void AF15Pawn::RollInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	UpdateRoll(GetWorld()->GetDeltaSeconds(), Input);
}

//Update PawnTransform
void AF15Pawn::UpdatePosition(float DeltaSeconds)
{
	if (ThrustSpeed < CurrentSpeed)
		CurrentSpeed = FMath::FInterpTo(CurrentSpeed, ThrustSpeed, DeltaSeconds, Drag);
	else
		CurrentSpeed = ThrustSpeed;

	FVector NewPosition = (CurrentSpeed * DeltaSeconds) * GetActorForwardVector();

	AppliedGravity = UKismetMathLibrary::MapRangeClamped(CurrentSpeed, 0.f, MinThrustToNotFall, Gravity, 0.f);

	float FallingScale = NewPosition.Z - (AppliedGravity * DeltaSeconds);
	NewPosition = FVector(NewPosition.X, NewPosition.Y, FallingScale);

	AddActorWorldOffset(NewPosition);
}

void AF15Pawn::UpdateYaw(float DeltaSeconds, float Yaw)
{
	TargetYaw = Yaw;
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaSeconds, 10.f);

	RudderScale = FMath::FInterpTo(0.f, CurrentYaw , DeltaSeconds, 15.f);
	
	FRotator NewRotation = FRotator(0.f, (CurrentYaw * DeltaSeconds * 80.f), 0.f);
	AddActorLocalRotation(NewRotation);
}

void AF15Pawn::UpdatePitch(float DeltaSeconds, float Pitch)
{
	TargetPitch = Pitch;
	CurrentPitch = FMath::FInterpTo(CurrentPitch, TargetPitch, DeltaSeconds, 10.f);

	FlapsScale = FMath::FInterpTo(0.f, CurrentPitch, DeltaSeconds, 15.f);
	StabilizersScale = CurrentPitch;

	FRotator NewRotation = FRotator((CurrentPitch * DeltaSeconds * 80.f), 0.f, 0.f);
	AddActorLocalRotation(NewRotation);
}

void AF15Pawn::UpdateRoll(float DeltaSeconds, float Roll)
{
	TargetRoll = Roll;
	CurrentRoll = FMath::FInterpTo(CurrentRoll, TargetRoll, DeltaSeconds, 10.f);

	AileronScale = FMath::FInterpTo(0.f, CurrentRoll, DeltaSeconds, 15.f);

	FRotator NewRotation = FRotator(0.f, 0.f, (CurrentRoll * DeltaSeconds * 80.f));
	AddActorLocalRotation(NewRotation);
}