#pragma once

#include "CoreMinimal.h"
#include "ESkillCheckCustomType.h"
#include "ChargeableSkillCheckResponse.generated.h"

class ADBDPlayer;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FChargeableSkillCheckResponse, bool, success, bool, Bonus, ADBDPlayer*, Player, bool, TriggerLoudNoise, bool, hadInput, ESkillCheckCustomType, Type);
