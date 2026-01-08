#pragma once

#include "CoreMinimal.h"
#include "DBDBadge_AllCamperComparator.h"
#include "DBDBadge_AllCamperProximity.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_AllCamperProximity : public UDBDBadge_AllCamperComparator
{
	GENERATED_BODY()

public:
	UDBDBadge_AllCamperProximity();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_AllCamperProximity) { return 0; }
