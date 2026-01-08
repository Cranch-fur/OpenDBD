#pragma once

#include "CoreMinimal.h"
#include "BlueprintLiveStreamInfo.generated.h"

USTRUCT(BlueprintType)
struct FBlueprintLiveStreamInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GameName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StreamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString URL;

public:
	GAMELIVESTREAMING_API FBlueprintLiveStreamInfo();
};

FORCEINLINE uint32 GetTypeHash(const FBlueprintLiveStreamInfo) { return 0; }
