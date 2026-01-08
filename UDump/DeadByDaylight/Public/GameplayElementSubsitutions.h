#pragma once

#include "CoreMinimal.h"
#include "EGameplayElementType.h"
#include "GameplayElementSubsitutions.generated.h"

USTRUCT(BlueprintType)
struct FGameplayElementSubsitutions
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameplayElementType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> Element;

public:
	DEADBYDAYLIGHT_API FGameplayElementSubsitutions();
};

FORCEINLINE uint32 GetTypeHash(const FGameplayElementSubsitutions) { return 0; }
