#pragma once

#include "CoreMinimal.h"
#include "LightPropagationVolumeSettings.generated.h"

USTRUCT(BlueprintType)
struct FLightPropagationVolumeSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVDirectionalOcclusionIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVDirectionalOcclusionRadius : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVDiffuseOcclusionExponent : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVSpecularOcclusionExponent : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVDiffuseOcclusionIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVSpecularOcclusionIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVSize : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVSecondaryOcclusionIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVSecondaryBounceIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVGeometryVolumeBias : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVVplInjectionBias : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOverride_LPVEmissiveInjectionIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVVplInjectionBias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LPVSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVSecondaryOcclusionIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVSecondaryBounceIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVGeometryVolumeBias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVEmissiveInjectionIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVDirectionalOcclusionIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVDirectionalOcclusionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVDiffuseOcclusionExponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVSpecularOcclusionExponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVDiffuseOcclusionIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float LPVSpecularOcclusionIntensity;

public:
	LIGHTPROPAGATIONVOLUMERUNTIME_API FLightPropagationVolumeSettings();
};

FORCEINLINE uint32 GetTypeHash(const FLightPropagationVolumeSettings) { return 0; }
