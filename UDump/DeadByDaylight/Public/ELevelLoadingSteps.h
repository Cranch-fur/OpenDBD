#pragma once

#include "CoreMinimal.h"
#include "ELevelLoadingSteps.generated.h"

UENUM()
enum class ELevelLoadingSteps : int64
{
	Invalid = 17408,
	WaitingForPlayersToBeSpawned,
	WaitingForLoadoutAndTheme,
	WaitingForIntroToBeDone,
	LoadingCompleted,
	ZeroEntry = 0,
};
