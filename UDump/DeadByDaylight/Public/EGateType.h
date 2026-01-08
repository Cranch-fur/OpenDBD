#pragma once

#include "CoreMinimal.h"
#include "EGateType.generated.h"

UENUM()
enum class EGateType : int64
{
	VE_AndGate = 17408,
	VE_TimerGate,
	ZeroEntry = 0,
};
