#pragma once

#include "CoreMinimal.h"
#include "EAttackZoneSet.generated.h"

UENUM()
enum class EAttackZoneSet : int64
{
	VE_OriginalZones = 17408,
	VE_WedgeZones,
	ZeroEntry = 0,
};
