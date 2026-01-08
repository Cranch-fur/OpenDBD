#pragma once

#include "CoreMinimal.h"
#include "SaveDataBase.h"
#include "CharacterKeyTuple.h"
#include "SavedDailyRitualContainerV7.h"
#include "SavedFearMarketOfferingInstanceV7.h"
#include "LegacySavedPlayerLoadoutData.h"
#include "SaveDataV7.generated.h"

USTRUCT(BlueprintType)
struct FSaveDataV7 : public FSaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString PlayerUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 SelectedCamperIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 SelectedSlasherIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> Offerings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool FirstTimePlaying;

	UPROPERTY(EditAnywhere, SaveGame)
	uint64 CurrentSeasonTicks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 LastConnectedCharacterIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString DisconnectPenaltyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 BonusExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 FearTokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString OngoingGameTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FCharacterKeyTuple> CharacterData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FSavedDailyRitualContainerV7 DailyRituals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FSavedFearMarketOfferingInstanceV7 FearMarket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedPlayerLoadoutData LastLoadout;

public:
	DEADBYDAYLIGHT_API FSaveDataV7();
};

FORCEINLINE uint32 GetTypeHash(const FSaveDataV7) { return 0; }
