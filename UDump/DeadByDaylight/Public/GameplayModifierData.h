#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GamePlayModifier.h"
#include "EGameplayModifierFlag.h"
#include "GameplayModifierData.generated.h"

class UGameplayModifierCondition;

USTRUCT(BlueprintType)
struct FGameplayModifierData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayModifierCondition>> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGamePlayModifier> Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EGameplayModifierFlag> Flags;

public:
	DEADBYDAYLIGHT_API FGameplayModifierData();
};

FORCEINLINE uint32 GetTypeHash(const FGameplayModifierData) { return 0; }
