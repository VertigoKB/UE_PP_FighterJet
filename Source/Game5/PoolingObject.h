// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ActorStatus.h"
#include "PoolingObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReturnCall, APoolingObject*, SelfRef);

UCLASS()
class GAME5_API APoolingObject : public AActor, public IActorStatus
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolingObject();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FReturnCall ObjectReturn;

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnObjectReturnToPool(APoolingObject* ToPool);

	virtual void Activate() override;
	virtual void Deactivate() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//Override at derived class or bp
	UFUNCTION(BlueprintNativeEvent)
	void OnActivate();
	virtual void OnActivate_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void OnDeactivate();
	virtual void OnDeactivate_Implementation();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	APoolingObject* SelfRef;

};
