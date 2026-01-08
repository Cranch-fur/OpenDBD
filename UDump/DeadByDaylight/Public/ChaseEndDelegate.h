#pragma once

#include "CoreMinimal.h"
#include "ChaseEndDelegate.generated.h"

class ACamperPlayer;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChaseEndDelegate, ACamperPlayer*, camper, float, chaseTime);
