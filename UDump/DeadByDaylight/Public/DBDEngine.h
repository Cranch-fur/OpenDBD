#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "DBDEngine.generated.h"

UCLASS(Blueprintable, NonTransient)
class DEADBYDAYLIGHT_API UDBDEngine : public UGameEngine
{
	GENERATED_BODY()

public:
	UDBDEngine();
};

FORCEINLINE uint32 GetTypeHash(const UDBDEngine) { return 0; }
