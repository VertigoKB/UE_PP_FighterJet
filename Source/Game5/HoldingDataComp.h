// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoldingDataComp.generated.h"

/*
이 컴포넌트는 재사용을 위한 컴포넌트가 아닙니다.
이 컴포넌트는 플레이어의 AF15Pawn에만 사용 가능합니다.
& This Component Not For Recycle.
& This Component Attachable to Player's AF15Pawn Actor.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API UHoldingDataComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHoldingDataComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<class UWorld> MainWorld = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<class UWorld> BattleField = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	//& About MoveCounter
	UFUNCTION(BlueprintCallable)
	void SetMoveCounterFromGameInst();
	UFUNCTION(BlueprintCallable)
	int32 GetMoveCounterFromComp();

	UPROPERTY(BlueprintReadWrite)
	int32 MoveCounter = 0;
	 
public:	//& About saving data to gameinstance
	UFUNCTION()
	void RequestSetData();
	UFUNCTION()
	void RequestDataFromGameInst();

private:
	UFUNCTION()
	bool Initialize();
	UFUNCTION()
	bool FindTargetWhenInit();

	UPROPERTY()
	bool bFlag = true;
	UPROPERTY()
	class UWorld* TheWorld = nullptr;
	UPROPERTY()
	class UFighterGameInst* WorldGameInst = nullptr;
	UPROPERTY()
	class AF15Pawn* Player = nullptr;
	UPROPERTY()
	TSubclassOf<AActor> TargetClass = nullptr;
	UPROPERTY()
	TArray<AActor*> TargetArray;
	UPROPERTY()
	class AGameChangerTarget* Target = nullptr;

	FTimerHandle TargetFinder;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float DEBUG = 0.f;
};
