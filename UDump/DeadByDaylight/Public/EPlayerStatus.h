#pragma once

#include "CoreMinimal.h"
#include "EPlayerStatus.generated.h"

UENUM()
enum class EPlayerStatus : int64
{
	HOOK = 17152,
	TRAP,
	DEAD,
	ESCAPED,
	INJURED,
	CRAWLING,
	SACRIFICED,
	DISCONNECTED,
	DEFAULT,
	ZeroEntry = 0,
};
