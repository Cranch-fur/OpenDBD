#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "PlayerStateMachine.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UPlayerStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:
	UPlayerStateMachine();
};

FORCEINLINE uint32 GetTypeHash(const UPlayerStateMachine) { return 0; }
