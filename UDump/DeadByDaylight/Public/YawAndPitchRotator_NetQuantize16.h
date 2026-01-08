#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YawAndPitchRotator_NetQuantize16.generated.h"

USTRUCT(BlueprintType)
struct FYawAndPitchRotator_NetQuantize16
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FRotator _value;

public:
	DEADBYDAYLIGHT_API FYawAndPitchRotator_NetQuantize16();
};

FORCEINLINE uint32 GetTypeHash(const FYawAndPitchRotator_NetQuantize16) { return 0; }
