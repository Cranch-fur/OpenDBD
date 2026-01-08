#pragma once

#include "CoreMinimal.h"
#include "EOverlayTabs.generated.h"

UENUM()
enum class EOverlayTabs : int64
{
	Settings = 17408,
	Controls,
	Help,
	Survivors,
	ZeroEntry = 0,
};
