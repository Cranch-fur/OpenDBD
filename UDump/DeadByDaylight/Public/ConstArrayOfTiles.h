#pragma once

#include "CoreMinimal.h"
#include "ConstArrayOfTiles.generated.h"

class ATile;

USTRUCT(BlueprintType)
struct FConstArrayOfTiles
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<ATile*> ArrayOfTiles;

public:
	DEADBYDAYLIGHT_API FConstArrayOfTiles();
};

FORCEINLINE uint32 GetTypeHash(const FConstArrayOfTiles) { return 0; }
