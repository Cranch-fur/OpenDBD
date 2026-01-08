#pragma once

#include "CoreMinimal.h"
#include "LegacySavedPlayerLoadoutData.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedPlayerLoadoutData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> ItemAddOns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> CamperPerks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<int32> CamperPerkLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName CamperFavor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> PowerAddOns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> SlasherPerks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<int32> SlasherPerkLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName SlasherFavor;

public:
	DEADBYDAYLIGHT_API FLegacySavedPlayerLoadoutData();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedPlayerLoadoutData) { return 0; }
