#pragma once

#include "CoreMinimal.h"
#include "EDensity.generated.h"

UENUM()
enum class EDensity : int64
{
	Unspecified = 16384,
	Light,
	Moderate,
	Heavy,
	Empty,
	ZeroEntry = 0,
};
