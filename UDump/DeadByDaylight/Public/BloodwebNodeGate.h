#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EItemRarity.h"
#include "EPlayerRole.h"
#include "EBloodwebNodeGateTypes.h"
#include "BloodwebNodeGate.generated.h"

USTRUCT(BlueprintType)
struct FBloodwebNodeGate : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBloodwebNodeGateTypes GateType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Param;

public:
	DEADBYDAYLIGHT_API FBloodwebNodeGate();
};

FORCEINLINE uint32 GetTypeHash(const FBloodwebNodeGate) { return 0; }
