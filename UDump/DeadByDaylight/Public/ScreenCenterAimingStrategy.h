#pragma once

#include "CoreMinimal.h"
#include "AimingStrategyInterface.h"
#include "Components/ActorComponent.h"
#include "ScreenCenterAimingStrategy.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UScreenCenterAimingStrategy : public UActorComponent, public IAimingStrategyInterface
{
	GENERATED_BODY()

public:
	UScreenCenterAimingStrategy();
};

FORCEINLINE uint32 GetTypeHash(const UScreenCenterAimingStrategy) { return 0; }
