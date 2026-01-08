#pragma once

#include "CoreMinimal.h"
#include "CharacterSavedInventoryData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSavedInventoryData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	uint32 Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

public:
	DEADBYDAYLIGHT_API FCharacterSavedInventoryData();
};

FORCEINLINE uint32 GetTypeHash(const FCharacterSavedInventoryData) { return 0; }
