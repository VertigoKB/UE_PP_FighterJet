// Fill out your copyright notice in the Description page of Project Settings.

#include "UserController.h"

//Engine
#include "TimerManager.h"
//#include "Kismet/GameplayStatics.h"

//Custom
#include "FighterPawn.h"



AUserController::AUserController()
{
	PrimaryActorTick.bCanEverTick = true; 

	bShowMouseCursor = false;
}

void AUserController::BeginPlay()
{
	Super::BeginPlay();
}

void AUserController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FTimerHandle TempTimer;
	/*
	GetWorld()->GetTimerManager().SetTimer(TempTimer, FTimerDelegate::CreateLambda([&]() {
		SetViewTargetWithBlend(Cast<AFighterPawn>(GetPawn()), 3.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
		}), 3.0f, false);*/
}

void AUserController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
/*
void AUserController::ThrottleCtrl(const FInputActionValue& Value)
{
	float ThrottleAxis = Value.Get<float>();

	CtrlerThrottle = FMath::Clamp(CtrlerThrottle + ThrottleAxis * 1.f, 0.f, MaxThrottle);
}

void AUserController::YawCtrl(const FInputActionValue& Value)
{
	if (!OnGround)
	{
		float NoseOutput = Value.Get<float>();

		FVector CurrentLocation = GetPawn()->GetActorLocation();
		FRotator NewRotation = GetPawn()->GetActorRotation();

		NewRotation.Yaw += NoseOutput;
		//GetPawn()->SetActorRotation(NewRotation);
		GetPawn()->SetActorLocationAndRotation(CurrentLocation, NewRotation, true);
	}
}
void AUserController::PitchCtrl(const FInputActionValue& Value)
{
	if (!OnGround)
	{
		float NoseOutput = Value.Get<float>();

		FVector Torque = FVector(NoseOutput * TorquePitchStrength, 0, 0);

		UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetPawn()->GetRootComponent());
		if (Primitive && Primitive->IsSimulatingPhysics())
			Primitive->AddTorqueInRadians(Torque, NAME_None, true);
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

void AUserController::ApplyDragForce()
{/*
	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetPawn()->GetRootComponent());
	if (Primitive)
	{
		FVector Velocity = Primitive->GetComponentVelocity();
		FVector DragForce = -Velocity.GetSafeNormal() * DragCoefficient * Velocity.SizeSquared();

		Primitive->AddForce(DragForce);
	}*/
//}
//regacy code
/*

void AUserController::ThrottleCtrl(const FInputActionValue& Value)
{
	float ThrottleAxis = Value.Get<float>();
	FVector Forward = GetPawn()->GetActorForwardVector();

	if (MaxThrottle > CtrlerThrottle)
		CtrlerThrottle += ThrottleAxis;
}



*/

//FloatingPawnMovement
/*
void AUserController::YawCtrl(const FInputActionValue& Value)
{
	float NoseOutput = Value.Get<float>();

	FVector CurrentLocation = GetPawn()->GetActorLocation();
	FRotator NewRotation = GetPawn()->GetActorRotation();

	NewRotation.Yaw += NoseOutput;
		//GetPawn()->SetActorRotation(NewRotation);
	GetPawn()->SetActorLocationAndRotation(CurrentLocation, NewRotation, true);
}
void AUserController::PitchCtrl(const FInputActionValue& Value)
{
	float NoseOutput = Value.Get<float>();

	FVector CurrentLocation = GetPawn()->GetActorLocation();

	FQuat CurrentRotation = GetPawn()->GetActorRotation().Quaternion();
	FQuat PitchRotation = FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(NoseOutput));
	FQuat NewRotation = CurrentRotation * PitchRotation;

	GetPawn()->SetActorLocationAndRotation(CurrentLocation, NewRotation, true);
}
void AUserController::RollCtrl(const FInputActionValue& Value)
{
	float NoseOutput = Value.Get<float>();

	FVector CurrentLocation = GetPawn()->GetActorLocation();
	FRotator NewRotation = GetPawn()->GetActorRotation();

	NewRotation.Roll += NoseOutput;

	GetPawn()->SetActorLocationAndRotation(CurrentLocation, NewRotation, true);
}

*/