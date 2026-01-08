#pragma once

#include "CoreMinimal.h"
#include "BloodwebNode.h"
#include "BloodWebRingPersistentData.generated.h"

USTRUCT(BlueprintType)
struct FBloodWebRingPersistentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBloodwebNode> NodeData;

public:
	DEADBYDAYLIGHT_API FBloodWebRingPersistentData();
};

FORCEINLINE uint32 GetTypeHash(const FBloodWebRingPersistentData) { return 0; }
