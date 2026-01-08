#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Triggered.h"
#include "TriggerReset.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"
#include "TriggerableWorldObjectComponent.generated.h"

class UPrimitiveComponent;
class AActor;
class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UTriggerableWorldObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FTriggered OnTriggered;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FTriggerReset OnTriggerReset;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _triggerResetTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _randomChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _rollFrequency;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Export, meta=(AllowPrivateAccess=true))
	TWeakObjectPtr<UPrimitiveComponent> _conditionalTriggerVolume;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Export, meta=(AllowPrivateAccess=true))
	TWeakObjectPtr<UPrimitiveComponent> _guaranteedTriggerVolume;

public:
	UFUNCTION(BlueprintCallable)
	void SetGuaranteedTriggerVolume(UPrimitiveComponent* Volume);

	UFUNCTION(BlueprintCallable)
	void SetConditionalTriggerVolume(UPrimitiveComponent* Volume);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_OnTriggerReset();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_OnTriggered(AActor* Instigator, FVector triggerLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetConditionalModifiers(ADBDPlayer* Player, float& outAdditive, float& outMultiplicative);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanPlayerTriggerGuaranteed(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanPlayerTriggerConditional(ADBDPlayer* Player);

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_Trigger(AActor* Instigator, FVector triggerLocation);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_ResetTrigger();

protected:
	UFUNCTION()
	void Authority_GuaranteedVolumeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

public:
	UTriggerableWorldObjectComponent();
};

FORCEINLINE uint32 GetTypeHash(const UTriggerableWorldObjectComponent) { return 0; }
