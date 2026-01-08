#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FearMarketItemData.h"
#include "FearMarketOfferingData.generated.h"

USTRUCT(BlueprintType)
struct FFearMarketOfferingData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFearMarketItemData> ObjectsForSale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StartTime;

public:
	DEADBYDAYLIGHT_API FFearMarketOfferingData();
};

FORCEINLINE uint32 GetTypeHash(const FFearMarketOfferingData) { return 0; }
