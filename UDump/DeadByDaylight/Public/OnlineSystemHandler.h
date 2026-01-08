#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineSystemHandler.generated.h"

class ADBDOnlineBeaconClient;
class UDBDServerInstance;
class AActor;

UCLASS(Blueprintable, Transient)
class UOnlineSystemHandler : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDOnlineBeaconClient* _reservationBeacon;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDServerInstance* _serverInstance;

public:
	UFUNCTION(BlueprintCallable)
	void ReservationComplete(int32 result);

	UFUNCTION(BlueprintCallable)
	void OnHostConnectionFailure();

	UFUNCTION(BlueprintCallable)
	void JoinSessionOnBeaconDestroyed(AActor* DestroyedActor);

public:
	UOnlineSystemHandler();
};

FORCEINLINE uint32 GetTypeHash(const UOnlineSystemHandler) { return 0; }
