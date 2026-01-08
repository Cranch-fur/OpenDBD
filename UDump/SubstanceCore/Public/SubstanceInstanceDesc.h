#pragma once

#include "CoreMinimal.h"
#include "SubstanceInputDesc.h"
#include "SubstanceInstanceDesc.generated.h"

USTRUCT(BlueprintType)
struct FSubstanceInstanceDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSubstanceInputDesc> Inputs;

public:
	SUBSTANCECORE_API FSubstanceInstanceDesc();
};

FORCEINLINE uint32 GetTypeHash(const FSubstanceInstanceDesc) { return 0; }
