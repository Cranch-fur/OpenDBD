#pragma once

#include "CoreMinimal.h"
#include "EGameType.h"
#include "EPlayerRole.h"
#include "WaitTimeAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FWaitTimeAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnterLobbyWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnterMatchWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchWaitStartUtc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchWaitStopUtc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LobbyWaitStartUtc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LobbyWaitStopUtc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameType GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PartySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FWaitTimeAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FWaitTimeAnalytics) { return 0; }
