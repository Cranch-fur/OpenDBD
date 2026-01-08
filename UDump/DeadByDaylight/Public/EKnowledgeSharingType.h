#pragma once

#include "CoreMinimal.h"
#include "EKnowledgeSharingType.generated.h"

UENUM()
enum class EKnowledgeSharingType : int64
{
	Possessor = 16384,
	Survivors,
	Killers,
	All,
	ZeroEntry = 0,
};
