#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterSnappingComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UCharacterSnappingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterSnappingComponent();
};

FORCEINLINE uint32 GetTypeHash(const UCharacterSnappingComponent) { return 0; }
