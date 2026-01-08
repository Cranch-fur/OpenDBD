#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlinePresenceExternalCommands.generated.h"

UCLASS(Blueprintable)
class ONLINEPRESENCE_API UOnlinePresenceExternalCommands : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_MirrorsWalletWithdraw(const FString& currencyName, int32 Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_MirrorsWalletGetCurrency(const FString& currencyName);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_MirrorsSaveTestValue(const FString& StateName, int32 Version);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_MirrorsGetValue(const FString& StateName);

	UFUNCTION(BlueprintCallable, Exec)
	bool DBD_IsDlcInstalled(const FString& inString);

	UFUNCTION(BlueprintCallable, Exec)
	bool DBD_GrantPromoItems();

	UFUNCTION(BlueprintCallable, Exec)
	bool DBD_GenerateTestItems();

public:
	UOnlinePresenceExternalCommands();
};

FORCEINLINE uint32 GetTypeHash(const UOnlinePresenceExternalCommands) { return 0; }
