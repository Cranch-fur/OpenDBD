#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AnimationMapping.h"
#include "AnimationMappingRow.generated.h"

USTRUCT(BlueprintType)
struct FAnimationMappingRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAnimationMapping> Mappings;

public:
	DEADBYDAYLIGHT_API FAnimationMappingRow();
};

FORCEINLINE uint32 GetTypeHash(const FAnimationMappingRow) { return 0; }
