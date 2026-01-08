#pragma once

#include "CoreMinimal.h"
#include "EDisconnectErrors.generated.h"

UENUM()
enum class EDisconnectErrors : int64
{
	None = 17152,
	SteamAuthFailure,
	SteamAuthFailureKickedFromServer,
	EACServerValidationFailure,
	EACValidationFailureKickedFromServer,
	EACClientNotRunning,
	EACClientIntegrityViolation,
	PlayerRemovedOnSuspend,
	LostConnectionToHost,
	LostConnectionToProfileService,
	MirrorsUnscheduledSessionDestruction,
	SessionKilledByMirrors,
	ZeroEntry = 0,
};
