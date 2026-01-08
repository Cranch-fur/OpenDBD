#pragma once

#include "CoreMinimal.h"
#include "Lock.generated.h"

class UObject;

USTRUCT(BlueprintType)
struct FLock
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<UObject> _lockingObject;

public:
	DEADBYDAYLIGHT_API FLock();
};

FORCEINLINE uint32 GetTypeHash(const FLock) { return 0; }
