#pragma once

#include "CoreMinimal.h"
#include "EGameplayModifierType.h"
#include "GamePlayModifier.generated.h"

USTRUCT(BlueprintType)
struct FGamePlayModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameplayModifierType ModifierType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ModifierValue;

public:
	DEADBYDAYLIGHT_API FGamePlayModifier();
};

FORCEINLINE uint32 GetTypeHash(const FGamePlayModifier) { return 0; }
