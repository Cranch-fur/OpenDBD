#pragma once

#include "CoreMinimal.h"
#include "EGameType.generated.h"

UENUM()
enum class EGameType : int64
{
	Online = 16384,
	PartyMode,
	SurvivorGroup,
	None,
	ZeroEntry = 0,
};
