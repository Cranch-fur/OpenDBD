#pragma once

#include "CoreMinimal.h"
#include "LegacySavedBloodWebPersistentData.h"
#include "LegacySavedPlayerLoadoutData.h"
#include "CharacterSavedInventoryDataV7.h"
#include "CharacterSavedProfileDataV7.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSavedProfileDataV7
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 BloodWebLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 PrestigeLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 TimesConfronted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FString> PrestigeDates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedBloodWebPersistentData BloodWebData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedPlayerLoadoutData CharacterLoadoutData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FCharacterSavedInventoryDataV7> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> CurrentCustomization;

public:
	DEADBYDAYLIGHT_API FCharacterSavedProfileDataV7();
};

FORCEINLINE uint32 GetTypeHash(const FCharacterSavedProfileDataV7) { return 0; }
