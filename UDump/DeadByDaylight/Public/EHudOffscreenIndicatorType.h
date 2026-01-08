#pragma once

#include "CoreMinimal.h"
#include "EHudOffscreenIndicatorType.generated.h"

UENUM()
enum class EHudOffscreenIndicatorType : int64
{
	NONE = 17152,
	WHITE,
	RED,
	ZeroEntry = 0,
};
