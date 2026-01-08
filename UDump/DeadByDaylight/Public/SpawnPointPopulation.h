#pragma once

#include "CoreMinimal.h"
#include "SpawnPointPopulation.generated.h"

class UTileSpawnPoint;

USTRUCT(BlueprintType)
struct FSpawnPointPopulation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UTileSpawnPoint*> Array;

public:
	DEADBYDAYLIGHT_API FSpawnPointPopulation();
};

FORCEINLINE uint32 GetTypeHash(const FSpawnPointPopulation) { return 0; }
