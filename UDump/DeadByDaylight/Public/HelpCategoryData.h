#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EHelpType.h"
#include "HelpTopicData.h"
#include "HelpCategoryData.generated.h"

USTRUCT(BlueprintType)
struct FHelpCategoryData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHelpType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHelpTopicData> Topics;

public:
	DEADBYDAYLIGHT_API FHelpCategoryData();
};

FORCEINLINE uint32 GetTypeHash(const FHelpCategoryData) { return 0; }
