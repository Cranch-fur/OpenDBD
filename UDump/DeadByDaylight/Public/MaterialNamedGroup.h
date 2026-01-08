#pragma once

#include "CoreMinimal.h"
#include "MaterialMap.h"
#include "MaterialNamedGroup.generated.h"

USTRUCT(BlueprintType)
struct FMaterialNamedGroup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GroupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialMap> Maps;

public:
	DEADBYDAYLIGHT_API FMaterialNamedGroup();
};

FORCEINLINE uint32 GetTypeHash(const FMaterialNamedGroup) { return 0; }
