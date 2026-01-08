#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "MaterialReplacerData.h"
#include "BPAttachementSocketData.h"
#include "CustomizationItemData.generated.h"

USTRUCT(BlueprintType)
struct FCustomizationItemData : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AssociatedCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PrestigeUlockLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PrestigeUnlockDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PartCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialReplacerData> MaterialsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDefaultItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBPAttachementSocketData> AttachementSocketBlueprints;

public:
	DEADBYDAYLIGHT_API FCustomizationItemData();
};

FORCEINLINE uint32 GetTypeHash(const FCustomizationItemData) { return 0; }
