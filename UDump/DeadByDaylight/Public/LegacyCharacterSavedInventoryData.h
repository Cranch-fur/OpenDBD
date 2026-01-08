#pragma once

#include "CoreMinimal.h"
#include "LegacyCharacterSavedInventoryData.generated.h"

USTRUCT(BlueprintType)
struct FLegacyCharacterSavedInventoryData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, SaveGame)
	uint32 Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int64 TimeObtainedSinceEpoch;

public:
	DEADBYDAYLIGHT_API FLegacyCharacterSavedInventoryData();
};

FORCEINLINE uint32 GetTypeHash(const FLegacyCharacterSavedInventoryData) { return 0; }
