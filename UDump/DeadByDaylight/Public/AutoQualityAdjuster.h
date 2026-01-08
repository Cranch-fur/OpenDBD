#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoQualityAdjuster.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AAutoQualityAdjuster : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetResolutionScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumMS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumMS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleIncreaseRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleDecreaseRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumAllowableScale;

public:
	AAutoQualityAdjuster();
};

FORCEINLINE uint32 GetTypeHash(const AAutoQualityAdjuster) { return 0; }
