#pragma once

#include "CoreMinimal.h"
#include "LightingSpecifics.generated.h"

USTRUCT(BlueprintType)
struct DEADBYDAYLIGHT_API FLightingSpecifics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<ABaseSky> LightBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxExposure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinExposure;

public:
	FLightingSpecifics();
};

FORCEINLINE uint32 GetTypeHash(const FLightingSpecifics) { return 0; }
