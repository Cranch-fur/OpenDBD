#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnActorDetected.h"
#include "Engine/HitResult.h"
#include "ActorDetectorComponent.generated.h"

class UPrimitiveComponent;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UActorDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnActorDetected OnActorDetected;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UClass*> CollectedTypes;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UPrimitiveComponent* _primitive;

public:
	UFUNCTION(BlueprintCallable)
	void SetDetectionPrimitive(UPrimitiveComponent* Primitive);

protected:
	UFUNCTION()
	void OnOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UActorDetectorComponent();
};

FORCEINLINE uint32 GetTypeHash(const UActorDetectorComponent) { return 0; }
