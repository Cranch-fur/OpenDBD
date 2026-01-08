#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "DBDBadge_Quit.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_Quit : public UDBDBadge
{
	GENERATED_BODY()

public:
	UDBDBadge_Quit();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_Quit) { return 0; }
