#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActorVariationElements.h"
#include "ActorVariationSpawner.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UActorVariationSpawner : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> Visualization;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActorVariationElements> Variations;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UClass* _toSpawn;

public:
	UActorVariationSpawner();
};

FORCEINLINE uint32 GetTypeHash(const UActorVariationSpawner) { return 0; }
