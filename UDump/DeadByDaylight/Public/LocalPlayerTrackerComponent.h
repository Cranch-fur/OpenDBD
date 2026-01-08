#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LocalPlayerTrackerDelegate.h"
#include "LocalPlayerTrackerComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API ULocalPlayerTrackerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FLocalPlayerTrackerDelegate TrackerOnLocallyObservedChanged;

public:
	UFUNCTION(BlueprintCallable)
	void TriggerOnLocallyObservedChanged();

public:
	ULocalPlayerTrackerComponent();
};

FORCEINLINE uint32 GetTypeHash(const ULocalPlayerTrackerComponent) { return 0; }
