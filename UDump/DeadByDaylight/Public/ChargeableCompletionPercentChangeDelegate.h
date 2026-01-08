#pragma once

#include "CoreMinimal.h"
#include "ChargeableCompletionPercentChangeDelegate.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargeableCompletionPercentChangeDelegate, float, PercentComplete);
