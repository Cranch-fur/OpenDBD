#pragma once

#include "CoreMinimal.h"
#include "SessionInfos.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfos
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FSessionInfos();
};

FORCEINLINE uint32 GetTypeHash(const FSessionInfos) { return 0; }
