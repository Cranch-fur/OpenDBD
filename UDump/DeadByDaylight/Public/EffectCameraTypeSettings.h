#pragma once

#include "CoreMinimal.h"
#include "EffectCameraTypeSettings.generated.h"

USTRUCT(BlueprintType)
struct FEffectCameraTypeSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CategoryName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool VisibilityInFirstPerson;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool VisibilityInThirdPerson;

public:
	DEADBYDAYLIGHT_API FEffectCameraTypeSettings();
};

FORCEINLINE uint32 GetTypeHash(const FEffectCameraTypeSettings) { return 0; }
