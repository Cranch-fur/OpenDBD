#pragma once

#include "CoreMinimal.h"
#include "EProceduralDebugMode.generated.h"

UENUM()
enum class EProceduralDebugMode : int64
{
	None = 17408,
	NoMap,
	TilesSmall,
	TilesBig,
	Count,
	ZeroEntry = 0,
};
