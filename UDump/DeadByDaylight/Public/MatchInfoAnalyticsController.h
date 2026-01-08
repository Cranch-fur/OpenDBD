#pragma once

#include "CoreMinimal.h"
#include "MatchInfoAnalyticsController.generated.h"

USTRUCT(BlueprintType)
struct FMatchInfoAnalyticsController
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FMatchInfoAnalyticsController();
};

FORCEINLINE uint32 GetTypeHash(const FMatchInfoAnalyticsController) { return 0; }
