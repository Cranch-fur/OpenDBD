#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EItemRarity.h"
#include "BloodwebChest.generated.h"

USTRUCT(BlueprintType)
struct FBloodwebChest : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> GivenItemRarity;

public:
	DEADBYDAYLIGHT_API FBloodwebChest();
};

FORCEINLINE uint32 GetTypeHash(const FBloodwebChest) { return 0; }
