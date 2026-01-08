#pragma once

#include "CoreMinimal.h"
#include "Engine/BlendableInterface.h"
#include "UObject/NoExportTypes.h"
#include "LightPropagationVolumeSettings.h"
#include "LightPropagationVolumeBlendable.generated.h"

UCLASS(Blueprintable, MinimalAPI)
class ULightPropagationVolumeBlendable : public UObject, public IBlendableInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	FLightPropagationVolumeSettings Settings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float BlendWeight;

public:
	ULightPropagationVolumeBlendable();
};

FORCEINLINE uint32 GetTypeHash(const ULightPropagationVolumeBlendable) { return 0; }
