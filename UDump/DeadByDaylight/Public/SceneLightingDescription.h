#pragma once

#include "CoreMinimal.h"
#include "LightingSpecifics.h"
#include "SceneLightingDescription.generated.h"

USTRUCT(BlueprintType)
struct DEADBYDAYLIGHT_API FSceneLightingDescription
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ThemeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLightingSpecifics LightingDetailsLowMedium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLightingSpecifics LightingDetailsHighUltra;

public:
	FSceneLightingDescription();
};

FORCEINLINE uint32 GetTypeHash(const FSceneLightingDescription) { return 0; }
