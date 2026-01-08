#pragma once

#include "CoreMinimal.h"
#include "ArrayOfTiles.generated.h"

class ATile;

USTRUCT(BlueprintType)
struct FArrayOfTiles
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<ATile*> ArrayOfTiles;

public:
	DEADBYDAYLIGHT_API FArrayOfTiles();
};

FORCEINLINE uint32 GetTypeHash(const FArrayOfTiles) { return 0; }
