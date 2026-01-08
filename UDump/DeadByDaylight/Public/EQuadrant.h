#pragma once

#include "CoreMinimal.h"
#include "EQuadrant.generated.h"

UENUM()
enum class EQuadrant : int64
{
	BottomRight = 16384,
	BottomLeft,
	TopLeft,
	TopRight,
	Count,
	ZeroEntry = 0,
};
