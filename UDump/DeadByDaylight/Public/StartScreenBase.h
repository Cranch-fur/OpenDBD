#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "StartScreenBase.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AStartScreenBase : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TravelToMenu();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BeginDestroyTravelSequence();

public:
	AStartScreenBase();
};

FORCEINLINE uint32 GetTypeHash(const AStartScreenBase) { return 0; }
