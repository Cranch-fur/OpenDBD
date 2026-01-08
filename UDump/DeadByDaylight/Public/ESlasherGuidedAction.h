#pragma once

#include "CoreMinimal.h"
#include "ESlasherGuidedAction.generated.h"

UENUM()
enum class ESlasherGuidedAction : int64
{
	VE_None = 17408,
	VE_Hooking,
	VE_PickingUp,
	VE_PuttingDown,
	ZeroEntry = 0,
};
