#pragma once

#include "CoreMinimal.h"
#include "CharacterSavedProfileDataV7.h"
#include "CharacterKeyTuple.generated.h"

USTRUCT(BlueprintType)
struct FCharacterKeyTuple
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FCharacterSavedProfileDataV7 Data;

public:
	DEADBYDAYLIGHT_API FCharacterKeyTuple();
};

FORCEINLINE uint32 GetTypeHash(const FCharacterKeyTuple) { return 0; }
