#pragma once

#include "CoreMinimal.h"
#include "BlockingNode.generated.h"

USTRUCT(BlueprintType)
struct FBlockingNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BlockingCount;

public:
	DEADBYDAYLIGHT_API FBlockingNode();
};

FORCEINLINE uint32 GetTypeHash(const FBlockingNode) { return 0; }
