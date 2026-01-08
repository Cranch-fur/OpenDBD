#pragma once

#include "CoreMinimal.h"
#include "EDBDBadgeConditionGroupLogicalOperator.h"
#include "DBDBadgeRewardCondition.h"
#include "DBDBadgeRewardConditionGroup.generated.h"

USTRUCT(BlueprintType)
struct FDBDBadgeRewardConditionGroup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDBadgeConditionGroupLogicalOperator Operator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDBDBadgeRewardCondition> Conditions;

public:
	DEADBYDAYLIGHT_API FDBDBadgeRewardConditionGroup();
};

FORCEINLINE uint32 GetTypeHash(const FDBDBadgeRewardConditionGroup) { return 0; }
