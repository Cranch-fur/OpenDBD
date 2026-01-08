#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Generator.generated.h"

UCLASS(Blueprintable)
class AGenerator : public AInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool activated;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FireLevelScoreEventOnFix;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetComplete(bool complete);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEscapeDoorActivated();

public:
	AGenerator();
};

FORCEINLINE uint32 GetTypeHash(const AGenerator) { return 0; }
