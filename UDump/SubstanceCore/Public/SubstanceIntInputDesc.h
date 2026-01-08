#pragma once

#include "CoreMinimal.h"
#include "SubstanceInputDesc.h"
#include "SubstanceIntInputDesc.generated.h"

USTRUCT(BlueprintType)
struct FSubstanceIntInputDesc : public FSubstanceInputDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Default;

public:
	SUBSTANCECORE_API FSubstanceIntInputDesc();
};

FORCEINLINE uint32 GetTypeHash(const FSubstanceIntInputDesc) { return 0; }
