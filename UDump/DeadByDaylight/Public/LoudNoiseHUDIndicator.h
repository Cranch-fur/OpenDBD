#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LoudNoiseIndicatorData.h"
#include "LoudNoiseAddedEvent.h"
#include "UObject/NoExportTypes.h"
#include "LoudNoiseHUDIndicator.generated.h"

class UStaticMesh;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API ULoudNoiseHUDIndicator : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<FLoudNoiseIndicatorData> TrackedNoises;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* MaskMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DistortionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EdgePadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CornerRoundness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScreenFadeOutDistance;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FLoudNoiseAddedEvent LoudNoiseAddedEvent;

public:
	UFUNCTION(BlueprintCallable)
	void AddTrackedNoise(FVector Location, float Duration);

public:
	ULoudNoiseHUDIndicator();
};

FORCEINLINE uint32 GetTypeHash(const ULoudNoiseHUDIndicator) { return 0; }
