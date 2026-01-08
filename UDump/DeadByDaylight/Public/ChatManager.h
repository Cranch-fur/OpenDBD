#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChatManager.generated.h"

class UDBDGameInstance;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AChatManager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_DisplayMessage(int32 playerIndex, const FString& PlayerName, const FString& msg);

public:
	AChatManager();
};

FORCEINLINE uint32 GetTypeHash(const AChatManager) { return 0; }
