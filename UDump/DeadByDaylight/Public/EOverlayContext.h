#pragma once

#include "CoreMinimal.h"
#include "EOverlayContext.generated.h"

UENUM()
enum class EOverlayContext : int64
{
	MainMenus = 17408,
	InGame,
	ZeroEntry = 0,
};
