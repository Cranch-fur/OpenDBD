#pragma once

#include "CoreMinimal.h"
#include "ECamperGuidedAction.generated.h"

UENUM()
enum class ECamperGuidedAction : int64
{
	VE_None = 17152,
	VE_PrepBeingKilled,
	VE_BeingKilled,
	VE_PrepGuidedAction,
	VE_BeingHelpedOffHookFront,
	VE_BeingHelpedOffHookBack,
	VE_BeingPickedUp,
	VE_BeingPutDown,
	VE_BeingCarried,
	VE_BeingPutOnHook,
	VE_BeingHealed,
	VE_BeingPulledFromCloset,
	VE_BeingHelpedFromTrap,
	ZeroEntry = 0,
};
