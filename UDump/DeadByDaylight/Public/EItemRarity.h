#pragma once

#include "CoreMinimal.h"
#include "EItemRarity.generated.h"

UENUM()
enum class EItemRarity : int64
{
	Common = 16640,
	Uncommon,
	Rare,
	VeryRare,
	UltraRare,
	Artifact,
	Spectral,
	Count,
	None,
	ZeroEntry = 0,
};
