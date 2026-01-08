#pragma once

#include "CoreMinimal.h"
#include "DBDBadge_AllCamperProximity.h"
#include "DBDBadge_SurvivorStealthy.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_SurvivorStealthy : public UDBDBadge_AllCamperProximity
{
	GENERATED_BODY()

public:
	UDBDBadge_SurvivorStealthy();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_SurvivorStealthy) { return 0; }
