#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EDBDScoreTypes.h"
#include "EDBDScoreCategory.h"
#include "ScoreValue.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct FScoreValue : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDScoreTypes Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBDScoreCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExperienceValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxExperienceValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* ExperienceDepreciationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OngoingWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

public:
	DEADBYDAYLIGHT_API FScoreValue();
};

FORCEINLINE uint32 GetTypeHash(const FScoreValue) { return 0; }
