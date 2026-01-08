#pragma once

#include "CoreMinimal.h"
#include "EGameType.h"
#include "DBDJoinParams.generated.h"

USTRUCT(BlueprintType)
struct FDBDJoinParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameType GameType;

public:
	DEADBYDAYLIGHT_API FDBDJoinParams();
};

FORCEINLINE uint32 GetTypeHash(const FDBDJoinParams) { return 0; }
