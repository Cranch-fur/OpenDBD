#pragma once

#include "CoreMinimal.h"
#include "WaitTimeAnalyticsController.generated.h"

USTRUCT(BlueprintType)
struct FWaitTimeAnalyticsController
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FWaitTimeAnalyticsController();
};

FORCEINLINE uint32 GetTypeHash(const FWaitTimeAnalyticsController) { return 0; }
