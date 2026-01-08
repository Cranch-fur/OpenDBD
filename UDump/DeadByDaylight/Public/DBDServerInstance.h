#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBDServerInstance.generated.h"

class UDBDHostSettingsParamsBase;
class UDBDGameInstance;

UCLASS(Blueprintable, Transient)
class UDBDServerInstance : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<UDBDHostSettingsParamsBase*> _pendingSessionUpdates;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TWeakObjectPtr<UDBDGameInstance> _gameInstance;

public:
	UDBDServerInstance();
};

FORCEINLINE uint32 GetTypeHash(const UDBDServerInstance) { return 0; }
