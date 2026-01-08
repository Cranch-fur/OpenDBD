#pragma once

#include "CoreMinimal.h"
#include "EscapeStrategyType.generated.h"

UENUM()
enum class EscapeStrategyType : int64
{
	EscapeDoor = 17152,
	EscapeHatch,
	ZeroEntry = 0,
};
