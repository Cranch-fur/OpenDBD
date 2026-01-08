#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LightingHelper.generated.h"

class ABaseSky;
class ULightingInterpolator;
class ULightingFactory;
class UPostProcessComponent;

UCLASS(Blueprintable)
class ULightingHelper : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ABaseSky* _levelLighting;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ABaseSky* _levelLightingWithoutExposure;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ULightingInterpolator* _lightingInterpolator;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ULightingFactory* _lightingFactory;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UPostProcessComponent* _postProcessCache;

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerExposureOffset(float Value);

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ModifyExposure(float Value);

public:
	ULightingHelper();
};

FORCEINLINE uint32 GetTypeHash(const ULightingHelper) { return 0; }
