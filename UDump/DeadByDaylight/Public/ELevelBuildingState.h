#pragma once

#include "CoreMinimal.h"
#include "ELevelBuildingState.generated.h"

UENUM()
enum class ELevelBuildingState : int64
{
	NotInitialized = 17408,
	SyncSeeds,
	GetAvailableItems,
	PendingGettingItems,
	PendingGettingLevels,
	GettingLevelsDone,
	GetThemedTiles,
	PendingGettingTiles,
	SpawnRequiredTiles,
	SpawningLevelTiles,
	SpawnInterTileElements,
	PendingInterTileElementsSpawning,
	SpawningActors,
	StallingForRemotes,
	BuildingDone,
	ZeroEntry = 0,
};
