#pragma once

#include "CoreMinimal.h"
#include "EItemRarity.h"
#include "EPlayerRole.h"
#include "EBloodwebNodeGateTypes.h"
#include "LegacySavedBloodwebNodeGate.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedBloodwebNodeGate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EBloodwebNodeGateTypes GateType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Param;

public:
	DEADBYDAYLIGHT_API FLegacySavedBloodwebNodeGate();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedBloodwebNodeGate) { return 0; }
