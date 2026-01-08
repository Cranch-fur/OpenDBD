#pragma once

#include "CoreMinimal.h"
#include "ERequestState.generated.h"

UENUM()
enum class ERequestState : int64
{
	VE_None = 17408,
	VE_Pending,
	VE_Success,
	VE_Fail,
	ZeroEntry = 0,
};
