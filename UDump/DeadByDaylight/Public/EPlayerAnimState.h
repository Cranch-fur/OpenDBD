#pragma once

#include "CoreMinimal.h"
#include "EPlayerAnimState.generated.h"

UENUM()
enum class EPlayerAnimState : int64
{
	VE_Default = 16640,
	VE_Injured,
	VE_InjuredCrouch,
	VE_Crouch,
	ZeroEntry = 0,
};
