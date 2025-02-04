#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MissilesNumbering.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UMissilesNumbering : public UInterface
{
	GENERATED_BODY()
};

class GAME5_API IMissilesNumbering
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MissileNumber(int32 num);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MissileLaunch(int32 num);
};