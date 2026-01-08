#pragma once

#include "CoreMinimal.h"
#include "ETileVariation.h"
#include "ActorVariationElements.generated.h"

USTRUCT(BlueprintType)
struct FActorVariationElements
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileVariation Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> Element;

public:
	DEADBYDAYLIGHT_API FActorVariationElements();
};

FORCEINLINE uint32 GetTypeHash(const FActorVariationElements) { return 0; }
