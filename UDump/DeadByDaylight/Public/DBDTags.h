#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBDTags.generated.h"

UCLASS(Blueprintable)
class UDBDTags : public UObject
{
	GENERATED_BODY()

public:
	UDBDTags();
};

FORCEINLINE uint32 GetTypeHash(const UDBDTags) { return 0; }
