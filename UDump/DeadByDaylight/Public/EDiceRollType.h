#pragma once

#include "CoreMinimal.h"
#include "EDiceRollType.generated.h"

UENUM()
enum class EDiceRollType : int64
{
	VE_EscapeHook = 17408,
	VE_EscapeTrap,
	VE_TrapInflictsDying,
	VE_TriggerSkillCheck,
	VE_TriggerFootNoise,
	VE_TriggerCrowAlert,
	ZeroEntry = 0,
};
