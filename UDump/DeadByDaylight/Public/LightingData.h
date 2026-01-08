#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SceneLightingDescription.h"
#include "LightingData.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ULightingData : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TAssetSubclassOf<ABaseSky> DefaultLighting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FSceneLightingDescription> Descriptions;

public:
	ULightingData();
};

FORCEINLINE uint32 GetTypeHash(const ULightingData) { return 0; }
