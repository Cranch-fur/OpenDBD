#pragma once

#include "CoreMinimal.h"
#include "EPathType.generated.h"

UENUM()
enum class EPathType : int64
{
	None = 16384,
	Straight,
	DeadEnd,
	Corner,
	Crossroads,
	TJunction,
	OrientationOnly,
	Unspecified,
	ZeroEntry = 0,
};
