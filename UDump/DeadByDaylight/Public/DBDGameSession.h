#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "DBDGameSession.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBDGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReservationTimeout;

public:
	ADBDGameSession();
};

FORCEINLINE uint32 GetTypeHash(const ADBDGameSession) { return 0; }
