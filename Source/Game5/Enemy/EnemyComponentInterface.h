#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyComponentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyComponentInterface : public UInterface
{
	GENERATED_BODY()
};

class GAME5_API IEnemyComponentInterface
{
	GENERATED_BODY()

public:
	virtual class UActorComponent* GetPlayerFinder() = 0;
	virtual class UActorComponent* GetPositionUpdater() = 0;
};