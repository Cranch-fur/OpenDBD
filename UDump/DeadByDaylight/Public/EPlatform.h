#pragma once

#include "CoreMinimal.h"
#include "EPlatform.generated.h"

UENUM()
enum class EPlatform : int64
{
	STEAM = 17152,
	XBOX,
	PSN,
	ZeroEntry = 0,
};
