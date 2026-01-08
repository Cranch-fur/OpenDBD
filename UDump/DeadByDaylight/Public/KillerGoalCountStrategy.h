#pragma once

#include "CoreMinimal.h"
#include "KillerGoalCountStrategy.generated.h"

UENUM()
enum class KillerGoalCountStrategy : int64
{
	Add = 17408,
	Set,
	ZeroEntry = 0,
};
