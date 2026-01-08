#pragma once

#include "CoreMinimal.h"
#include "EServerJoiningDataType.h"
#include "DBDJoinParamSetting.h"
#include "ServerJoiningData.generated.h"

USTRUCT(BlueprintType)
struct FServerJoiningData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	uint64 SessionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SessionIdStr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EServerJoiningDataType Step;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDBDJoinParamSetting> Settings;

public:
	DEADBYDAYLIGHT_API FServerJoiningData();
};

FORCEINLINE uint32 GetTypeHash(const FServerJoiningData) { return 0; }
