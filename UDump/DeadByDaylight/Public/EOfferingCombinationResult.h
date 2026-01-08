#pragma once

#include "CoreMinimal.h"
#include "EOfferingCombinationResult.generated.h"

UENUM()
enum class EOfferingCombinationResult : int64
{
	None = 16640,
	Stacked,
	CoConsummed,
	Overruling,
	Cancelled,
	ZeroEntry = 0,
};
