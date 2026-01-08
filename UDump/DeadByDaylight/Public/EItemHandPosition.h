#pragma once

#include "CoreMinimal.h"
#include "EItemHandPosition.generated.h"

UENUM()
enum class EItemHandPosition : int64
{
	None = 16640,
	HandleItem,
	AimItem,
	SmallItem,
	FirecrackerItem,
	ZeroEntry = 0,
};
