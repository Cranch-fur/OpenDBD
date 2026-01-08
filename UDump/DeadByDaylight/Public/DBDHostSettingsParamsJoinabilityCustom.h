#pragma once

#include "CoreMinimal.h"
#include "DBDHostSettingsParamsBase.h"
#include "DBDHostSettingsParamsJoinabilityCustom.generated.h"

UCLASS(Blueprintable, NonTransient)
class UDBDHostSettingsParamsJoinabilityCustom : public UDBDHostSettingsParamsBase
{
	GENERATED_BODY()

public:
	UDBDHostSettingsParamsJoinabilityCustom();
};

FORCEINLINE uint32 GetTypeHash(const UDBDHostSettingsParamsJoinabilityCustom) { return 0; }
