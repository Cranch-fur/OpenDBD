#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BloodwebNodeCost.generated.h"

USTRUCT(BlueprintType)
struct FBloodwebNodeCost : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

public:
	DEADBYDAYLIGHT_API FBloodwebNodeCost();
};

FORCEINLINE uint32 GetTypeHash(const FBloodwebNodeCost) { return 0; }
