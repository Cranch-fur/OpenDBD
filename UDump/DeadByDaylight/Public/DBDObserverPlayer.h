#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DBDObserverPlayer.generated.h"

class UDBDPlayerData;

UCLASS(Blueprintable)
class ADBDObserverPlayer : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UDBDPlayerData* _playerData;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_PlayerReady();

public:
	ADBDObserverPlayer();
};

FORCEINLINE uint32 GetTypeHash(const ADBDObserverPlayer) { return 0; }
