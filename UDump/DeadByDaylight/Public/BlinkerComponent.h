#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/NoExportTypes.h"
#include "EBlinkerState.h"
#include "BlinkerComponent.generated.h"

class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UBlinkerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FVector UpdateTargetLocation(ADBDPlayer* Player, float blinkDistance, const TArray<float>& scanPercentLocations, float raycastStartHeight, float raycastEndHeight);

	UFUNCTION(BlueprintCallable)
	void SetBlinkerState(EBlinkerState blinkerState);

	UFUNCTION(BlueprintCallable)
	FVector LocalAddRandomToTargetLocation(ADBDPlayer* Player, FVector TargetLocation, float Distance);

	UFUNCTION(BlueprintPure)
	bool IsReady() const;

	UFUNCTION(BlueprintPure)
	EBlinkerState GetBlinkerState() const;

	UFUNCTION(BlueprintPure)
	float GetAutomaticBlinkDelay(int32 chainBlinkCount) const;

public:
	UBlinkerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UBlinkerComponent) { return 0; }
