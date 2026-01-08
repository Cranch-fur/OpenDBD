#pragma once

#include "CoreMinimal.h"
#include "EHintCategory.generated.h"

UENUM()
enum class EHintCategory : int64
{
	Lore = 17152,
	Survivor,
	Killer,
	ZeroEntry = 0,
};
