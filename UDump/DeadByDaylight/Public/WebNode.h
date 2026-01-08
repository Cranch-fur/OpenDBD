#pragma once

#include "CoreMinimal.h"
#include "WebNode.generated.h"

USTRUCT(BlueprintType)
struct FWebNode
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FWebNode();
};

FORCEINLINE uint32 GetTypeHash(const FWebNode) { return 0; }
