#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "DBDBadge_KillerCamper.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_KillerCamper : public UDBDBadge
{
	GENERATED_BODY()

public:
	UDBDBadge_KillerCamper();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_KillerCamper) { return 0; }
