#pragma once

#include "CoreMinimal.h"
#include "EOverlayButtonOptions.generated.h"

UENUM()
enum class EOverlayButtonOptions : int64
{
	HIDDEN = 17152,
	DISABLED,
	ENABLED,
	ZeroEntry = 0,
};
