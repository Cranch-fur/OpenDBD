#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "EStatusEffectType.h"
#include "StatusEffectProperties.generated.h"

USTRUCT(BlueprintType)
struct FStatusEffectProperties : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<UStatusEffect> StatusEffectBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelToDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusEffectType StatusEffectType;

public:
	DEADBYDAYLIGHT_API FStatusEffectProperties();
};

FORCEINLINE uint32 GetTypeHash(const FStatusEffectProperties) { return 0; }
