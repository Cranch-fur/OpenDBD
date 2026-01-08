#pragma once

#include "CoreMinimal.h"
#include "EGameIntroSteps.generated.h"

UENUM()
enum class EGameIntroSteps : int64
{
	Idling = 17408,
	PlayingIntro,
	LoopingCamView,
	ZeroEntry = 0,
};
