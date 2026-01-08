#pragma once

#include "CoreMinimal.h"
#include "ScoreEventArray.h"
#include "PlayerstateDataCache.generated.h"

USTRUCT(BlueprintType)
struct FPlayerstateDataCache
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FScoreEventArray> ScoreEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeInactive;

public:
	DEADBYDAYLIGHT_API FPlayerstateDataCache();
};

FORCEINLINE uint32 GetTypeHash(const FPlayerstateDataCache) { return 0; }
