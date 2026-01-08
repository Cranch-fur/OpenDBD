#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BloodwebNodeGateDistribution.generated.h"

USTRUCT(BlueprintType)
struct FBloodwebNodeGateDistribution : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;

public:
	DEADBYDAYLIGHT_API FBloodwebNodeGateDistribution();
};

FORCEINLINE uint32 GetTypeHash(const FBloodwebNodeGateDistribution) { return 0; }
