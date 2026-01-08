#pragma once

#include "CoreMinimal.h"
#include "ETutorialTypes.generated.h"

UENUM()
enum class ETutorialTypes : int64
{
	Survivor = 17152,
	Killer,
	Both,
	ZeroEntry = 0,
};
