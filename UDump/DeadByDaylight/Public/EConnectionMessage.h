#pragma once

#include "CoreMinimal.h"
#include "EConnectionMessage.generated.h"

UENUM()
enum class EConnectionMessage : int64
{
	NONE = 17152,
	JOINED_LOBBY,
	CREATED_LOBBY,
	FAILED_TO_JOIN,
	NO_LOBBIES_FOUND,
	HOST_LEFT,
	SEARCHING_FOR_GAMES,
	CANCELED_MATCHMAKING,
	ZeroEntry = 0,
};
