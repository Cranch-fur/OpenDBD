#pragma once

#include "CoreMinimal.h"
#include "ESubstanceInputType.h"
#include "SubstanceInputDesc.generated.h"

USTRUCT(BlueprintType)
struct FSubstanceInputDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESubstanceInputType> Type;

public:
	SUBSTANCECORE_API FSubstanceInputDesc();
};

FORCEINLINE uint32 GetTypeHash(const FSubstanceInputDesc) { return 0; }
