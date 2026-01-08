#pragma once

#include "CoreMinimal.h"
#include "EGameplayElementType.h"
#include "ManagedGameplayElementsPopulation.generated.h"

USTRUCT(BlueprintType)
struct FManagedGameplayElementsPopulation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameplayElementType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinPopulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPopulation;

public:
	DEADBYDAYLIGHT_API FManagedGameplayElementsPopulation();
};

FORCEINLINE uint32 GetTypeHash(const FManagedGameplayElementsPopulation) { return 0; }
