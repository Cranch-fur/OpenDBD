#pragma once

#include "CoreMinimal.h"
#include "LegacySavedBloodWebRingPersistentData.h"
#include "LegacySavedBloodWebPersistentData.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedBloodWebPersistentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, SaveGame)
	int32 VersionNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacySavedBloodWebRingPersistentData> RingData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> Paths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString EntityCurrentNode;

public:
	DEADBYDAYLIGHT_API FLegacySavedBloodWebPersistentData();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedBloodWebPersistentData) { return 0; }
