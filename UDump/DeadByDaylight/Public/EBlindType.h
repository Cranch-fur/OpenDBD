#pragma once

#include "CoreMinimal.h"
#include "EBlindType.generated.h"

UENUM()
enum class EBlindType : int64
{
	VE_None = 16640,
	VE_Flashlight,
	VE_SacrificeSuspended,
	VE_Firecracker,
	ZeroEntry = 0,
};
