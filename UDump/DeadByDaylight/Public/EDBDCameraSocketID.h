#pragma once

#include "CoreMinimal.h"
#include "EDBDCameraSocketID.generated.h"

UENUM()
enum class EDBDCameraSocketID : int64
{
	VE_None = 140694803818752,
	VE_Default,
	VE_Killcam,
	VE_Reaction,
	VE_Struggle,
	VE_Sacrifice,
	ZeroEntry = 0,
};
