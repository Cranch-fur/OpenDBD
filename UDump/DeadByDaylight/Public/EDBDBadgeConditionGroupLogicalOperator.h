#pragma once

#include "CoreMinimal.h"
#include "EDBDBadgeConditionGroupLogicalOperator.generated.h"

UENUM()
enum class EDBDBadgeConditionGroupLogicalOperator : int64
{
	And = 17408,
	Or,
	ZeroEntry = 0,
};
