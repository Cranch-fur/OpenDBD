#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InteractionPlayerProperties.generated.h"

class ADBDPlayer;

USTRUCT(BlueprintType)
struct FInteractionPlayerProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AverageVelocityAtStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaAtStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerPositionAtStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADBDPlayer* Requester;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SnapPositionAtStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SnapRotationAtStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SnapDistanceAtStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SnapTimeAtStart;

public:
	DEADBYDAYLIGHT_API FInteractionPlayerProperties();
};

FORCEINLINE uint32 GetTypeHash(const FInteractionPlayerProperties) { return 0; }
