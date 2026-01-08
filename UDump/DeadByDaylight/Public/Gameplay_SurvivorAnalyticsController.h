#pragma once

#include "CoreMinimal.h"
#include "Gameplay_SurvivorAnalyticsController.generated.h"

USTRUCT(BlueprintType)
struct FGameplay_SurvivorAnalyticsController
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FGameplay_SurvivorAnalyticsController();
};

FORCEINLINE uint32 GetTypeHash(const FGameplay_SurvivorAnalyticsController) { return 0; }
