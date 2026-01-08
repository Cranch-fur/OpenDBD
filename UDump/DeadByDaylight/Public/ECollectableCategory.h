#pragma once

#include "CoreMinimal.h"
#include "ECollectableCategory.generated.h"

UENUM()
enum class ECollectableCategory : int64
{
	Common = 17152,
	Rare,
	ZeroEntry = 0,
};
