#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreTypes.h"
#include "TrackedActivityEvent.generated.h"

USTRUCT(BlueprintType)
struct FTrackedActivityEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDScoreTypes TrackedEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

public:
	DEADBYDAYLIGHT_API FTrackedActivityEvent();
};

FORCEINLINE uint32 GetTypeHash(const FTrackedActivityEvent) { return 0; }
