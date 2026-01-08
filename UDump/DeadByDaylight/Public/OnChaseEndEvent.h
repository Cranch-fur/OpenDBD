#pragma once

#include "CoreMinimal.h"
#include "OnChaseEndEvent.generated.h"

class ACamperPlayer;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChaseEndEvent, ACamperPlayer*, camper);
