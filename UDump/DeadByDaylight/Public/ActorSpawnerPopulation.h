#pragma once

#include "CoreMinimal.h"
#include "ActorSpawnerPopulation.generated.h"

class UActorSpawner;

USTRUCT(BlueprintType)
struct FActorSpawnerPopulation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UActorSpawner*> Array;

public:
	DEADBYDAYLIGHT_API FActorSpawnerPopulation();
};

FORCEINLINE uint32 GetTypeHash(const FActorSpawnerPopulation) { return 0; }
