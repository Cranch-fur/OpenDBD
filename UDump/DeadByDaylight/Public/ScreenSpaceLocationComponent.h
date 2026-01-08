#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/NoExportTypes.h"
#include "ScreenSpaceLocationComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UScreenSpaceLocationComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScreenSpaceX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScreenSpaceY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FrameDelay;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FVector2D _currentResolution;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<FTransform> _previousTransforms;

public:
	UFUNCTION(BlueprintPure)
	bool HasValidPosition() const;

	UFUNCTION(BlueprintPure)
	FVector2D GetCurrentResolution() const;

public:
	UScreenSpaceLocationComponent();
};

FORCEINLINE uint32 GetTypeHash(const UScreenSpaceLocationComponent) { return 0; }
