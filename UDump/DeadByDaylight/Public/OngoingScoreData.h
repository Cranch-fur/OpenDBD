#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreTypes.h"
#include "OngoingScoreData.generated.h"

USTRUCT(BlueprintType)
struct FOngoingScoreData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDScoreTypes ScoreType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSinceScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ApplyScoreTimeout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccumulatedPercent;

public:
	DEADBYDAYLIGHT_API FOngoingScoreData();
};

FORCEINLINE uint32 GetTypeHash(const FOngoingScoreData) { return 0; }
