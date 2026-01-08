#pragma once

#include "CoreMinimal.h"
#include "ETileVariation.generated.h"

UENUM()
enum class ETileVariation : int64
{
	None = 16384,
	BlueTag,
	PinkTag,
	YellowTag,
	GreenTag,
	Empty,
	ZeroEntry = 0,
};
