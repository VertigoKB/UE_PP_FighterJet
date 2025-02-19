// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFinder.h"
#include "EnemySu33Pawn.h"
#include "EnemyPlaneController.h"

// Sets default values for this component's properties
UPlayerFinder::UPlayerFinder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerFinder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (Initialize())
	{
		World->GetTimerManager().SetTimer(RelativePositionCheckTimer, FTimerDelegate::CreateLambda([this]() {
			CompOwner->Decision = GetPlayerPositionRelativeToCompOwner();
			DistanceToTarget = CompOwner->GetDistanceTo(Target);
			}), 0.1f, true);
	}
}


// Called every frame
//void UPlayerFinder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

bool UPlayerFinder::Initialize()
{
	World = GetWorld();
	if (!World)
		return false;

	CompOwner = Cast<AEnemySu33Pawn>(GetOwner());
	if (!CompOwner)
		return false;

	AIController = Cast<AEnemyPlaneController>(CompOwner->GetController());
	if (!AIController)
		return false;

	Target = World->GetFirstPlayerController()->GetPawn();
	if (!Target)
		return false;

	return true;
}

FPlayerRelativePosition UPlayerFinder::GetPlayerPositionRelativeToCompOwner()
{
	FPlayerRelativePosition Result;

	if (!Target || !CompOwner) return Result;

	FVector OwnerLocation = CompOwner->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FVector DirectionToPlayer = TargetLocation - OwnerLocation;
	DirectionToPlayer.Normalize();
	FVector ForwardVector = CompOwner->GetActorForwardVector();
	FVector UpVector = CompOwner->GetActorUpVector();
	FVector RightVector = CompOwner->GetActorRightVector();

	float DotForward = ForwardVector.Dot(DirectionToPlayer);
	float DotUp = UpVector.Dot(DirectionToPlayer);
	float DotRight = RightVector.Dot(DirectionToPlayer);

	float CosAngle = FMath::Cos(FMath::DegreesToRadians(ViewAngle));

	Result.bIsInFront = DotForward > 0.f;
	Result.bIsAbove = DotUp > 0.f;
	Result.bIsRight = DotRight > 0.f;
	
	//Result.bIsForwardInsight = (DotForward > CosAngle);
	//Result.bIsAboveInsight = (-CosAngle < DotUp ) && (DotUp > CosAngle);
	//Result.bIsLeftRightInsight = (-CosAngle < DotRight) && (DotRight > CosAngle);
	//
	//Result.bIsInRange = DistanceToTarget <= EffectiveRange;
	Result.bIsTooClose = DistanceToTarget <= TooCloseRange;

	//Result.bIsInSight = (Result.bIsInFront && Result.bIsAboveInsight && Result.bIsLeftRightInsight);

	return Result;
}
