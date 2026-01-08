#pragma once

#include "CoreMinimal.h"
#include "EGameplayElementType.generated.h"

UENUM()
enum class EGameplayElementType : int64
{
	Generic = 17152,
	MeatLocker_Small,
	TileLights,
	MeatLocker_Big,
	Searchable,
	EdgeObjects,
	LivingWorldObjects,
	Hatch,
	Bookshelves,
	Totems,
	QuadrantSpawn,
	ZeroEntry = 0,
};
