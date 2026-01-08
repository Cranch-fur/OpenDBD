#pragma once

#include "CoreMinimal.h"
#include "ESkillCheckCustomType.h"
#include "OnDesactivateSkillCheck.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDesactivateSkillCheck, bool, hadInput, bool, success, bool, Bonus, ESkillCheckCustomType, Type);
