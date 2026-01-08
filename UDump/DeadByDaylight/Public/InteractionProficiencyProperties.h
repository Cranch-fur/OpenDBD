#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InteractionProficiencyProperties.generated.h"

USTRUCT(BlueprintType)
struct FInteractionProficiencyProperties : public FTableRowBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ProficiencyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<UInteractionProficiency> ProficiencyBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> BuffLevelThresholds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> DebuffLevelThresholds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconFilePath;

public:
	DEADBYDAYLIGHT_API FInteractionProficiencyProperties();
};

FORCEINLINE uint32 GetTypeHash(const FInteractionProficiencyProperties) { return 0; }
