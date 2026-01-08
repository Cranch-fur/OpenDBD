#pragma once

#include "CoreMinimal.h"
#include "BadgeAnalyticEntry.generated.h"

USTRUCT(BlueprintType)
struct FBadgeAnalyticEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

public:
	DEADBYDAYLIGHT_API FBadgeAnalyticEntry();
};

FORCEINLINE uint32 GetTypeHash(const FBadgeAnalyticEntry) { return 0; }
