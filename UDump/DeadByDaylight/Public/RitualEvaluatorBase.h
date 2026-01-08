#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EDBDScoreTypes.h"
#include "RitualEvaluatorBase.generated.h"

class AActor;

UCLASS(Blueprintable)
class URitualEvaluatorBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EvaluateEvent(EDBDScoreTypes event, AActor* Instigator, AActor* target);

public:
	URitualEvaluatorBase();
};

FORCEINLINE uint32 GetTypeHash(const URitualEvaluatorBase) { return 0; }
