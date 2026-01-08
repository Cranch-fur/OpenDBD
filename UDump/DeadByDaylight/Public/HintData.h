#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EHintCategory.h"
#include "HintData.generated.h"

USTRUCT(BlueprintType)
struct FHintData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHintCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 levelMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 levelMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 relevantCharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DLCId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DLCIdPS4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DLCIdXB1;

public:
	DEADBYDAYLIGHT_API FHintData();
};

FORCEINLINE uint32 GetTypeHash(const FHintData) { return 0; }
