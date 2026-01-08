#pragma once

#include "CoreMinimal.h"
#include "ObjectRandomizer.h"
#include "SpawnerElement.h"
#include "ActorSpawnerRandomizer.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UActorSpawnerRandomizer : public UObjectRandomizer
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnerElement> ActivatedSpawnerContentLists;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnerElement> DeactivatedSpawnerContentLists;

public:
	UActorSpawnerRandomizer();
};

FORCEINLINE uint32 GetTypeHash(const UActorSpawnerRandomizer) { return 0; }
