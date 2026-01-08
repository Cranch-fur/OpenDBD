#pragma once

#include "CoreMinimal.h"
#include "EscapeStrategyType.h"
#include "EscapeTypeObjects.generated.h"

USTRUCT(BlueprintType)
struct FEscapeTypeObjects
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EscapeStrategyType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TAssetSubclassOf<AActor>> EscapeObjects;

public:
	DEADBYDAYLIGHT_API FEscapeTypeObjects();
};

FORCEINLINE uint32 GetTypeHash(const FEscapeTypeObjects) { return 0; }
