#pragma once

#include "CoreMinimal.h"
#include "EHudIndicatorIconType.generated.h"

UENUM()
enum class EHudIndicatorIconType : int64
{
	NONE = 17152,
	LOUD_SOUND,
	RUNNING_FOOTSTEPS,
	ZeroEntry = 0,
};
