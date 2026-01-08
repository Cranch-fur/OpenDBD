#pragma once

#include "CoreMinimal.h"
#include "MaterialMap.generated.h"

class UMaterialInterface;

USTRUCT(BlueprintType)
struct FMaterialMap
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* SrcMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DstMaterial;

public:
	DEADBYDAYLIGHT_API FMaterialMap();
};

FORCEINLINE uint32 GetTypeHash(const FMaterialMap) { return 0; }
