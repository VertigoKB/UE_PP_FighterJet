// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../CommonData/TDoOnce.h"
#include "../CommonData/PlayerRelativePosition.h"
#include "EnemyPlaneController.generated.h"

//Don't recycle this component.
//This component made for separate code.
//FSM Graph
//https://miro.com/app/board/uXjVIe2wB5I=/

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle = 0	UMETA(DisplayName = "Idle"),
	Maneuver	UMETA(DisplayName = "Maneuver"),
	Attack		UMETA(DisplayName = "Attack"),
	Stabilize	UMETA(DisplayName = "Stabilize"),
	Death		UMETA(DisplayName = "Death")
};

UCLASS()
class GAME5_API AEnemyPlaneController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyPlaneController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UFUNCTION()
	bool Initialize();

protected:
	UFUNCTION()
	void OnIdleTick();
	UFUNCTION()
	void OnStabilizeTick();
	UFUNCTION()
	void OnManeuverTick();
	UFUNCTION()
	void OnAttackTick();
	UFUNCTION()
	void OnDeath();

protected:
	UFUNCTION()
	void OnStabilizeOnce();
	UFUNCTION()
	void OnManeuverOnce();
	UFUNCTION()
	void OnAttackOnce();

protected:
	void ReceiveDelegateCall(bool* ReceiveTarget);


	UFUNCTION()
	void CompareFloat(float condA, float condB, FName CompOp);
	UFUNCTION()
	void StopManeuveringWhenLimit();


protected:

	UFUNCTION()
	void IsTooClose();
	UFUNCTION()
	void IsAbove();
	UFUNCTION()
	void IsFront();


protected:
	bool* DelegateDone = nullptr;
	float FloatConditionA = 0.f;
	float FloatConditionB = 0.f;
	FName CompOperator = NAME_None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bLockable = false;

	UPROPERTY()
	bool bRollingDone = false;
	UPROPERTY()
	bool bPullUpDone = false;
	UPROPERTY()
	bool bImmelmannTurnDone = false;
	UPROPERTY()
	bool bStabilizeDone = false;

	UPROPERTY(EditDefaultsOnly)
	float MaxManeuverTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState State = EEnemyState::Stabilize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerRelativePosition PosState;

	TDoOnce OnceManeuverNode;
	TDoOnce OnceAttackNode;
	TDoOnce OnceStabilizeNode;

	TDoOnce OnceANode;
	TDoOnce OnceBNode;
	TDoOnce OnceCNode;

	FTimerHandle RelativeLocGetter;
	FTimerHandle ManeuverTimer;
	FTimerHandle PosStateUpdater;

protected:
	UPROPERTY()
	TObjectPtr<class AEnemySu33Pawn> MyPawn;
	UPROPERTY()
	TObjectPtr<class AActor> Target;
	UPROPERTY()
	TObjectPtr<class UEnemyPositionUpdater> PositionUpdater;

	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> Intelligence;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

private:
	inline void InitFalseDelegateBoolean() {
		bRollingDone = false;
		bPullUpDone = false;
		bImmelmannTurnDone = false;
		bStabilizeDone = false;
	}

	inline void SetMaxManeuverTimer() {
		GetWorld()->GetTimerManager().SetTimer(ManeuverTimer, FTimerDelegate::CreateLambda([this]() {
			State = EEnemyState::Stabilize;
			}), MaxManeuverTime, false);
	}

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bDebugBoolean = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName MyDebugName = NAME_None;
};