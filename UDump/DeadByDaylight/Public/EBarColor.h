#pragma once

#include "CoreMinimal.h"
#include "EBarColor.generated.h"

UENUM()
enum class EBarColor : int64
{
	Default = 17408,
	Yellow,
	Red,
	ZeroEntry = 0,
};
