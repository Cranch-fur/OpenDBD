#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "EItemRarity.h"
#include "PerkProperties.generated.h"

USTRUCT(BlueprintType)
struct FPerkProperties : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AssociatedPlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MandatoryOnBloodweblevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeachableOnBloodweblevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<UPerk> PerkBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize)
	TArray<EItemRarity> PerkLevelRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize)
	FText PerkLevel1Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize)
	FText PerkLevel2Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize)
	FText PerkLevel3Description;

public:
	DEADBYDAYLIGHT_API FPerkProperties();
};

FORCEINLINE uint32 GetTypeHash(const FPerkProperties) { return 0; }
