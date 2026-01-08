#pragma once

#include "CoreMinimal.h"
#include "Float_NetQuantize8.generated.h"

USTRUCT(BlueprintType)
struct FFloat_NetQuantize8
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _value;

public:
	DEADBYDAYLIGHT_API FFloat_NetQuantize8();
};

FORCEINLINE uint32 GetTypeHash(const FFloat_NetQuantize8) { return 0; }
