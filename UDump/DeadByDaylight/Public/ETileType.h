#pragma once

#include "CoreMinimal.h"
#include "ETileType.generated.h"

UENUM()
enum class ETileType : int64
{
	None = 16384,
	Blocker,
	Any,
	Forest,
	Building,
	Maze,
	Landmark,
	Signature,
	ZeroEntry = 0,
};
