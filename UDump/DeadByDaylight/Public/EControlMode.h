#pragma once

#include "CoreMinimal.h"
#include "EControlMode.generated.h"

UENUM()
enum class EControlMode : int64
{
	MOUSE_KB = 17152,
	XBOX,
	PS,
	VITA,
	UNDEFINED,
	ZeroEntry = 0,
};
