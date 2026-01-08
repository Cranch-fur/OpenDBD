#pragma once

#include "CoreMinimal.h"
#include "EBadgeEvaluation.generated.h"

UENUM()
enum class EBadgeEvaluation : int64
{
	Authority = 17408,
	Local,
	ZeroEntry = 0,
};
