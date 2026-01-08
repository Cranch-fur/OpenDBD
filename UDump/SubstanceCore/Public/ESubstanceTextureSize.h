#pragma once

#include "CoreMinimal.h"
#include "ESubstanceTextureSize.generated.h"

UENUM(BlueprintType)
enum ESubstanceTextureSize
{
	ERL_16 = 21760,
	ERL_32,
	ERL_64,
	ERL_128,
	ERL_256,
	ERL_512,
	ERL_1024,
	ERL_2048,
	ZeroEntry = 0,
};
