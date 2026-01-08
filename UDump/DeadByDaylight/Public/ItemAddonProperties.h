#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "ItemAddonProperties.generated.h"

USTRUCT(BlueprintType)
struct FItemAddonProperties : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<UItemAddon> ItemAddonBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ParentItems;

public:
	DEADBYDAYLIGHT_API FItemAddonProperties();
};

FORCEINLINE uint32 GetTypeHash(const FItemAddonProperties) { return 0; }
