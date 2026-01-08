#pragma once

#include "CoreMinimal.h"
#include "BloodwebNodeProperties.h"
#include "BloodwebNodeGate.h"
#include "EBloodwebNodeState.h"
#include "BloodwebChest.h"
#include "BloodwebNode.generated.h"

USTRUCT(BlueprintType)
struct FBloodwebNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBloodwebNodeProperties Properties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBloodwebNodeGate> Gates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBloodwebNodeState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NodeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ContentId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBloodwebChest BloodwebChest;

public:
	DEADBYDAYLIGHT_API FBloodwebNode();
};

FORCEINLINE uint32 GetTypeHash(const FBloodwebNode) { return 0; }
