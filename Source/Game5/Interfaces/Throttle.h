#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Throttle.generated.h"

UINTERFACE(MinimalAPI)
class UThrottle : public UInterface
{
	GENERATED_BODY()
};

class GAME5_API IThrottle
{
	GENERATED_BODY()

public:
	virtual void Test() = 0;

	virtual float GetThrottle() const = 0;
	virtual void SetThrottle(float Value) = 0;

	static constexpr float MaxThrottle = 600.f;
};

/*
Throttle	ScaleValue
600.f		1.f
300.f		0.5f
60.f		0.1f
1.f			1/600 .f (0.0016666666666667)

0.f			0.f
*/