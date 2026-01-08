#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BeMaterialVector.generated.h"

USTRUCT(BlueprintType)
struct FBeMaterialVector
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Vector;

public:
	BEUTIL_API FBeMaterialVector();
};

FORCEINLINE uint32 GetTypeHash(const FBeMaterialVector) { return 0; }
