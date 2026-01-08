#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BloodWebPersistentData.h"
#include "PlayerLoadoutData.h"
#include "CharacterSavedInventoryData.h"
#include "CharacterSavedProfileData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSavedProfileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BloodWebLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PrestigeLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimesConfronted;

	UPROPERTY(EditAnywhere)
	TArray<FDateTime> PrestigeEarnedDates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBloodWebPersistentData BloodWebData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerLoadoutData CharacterLoadoutData;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterSavedInventoryData> InventoryData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CurrentCustomization;

public:
	DEADBYDAYLIGHT_API FCharacterSavedProfileData();
};

FORCEINLINE uint32 GetTypeHash(const FCharacterSavedProfileData) { return 0; }
