#pragma once

#include "CoreMinimal.h"
#include "EGFxAlign.generated.h"

UENUM(BlueprintType)
namespace EGFxAlign
{
	enum Type
	{
		Center = 18688,
		TopCenter,
		BottomCenter,
		CenterLeft,
		CenterRight,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		ZeroEntry = 0,
	};
}
