#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratorDrivenActor.generated.h"

class AGenerator;

UCLASS(Blueprintable)
class AGeneratorDrivenActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _activated;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	AGenerator* _generator;

public:
	UFUNCTION(BlueprintCallable)
	void SetSpawnedObject(AGenerator* Generator);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInitGeneratorDrivenActor(AGenerator* Generator);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnActivateGeneratorDrivenActor();

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_ActivationChecked(bool activated);

public:
	AGeneratorDrivenActor();
};

FORCEINLINE uint32 GetTypeHash(const AGeneratorDrivenActor) { return 0; }
