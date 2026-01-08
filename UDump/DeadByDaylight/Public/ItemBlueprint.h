#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBlueprint.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AItemBlueprint : public AActor
{
	GENERATED_BODY()

public:
	AItemBlueprint();
};

FORCEINLINE uint32 GetTypeHash(const AItemBlueprint) { return 0; }
