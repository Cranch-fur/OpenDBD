#pragma once

#include "CoreMinimal.h"
#include "EGameState.generated.h"

UENUM()
enum class EGameState : int64
{
	VE_Active = 16640,
	VE_Dead,
	VE_Escaped,
	VE_EscapedInjured,
	VE_Sacrificed,
	VE_None,
	VE_Disconnected,
	VE_ManuallyLeftMatch,
	ZeroEntry = 0,
};
