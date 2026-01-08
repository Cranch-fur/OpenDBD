#pragma once

#include "CoreMinimal.h"
#include "CharacterSavedProfileData.h"
#include "DailyRitualContainer.h"
#include "FearMarketOfferingInstance.h"
#include "PlayerLoadoutData.h"
#include "UObject/NoExportTypes.h"
#include "PlayerSavedProfileDataLocal.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSavedProfileDataLocal
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Tokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Offerings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FirstTimePlaying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Wins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Losses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FCharacterSavedProfileData> CharacterData;

	UPROPERTY(EditAnywhere)
	uint64 CurrentSeasonTicks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDailyRitualContainer DailyRituals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFearMarketOfferingInstance FearMarket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerLoadoutData LastConnectedLoadout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LastConnectedCharacterIndex;

	UPROPERTY(EditAnywhere)
	FDateTime DisconnectPenaltyTime;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _bonusExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _fearTokens;

	UPROPERTY(EditAnywhere)
	FDateTime _ongoingGameTime;

public:
	DEADBYDAYLIGHT_API FPlayerSavedProfileDataLocal();
};

FORCEINLINE uint32 GetTypeHash(const FPlayerSavedProfileDataLocal) { return 0; }
