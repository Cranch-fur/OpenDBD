#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBDEasyAntiCheat.generated.h"

class UDBDGameInstance;

UCLASS(Blueprintable)
class UDBDEasyAntiCheat : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _connectedToServer;

public:
	UDBDEasyAntiCheat();
};

FORCEINLINE uint32 GetTypeHash(const UDBDEasyAntiCheat) { return 0; }
