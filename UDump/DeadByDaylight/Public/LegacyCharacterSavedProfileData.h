#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LegacySavedBloodWebPersistentData.h"
#include "LegacySavedPlayerLoadoutData.h"
#include "LegacyCharacterSavedInventoryData.h"
#include "LegacyCharacterSavedProfileData.generated.h"

USTRUCT(BlueprintType)
struct FLegacyCharacterSavedProfileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 VersionNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 BloodPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 BloodWebLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 PrestigeLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 TimesConfronted;

	UPROPERTY(EditAnywhere, SaveGame)
	TArray<FDateTime> PrestigeDates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedBloodWebPersistentData BloodWebData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedPlayerLoadoutData CharacterLoadoutData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacyCharacterSavedInventoryData> InventoryData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> CurrentCustomization;

public:
	DEADBYDAYLIGHT_API FLegacyCharacterSavedProfileData();
};

FORCEINLINE uint32 GetTypeHash(const FLegacyCharacterSavedProfileData) { return 0; }
