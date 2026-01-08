#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBDDebugInputComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBDDebugInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBDDebugInputComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDDebugInputComponent) { return 0; }
