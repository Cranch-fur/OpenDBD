#pragma once

#include "CoreMinimal.h"
#include "EGameplayElementType.h"
#include "GameplayElementsPopulation.generated.h"

USTRUCT(BlueprintType)
struct FGameplayElementsPopulation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameplayElementType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinPopulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPopulation;

public:
	DEADBYDAYLIGHT_API FGameplayElementsPopulation();
};

FORCEINLINE uint32 GetTypeHash(const FGameplayElementsPopulation) { return 0; }
