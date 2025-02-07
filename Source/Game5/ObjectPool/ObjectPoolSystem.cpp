// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSystem.h"

#include "PoolingObject.h"

// Sets default values for this component's properties
UObjectPoolSystem::UObjectPoolSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Offset.SetLocation(SpawnLocation);
	Offset.SetRotation(SpawnRotate.Quaternion());
	Offset.SetScale3D(SpawnScale);
	
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

}


// Called when the game starts
void UObjectPoolSystem::BeginPlay()
{
	Super::BeginPlay();
	// ...
	Pool.Empty();
	
	if (BaseObject.GetDefaultObject())
	{
		for (int i = 0; i < PoolSize; i++) {
			SpawnObjectToPool();
		}
	}
	
}


// Called every frame
void UObjectPoolSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UObjectPoolSystem::SpawnObjectToPool()
{
	APoolingObject* SpawnedActor;
	SpawnedActor = GetWorld()->SpawnActor<APoolingObject>(BaseObject.Get(), Offset, spawnParam);
	SpawnedActor->SetOwner(this->GetOwner());
	SpawnedActor->Deactivate();
	MovetoPoolChild(SpawnedActor);
	SpawnedActor->ObjectReturn.AddUniqueDynamic(this, &UObjectPoolSystem::MovetoPoolChild);
}

void UObjectPoolSystem::MovetoPoolChild(APoolingObject* ToPool)
{
	ToPool->K2_AttachToActor(this->GetOwner(), NAME_None, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	//ToPool->AttachToActor(this->GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	ToPool->SetActorRelativeLocation(Offset.GetLocation());

	Pool.Emplace(ToPool);
}

APoolingObject* UObjectPoolSystem::GetObject(FTransform SpawnTransform)
{
	APoolingObject* Result = nullptr;

	if (Pool.Num() > 0)
	{
		Result = Pool[0];
		Pool.RemoveSingle(Result);
		Result->SetActorTransform(SpawnTransform);
		Result->Activate();
		return Result;
	}
	else
	{
		SpawnObjectToPool();
		GetObject(SpawnTransform);
		return Result;
	}
}
