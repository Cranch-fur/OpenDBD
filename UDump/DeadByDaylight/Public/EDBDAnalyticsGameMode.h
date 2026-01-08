#pragma once

#include "CoreMinimal.h"
#include "EDBDAnalyticsGameMode.generated.h"

UENUM()
enum class EDBDAnalyticsGameMode : int64
{
	AnalyticsServer = 16384,
	AnalyticsClient,
	AnalyticsLoading,
	AnalyticsLobby,
	AnalyticsMenu,
	AnalyticsPostGame,
	AnalyticsPerfTests,
	ZeroEntry = 0,
};
