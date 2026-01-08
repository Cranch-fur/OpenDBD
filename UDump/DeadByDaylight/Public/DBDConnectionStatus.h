#pragma once

#include "CoreMinimal.h"
#include "DBDConnectionStatus.generated.h"

USTRUCT(BlueprintType)
struct FDBDConnectionStatus
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FDBDConnectionStatus();
};

FORCEINLINE uint32 GetTypeHash(const FDBDConnectionStatus) { return 0; }
