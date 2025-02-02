#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETemp : uint8
{
	Temp1,
	Temp2,
	Temp3
};

UENUM()
enum class EMissileSocket : uint8
{
	MissileLO = 0,
	MissileRO,
	MissileLI,
	MissileRI
};