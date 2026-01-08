#pragma once

#include "CoreMinimal.h"
#include "ETileSpawnPointType.generated.h"

UENUM()
enum class ETileSpawnPointType : int64
{
	Unspecified = 16384,
	Survivor,
	SurvivorItem,
	Killer,
	KillerItem,
	InteractableObject,
	ZeroEntry = 0,
};
