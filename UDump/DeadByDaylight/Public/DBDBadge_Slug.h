#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "DBDBadge_Slug.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_Slug : public UDBDBadge
{
	GENERATED_BODY()

public:
	UDBDBadge_Slug();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_Slug) { return 0; }
