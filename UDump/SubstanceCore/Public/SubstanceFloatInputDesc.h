#pragma once

#include "CoreMinimal.h"
#include "SubstanceInputDesc.h"
#include "SubstanceFloatInputDesc.generated.h"

USTRUCT(BlueprintType)
struct FSubstanceFloatInputDesc : public FSubstanceInputDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Default;

public:
	SUBSTANCECORE_API FSubstanceFloatInputDesc();
};

FORCEINLINE uint32 GetTypeHash(const FSubstanceFloatInputDesc) { return 0; }
