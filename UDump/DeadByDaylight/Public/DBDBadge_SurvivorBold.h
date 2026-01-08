#pragma once

#include "CoreMinimal.h"
#include "DBDBadge_AllCamperProximity.h"
#include "DBDBadge_SurvivorBold.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_SurvivorBold : public UDBDBadge_AllCamperProximity
{
	GENERATED_BODY()

public:
	UDBDBadge_SurvivorBold();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_SurvivorBold) { return 0; }
