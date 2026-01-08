#pragma once

#include "CoreMinimal.h"
#include "ESkillCheckCustomType.h"
#include "ChargeableDefSkillCheckResponse.generated.h"

class ADBDPlayer;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FChargeableDefSkillCheckResponse, bool, success, bool, Bonus, ADBDPlayer*, Player, bool, TriggerLoudNoise, ESkillCheckCustomType, Type);
