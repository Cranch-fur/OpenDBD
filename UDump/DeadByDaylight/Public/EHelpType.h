#pragma once

#include "CoreMinimal.h"
#include "EHelpType.generated.h"

UENUM()
enum class EHelpType : int64
{
	General = 17152,
	Survivor,
	Killer,
	ZeroEntry = 0,
};
