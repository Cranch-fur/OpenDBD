#pragma once

#include "CoreMinimal.h"
#include "ActorSpawner.h"
#include "LightFunctionActorSpawner.generated.h"

class UTexture;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class ULightFunctionActorSpawner : public UActorSpawner
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* LightFunctionTexture;

public:
	ULightFunctionActorSpawner();
};

FORCEINLINE uint32 GetTypeHash(const ULightFunctionActorSpawner) { return 0; }
