#pragma once

#include "CoreMinimal.h"
#include "RankDefinition.generated.h"

USTRUCT(BlueprintType)
struct FRankDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkullsRequired;

public:
	DEADBYDAYLIGHT_API FRankDefinition();
};

FORCEINLINE uint32 GetTypeHash(const FRankDefinition) { return 0; }
