#pragma once

#include "CoreMinimal.h"
#include "DBDCharacterMovementComponent.h"
#include "SlasherMovementComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class USlasherMovementComponent : public UDBDCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USlasherMovementComponent();
};

FORCEINLINE uint32 GetTypeHash(const USlasherMovementComponent) { return 0; }
