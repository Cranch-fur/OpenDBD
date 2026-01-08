#pragma once

#include "CoreMinimal.h"
#include "EAuthoritativeMovementFlag.generated.h"

UENUM()
enum class EAuthoritativeMovementFlag : int64
{
	INTERACTION = 16896,
	SLASHED,
	CHEAT,
	DROPPED,
	NOPUSH,
	COUNT,
	ZeroEntry = 0,
};
