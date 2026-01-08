#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BloodwebProgressionValue.generated.h"

USTRUCT(BlueprintType)
struct FBloodwebProgressionValue : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingNodeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingNodeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingNodeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingCommonItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingCommonItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingCommonItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingUncommonItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingUncommonItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingUncommonItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingVeryRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingVeryRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingVeryRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingUltraRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingUltraRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingUltraRareItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingArtifactItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingArtifactItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingArtifactItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InnerRingSpectralItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MiddleRingSpectralItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OuterRingSpectralItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EntityStartingRound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ItemUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AddOn01Unlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AddOn02Unlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OfferingUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Perk01Unlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Perk02Unlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Perk03Unlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Perk04Unlocked;

public:
	DEADBYDAYLIGHT_API FBloodwebProgressionValue();
};

FORCEINLINE uint32 GetTypeHash(const FBloodwebProgressionValue) { return 0; }
