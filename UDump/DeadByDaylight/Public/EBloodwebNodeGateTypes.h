#pragma once

#include "CoreMinimal.h"
#include "EBloodwebNodeGateTypes.generated.h"

UENUM()
enum class EBloodwebNodeGateTypes : int64
{
	RequiredRank = 16640,
	RequiredItem,
	Count,
	ZeroEntry = 0,
};
