#pragma once

#include "CoreMinimal.h"
#include "LegacySavedFearMarketItemInstance.h"
#include "SavedFearMarketOfferingInstanceV7.generated.h"

USTRUCT(BlueprintType)
struct FSavedFearMarketOfferingInstanceV7
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacySavedFearMarketItemInstance> ObjectsForSale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString EndTime;

public:
	DEADBYDAYLIGHT_API FSavedFearMarketOfferingInstanceV7();
};

FORCEINLINE uint32 GetTypeHash(const FSavedFearMarketOfferingInstanceV7) { return 0; }
