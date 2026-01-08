#pragma once

#include "CoreMinimal.h"
#include "CharacterValidationData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterValidationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PrestigeLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BloodWebLevel;

public:
	DEADBYDAYLIGHT_API FCharacterValidationData();
};

FORCEINLINE uint32 GetTypeHash(const FCharacterValidationData) { return 0; }
