#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InterpolationDoneEvent.h"
#include "LightingInterpolator.generated.h"

class ABaseSky;
class USkyLightComponent;
class ULightComponent;
class UExponentialHeightFogComponent;

UCLASS(Blueprintable)
class ULightingInterpolator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FInterpolationDoneEvent OnInterpolationDone;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ABaseSky* _targetLighting;

public:
	UFUNCTION(BlueprintCallable)
	static void LerpSkylight(USkyLightComponent* target, USkyLightComponent* A, USkyLightComponent* B, float Alpha);

	UFUNCTION(BlueprintCallable)
	static void LerpLight(ULightComponent* target, ULightComponent* A, ULightComponent* B, float Alpha);

	UFUNCTION(BlueprintCallable)
	static void LerpHeightFog(UExponentialHeightFogComponent* target, UExponentialHeightFogComponent* A, UExponentialHeightFogComponent* B, float Alpha);

public:
	ULightingInterpolator();
};

FORCEINLINE uint32 GetTypeHash(const ULightingInterpolator) { return 0; }
