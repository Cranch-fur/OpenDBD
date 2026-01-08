#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EInventoryItemType.h"
#include "LevelUpDetails.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpDetails : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Prestige;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AssociatedCharacter;

public:
	DEADBYDAYLIGHT_API FLevelUpDetails();
};

FORCEINLINE uint32 GetTypeHash(const FLevelUpDetails) { return 0; }
