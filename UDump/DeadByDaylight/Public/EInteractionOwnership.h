#pragma once

#include "CoreMinimal.h"
#include "EInteractionOwnership.generated.h"

UENUM()
enum class EInteractionOwnership : int64
{
	AnyCanUse = 17408,
	OnlyOwnerCanUse,
	OwnerCannotUse,
	ZeroEntry = 0,
};
