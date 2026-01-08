#pragma once

#include "CoreMinimal.h"
#include "FearMarketItemData.generated.h"

USTRUCT(BlueprintType)
struct FFearMarketItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExperienceConversion;

public:
	DEADBYDAYLIGHT_API FFearMarketItemData();
};

FORCEINLINE uint32 GetTypeHash(const FFearMarketItemData) { return 0; }
