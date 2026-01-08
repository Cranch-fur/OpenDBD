#pragma once

#include "CoreMinimal.h"
#include "EDetectionZone.generated.h"

UENUM()
enum class EDetectionZone : int64
{
	VE_None = 16640,
	VE_Slash,
	VE_Pounce,
	VE_Chainsaw,
	VE_Obstructed,
	VE_Interruption,
	VE_ChainsawLockExtended,
	VE_Damage,
	VE_ChainsawObstruction,
	VE_Blink,
	VE_ChainsawDamageExtended,
	VE_Stalker,
	VE_Lunge,
	ZeroEntry = 0,
};
