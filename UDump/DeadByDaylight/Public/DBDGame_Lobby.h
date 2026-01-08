#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DBDGame_Lobby.generated.h"

UCLASS(Blueprintable, NonTransient)
class ADBDGame_Lobby : public AGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ForceStartMatch();

public:
	ADBDGame_Lobby();
};

FORCEINLINE uint32 GetTypeHash(const ADBDGame_Lobby) { return 0; }
