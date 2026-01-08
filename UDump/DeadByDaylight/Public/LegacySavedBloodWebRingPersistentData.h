#pragma once

#include "CoreMinimal.h"
#include "LegacySavedBloodwebNode.h"
#include "LegacySavedBloodWebRingPersistentData.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedBloodWebRingPersistentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacySavedBloodwebNode> NodeData;

public:
	DEADBYDAYLIGHT_API FLegacySavedBloodWebRingPersistentData();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedBloodWebRingPersistentData) { return 0; }
