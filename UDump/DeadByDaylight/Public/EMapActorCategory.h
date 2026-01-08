#pragma once

#include "CoreMinimal.h"
#include "EMapActorCategory.generated.h"

UENUM()
enum class EMapActorCategory : int64
{
	None = 17408,
	Objective,
	BlackLock,
	KillerObject,
	ExitGate,
	ZeroEntry = 0,
};
