#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreTypes.h"
#include "EDBDValueEqualityCondition.h"
#include "DBDBadgeRewardCondition.generated.h"

USTRUCT(BlueprintType)
struct FDBDBadgeRewardCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDScoreTypes ScoreType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDValueEqualityCondition EqualityCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

public:
	DEADBYDAYLIGHT_API FDBDBadgeRewardCondition();
};

FORCEINLINE uint32 GetTypeHash(const FDBDBadgeRewardCondition) { return 0; }
