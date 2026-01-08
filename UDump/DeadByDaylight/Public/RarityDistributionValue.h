#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RarityDistributionValue.generated.h"

USTRUCT(BlueprintType)
struct FRarityDistributionValue : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRing;

public:
	DEADBYDAYLIGHT_API FRarityDistributionValue();
};

FORCEINLINE uint32 GetTypeHash(const FRarityDistributionValue) { return 0; }
