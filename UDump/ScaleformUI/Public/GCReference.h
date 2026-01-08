#pragma once

#include "CoreMinimal.h"
#include "GCReference.generated.h"

class UObject;

USTRUCT(BlueprintType)
struct FGCReference
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RefCount;

public:
	SCALEFORMUI_API FGCReference();
};

FORCEINLINE uint32 GetTypeHash(const FGCReference) { return 0; }
