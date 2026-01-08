#pragma once

#include "CoreMinimal.h"
#include "CharacterValidationData.h"
#include "SaveValidationData.generated.h"

USTRUCT(BlueprintType)
struct FSaveValidationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FCharacterValidationData> CharacterData;

public:
	DEADBYDAYLIGHT_API FSaveValidationData();
};

FORCEINLINE uint32 GetTypeHash(const FSaveValidationData) { return 0; }
