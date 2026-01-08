#pragma once

#include "CoreMinimal.h"
#include "Bitfield.generated.h"

USTRUCT(BlueprintType)
struct FBitfield
{
	GENERATED_BODY()

public:
	DEADBYDAYLIGHT_API FBitfield();
};

FORCEINLINE uint32 GetTypeHash(const FBitfield) { return 0; }
