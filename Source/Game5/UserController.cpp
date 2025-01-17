// Fill out your copyright notice in the Description page of Project Settings.

#include "UserController.h"

//Engine
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

//Custom
#include "FighterPawn.h"



AUserController::AUserController()
{
	PrimaryActorTick.bCanEverTick = true; 

	const ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Game/Blueprints/Input/IMC_Game5InputMapping.IMC_Game5InputMapping"));
	if (IMC.Succeeded())
		Game5InputMapping = IMC.Object;

	const ConstructorHelpers::FObjectFinder<UInputDataAsset> IDA(TEXT("/Game/Blueprints/Input/InputDataBind.InputDataBind"));
	if (IDA.Succeeded())
		InputActions = IDA.Object;


	//Temp, For Finding Fighter
		

	bShowMouseCursor = false;
}

void AUserController::BeginPlay()
{
	Super::BeginPlay();
}

void AUserController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//AllocatedPawn = Cast<AFighterPawn>(GetPawn());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(Game5InputMapping, 0);

	UEnhancedInputComponent* UserInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (UserInput)
	{
		UserInput->BindAction(InputActions->Throttle, ETriggerEvent::Triggered, this, &AUserController::ThrottleCtrl);
		UserInput->BindAction(InputActions->NoseYaw, ETriggerEvent::Triggered, this, &AUserController::YawCtrl);
		UserInput->BindAction(InputActions->NosePitch, ETriggerEvent::Triggered, this, &AUserController::PitchCtrl);
		UserInput->BindAction(InputActions->NoseRoll, ETriggerEvent::Triggered, this, &AUserController::RollCtrl);
	}

	FTimerHandle TempTimer;

	GetWorld()->GetTimerManager().SetTimer(TempTimer, FTimerDelegate::CreateLambda([&]() {
		SetViewTargetWithBlend(Cast<AFighterPawn>(GetPawn()), 3.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
		}), 3.0f, false);
}

void AUserController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AUserController::ThrottleCtrl(const FInputActionValue& Value)
{
	float ThrottleAxis = Value.Get<float>();

	//if (!AllocatedPawn)
	//	return;

	FVector Forward = GetPawn()->GetActorForwardVector();

	ThrottleOutput += ThrottleAxis;
	GetPawn()->AddMovementInput(Forward, ThrottleAxis);
	//UE_LOG(LogTemp, Warning, TEXT("ForwardVector : {%f,%f,%f} / AUserController::ThrottleCtrl"), Forward.X, Forward.Y, Forward.Z )
}

void AUserController::YawCtrl(const FInputActionValue& Value)
{
	if (!OnGround)
	{
		float NoseOutput = Value.Get<float>();

		FVector CurrentLocation = GetPawn()->GetActorLocation();
		FRotator NewRotation = GetPawn()->GetActorRotation();

		NewRotation.Yaw += NoseOutput;
		NewRotation.Yaw = RotatorClear(NewRotation.Yaw);
		//GetPawn()->SetActorRotation(NewRotation);
		GetPawn()->SetActorLocationAndRotation(CurrentLocation, NewRotation, true);
	}
}
void AUserController::PitchCtrl(const FInputActionValue& Value)
{
	if (!OnGround)
	{
		float NoseOutput = Value.Get<float>();

		FVector CurrentLocation = GetPawn()->GetActorLocation();

		FQuat CurrentRotation = GetPawn()->GetActorRotation().Quaternion();
		FQuat PitchRotation = FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(NoseOutput));
		FQuat NewRotation = CurrentRotation * PitchRotation;

		GetPawn()->SetActorLocationAndRotation(CurrentLocation, NewRotation, true);
	}
}
void AUserController::RollCtrl(const FInputActionValue& Value)
{
	if (!OnGround)
	{
		float NoseOutput = Value.Get<float>();

		FVector CurrentLocation = GetPawn()->GetActorLocation();
		FRotator NewRotation = GetPawn()->GetActorRotation();

		NewRotation.Roll += NoseOutput;
		NewRotation.Roll = RotatorClear(NewRotation.Roll);

		GetPawn()->SetActorLocationAndRotation(CurrentLocation, NewRotation, true);
	}
}

float AUserController::RotatorClear(float Axis)
{
	if (Axis > 360.f || Axis < -360.f)
		return 0.f;
	else
		return Axis;
}

void AUserController::AfterBunnerSpeed()
{
}

//regacy code
/*

void AUserController::YawCtrl(const FInputActionValue& Value)
{
	float NoseOutput = Value.Get<float>();
	FRotator DeltaRotation = FRotator(0, NoseOutput, 0);
	GetPawn()->AddActorWorldRotation(DeltaRotation);
	//SetControlRotation(DeltaRotation);
}
void AUserController::PitchCtrl(const FInputActionValue& Value)
{
	float NoseOutput = Value.Get<float>();
	FRotator DeltaRotation = FRotator(NoseOutput, 0, 0);
	GetPawn()->AddActorWorldRotation(DeltaRotation);
	//SetControlRotation(DeltaRotation);
}
void AUserController::RollCtrl(const FInputActionValue& Value)
{
	float NoseOutput = Value.Get<float>();
	FRotator DeltaRotation = FRotator(0, 0, NoseOutput);
	GetPawn()->AddActorWorldRotation(DeltaRotation);
	//SetControlRotation(DeltaRotation);
}
*/