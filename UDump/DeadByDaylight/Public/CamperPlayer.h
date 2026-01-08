#pragma once

#include "CoreMinimal.h"
#include "DBDPlayer.h"
#include "OnPickedUpEvent.h"
#include "OnHealedEvent.h"
#include "EGender.h"
#include "OnChaseEndEvent.h"
#include "DBDTimer.h"
#include "UObject/NoExportTypes.h"
#include "CameraAttachment.h"
#include "ECamperImmobilizeState.h"
#include "ECamperGuidedAction.h"
#include "ECamperDamageState.h"
#include "Engine/HitResult.h"
#include "EAuthoritativeMovementFlag.h"
#include "CamperPlayer.generated.h"

class UBoxOcclusionQueryComponent;
class UStalkedComponent;
class UCurveFloat;
class UCameraComponent;
class UCamperStillnessTrackerComponent;
class AActor;
class USphereComponent;
class UCapsuleComponent;
class UDBDPawnSensingComponent;
class UChargeableComponent;
class USceneComponent;
class UPrimitiveComponent;
class APawn;
class UInteractionDefinition;
class UDBDSpringArmComponent;
class UAnimationMontageSlave;
class UDBDClipRegionComponent;
class UDBDCamperData;

UCLASS(Blueprintable)
class ACamperPlayer : public ADBDPlayer
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnPickedUpEvent OnPickedUpDelegate;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnHealedEvent OnHealedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMashCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMashDecayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CameraSocketName_Hooked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StruggleMashDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WiggleMashDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UBoxOcclusionQueryComponent* _pixelCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStalkedComponent* StalkedComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraResetToleranceYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraResetTolerancePitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraRecenterOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraRecenterOffsetPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGender Gender;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnChaseEndEvent OnChaseEnded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* DropStaggerDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* DropStaggerMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeforDeathWhileCrawling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeforDeathWhileHooked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondsUntilFootprintTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TriggerAfflictionHUDIntro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UCameraComponent* Camera;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UCamperStillnessTrackerComponent* _stillnessTracker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PartiallyHiddenStillnessThreshold;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	AActor* _escape;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	AActor* _overlappingEscape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USphereComponent* ProximityZone;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer _dropStaggerTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer _dropStaggerReductionCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UCapsuleComponent* SlashableZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UCapsuleComponent* HookSlashableZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchCapsuleHalfHeight;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UDBDPawnSensingComponent* _camperSensor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ADBDPlayer*> _foundCampers;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ADBDPlayer*> _potentialSaviors;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FVector _cameraAnchorOriginalRelativeCameraLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FCameraAttachment _defaultCameraAttachment;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FCameraAttachment _currentCameraAttachment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UChargeableComponent* _hpSlot01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UChargeableComponent* _hpSlot02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UChargeableComponent* _carryEscapeProgress;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _drainTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _dyingTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _injuredBleedoutTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _hideTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	bool _isBeingChased;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	bool _isInInjuredBleedout;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	int32 _numHealthyHeals;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	int32 _hookedCount;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateSlasherProximityEvents(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void UpdateHealingTimer(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void UpdateDrainTimer(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void TriggerWiggleSkillCheck();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TriggerPutDownAnimation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TriggerPickUpAnimation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TriggerBloodTrail();

	UFUNCTION(BlueprintCallable)
	void TriggerAfflictionHUDFeedback();

	UFUNCTION(BlueprintCallable)
	void StopHealingTimer();

	UFUNCTION(BlueprintCallable)
	void StartHealingTimer(float Time);

	UFUNCTION(BlueprintCallable)
	void SetOverlappingEscape(AActor* Escape);

	UFUNCTION(BlueprintCallable)
	void SetIsSacrificed(bool sacrificed);

	UFUNCTION(BlueprintCallable, Exec)
	void SetImmobilized(ECamperImmobilizeState State);

	UFUNCTION(BlueprintCallable)
	void SetGuidedActionLocal(ECamperGuidedAction Action);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetGuidedAction(ECamperGuidedAction Action);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetEnableHookSlashableZone(bool enable);

	UFUNCTION(BlueprintCallable)
	void SetDamageState(ECamperDamageState E, bool FireScoreEvent);

	UFUNCTION(BlueprintCallable)
	void SetCameraAttachment(USceneComponent* Parent, FName SocketName);

	UFUNCTION(BlueprintCallable)
	void SetBeingCarried(bool IsBeingCarried, ADBDPlayer* carrier);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetOnHookDrainToNextStage(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetItemNormalizedEnergyLevel(float normalizedLevel);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetItemEnergyLevel(float Level);

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetImmobilized(ECamperImmobilizeState State);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetHookEscapeAutoSuccess(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetEnableHookSlashableZone(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendLeftRightMashedInput(bool Pressed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendInteractMashedInput(bool Pressed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_RemoveHealth();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_MakeInvulnerable();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_Kill(bool sacrificed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_IncreaseHealth();

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_CheatEscaped();

public:
	UFUNCTION(BlueprintCallable)
	void ResetPotentialSaviors();

	UFUNCTION(BlueprintCallable)
	void ResetHideTimer();

	UFUNCTION(BlueprintCallable)
	void ResetCameraAttachment();

	UFUNCTION(BlueprintCallable)
	void OnUnhooked();

private:
	UFUNCTION()
	void OnProximityEntered(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnPawnSensed(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
	void OnHealthBarCharged(bool complete);

public:
	UFUNCTION(BlueprintCallable)
	void OnDroppedStart();

	UFUNCTION(BlueprintCallable)
	void OnDroppedEnd(bool clearLeader);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageHit(const FHitResult& Hit, bool causedKO);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetOnHookDrainToNextStage(bool Enabled);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetImmobilized(ECamperImmobilizeState State);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetHookEscapeAutoSuccess(bool Enabled);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetGuidedAction(ECamperGuidedAction Action);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetEnableHookSlashableZone(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag flag, bool Value);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_OnKO();

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_OnHealed();

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_OnFailedStruggling(UInteractionDefinition* Interaction);

public:
	UFUNCTION(BlueprintCallable)
	void MoveToOwnerCarryJoint();

	UFUNCTION(BlueprintPure)
	bool IsUnintentionallyImmobilized() const;

	UFUNCTION(BlueprintPure)
	bool IsTrapped() const;

	UFUNCTION(BlueprintPure)
	bool IsTotallyInsane() const;

	UFUNCTION(BlueprintCallable)
	bool IsSuccessfullyHidden() const;

	UFUNCTION(BlueprintPure)
	bool IsSacrificed() const;

	UFUNCTION(BlueprintPure)
	bool IsObsessionTarget() const;

	UFUNCTION(BlueprintPure)
	bool IsMovementGuided() const;

	UFUNCTION(BlueprintPure)
	bool IsKO() const;

	UFUNCTION(BlueprintPure)
	bool IsInsideCloset() const;

	UFUNCTION(BlueprintPure)
	bool IsInNeed() const;

	UFUNCTION(BlueprintPure)
	bool IsInjuredBleedoutTimerExpired() const;

	UFUNCTION(BlueprintPure)
	bool IsInjured() const;

	UFUNCTION(BlueprintPure)
	bool IsInInjuredBleedout() const;

	UFUNCTION(BlueprintPure)
	bool IsInBasement() const;

	UFUNCTION(BlueprintPure)
	bool IsImmobilized() const;

	UFUNCTION(BlueprintPure)
	bool IsHooked() const;

	UFUNCTION(BlueprintPure)
	bool IsHealthy() const;

	UFUNCTION(BlueprintPure)
	bool IsHealingTimerExpired() const;

	UFUNCTION(BlueprintPure)
	bool IsGuidedBySlasher() const;

	UFUNCTION(BlueprintPure)
	bool IsEscaped() const;

	UFUNCTION(BlueprintPure)
	bool IsDyingTimerExpired() const;

	UFUNCTION(BlueprintPure)
	bool IsDrainTimerExpired() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsCrawling() const;

	UFUNCTION(BlueprintPure)
	bool IsCameraDetached() const;

	UFUNCTION(BlueprintPure)
	bool IsCameraAttached() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingPutOnHook() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingPutDown() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingPickedUp() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingKilled() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingInteractedWith() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingHealed() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingChased() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingCarried() const;

	UFUNCTION(BlueprintCallable)
	void IncreaseHPSlotSize(float percent);

	UFUNCTION(BlueprintPure)
	bool HasHitEvents() const;

	UFUNCTION(BlueprintPure)
	bool HasGuidedAction() const;

	UFUNCTION(BlueprintPure)
	float GetWiggleFreeChargePercent() const;

	UFUNCTION(BlueprintPure)
	FRotator GetToAimPointRotation() const;

	UFUNCTION(BlueprintPure)
	float GetStillness() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UDBDSpringArmComponent* GetSpringArmComponent();

	UFUNCTION(BlueprintPure)
	float GetPercentInjuredBleedoutTimer() const;

	UFUNCTION(BlueprintPure)
	float GetPercentHealingTimer() const;

	UFUNCTION(BlueprintPure)
	float GetPercentDyingTimer() const;

	UFUNCTION(BlueprintPure)
	float GetPercentDropStaggerCooldownTimer() const;

	UFUNCTION(BlueprintPure)
	float GetPercentDrainTimer() const;

	UFUNCTION(BlueprintPure)
	AActor* GetOverlappingEscape() const;

	UFUNCTION(BlueprintPure)
	float GetObsessionTargetWeight() const;

	UFUNCTION(BlueprintPure)
	int32 GetNumberOfEntityRenderRegions() const;

	UFUNCTION(BlueprintPure)
	UAnimationMontageSlave* GetMontageFollower();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMadnessTierBPNative();

	UFUNCTION(BlueprintPure)
	ECamperImmobilizeState GetImmobilizedState() const;

	UFUNCTION(BlueprintPure)
	ECamperGuidedAction GetGuidedAction() const;

	UFUNCTION(BlueprintPure)
	UDBDClipRegionComponent* GetEntityRenderRegionAtIndex(int32 InIndex) const;

	UFUNCTION(BlueprintPure)
	float GetDropStaggerTimeLeft() const;

	UFUNCTION(BlueprintPure)
	float GetDropStaggerPercentTimeElapsed() const;

	UFUNCTION(BlueprintPure)
	ECamperDamageState GetDamageState() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentTotalHealthBarPercent() const;

	UFUNCTION(BlueprintCallable)
	UChargeableComponent* GetCurrentHPSlot();

	UFUNCTION(BlueprintCallable)
	UCamperStillnessTrackerComponent* GetCamperStillnessComponent();

	UFUNCTION(BlueprintPure)
	UDBDCamperData* GetCamperData() const;

	UFUNCTION(BlueprintPure)
	TArray<UDBDClipRegionComponent*> GetAllEntityRenderRegions() const;

	UFUNCTION(BlueprintPure)
	FVector GetAimPoint() const;

	UFUNCTION(BlueprintCallable)
	void Escape(AActor* Escape);

	UFUNCTION(BlueprintCallable)
	void DetachFromGuidingPlayer();

	UFUNCTION(BlueprintCallable)
	void DetachCamera();

	UFUNCTION(BlueprintCallable)
	void DBDApplyDamage(bool& didDamage, bool& causedKO);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_LeaveParadise();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ForceRun(bool Forced);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_CheatEscaped();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AutoLeftRightMash(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AutoInteractionMash(bool Enabled);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DamageStateChangedEvent(ECamperDamageState DamageState);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool ComputeHookEscapeResult();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool ComputeBeartrapEscapeResult();

	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation)
	void Client_OnSlashedOutOfTrap(ADBDPlayer* Requester);

	UFUNCTION(BlueprintPure)
	bool CanStruggle() const;

	UFUNCTION(BlueprintPure)
	bool CanRushQuietly() const;

	UFUNCTION(BlueprintPure)
	bool CanInteractWithItems() const;

	UFUNCTION(BlueprintPure)
	bool CanHealSelf() const;

	UFUNCTION(BlueprintPure)
	bool CanGainInsanity() const;

	UFUNCTION(BlueprintPure)
	bool CanBeShocked() const;

	UFUNCTION(BlueprintPure)
	bool CanBePickedUp() const;

	UFUNCTION(BlueprintPure)
	bool CanBeKilled() const;

	UFUNCTION(BlueprintPure)
	bool CanBeImmediatelyKO() const;

	UFUNCTION(BlueprintPure)
	bool CanBeHealedFromInjured() const;

	UFUNCTION(BlueprintPure)
	bool CanBeHealed() const;

	UFUNCTION(BlueprintPure)
	bool CanBecomeHealthy() const;

	UFUNCTION(BlueprintCallable)
	bool CanBeChased() const;

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_SendRunInput_Server(bool Pressed);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Broadcast_SendRunInput_Multicast(bool Pressed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_SendItemPreventRun_Server(bool prevent);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Broadcast_SendItemPreventRun_Multicast(bool prevent);

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AuthoritySetHealth(ECamperDamageState health);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetDrainTimerPercentLeft(float percentTime);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetDrainTimerPercentElapsed(float percentTime);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_ResetWiggleFreeCharge();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_OnSlashed(ADBDPlayer* attacker);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_OnFailedStruggling(UInteractionDefinition* Interaction);

	UFUNCTION(BlueprintCallable)
	void Authority_ApplyDamageToState(bool& didDamage, bool& causedKO, ECamperDamageState targetState);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_AddWiggleFreeChargePercentNoModifiers(float ChargePercent);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_AddWiggleFreeCharge(float charge);

	UFUNCTION(BlueprintCallable)
	void AttachToGuidingPlayer();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ApplyHeal();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ApplyFullHeal();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ACamperPlayer();
};

FORCEINLINE uint32 GetTypeHash(const ACamperPlayer) { return 0; }
