#pragma once

#include "CoreMinimal.h"
#include "SelectedOffering.generated.h"

USTRUCT(BlueprintType)
struct FSelectedOffering
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ownerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OfferingName;

public:
	DEADBYDAYLIGHT_API FSelectedOffering();
};

FORCEINLINE uint32 GetTypeHash(const FSelectedOffering) { return 0; }
