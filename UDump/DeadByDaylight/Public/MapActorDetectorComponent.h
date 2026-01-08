#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnMapActorDetected.h"
#include "EMapActorCategory.h"
#include "Engine/HitResult.h"
#include "MapActorDetectorComponent.generated.h"

class UPrimitiveComponent;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UMapActorDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnMapActorDetected OnActorDetected;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EMapActorCategory> CollectedCategories;

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
	UMapActorDetectorComponent();
};

FORCEINLINE uint32 GetTypeHash(const UMapActorDetectorComponent) { return 0; }
