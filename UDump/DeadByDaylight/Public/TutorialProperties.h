#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ETutorialTypes.h"
#include "TutorialProperties.generated.h"

USTRUCT(BlueprintType)
struct FTutorialProperties : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETutorialTypes Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Path;

public:
	DEADBYDAYLIGHT_API FTutorialProperties();
};

FORCEINLINE uint32 GetTypeHash(const FTutorialProperties) { return 0; }
