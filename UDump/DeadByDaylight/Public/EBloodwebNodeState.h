#pragma once

#include "CoreMinimal.h"
#include "EBloodwebNodeState.generated.h"

UENUM()
enum class EBloodwebNodeState : int64
{
	Inactive = 16640,
	Available,
	Locked,
	Collected,
	Consumed,
	ZeroEntry = 0,
};
