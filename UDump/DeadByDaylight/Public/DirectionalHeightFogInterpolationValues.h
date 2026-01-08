#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DirectionalHeightFogInterpolationValues.generated.h"

USTRUCT(BlueprintType)
struct FDirectionalHeightFogInterpolationValues
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor FogInscatteringColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DirectionalInscatteringExponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DirectionalInscatteringStartDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DirectionalInscatteringColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogHeightFalloff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogMaxOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceUntilFlatFog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceUntilNoFog;

public:
	DEADBYDAYLIGHT_API FDirectionalHeightFogInterpolationValues();
};

FORCEINLINE uint32 GetTypeHash(const FDirectionalHeightFogInterpolationValues) { return 0; }
