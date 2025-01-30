// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingObject.h"

// Sets default values
APoolingObject::APoolingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelfRef = this;

}

void APoolingObject::OnObjectReturnToPool(APoolingObject* ToPool)
{
	if (ToPool)
	{
		ObjectReturn.Broadcast(ToPool);
	}
}

// Called when the game starts or when spawned
void APoolingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void APoolingObject::Activate()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
	OnActivate();
}

void APoolingObject::Deactivate()
{
	OnDeactivate();
	OnObjectReturnToPool(this);
}

void APoolingObject::OnActivate_Implementation()
{
}

void APoolingObject::OnDeactivate_Implementation()
{
}

// Called every frame
void APoolingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

