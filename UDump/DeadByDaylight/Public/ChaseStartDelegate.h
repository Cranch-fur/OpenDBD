#pragma once

#include "CoreMinimal.h"
#include "ChaseStartDelegate.generated.h"

class ACamperPlayer;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChaseStartDelegate, ACamperPlayer*, camper);
