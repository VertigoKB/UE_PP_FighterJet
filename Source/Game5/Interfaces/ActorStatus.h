#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorStatus.generated.h"

UINTERFACE(MinimalAPI)
class UActorStatus : public UInterface
{
	GENERATED_BODY()
};

class GAME5_API IActorStatus
{
	GENERATED_BODY()

public:
	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	//virtual float GetThrottle() const = 0;
	//virtual void SetThrottle(float Value) = 0;

	//static constexpr float MaxThrottle = 600.f;
};