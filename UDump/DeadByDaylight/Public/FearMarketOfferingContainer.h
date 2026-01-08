#pragma once

#include "CoreMinimal.h"
#include "FearMarketOfferingInstance.h"
#include "FearMarketOfferingContainer.generated.h"

USTRUCT(BlueprintType)
struct FFearMarketOfferingContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFearMarketOfferingInstance> AvailableOfferings;

public:
	DEADBYDAYLIGHT_API FFearMarketOfferingContainer();
};

FORCEINLINE uint32 GetTypeHash(const FFearMarketOfferingContainer) { return 0; }
