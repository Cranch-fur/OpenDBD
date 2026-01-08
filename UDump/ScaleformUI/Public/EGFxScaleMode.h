#pragma once

#include "CoreMinimal.h"
#include "EGFxScaleMode.generated.h"

UENUM(BlueprintType)
namespace EGFxScaleMode
{
	enum Type
	{
		NoScale = 18688,
		ShowAll,
		ExactFit,
		NoBorder,
		ZeroEntry = 0,
	};
}
