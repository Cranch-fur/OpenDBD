#pragma once

#include "CoreMinimal.h"
#include "EBloodwebNodeContentType.generated.h"

UENUM()
enum class EBloodwebNodeContentType : int64
{
	Empty = 16640,
	Perks,
	PerkBuffs,
	Offerings,
	Items,
	AddOn,
	Chests,
	Count,
	ZeroEntry = 0,
};
