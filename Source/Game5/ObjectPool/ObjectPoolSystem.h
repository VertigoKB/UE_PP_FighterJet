// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolSystem.generated.h"


class APoolingObject;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME5_API UObjectPoolSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolSystem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PoolingObject")
	TSubclassOf<APoolingObject> BaseObject;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectPool")
	int32 PoolSize = 1;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	APoolingObject* GetObject(FTransform SpawnTransform);

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<APoolingObject*> Pool;

	FTransform Offset;
	UPROPERTY(BlueprintReadWrite)
	FVector SpawnLocation = FVector(0, 0, -10000.f);
	UPROPERTY(BlueprintReadWrite)
	FRotator SpawnRotate = FRotator(0.f, 0.f, 0.f);
	UPROPERTY(BlueprintReadWrite)
	FVector SpawnScale = FVector(1.f, 1.f, 1.f);

	FActorSpawnParameters spawnParam;

	UFUNCTION()
	void SpawnObjectToPool();
	UFUNCTION()
	void MovetoPoolChild(APoolingObject* ToPool);
};
