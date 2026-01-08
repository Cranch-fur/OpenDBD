#pragma once

#include "CoreMinimal.h"
#include "EAttackSuccess.generated.h"

UENUM()
enum class EAttackSuccess : int64
{
	VE_None = 16640,
	VE_Success,
	VE_Failure,
	VE_Obstructed,
	ZeroEntry = 0,
};
