#pragma once

#include "CoreMinimal.h"
#include "EKillerJoiningState.generated.h"

UENUM(BlueprintType)
enum class EKillerJoiningState : uint8
{
	Disconnected,
	Connected,
	ConnectionFailed,
};
