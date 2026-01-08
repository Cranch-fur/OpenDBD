#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "MeatHookStateMachine.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UMeatHookStateMachine : public UStateMachine
{
	GENERATED_BODY()

public:
	UMeatHookStateMachine();
};

FORCEINLINE uint32 GetTypeHash(const UMeatHookStateMachine) { return 0; }
