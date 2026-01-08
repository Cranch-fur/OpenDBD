#pragma once

#include "CoreMinimal.h"
#include "EBloodwebNodeContentType.h"
#include "EItemRarity.h"
#include "LegacySavedBloodwebNodeProperties.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedBloodwebNodeProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EBloodwebNodeContentType ContentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FString> Tags;

public:
	DEADBYDAYLIGHT_API FLegacySavedBloodwebNodeProperties();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedBloodwebNodeProperties) { return 0; }
