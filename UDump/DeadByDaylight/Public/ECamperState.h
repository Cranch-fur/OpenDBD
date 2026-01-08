#pragma once

#include "CoreMinimal.h"
#include "ECamperState.generated.h"

UENUM()
enum class ECamperState : int64
{
	VE_None = 16640,
	VE_Navigate,
	VE_Interact,
	ZeroEntry = 0,
};
