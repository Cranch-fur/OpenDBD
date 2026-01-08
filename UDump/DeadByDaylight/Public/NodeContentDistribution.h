#pragma once

#include "CoreMinimal.h"
#include "NodeContentDistribution.generated.h"

USTRUCT(BlueprintType)
struct FNodeContentDistribution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCount;

public:
	DEADBYDAYLIGHT_API FNodeContentDistribution();
};

FORCEINLINE uint32 GetTypeHash(const FNodeContentDistribution) { return 0; }
