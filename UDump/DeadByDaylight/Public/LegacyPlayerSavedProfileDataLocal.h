#pragma once

#include "CoreMinimal.h"
#include "LegacyCharacterSavedProfileData.h"
#include "LegacySavedDailyRitualContainer.h"
#include "LegacySavedFearMarketOfferingInstance.h"
#include "LegacySavedPlayerLoadoutData.h"
#include "UObject/NoExportTypes.h"
#include "LegacyPlayerSavedProfileDataLocal.generated.h"

USTRUCT(BlueprintType)
struct FLegacyPlayerSavedProfileDataLocal
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Tokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> Offerings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool FirstTimePlaying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Wins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Losses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TMap<int32, FLegacyCharacterSavedProfileData> CharacterData;

	UPROPERTY(EditAnywhere, SaveGame)
	uint64 CurrentSeasonTicks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedDailyRitualContainer DailyRituals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedFearMarketOfferingInstance FearMarket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedPlayerLoadoutData LastConnectedLoadout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 LastConnectedCharacterIndex;

	UPROPERTY(EditAnywhere, SaveGame)
	FDateTime DisconnectPenaltyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 _experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 _bonusExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 _unclampedExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 _fearTokens;

	UPROPERTY(EditAnywhere, SaveGame)
	FDateTime _ongoingGameTime;

public:
	DEADBYDAYLIGHT_API FLegacyPlayerSavedProfileDataLocal();
};

FORCEINLINE uint32 GetTypeHash(const FLegacyPlayerSavedProfileDataLocal) { return 0; }
