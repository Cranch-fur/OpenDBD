#pragma once

#include "CoreMinimal.h"
#include "EDBDValueEqualityCondition.generated.h"

UENUM()
enum class EDBDValueEqualityCondition : int64
{
	GreaterThanEqual = 17408,
	GreaterThan,
	LessThanEqual,
	LessThan,
	Equal,
	ZeroEntry = 0,
};
