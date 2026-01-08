#pragma once

#include "CoreMinimal.h"
#include "ELoginType.generated.h"

UENUM()
enum class ELoginType : int64
{
	Reservation = 140694801843200,
	Join,
	None,
	ZeroEntry = 0,
};
