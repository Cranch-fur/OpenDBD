#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBDHostSettingsParamsBase.generated.h"

UCLASS(Blueprintable, Transient)
class UDBDHostSettingsParamsBase : public UObject
{
	GENERATED_BODY()

public:
	UDBDHostSettingsParamsBase();
};

FORCEINLINE uint32 GetTypeHash(const UDBDHostSettingsParamsBase) { return 0; }
