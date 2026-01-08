#pragma once

#include "CoreMinimal.h"
#include "EQuadrantSpawnType.generated.h"

UENUM()
enum class EQuadrantSpawnType : int64
{
	L_Shape = 16384,
	Square,
	Rect_X,
	Rect_Y,
	Rect,
	None,
	ZeroEntry = 0,
};
