#pragma once

#include "CoreMinimal.h"
#include "ESubstanceEngineType.generated.h"

UENUM(BlueprintType)
enum ESubstanceEngineType
{
	SET_CPU = 21760,
	SET_GPU,
	ZeroEntry = 0,
};
