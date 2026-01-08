#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TunableValue.generated.h"

USTRUCT(BlueprintType)
struct FTunableValue : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DescriptorTags;

public:
	DEADBYDAYLIGHT_API FTunableValue();
};

FORCEINLINE uint32 GetTypeHash(const FTunableValue) { return 0; }
