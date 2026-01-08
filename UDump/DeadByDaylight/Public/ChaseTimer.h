#pragma once

#include "CoreMinimal.h"
#include "DBDTimer.h"
#include "ChaseTimer.generated.h"

class ACamperPlayer;

USTRUCT(BlueprintType)
struct FChaseTimer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer CooldownTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	float TotalChaseTime;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	ACamperPlayer* camper;

public:
	DEADBYDAYLIGHT_API FChaseTimer();
};

FORCEINLINE uint32 GetTypeHash(const FChaseTimer) { return 0; }
