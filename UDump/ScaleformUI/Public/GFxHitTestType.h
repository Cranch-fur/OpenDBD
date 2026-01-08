#pragma once

#include "CoreMinimal.h"
#include "GFxHitTestType.generated.h"

UENUM(BlueprintType)
namespace GFxHitTestType
{
	enum Type
	{
		HitTest_Bounds = 4294967296,
		HitTest_Shapes,
		HitTest_ButtonEvents,
		HitTest_ShapesNoInvisible,
		ZeroEntry = 0,
	};
}
