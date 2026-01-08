#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "EGameplayModifierType.h"
#include "EGameplayModifierFlag.h"
#include "Firecracker.generated.h"

class UGameplayModifierContainer;
class AActor;
class ADBDPlayer;
class ACollectable;
class UPrimitiveComponent;

UCLASS(Blueprintable)
class AFirecracker : public AInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool DebugDisplayEnabled;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UGameplayModifierContainer*> _effectModifiers;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _exploded;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<AActor*> _actorsInRange;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _playerOwner;

public:
	UFUNCTION(BlueprintPure)
	bool ShouldDeafen(const ADBDPlayer* Player, float& outDuration) const;

	UFUNCTION(BlueprintPure)
	bool ShouldBlind(const ADBDPlayer* Player, float& outDuration) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFuseBurnUpdate(float DeltaSeconds, float fuseTimeLeftPercent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFuseBurnExit();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFuseBurnEnter();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnExplode();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEffectsInitialized();

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_InitFromSpawner(ACollectable* spawner);

	UFUNCTION(BlueprintPure)
	bool HasModifierOfType(EGameplayModifierType GameplayModifierType) const;

	UFUNCTION(BlueprintPure)
	bool HasFlag(EGameplayModifierFlag GameplayModifierFlag) const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetPlayerOwner() const;

	UFUNCTION(BlueprintPure)
	float GetModifierValue(EGameplayModifierType GameplayModifierType) const;

	UFUNCTION(BlueprintPure)
	float GetExplosionRange() const;

	UFUNCTION(BlueprintPure)
	float GetExplosionEffectDuration() const;

	UFUNCTION(BlueprintPure)
	float GetExplosionDelay() const;

	UFUNCTION(BlueprintPure)
	bool GetExploded() const;

protected:
	UFUNCTION(BlueprintPure)
	TArray<UGameplayModifierContainer*> GetEffects() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UPrimitiveComponent* GetEffectArea() const;

public:
	UFUNCTION(BlueprintPure)
	float GetDeafnessEffectDuration(const ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure)
	float GetBlindnessEffectDuration(const ADBDPlayer* Player) const;

public:
	AFirecracker();
};

FORCEINLINE uint32 GetTypeHash(const AFirecracker) { return 0; }
