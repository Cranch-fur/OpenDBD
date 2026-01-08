#pragma once

#include "CoreMinimal.h"
#include "BadgeProgressionAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FBadgeProgressionAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BadgeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BadgeValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FBadgeProgressionAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FBadgeProgressionAnalytics) { return 0; }
