#pragma once

#include "CoreMinimal.h"
#include "EPlayerRole.generated.h"

UENUM()
enum class EPlayerRole : int64
{
	VE_None = 16640,
	VE_Slasher,
	VE_Camper,
	VE_Observer,
	ZeroEntry = 0,
};
