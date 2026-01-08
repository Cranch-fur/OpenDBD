#pragma once

#include "CoreMinimal.h"
#include "EItemRarity.h"
#include "LegacySavedBloodwebChest.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedBloodwebChest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<float> GivenItemRarity;

public:
	DEADBYDAYLIGHT_API FLegacySavedBloodwebChest();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedBloodwebChest) { return 0; }
