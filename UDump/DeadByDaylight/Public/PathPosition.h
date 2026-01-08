#pragma once

#include "CoreMinimal.h"
#include "PathPosition.generated.h"

USTRUCT(BlueprintType)
struct FPathPosition
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FPathPosition();
};

FORCEINLINE uint32 GetTypeHash(const FPathPosition) { return 0; }
