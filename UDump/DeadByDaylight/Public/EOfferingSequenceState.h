#pragma once

#include "CoreMinimal.h"
#include "EOfferingSequenceState.generated.h"

UENUM()
enum class EOfferingSequenceState : int64
{
	SmokeIn = 17408,
	FadeOut,
	FadeIn,
	NotInitialized,
	Idle,
	Init,
	InitDone,
	Reveal,
	RevealDone,
	ShowInteraction,
	Finalize,
	Done,
	ZeroEntry = 0,
};
