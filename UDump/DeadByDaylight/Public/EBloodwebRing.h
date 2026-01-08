#pragma once

#include "CoreMinimal.h"
#include "EBloodwebRing.generated.h"

UENUM()
enum class EBloodwebRing : int64
{
	CenterRing = 16640,
	InnerRing,
	MiddleRing,
	OuterRing,
	EntityRing,
	RingCount,
	ZeroEntry = 0,
};
