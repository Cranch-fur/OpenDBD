#pragma once

#include "CoreMinimal.h"
#include "EBadgeValueStrategy.generated.h"

UENUM()
enum class EBadgeValueStrategy : int64
{
	Additive = 17408,
	Max,
	ZeroEntry = 0,
};
