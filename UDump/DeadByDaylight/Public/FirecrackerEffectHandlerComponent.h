#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnFirecrackerInRangeBeginEvent.h"
#include "OnFirecrackerInRangeUpdateEvent.h"
#include "OnFirecrackerInRangeEndEvent.h"
#include "FirecrackerEffectData.h"
#include "FirecrackerEffectHandlerComponent.generated.h"

class AFirecracker;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UFirecrackerEffectHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnFirecrackerInRangeBeginEvent OnFirecrackerInRangeBegin;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnFirecrackerInRangeUpdateEvent OnFirecrackerInRangeUpdate;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnFirecrackerInRangeEndEvent OnFirecrackerInRangeEnd;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FFirecrackerEffectData> _effectDataList;

public:
	UFUNCTION(BlueprintCallable)
	void SetFirecrackerInRange(AFirecracker* Firecracker, bool inRange);

private:
	UFUNCTION(BlueprintCallable)
	void OnFirecrackerDestroyed(AActor* DestroyedActor);

public:
	UFirecrackerEffectHandlerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UFirecrackerEffectHandlerComponent) { return 0; }
