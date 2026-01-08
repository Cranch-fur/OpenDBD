#pragma once

#include "CoreMinimal.h"
#include "DBDPlayer.h"
#include "ChaseEndDelegate.h"
#include "ChaseStartDelegate.h"
#include "OffensiveActionDelegate.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "EKillerMoodInfluence.h"
#include "EKillerAbilities.h"
#include "ChaseTimer.h"
#include "DBDTimer.h"
#include "ESlasherGuidedAction.h"
#include "EAttackZoneSet.h"
#include "Engine/HitResult.h"
#include "EAttackType.h"
#include "EDetectionZone.h"
#include "EStunType.h"
#include "EBlindType.h"
#include "SlasherPlayer.generated.h"

class ACamperExposerInstance;
class UCurveFloat;
class UAkComponent;
class ACamperPlayer;
class UDBDPawnSensingComponent;
class UStillnessTrackerComponent;
class UItemAddon;
class ULoudNoiseHUDIndicator;
class UFlashLightReceiverComponent;
class UChargeableComponent;
class AActor;
class APawn;
class UPhysicalMaterial;

UCLASS(Blueprintable)
class ASlasherPlayer : public ADBDPlayer
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChaseEndDelegate OnChaseEnded;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChaseStartDelegate OnChaseStarted;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOffensiveActionDelegate OnOffensiveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PutDownTraceLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefaultSlasherPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoudNoiseDisplayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TerrorRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetPriorityFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACamperExposerInstance> CamperExposer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKillerMoodInfluence KillerMoodInfluence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EKillerAbilities> KillerAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowKillerPowerDebugInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TempUseRageMechanic;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* AttackForwardInputSmoothingCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Export)
	TWeakObjectPtr<UAkComponent> _audioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _attackAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _slasherStealthRatio;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	float _slasherLightIntensity;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	ACamperPlayer* _guidedCamper;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TMap<ADBDPlayer*, FChaseTimer> _chaseTimers;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TMap<ADBDPlayer*, FChaseTimer> _stalkTimers;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UDBDPawnSensingComponent* _camperSensor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UStillnessTrackerComponent* _stillnessTracker;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=UpdateLoadoutFromInventory, Transient, Instanced)
	TArray<UItemAddon*> _powerAddons;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer _recentlyCloakedTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer _flashlightBlindEvasionScoreTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer _flashlightBurnoutEvasionScoreTimer;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	float _stalkTierWalkSpeedMultiplier;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	ULoudNoiseHUDIndicator* _loudNoiseIndicator;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UFlashLightReceiverComponent* _fullBodyFlashLightReceiver;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UChargeableComponent* _invisibilityBurnoutChargeableComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	float _stealthDelay;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	float _stealthIncreaseRate;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	float _stealthDecreaseRate;

	UPROPERTY(EditInstanceOnly, Transient)
	FDateTime _stealthStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _isKilling;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	float _ragePercent;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateSlasherStealthRatio(float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateRageTierEffect(int32 previousTier, int32 currentTier);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TriggerPutDownAnimation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TriggerPickUpAnimation();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnSlasherPower();

public:
	UFUNCTION(BlueprintCallable)
	void SetStealthRatio(float stealthRatio);

	UFUNCTION(BlueprintCallable)
	void SetStalkTierWalkSpeedMultiplier(float stalkTierWalkSpeedMultiplier);

	UFUNCTION(BlueprintCallable)
	void SetLightIntensity(float Intensity);

	UFUNCTION(BlueprintCallable)
	void SetIsKilling(bool isKilling);

	UFUNCTION(BlueprintCallable)
	void SetIsInBlinkCooldown(bool IsInBlinkCooldown);

	UFUNCTION(BlueprintCallable)
	void SetIsChainBlinking(bool IsChainBlinking);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetInteractingCamperBlueprint(ACamperPlayer* camper);

	UFUNCTION(BlueprintCallable)
	void SetGuidedCamper(ACamperPlayer* camper);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetGuidedAction(ESlasherGuidedAction Action);

	UFUNCTION(BlueprintCallable)
	void SetChainsawSprinting(bool chainsawSprinting);

	UFUNCTION(BlueprintCallable)
	void SetBeingWiggled(bool Value);

	UFUNCTION(BlueprintCallable)
	void SetBaseTerrorRadius(float baseTerrorRadius);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetAttackDetectionZoneSet(EAttackZoneSet attackZoneSet);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetLungeType(int32 lungeType);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendAttackInput(bool Pressed);

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_AllowKillingAfterHooking(int32 numKills);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_AllowKilling(int32 numKills);

private:
	UFUNCTION(BlueprintCallable)
	void OnSurvivorsLeftChanged(int32 survivorRemaining);

public:
	UFUNCTION(BlueprintCallable)
	void OnSurvivorKilled(ACamperPlayer* playerToKill, bool consumeKill);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSuccessfulInterruption(ACamperPlayer* interruptedSurvivor);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnStalkModeChanged(bool stalkMode);

private:
	UFUNCTION(BlueprintCallable)
	void OnSoundTriggered(AActor* originator, FVector Location, bool shouldTrack, float audibleRange);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnPickupStart_BP();

private:
	UFUNCTION(BlueprintCallable)
	void OnLoudNoiseIndicatorDestroyed();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnKillerAbilityUpdateActivate(EKillerAbilities KillerAbility, float percent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnKillerAbilityEndDeactivate(EKillerAbilities KillerAbility, bool Forced);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnKillerAbilityEndActivate(EKillerAbilities KillerAbility);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnKillerAbilityBeginDeactivate(EKillerAbilities KillerAbility, bool Forced);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnKillerAbilityBeginActivate(EKillerAbilities KillerAbility);

private:
	UFUNCTION(BlueprintCallable)
	void OnInvisibilityChargeEmptied();

	UFUNCTION(BlueprintCallable)
	void OnInvisibilityBurnoutChargeCompletionChanged(bool COMPLETED);

public:
	UFUNCTION(BlueprintCallable)
	void OnChaseTargetFound(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCamperHit(const FHitResult& Hit);

private:
	UFUNCTION(BlueprintCallable)
	void OnBlindChargeEmptied();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetLungeType(int32 lungeType);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetGuidedAction(ESlasherGuidedAction Action);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetAttackDetectionZoneSet(EAttackZoneSet attackZoneSet);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_OnChaseStart(ACamperPlayer* camper);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_OnChaseEnd(ACamperPlayer* camper, float chaseTime);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_DisplayAttackZones(bool Display);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_CamperHitAesthetic(ACamperPlayer* camper, bool causedKO);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_AllowKillingAfterHooking(int32 numKills);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_AllowKilling(int32 numKills);

public:
	UFUNCTION(BlueprintCallable)
	void Local_RequestAttack(EAttackType attackType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsUncloaking() const;

	UFUNCTION(BlueprintPure)
	bool IsInChase() const;

	UFUNCTION(BlueprintPure)
	bool IsInBlinkCooldown() const;

	UFUNCTION(BlueprintPure)
	bool IsHooking() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCloaking() const;

	UFUNCTION(BlueprintPure)
	bool IsChainsawSprinting() const;

	UFUNCTION(BlueprintPure)
	bool IsChainBlinking() const;

	UFUNCTION(BlueprintPure)
	bool IsCarrying() const;

	UFUNCTION(BlueprintPure)
	bool IsASurvivorInTerrorRadius() const;

	UFUNCTION(BlueprintPure)
	bool IsAllowedToKill(ACamperPlayer* camper) const;

	UFUNCTION(BlueprintPure)
	bool HasKillerAbility(EKillerAbilities KillerAbility) const;

	UFUNCTION(BlueprintPure)
	float GetTerrorRadius() const;

	UFUNCTION(BlueprintPure)
	float GetStealthRatio() const;

	UFUNCTION(BlueprintPure)
	float GetStalkTierWalkSpeedMultiplier() const;

	UFUNCTION(BlueprintPure)
	int32 GetRageTierFromRagePercent(float ragePercent) const;

	UFUNCTION(BlueprintPure)
	int32 GetRageTier() const;

	UFUNCTION(BlueprintPure)
	float GetRagePercent() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetPlayerDropOffPoint() const;

	UFUNCTION(BlueprintPure)
	ACamperPlayer* GetObsessionTarget() const;

	UFUNCTION(BlueprintPure)
	FRotator GetLookRotation() const;

	UFUNCTION(BlueprintPure)
	float GetLongestChaseDuration() const;

	UFUNCTION(BlueprintPure)
	float GetLightIntensity() const;

	UFUNCTION(BlueprintPure)
	TArray<UItemAddon*> GetItemAddons() const;

	UFUNCTION(BlueprintPure)
	ACamperPlayer* GetInteractingCamper() const;

	UFUNCTION(BlueprintPure)
	ACamperPlayer* GetGuidedCamper() const;

	UFUNCTION(BlueprintPure)
	ESlasherGuidedAction GetGuidedAction() const;

	UFUNCTION(BlueprintPure)
	float GetBaseTerrorRadius() const;

	UFUNCTION(BlueprintPure)
	float GetAnimDirection() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DisplayAttackZones(bool Display);

	UFUNCTION(BlueprintPure)
	bool DetectObstruction(UPhysicalMaterial*& physicalMaterialOut, FVector& positionOut, FVector& normalOut, EDetectionZone detectionZoneID) const;

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetAttackZones(EAttackZoneSet attackZoneSet);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DisplayAttackZones(bool Display);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AllowKilling();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanPerformKillerAbility(EKillerAbilities KillerAbility) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanGainRage() const;

	UFUNCTION(BlueprintCallable)
	bool CanEscapeCarry(ACamperPlayer* Player);

	UFUNCTION(BlueprintPure)
	bool CanBeBlinded() const;

	UFUNCTION(BlueprintCallable)
	void CameraUpdated();

	UFUNCTION(BlueprintPure=false, BlueprintCallable)
	void BroadcastOffensiveAction() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_StopBlinding(AActor* effector);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetRagePercent(float ragePercent);

	UFUNCTION(BlueprintCallable)
	void Authority_RequestStun(EStunType stunType, ADBDPlayer* Requester);

	UFUNCTION(BlueprintCallable)
	void Authority_Blinded(EBlindType blindType, float coolDown, AActor* effector);

	UFUNCTION(BlueprintCallable)
	void Authority_ApplyDamageToTarget(ADBDPlayer* target, bool forceCauseKO);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_AllowKilling(int32 numKills);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_AddRagePercent(float ragePercent);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_AddInvisibilityBurnoutCharge(ADBDPlayer* blinder, float charge, AActor* effector);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ASlasherPlayer();
};

FORCEINLINE uint32 GetTypeHash(const ASlasherPlayer) { return 0; }
