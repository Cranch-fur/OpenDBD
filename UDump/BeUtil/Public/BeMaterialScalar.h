#pragma once

#include "CoreMinimal.h"
#include "BeMaterialScalar.generated.h"

USTRUCT(BlueprintType)
struct FBeMaterialScalar
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scalar;

public:
	BEUTIL_API FBeMaterialScalar();
};

FORCEINLINE uint32 GetTypeHash(const FBeMaterialScalar) { return 0; }
