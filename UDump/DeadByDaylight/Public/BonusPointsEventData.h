#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BonusPointsEventData.generated.h"

USTRUCT(BlueprintType)
struct FBonusPointsEventData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Multiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EndTime;

public:
	DEADBYDAYLIGHT_API FBonusPointsEventData();
};

FORCEINLINE uint32 GetTypeHash(const FBonusPointsEventData) { return 0; }
