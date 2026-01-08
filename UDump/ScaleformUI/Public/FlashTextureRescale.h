#pragma once

#include "CoreMinimal.h"
#include "FlashTextureRescale.generated.h"

UENUM(BlueprintType)
enum FlashTextureRescale
{
	FlashTextureScale_High = 18688,
	FlashTextureScale_Low,
	FlashTextureScale_NextLow,
	FlashTextureScale_Mult4,
	FlashTextureScale_None,
	ZeroEntry = 0,
};
