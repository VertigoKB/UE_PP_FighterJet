#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DataSavior.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDataSavior : public UInterface
{
	GENERATED_BODY()
};

class GAME5_API IDataSavior
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetDataFromInstance();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDataAtInstance();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveCountIncrement();
};