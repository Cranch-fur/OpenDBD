#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnNextStep.h"
#include "Sequencer.generated.h"

class AActor;

UCLASS(Blueprintable)
class USequencer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Transient)
	FOnNextStep OnNextStep;

public:
	UFUNCTION(BlueprintCallable)
	void Unregister(AActor* Step);

	UFUNCTION(BlueprintCallable)
	void Register(AActor* Step);

private:
	UFUNCTION(BlueprintCallable)
	void CleanSequence(AActor* DestroyedActor);

public:
	USequencer();
};

FORCEINLINE uint32 GetTypeHash(const USequencer) { return 0; }
