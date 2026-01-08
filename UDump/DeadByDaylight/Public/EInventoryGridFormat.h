#pragma once

#include "CoreMinimal.h"
#include "EInventoryGridFormat.generated.h"

UENUM()
enum class EInventoryGridFormat : int64
{
	DEFAULT = 17408,
	DIAMOND,
	HONEYCOMB,
	ZeroEntry = 0,
};
