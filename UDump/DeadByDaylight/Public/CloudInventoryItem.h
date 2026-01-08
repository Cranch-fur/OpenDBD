#pragma once

#include "CoreMinimal.h"
#include "CloudInventoryItem.generated.h"

USTRUCT(BlueprintType)
struct FCloudInventoryItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefinitionId;

	UPROPERTY(EditAnywhere)
	uint64 InstanceId;

public:
	DEADBYDAYLIGHT_API FCloudInventoryItem();
};

FORCEINLINE uint32 GetTypeHash(const FCloudInventoryItem) { return 0; }
