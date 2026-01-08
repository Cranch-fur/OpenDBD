#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ESubstanceGenerationMode.h"
#include "SubstanceInstanceFactory.generated.h"

UCLASS(Blueprintable, MinimalAPI)
class USubstanceInstanceFactory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESubstanceGenerationMode> GenerationMode;

public:
	USubstanceInstanceFactory();
};

FORCEINLINE uint32 GetTypeHash(const USubstanceInstanceFactory) { return 0; }
