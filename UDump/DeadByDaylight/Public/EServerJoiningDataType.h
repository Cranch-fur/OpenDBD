#pragma once

#include "CoreMinimal.h"
#include "EServerJoiningDataType.generated.h"

UENUM()
enum class EServerJoiningDataType : int64
{
	Idle = 17152,
	KillerServerFound,
	TravelToKiller,
	ZeroEntry = 0,
};
