#pragma once

#include "CoreMinimal.h"
#include "EOfferingAnimState.generated.h"

UENUM()
enum class EOfferingAnimState : int64
{
	None = 17408,
	Idle,
	Init,
	Reveal,
	ShowInteraction,
	Burned,
	Returned,
	Cancel,
	Hidden,
	ZeroEntry = 0,
};
