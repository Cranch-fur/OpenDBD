#pragma once

#include "CoreMinimal.h"
#include "EBlinkerState.generated.h"

UENUM()
enum class EBlinkerState : int64
{
	VE_Ready = 17152,
	VE_ChainBlink,
	VE_Cooldown,
	ZeroEntry = 0,
};
