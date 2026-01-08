#pragma once

#include "CoreMinimal.h"
#include "CharacterSavedInventoryDataV7.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSavedInventoryDataV7
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Num;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString I;

public:
	DEADBYDAYLIGHT_API FCharacterSavedInventoryDataV7();
};

FORCEINLINE uint32 GetTypeHash(const FCharacterSavedInventoryDataV7) { return 0; }
