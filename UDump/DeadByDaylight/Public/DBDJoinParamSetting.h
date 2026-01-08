#pragma once

#include "CoreMinimal.h"
#include "DBDJoinParamSetting.generated.h"

USTRUCT(BlueprintType)
struct FDBDJoinParamSetting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value;

public:
	DEADBYDAYLIGHT_API FDBDJoinParamSetting();
};

FORCEINLINE uint32 GetTypeHash(const FDBDJoinParamSetting) { return 0; }
