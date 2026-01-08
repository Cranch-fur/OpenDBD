#pragma once

#include "CoreMinimal.h"
#include "AnimationMontageDescriptor.h"
#include "MontagePlaybackDefinition.generated.h"

USTRUCT(BlueprintType)
struct FMontagePlaybackDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnimationMontageDescriptor Descriptor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Follower;

public:
	DEADBYDAYLIGHT_API FMontagePlaybackDefinition();
};

FORCEINLINE uint32 GetTypeHash(const FMontagePlaybackDefinition) { return 0; }
