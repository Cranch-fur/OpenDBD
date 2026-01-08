#pragma once

#include "CoreMinimal.h"
#include "DailyRitualLevel.generated.h"

USTRUCT(BlueprintType)
struct FDailyRitualLevel
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Threshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Tolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DisplayTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExpReward;

public:
	DEADBYDAYLIGHT_API FDailyRitualLevel();
};

FORCEINLINE uint32 GetTypeHash(const FDailyRitualLevel) { return 0; }
