#pragma once

#include "CoreMinimal.h"
#include "EOfflineLobbyState.generated.h"

UENUM()
enum class EOfflineLobbyState : int64
{
	None = 17408,
	RoleSelection,
	OfflineLobby,
	ZeroEntry = 0,
};
