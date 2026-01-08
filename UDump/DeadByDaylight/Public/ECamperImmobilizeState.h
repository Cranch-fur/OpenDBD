#pragma once

#include "CoreMinimal.h"
#include "ECamperImmobilizeState.generated.h"

UENUM()
enum class ECamperImmobilizeState : int64
{
	VE_None = 17152,
	VE_Hooked,
	VE_Trapped,
	VE_Hiding,
	ZeroEntry = 0,
};
