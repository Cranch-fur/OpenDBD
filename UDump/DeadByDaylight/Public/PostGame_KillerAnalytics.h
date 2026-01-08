#pragma once

#include "CoreMinimal.h"
#include "PostGame_KillerAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FPostGame_KillerAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BadgeFirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BadgeSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BloodPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Brutality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Deviousness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hunter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Sacrifice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EscapeesCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SacrificedCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KilledCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DisconnectCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FPostGame_KillerAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FPostGame_KillerAnalytics) { return 0; }
