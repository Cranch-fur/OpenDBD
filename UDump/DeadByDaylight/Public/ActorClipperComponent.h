#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClippedActor.h"
#include "Engine/HitResult.h"
#include "ActorClipperComponent.generated.h"

class UShapeComponent;
class AActor;
class UPrimitiveComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UActorClipperComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UShapeComponent* Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> _tags;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<AActor*, FClippedActor> _clippedActors;

public:
	UFUNCTION(BlueprintCallable)
	void SetClippingEnabled(bool Enabled);

protected:
	UFUNCTION(BlueprintCallable)
	void OnOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UActorClipperComponent();
};

FORCEINLINE uint32 GetTypeHash(const UActorClipperComponent) { return 0; }
