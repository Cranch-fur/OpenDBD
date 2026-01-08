#pragma once

#include "CoreMinimal.h"
#include "ClippedActor.generated.h"

class USceneComponent;
class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct FClippedActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Transient, Export)
	TArray<TWeakObjectPtr<USceneComponent>> DisplayComponents;

	UPROPERTY(EditInstanceOnly, Transient, Export)
	TMap<UPrimitiveComponent*, TWeakObjectPtr<UPrimitiveComponent>> CollidingPrimitives;

public:
	DEADBYDAYLIGHT_API FClippedActor();
};

FORCEINLINE uint32 GetTypeHash(const FClippedActor) { return 0; }
