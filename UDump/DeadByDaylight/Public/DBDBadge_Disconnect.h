#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "DBDBadge_Disconnect.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_Disconnect : public UDBDBadge
{
	GENERATED_BODY()

public:
	UDBDBadge_Disconnect();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_Disconnect) { return 0; }
