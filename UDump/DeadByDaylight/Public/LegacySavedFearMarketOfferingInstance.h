#pragma once

#include "CoreMinimal.h"
#include "LegacySavedFearMarketItemInstance.h"
#include "UObject/NoExportTypes.h"
#include "LegacySavedFearMarketOfferingInstance.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedFearMarketOfferingInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacySavedFearMarketItemInstance> ObjectsForSale;

	UPROPERTY(EditAnywhere, SaveGame)
	FDateTime StartTime;

	UPROPERTY(EditAnywhere, SaveGame)
	FDateTime EndTime;

public:
	DEADBYDAYLIGHT_API FLegacySavedFearMarketOfferingInstance();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedFearMarketOfferingInstance) { return 0; }
