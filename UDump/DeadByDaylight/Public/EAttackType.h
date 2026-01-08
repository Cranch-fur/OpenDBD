#pragma once

#include "CoreMinimal.h"
#include "EAttackType.generated.h"

UENUM()
enum class EAttackType : int64
{
	VE_None = 16640,
	VE_Slash,
	VE_Pounce,
	VE_Chainsaw,
	VE_Blink,
	VE_Lunge,
	ZeroEntry = 0,
};
