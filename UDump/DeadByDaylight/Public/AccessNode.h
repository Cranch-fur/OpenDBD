#pragma once

#include "CoreMinimal.h"
#include "AccessNode.generated.h"

USTRUCT(BlueprintType)
struct FAccessNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AccessCost;

public:
	DEADBYDAYLIGHT_API FAccessNode();
};

FORCEINLINE uint32 GetTypeHash(const FAccessNode) { return 0; }
