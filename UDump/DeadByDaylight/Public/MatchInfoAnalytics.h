#pragma once

#include "CoreMinimal.h"
#include "EGameType.h"
#include "EPlayerRole.h"
#include "MatchInfoAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FMatchInfoAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameType GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PartySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Prestige;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Pips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MapSeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FMatchInfoAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FMatchInfoAnalytics) { return 0; }
