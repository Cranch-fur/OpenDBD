#pragma once

#include "CoreMinimal.h"
#include "ScoreEvent.h"
#include "ScoreEventArray.generated.h"

USTRUCT(BlueprintType)
struct FScoreEventArray
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FScoreEvent> Array;

public:
	DEADBYDAYLIGHT_API FScoreEventArray();
};

FORCEINLINE uint32 GetTypeHash(const FScoreEventArray) { return 0; }
