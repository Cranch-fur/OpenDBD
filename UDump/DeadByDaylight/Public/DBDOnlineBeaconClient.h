#pragma once

#include "CoreMinimal.h"
#include "OnlineBeacon.h"
#include "ReservationCompletedDelegate.h"
#include "DBDOnlineBeaconClient.generated.h"

class UNetConnection;

UCLASS(Blueprintable, NonTransient)
class ADBDOnlineBeaconClient : public AOnlineBeacon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FReservationCompletedDelegate OnReservationCompleted;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNetConnection* BeaconConnection;

public:
	ADBDOnlineBeaconClient();
};

FORCEINLINE uint32 GetTypeHash(const ADBDOnlineBeaconClient) { return 0; }
