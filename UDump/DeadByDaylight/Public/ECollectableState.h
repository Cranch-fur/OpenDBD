#pragma once

#include "CoreMinimal.h"
#include "ECollectableState.generated.h"

UENUM()
enum class ECollectableState : int64
{
	OnGround = 17152,
	Stored,
	Equipped,
	ToBeDropped,
	ZeroEntry = 0,
};
