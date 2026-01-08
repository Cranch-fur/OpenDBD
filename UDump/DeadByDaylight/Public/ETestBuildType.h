#pragma once

#include "CoreMinimal.h"
#include "ETestBuildType.generated.h"

UENUM()
enum class ETestBuildType : int64
{
	None = 17152,
	PublicTestBuild,
	PaxBuild,
	ConsolePreAlphaBuild,
	ZeroEntry = 0,
};
