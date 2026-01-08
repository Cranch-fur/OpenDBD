#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TrackedActivityEvent.h"
#include "ActivityTrackerComponent.generated.h"

class ADBDPlayerState;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UActivityTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTrackedActivityEvent> TrackedEvents;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TWeakObjectPtr<ADBDPlayerState> _player;

public:
	UActivityTrackerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UActivityTrackerComponent) { return 0; }
