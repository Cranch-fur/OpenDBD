#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ESubstanceGenerationMode.h"
#include "ESubstanceEngineType.h"
#include "SubstanceSettings.generated.h"

UCLASS(Blueprintable)
class SUBSTANCECORE_API USubstanceSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MemoryBudgetMb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CPUCores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AsyncLoadMipClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESubstanceGenerationMode> DefaultGenerationMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESubstanceEngineType> SubstanceEngine;

public:
	USubstanceSettings();
};

FORCEINLINE uint32 GetTypeHash(const USubstanceSettings) { return 0; }
