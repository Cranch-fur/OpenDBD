#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuthenticationInfo.h"
#include "DBDAuthentication.generated.h"

class UDBDGameInstance;

UCLASS(Blueprintable)
class UDBDAuthentication : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FAuthenticationInfo> _localInfos;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FAuthenticationInfo> _remoteInfos;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

public:
	UDBDAuthentication();
};

FORCEINLINE uint32 GetTypeHash(const UDBDAuthentication) { return 0; }
