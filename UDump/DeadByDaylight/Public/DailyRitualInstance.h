#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreTypes.h"
#include "EPlayerRole.h"
#include "DailyRitualInstance.generated.h"

USTRUCT(BlueprintType)
struct FDailyRitualInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RitualKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EDBDScoreTypes> TrackedEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> CharacterIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EPlayerRole> Roles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Progress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Threshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Tolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisplayThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExpReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Rewarded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StateChanged;

public:
	DEADBYDAYLIGHT_API FDailyRitualInstance();
};

FORCEINLINE uint32 GetTypeHash(const FDailyRitualInstance) { return 0; }
