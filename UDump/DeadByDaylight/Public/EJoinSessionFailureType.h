#pragma once

#include "CoreMinimal.h"
#include "EJoinSessionFailureType.generated.h"

UENUM()
enum class EJoinSessionFailureType : int64
{
	None = 17408,
	JoiningAlreadyStartedGame,
	JoiningPrivateGame,
	JoiningAlreadyFullGame,
	JoiningInvitation,
	ZeroEntry = 0,
};
