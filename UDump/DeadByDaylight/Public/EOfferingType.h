#pragma once

#include "CoreMinimal.h"
#include "EOfferingType.generated.h"

UENUM()
enum class EOfferingType : int64
{
	None = 16640,
	Atmosphere,
	Lighting,
	Points,
	Zone,
	Position,
	Chest,
	Hook,
	Protection,
	Hatch,
	Odds,
	Killing,
	World,
	Luck,
	Killer,
	Count,
	ZeroEntry = 0,
};
