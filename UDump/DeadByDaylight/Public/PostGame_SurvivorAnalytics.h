#pragma once

#include "CoreMinimal.h"
#include "ECamperDamageState.h"
#include "PostGame_SurvivorAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FPostGame_SurvivorAnalytics
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
	int32 Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Survival;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Altruism;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Boldness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Outcome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECamperDamageState DamageState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HookedCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FPostGame_SurvivorAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FPostGame_SurvivorAnalytics) { return 0; }
