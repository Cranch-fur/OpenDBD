#pragma once

#include "CoreMinimal.h"
#include "ELoadProgress.generated.h"

UENUM()
enum class ELoadProgress : int64
{
	NotStarted = 17408,
	Init,
	ValidateOnlineSubsystem,
	CheckingPlatformService,
	ConnectingToMirrors,
	StartEAC,
	LoadingProfile,
	LoadingStats,
	LoadingMirrorCurrencies,
	Complete,
	ZeroEntry = 0,
};
