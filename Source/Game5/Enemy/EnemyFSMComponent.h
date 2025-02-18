// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../CommonData/TDoOnce.h"
#include "EnemyFSMComponent.generated.h"

//Don't recycle this component.
//This component made for separate code.
//FSM Graph
//https://miro.com/app/board/uXjVIe2wB5I=/

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle = 0	UMETA(DisplayName = "Idle"),
	Maneuver	UMETA(DisplayName = "Maneuver"),
	Attack		UMETA(DisplayName = "Attack"),
	Stabilize	UMETA(DisplayName = "Stabilize"),
	Death		UMETA(DisplayName = "Death")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API UEnemyFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


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

protected:

	UFUNCTION()
	void IsTooClose();


protected:
	bool* TargetDone = nullptr;

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

	EEnemyState State = EEnemyState::Stabilize;

	TDoOnce OnceManeuverNode;
	TDoOnce OnceAttackNode;
	TDoOnce OnceStabilizeNode;

	TDoOnce OnceANode;
	TDoOnce OnceBNode;
	TDoOnce OnceCNode;

	FTimerHandle RelativeLocGetter;
	FTimerHandle ManeuverTimer;

protected:
	UPROPERTY()
	TObjectPtr<class AEnemySu33Pawn> CompOwner;
	UPROPERTY()
	TObjectPtr<class UWorld> World;
	UPROPERTY()
	TObjectPtr<class AActor> Target;
	UPROPERTY()
	TObjectPtr<class UEnemyPositionUpdater> PositionUpdater;

private:
	inline void InitFalseDelegateBoolean() {
		bRollingDone = false;
		bPullUpDone = false;
		bImmelmannTurnDone = false;
		bStabilizeDone = false;
	}
	inline void SetStateStabilize(float ElapsedTime, float MaxTime) {
		if (ElapsedTime >= MaxTime)
			State = EEnemyState::Stabilize;
	}

	float Debug = 0.f;
};
