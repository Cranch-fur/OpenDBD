#pragma once

#include "CoreMinimal.h"
#include "LegacySavedBloodwebNodeProperties.h"
#include "LegacySavedBloodwebNodeGate.h"
#include "EBloodwebNodeState.h"
#include "LegacySavedBloodwebChest.h"
#include "LegacySavedBloodwebNode.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedBloodwebNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedBloodwebNodeProperties Properties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacySavedBloodwebNodeGate> Gates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EBloodwebNodeState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString NodeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName ContentId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacySavedBloodwebChest BloodwebChest;

public:
	DEADBYDAYLIGHT_API FLegacySavedBloodwebNode();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedBloodwebNode) { return 0; }
