#pragma once

#include "CoreMinimal.h"
#include "EGameState.h"
#include "EPlayerRole.h"
#include "PlayerstateDataCache.h"
#include "CachedPlayerScoreData.generated.h"

USTRUCT(BlueprintType)
struct FCachedPlayerScoreData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SteamId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerRank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameState PlayerGameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IsMyScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipedFavorId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> EquipedPerks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> EquipedPerkLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquippedItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> EquippedItemAddons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerstateDataCache PlayerstateDataCache;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillerGoalPoints;

public:
	DEADBYDAYLIGHT_API FCachedPlayerScoreData();
};

FORCEINLINE uint32 GetTypeHash(const FCachedPlayerScoreData) { return 0; }
