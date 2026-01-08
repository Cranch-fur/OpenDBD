#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectsLocatorTargets.h"
#include "EffectsLocator.generated.h"

class UMaterialInstanceDynamic;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AEffectsLocator : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectsLocatorTargets> AffectedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActorCaptureRadius;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnLostActor(AActor* lostActor);

	UFUNCTION(BlueprintCallable)
	void OnLeaveCollisionArea(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFoundActor(AActor* foundActor);

	UFUNCTION(BlueprintCallable)
	void OnEnterCollisionArea(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnActorTick(UMaterialInstanceDynamic* actorMaterialInstanceDynamic);

public:
	AEffectsLocator();
};

FORCEINLINE uint32 GetTypeHash(const AEffectsLocator) { return 0; }
