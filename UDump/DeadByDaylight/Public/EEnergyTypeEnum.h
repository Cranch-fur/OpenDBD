#pragma once

#include "CoreMinimal.h"
#include "EEnergyTypeEnum.generated.h"

UENUM()
enum class EEnergyTypeEnum : int64
{
	EInvalid = 17152,
	EBattery,
	EHealth,
	ZeroEntry = 0,
};
