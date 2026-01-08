#pragma once

#include "CoreMinimal.h"
#include "Gameplay_KillerAnalyticsController.generated.h"

USTRUCT(BlueprintType)
struct FGameplay_KillerAnalyticsController
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FGameplay_KillerAnalyticsController();
};

FORCEINLINE uint32 GetTypeHash(const FGameplay_KillerAnalyticsController) { return 0; }
