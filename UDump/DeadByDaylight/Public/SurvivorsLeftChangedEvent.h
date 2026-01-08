#pragma once

#include "CoreMinimal.h"
#include "SurvivorsLeftChangedEvent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSurvivorsLeftChangedEvent, int32, survivorRemaining);
