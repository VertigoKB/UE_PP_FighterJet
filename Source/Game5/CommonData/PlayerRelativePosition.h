#pragma once

#include "CoreMinimal.h"
#include "PlayerRelativePosition.generated.h"


USTRUCT(BlueprintType)
struct FPlayerRelativePosition
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInFront;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAbove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLeftRightInsight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTooClose;

	FPlayerRelativePosition() :
		bIsInFront(false),
		bIsAbove(false),
		bIsRight(false),
		bIsTooClose(false) {}
};

/*
	DEPRECATED

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bIsForwardInsight;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bIsAboveInsight;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bIsLeftRightInsight;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bIsInSight;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bIsInRange;

		//bIsForwardInsight(false),
		//bIsAboveInsight(false),
		//bIsLeftRightInsight(false),
		//bIsInSight(false),
		//bIsInRange(false),

*/