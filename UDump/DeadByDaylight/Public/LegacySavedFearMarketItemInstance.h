#pragma once

#include "CoreMinimal.h"
#include "LegacySavedFearMarketItemInstance.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedFearMarketItemInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 ExperienceConversion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool Purchased;

public:
	DEADBYDAYLIGHT_API FLegacySavedFearMarketItemInstance();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedFearMarketItemInstance) { return 0; }
