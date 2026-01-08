#pragma once

#include "CoreMinimal.h"
#include "HelpTopicData.generated.h"

USTRUCT(BlueprintType)
struct FHelpTopicData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPath;

public:
	DEADBYDAYLIGHT_API FHelpTopicData();
};

FORCEINLINE uint32 GetTypeHash(const FHelpTopicData) { return 0; }
