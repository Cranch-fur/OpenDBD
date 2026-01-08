#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EGateType.h"
#include "GateFactory.generated.h"

class AGate;

UCLASS(Blueprintable)
class AGateFactory : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	AGate* GetGate(EGateType GateType);

public:
	AGateFactory();
};

FORCEINLINE uint32 GetTypeHash(const AGateFactory) { return 0; }
