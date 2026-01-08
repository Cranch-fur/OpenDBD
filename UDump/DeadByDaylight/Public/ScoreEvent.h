#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreTypes.h"
#include "ScoreEvent.generated.h"

USTRUCT(BlueprintType)
struct FScoreEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDScoreTypes ScoreType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExperienceAwarded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BonusPointModifier;

public:
	DEADBYDAYLIGHT_API FScoreEvent();
};

FORCEINLINE uint32 GetTypeHash(const FScoreEvent) { return 0; }
