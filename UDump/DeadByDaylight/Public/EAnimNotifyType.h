#pragma once

#include "CoreMinimal.h"
#include "EAnimNotifyType.generated.h"

UENUM()
enum class EAnimNotifyType : int64
{
	VE_None = 16640,
	VE_Pickup,
	VE_Release,
	ZeroEntry = 0,
};
