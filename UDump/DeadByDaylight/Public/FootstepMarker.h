#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FootstepMarker.generated.h"

class ADBDPlayer;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AFootstepMarker : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Initialize(ADBDPlayer* originatingPlayer);

public:
	AFootstepMarker();
};

FORCEINLINE uint32 GetTypeHash(const AFootstepMarker) { return 0; }
