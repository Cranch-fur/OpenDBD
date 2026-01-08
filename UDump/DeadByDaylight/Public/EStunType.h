#pragma once

#include "CoreMinimal.h"
#include "EStunType.generated.h"

UENUM()
enum class EStunType : int64
{
	VE_None = 16640,
	VE_Bookshelf,
	VE_BearTrap,
	VE_Flashlight,
	VE_WiggleFree,
	VE_EvilWithin,
	VE_Kicked,
	ZeroEntry = 0,
};
