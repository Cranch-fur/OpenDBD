#pragma once

#include "CoreMinimal.h"
#include "EDirection.generated.h"

UENUM()
enum class EDirection : int64
{
	Up = 16384,
	Down,
	Left,
	Right,
	ZeroEntry = 0,
};
