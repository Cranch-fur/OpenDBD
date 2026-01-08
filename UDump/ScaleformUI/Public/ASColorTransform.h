#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ASColorTransform.generated.h"

USTRUCT(BlueprintType)
struct SCALEFORMUI_API FASColorTransform
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Multiply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Add;

public:
	FASColorTransform();
};

FORCEINLINE uint32 GetTypeHash(const FASColorTransform) { return 0; }
