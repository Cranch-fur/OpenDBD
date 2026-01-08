#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BloodwebPrestigeModifiers.generated.h"

USTRUCT(BlueprintType)
struct FBloodwebPrestigeModifiers : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CommonItemProbabilityModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UncommonItemProbabilityModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RareItemProbabilityModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VeryRareItemProbabilityModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UltraRareItemProbabilityModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArtifactItemProbabilityModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpectralItemProbabilityModifier;

public:
	DEADBYDAYLIGHT_API FBloodwebPrestigeModifiers();
};

FORCEINLINE uint32 GetTypeHash(const FBloodwebPrestigeModifiers) { return 0; }
