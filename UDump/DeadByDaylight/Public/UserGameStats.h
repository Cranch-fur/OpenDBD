#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreCategory.h"
#include "UserGameStats.generated.h"

USTRUCT(BlueprintType)
struct FUserGameStats
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool Disconnected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FearTokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skulls;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TMap<EDBDScoreCategory, int32> BonusExperience;

public:
	DEADBYDAYLIGHT_API FUserGameStats();
};

FORCEINLINE uint32 GetTypeHash(const FUserGameStats) { return 0; }
