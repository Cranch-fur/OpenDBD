#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreTypes.h"
#include "EPlayerRole.h"
#include "LegacySavedDailyRitualInstance.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedDailyRitualInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName RitualKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<EDBDScoreTypes> TrackedEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<int32> CharacterIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<EPlayerRole> Roles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float Progress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float Threshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float Tolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float DisplayThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float ExpReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool Rewarded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool StateChanged;

public:
	DEADBYDAYLIGHT_API FLegacySavedDailyRitualInstance();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedDailyRitualInstance) { return 0; }
