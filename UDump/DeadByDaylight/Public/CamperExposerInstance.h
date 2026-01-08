#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CamperExposerInstance.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ACamperExposerInstance : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool IsInterior;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	float AggravationLevel;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartSpawnSequence();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartExitSequence();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetIsInteriorBP(bool interior);

public:
	UFUNCTION(BlueprintCallable)
	void ExitSequenceComplete();

public:
	ACamperExposerInstance();
};

FORCEINLINE uint32 GetTypeHash(const ACamperExposerInstance) { return 0; }
