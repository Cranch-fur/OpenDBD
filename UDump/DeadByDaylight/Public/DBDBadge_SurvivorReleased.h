#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "DBDBadge_SurvivorReleased.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_SurvivorReleased : public UDBDBadge
{
	GENERATED_BODY()

public:
	UDBDBadge_SurvivorReleased();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_SurvivorReleased) { return 0; }
