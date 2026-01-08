#pragma once

#include "CoreMinimal.h"
#include "LobbiesSearchMessageDefinition.generated.h"

USTRUCT(BlueprintType)
struct FLobbiesSearchMessageDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MoreThanCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SurvivorMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText KillerMessage;

public:
	DEADBYDAYLIGHT_API FLobbiesSearchMessageDefinition();
};

FORCEINLINE uint32 GetTypeHash(const FLobbiesSearchMessageDefinition) { return 0; }
