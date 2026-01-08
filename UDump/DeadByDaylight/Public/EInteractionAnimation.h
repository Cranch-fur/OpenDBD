#pragma once

#include "CoreMinimal.h"
#include "EInteractionAnimation.generated.h"

UENUM()
enum class EInteractionAnimation : int64
{
	VE_None = 16640,
	VE_Generator,
	VE_PullDownLeft,
	VE_PullDownRight,
	VE_Hiding,
	VE_SearchCloset,
	VE_HealingOther,
	VE_OpenEscape,
	VE_StruggleFree,
	VE_HealOther,
	VE_HealSelf,
	VE_PickedUp,
	VE_Unused01,
	VE_Dropped,
	VE_Unused02,
	VE_BeingHooked,
	VE_Sabotage,
	VE_ChargeBlink,
	VE_ThrowFirecracker,
	ZeroEntry = 0,
};
