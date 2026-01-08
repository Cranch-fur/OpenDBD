#pragma once

#include "CoreMinimal.h"
#include "ReportAnalyticsController.generated.h"

USTRUCT(BlueprintType)
struct FReportAnalyticsController
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FReportAnalyticsController();
};

FORCEINLINE uint32 GetTypeHash(const FReportAnalyticsController) { return 0; }
