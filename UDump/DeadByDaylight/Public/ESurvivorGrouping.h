#pragma once

#include "CoreMinimal.h"
#include "ESurvivorGrouping.generated.h"

UENUM()
enum class ESurvivorGrouping : int64
{
	Invalid = 17152,
	Separated,
	Grouped2_2,
	Grouped3_1,
	Grouped4_0,
	ZeroEntry = 0,
};
