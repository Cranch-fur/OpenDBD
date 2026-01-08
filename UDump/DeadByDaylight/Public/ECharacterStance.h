#pragma once

#include "CoreMinimal.h"
#include "ECharacterStance.generated.h"

UENUM()
enum class ECharacterStance : int64
{
	VE_Stand = 16896,
	VE_Crouch,
	VE_Crawl,
	ZeroEntry = 0,
};
