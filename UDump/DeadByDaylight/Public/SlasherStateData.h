#pragma once

#include "CoreMinimal.h"
#include "SlasherStateData.generated.h"

USTRUCT(BlueprintType)
struct FSlasherStateData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlasherSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipedPowerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> EquipedPowerAddonIds;

public:
	DEADBYDAYLIGHT_API FSlasherStateData();
};

FORCEINLINE uint32 GetTypeHash(const FSlasherStateData) { return 0; }
