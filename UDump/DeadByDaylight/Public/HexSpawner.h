#pragma once

#include "CoreMinimal.h"
#include "ActorSpawner.h"
#include "HexSpawner.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UHexSpawner : public UActorSpawner
{
	GENERATED_BODY()

public:
	UHexSpawner();
};

FORCEINLINE uint32 GetTypeHash(const UHexSpawner) { return 0; }
