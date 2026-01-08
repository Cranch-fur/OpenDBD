#pragma once

#include "CoreMinimal.h"
#include "DBDCharacterMovementComponent.h"
#include "CamperMovementComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UCamperMovementComponent : public UDBDCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRunSpeedFatigued;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRunSpeedInjured;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeedInjured;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDashAcceleration;

public:
	UCamperMovementComponent();
};

FORCEINLINE uint32 GetTypeHash(const UCamperMovementComponent) { return 0; }
