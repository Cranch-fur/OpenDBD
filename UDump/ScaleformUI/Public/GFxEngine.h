#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GCReference.h"
#include "GFxEngine.generated.h"

UCLASS(Blueprintable, MinimalAPI, Transient)
class UGFxEngine : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGCReference> GCReferences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RefCount;

public:
	UGFxEngine();
};

FORCEINLINE uint32 GetTypeHash(const UGFxEngine) { return 0; }
