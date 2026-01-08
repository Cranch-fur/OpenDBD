#pragma once

#include "CoreMinimal.h"
#include "EGFxTimingMode.generated.h"

UENUM(BlueprintType)
namespace EGFxTimingMode
{
	enum Type
	{
		Game = 18688,
		Real,
		ZeroEntry = 0,
	};
}
