#pragma once

#include "CoreMinimal.h"
#include "MatchInfoAnalyticsController.h"
#include "WaitTimeAnalyticsController.h"
#include "Gameplay_KillerAnalyticsController.h"
#include "Gameplay_SurvivorAnalyticsController.h"
#include "ReportAnalyticsController.h"
#include "AnalyticsManager.generated.h"

USTRUCT(BlueprintType)
struct FAnalyticsManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMatchInfoAnalyticsController MatchInfoAnalytics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWaitTimeAnalyticsController WaitTimeAnalytics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplay_KillerAnalyticsController GameplayKillerAnalytics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplay_SurvivorAnalyticsController GameplaySurvivorAnalytics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FReportAnalyticsController ReportAnalytics;

public:
	DEADBYDAYLIGHT_API FAnalyticsManager();
};

FORCEINLINE uint32 GetTypeHash(const FAnalyticsManager) { return 0; }
