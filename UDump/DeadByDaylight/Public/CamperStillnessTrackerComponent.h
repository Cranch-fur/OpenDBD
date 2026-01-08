#pragma once

#include "CoreMinimal.h"
#include "StillnessTrackerComponent.h"
#include "Templates/SubclassOf.h"
#include "Engine/HitResult.h"
#include "CamperStillnessTrackerComponent.generated.h"

class ACamperExposerInstance;
class UExposerInteriorZoneComponent;
class UPrimitiveComponent;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UCamperStillnessTrackerComponent : public UStillnessTrackerComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ExposerSpawnTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ExposerDespawnTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChickenShitModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoudNoiseTime;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TSubclassOf<ACamperExposerInstance> _exposerInstanceToSpawn;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ACamperExposerInstance*> _exposerInstances;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UExposerInteriorZoneComponent* _interiorZone;

protected:
	UFUNCTION(BlueprintCallable)
	void OnOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintCallable)
	void ExitInteriorZone(UExposerInteriorZoneComponent* zone);

	UFUNCTION(BlueprintCallable)
	void EnterInteriorZone(UExposerInteriorZoneComponent* zone);

public:
	UCamperStillnessTrackerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UCamperStillnessTrackerComponent) { return 0; }
