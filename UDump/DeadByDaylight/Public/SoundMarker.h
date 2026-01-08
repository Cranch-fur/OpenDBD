#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EHudIndicatorIconType.h"
#include "EHudOffscreenIndicatorType.h"
#include "SoundMarker.generated.h"

UCLASS(Blueprintable)
class ASoundMarker : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHudIndicatorIconType HudIndicatorIconType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHudOffscreenIndicatorType HudOffscreenIndicatorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DistanceLimit;

public:
	ASoundMarker();
};

FORCEINLINE uint32 GetTypeHash(const ASoundMarker) { return 0; }
