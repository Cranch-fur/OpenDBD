#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ObjectRandomizer.generated.h"

class UActorSpawner;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UObjectRandomizer : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UActorSpawner*> Spawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinPopulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPopulation;

public:
	UObjectRandomizer();
};

FORCEINLINE uint32 GetTypeHash(const UObjectRandomizer) { return 0; }
