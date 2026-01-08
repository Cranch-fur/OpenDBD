#pragma once

#include "CoreMinimal.h"
#include "EChargeableInteractionBarType.generated.h"

UENUM()
enum class EChargeableInteractionBarType : int64
{
	VE_Normal = 17408,
	VE_ProgressPips,
	ZeroEntry = 0,
};
