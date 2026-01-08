#pragma once

#include "CoreMinimal.h"
#include "EBadgeGroup.generated.h"

UENUM()
enum class EBadgeGroup : int64
{
	PrimaryGoal = 17408,
	SecondaryObjective,
	BadgeGroupCount,
	ZeroEntry = 0,
};
