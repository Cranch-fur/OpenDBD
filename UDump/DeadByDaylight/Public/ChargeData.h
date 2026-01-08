#pragma once

#include "CoreMinimal.h"
#include "ChargeData.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FChargeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	AActor* Instigator;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	float ChargeAmount;

public:
	DEADBYDAYLIGHT_API FChargeData();
};

FORCEINLINE uint32 GetTypeHash(const FChargeData) { return 0; }
