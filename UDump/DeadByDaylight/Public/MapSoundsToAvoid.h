#pragma once

#include "CoreMinimal.h"
#include "MapSoundsToAvoid.generated.h"

class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct DEADBYDAYLIGHT_API FMapSoundsToAvoid
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UPrimitiveComponent* Component;

public:
	FMapSoundsToAvoid();
};

FORCEINLINE uint32 GetTypeHash(const FMapSoundsToAvoid) { return 0; }
