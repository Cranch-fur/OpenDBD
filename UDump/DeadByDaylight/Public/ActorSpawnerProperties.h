#pragma once

#include "CoreMinimal.h"
#include "ActorSpawnerProperties.generated.h"

USTRUCT(BlueprintType)
struct FActorSpawnerProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> SceneElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

public:
	DEADBYDAYLIGHT_API FActorSpawnerProperties();
};

FORCEINLINE uint32 GetTypeHash(const FActorSpawnerProperties) { return 0; }
