#pragma once

#include "CoreMinimal.h"
#include "WeightedItem.generated.h"

USTRUCT(BlueprintType)
struct FWeightedItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> Element;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

public:
	DEADBYDAYLIGHT_API FWeightedItem();
};

FORCEINLINE uint32 GetTypeHash(const FWeightedItem) { return 0; }
