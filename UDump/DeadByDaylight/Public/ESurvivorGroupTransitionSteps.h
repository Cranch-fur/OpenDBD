#pragma once

#include "CoreMinimal.h"
#include "ESurvivorGroupTransitionSteps.generated.h"

UENUM()
enum class ESurvivorGroupTransitionSteps : int64
{
	Idle = 17152,
	JoiningKiller,
	TravellingToKiller,
	ZeroEntry = 0,
};
