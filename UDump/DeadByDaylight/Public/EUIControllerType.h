#pragma once

#include "CoreMinimal.h"
#include "EUIControllerType.generated.h"

UENUM()
enum class EUIControllerType : int64
{
	KeyboardMouse = 17152,
	GamePad,
	ZeroEntry = 0,
};
