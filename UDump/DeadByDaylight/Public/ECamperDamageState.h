#pragma once

#include "CoreMinimal.h"
#include "ECamperDamageState.generated.h"

UENUM()
enum class ECamperDamageState : int64
{
	VE_Healthy = 17152,
	VE_Injured,
	VE_KO,
	VE_Dead,
	ZeroEntry = 0,
};
