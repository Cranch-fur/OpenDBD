#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimationMontageDescriptor.h"
#include "AnimationMontageSlave.generated.h"

class UMontagePlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UAnimationMontageSlave : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetLeader(UMontagePlayer* leader);

private:
	UFUNCTION(BlueprintCallable)
	void OnMontageStopped(const FAnimationMontageDescriptor ID);

	UFUNCTION(BlueprintCallable)
	void OnMontageStarted(const FAnimationMontageDescriptor ID, const float Rate);

public:
	UAnimationMontageSlave();
};

FORCEINLINE uint32 GetTypeHash(const UAnimationMontageSlave) { return 0; }
