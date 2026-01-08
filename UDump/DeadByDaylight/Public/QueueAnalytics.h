#pragma once

#include "CoreMinimal.h"
#include "QueueAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FQueueAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimesQueuedSurvivor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimesQueuedKiller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MatchesAsSurvivor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MatchesAsKiller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FQueueAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FQueueAnalytics) { return 0; }
