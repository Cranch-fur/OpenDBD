#pragma once

#include "CoreMinimal.h"
#include "FearMarketItemInstance.generated.h"

USTRUCT(BlueprintType)
struct FFearMarketItemInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExperienceConversion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Purchased;

public:
	DEADBYDAYLIGHT_API FFearMarketItemInstance();
};

FORCEINLINE uint32 GetTypeHash(const FFearMarketItemInstance) { return 0; }
