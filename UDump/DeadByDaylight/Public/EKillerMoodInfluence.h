#pragma once

#include "CoreMinimal.h"
#include "EKillerMoodInfluence.generated.h"

UENUM()
enum class EKillerMoodInfluence : int64
{
	VE_None = 16640,
	VE_Chuckles,
	VE_Banshee,
	VE_Hillbilly,
	VE_Nurse,
	VE_Shape,
	ZeroEntry = 0,
};
