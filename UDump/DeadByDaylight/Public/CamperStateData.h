#pragma once

#include "CoreMinimal.h"
#include "CamperStateData.generated.h"

USTRUCT(BlueprintType)
struct FCamperStateData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CamperSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipedItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> EquipedItemAddonIds;

public:
	DEADBYDAYLIGHT_API FCamperStateData();
};

FORCEINLINE uint32 GetTypeHash(const FCamperStateData) { return 0; }
