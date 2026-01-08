#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreTypes.h"
#include "ScoreAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FScoreAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDScoreTypes ScoreCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ScoreCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScoreExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FScoreAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FScoreAnalytics) { return 0; }
