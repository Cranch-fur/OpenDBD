#pragma once

#include "CoreMinimal.h"
#include "SessionInfos.h"
#include "GamePresetData.h"
#include "GamePersistentData.generated.h"

USTRUCT(BlueprintType)
struct FGamePersistentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpectatorCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSessionInfos SessionInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSessionInfos LastSessionInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGamePresetData GamePresetData;

public:
	DEADBYDAYLIGHT_API FGamePersistentData();
};

FORCEINLINE uint32 GetTypeHash(const FGamePersistentData) { return 0; }
