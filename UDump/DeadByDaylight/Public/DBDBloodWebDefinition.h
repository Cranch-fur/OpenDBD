#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBDBloodWebDefinition.generated.h"

UCLASS(Blueprintable)
class UDBDBloodWebDefinition : public UObject
{
	GENERATED_BODY()

public:
	UDBDBloodWebDefinition();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBloodWebDefinition) { return 0; }
