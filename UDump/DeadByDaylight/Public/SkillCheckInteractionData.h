#pragma once

#include "CoreMinimal.h"
#include "SkillCheckInteractionData.generated.h"

USTRUCT(BlueprintType)
struct FSkillCheckInteractionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitAreaStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitAreaLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BonusAreaLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Progress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHexed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 xOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 yOffset;

public:
	DEADBYDAYLIGHT_API FSkillCheckInteractionData();
};

FORCEINLINE uint32 GetTypeHash(const FSkillCheckInteractionData) { return 0; }
