#pragma once

#include "CoreMinimal.h"
#include "FearMarketItemInstance.h"
#include "UObject/NoExportTypes.h"
#include "FearMarketOfferingInstance.generated.h"

USTRUCT(BlueprintType)
struct FFearMarketOfferingInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFearMarketItemInstance> ObjectsForSale;

	UPROPERTY(EditAnywhere)
	FDateTime StartTime;

	UPROPERTY(EditAnywhere)
	FDateTime EndTime;

public:
	DEADBYDAYLIGHT_API FFearMarketOfferingInstance();
};

FORCEINLINE uint32 GetTypeHash(const FFearMarketOfferingInstance) { return 0; }
