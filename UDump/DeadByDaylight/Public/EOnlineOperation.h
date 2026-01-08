#pragma once

#include "CoreMinimal.h"
#include "EOnlineOperation.generated.h"

UENUM()
enum class EOnlineOperation : int64
{
	None = 17408,
	HostGame,
	EndingSession,
	DestroyingSession,
	JoinGame,
	JoinCancelled,
	FindCustomSessions,
	OnlineCheck,
	ZeroEntry = 0,
};
