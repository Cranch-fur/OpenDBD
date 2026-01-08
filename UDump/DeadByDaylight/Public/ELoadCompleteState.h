#pragma once

#include "CoreMinimal.h"
#include "ELoadCompleteState.generated.h"

UENUM()
enum class ELoadCompleteState : int64
{
	InProgress = 17408,
	Success,
	FailedEAC,
	FailedRecoverable,
	FailedRecoverableProfileLoad,
	FailedBlocking,
	FailedBlockingNoDBDServer,
	FailedBlockingNotConnected,
	ZeroEntry = 0,
};
