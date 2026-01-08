#pragma once

#include "CoreMinimal.h"
#include "EAttackSubstate.generated.h"

UENUM()
enum class EAttackSubstate : int64
{
	VE_None = 16640,
	VE_Open,
	VE_Hitting,
	VE_HitSucceed,
	VE_HitMiss,
	VE_HitObstructed,
	VE_Done,
	ZeroEntry = 0,
};
