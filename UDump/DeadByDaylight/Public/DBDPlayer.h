#pragma once

#include "CoreMinimal.h"
#include "PerkOwnerInterface.h"
#include "PerkManagerOwnerInterface.h"
#include "AnimTagProvider.h"
#include "GameFramework/Character.h"
#include "Templates/SubclassOf.h"
#include "MovementInputDelegate.h"
#include "UObject/NoExportTypes.h"
#include "AnimationStateDependencyUpdateDelegate.h"
#include "OnAttackedEvent.h"
#include "OnSensedDelegate.h"
#include "AnimNotifyDelegate_Pickup.h"
#include "AnimNotifyDelegate_Release.h"
#include "SecondaryActionInputDelegate.h"
#include "OnFirstPersonModeChanged.h"
#include "ECamperState.h"
#include "Offering.h"
#include "EAttackSubstate.h"
#include "DataTableProxy.h"
#include "EffectCameraTypeSettings.h"
#include "DBDTimer.h"
#include "EDetectionZone.h"
#include "EInputInteractionType.h"
#include "EDBDScoreTypes.h"
#include "EAnimNotifyType.h"
#include "Engine/EngineTypes.h"
#include "EInteractionAnimation.h"
#include "EAuthoritativeMovementFlag.h"
#include "EStunType.h"
#include "AnimationMontageDescriptor.h"
#include "EAttackType.h"
#include "Engine/HitResult.h"
#include "EPlayerRole.h"
#include "InteractionPlayerProperties.h"
#include "Components/SkinnedMeshComponent.h"
#include "AnimData.h"
#include "EBlindType.h"
#include "DBDPlayer.generated.h"

class AInteractable;
class USceneComponent;
class USpringArmComponent;
class UCurveFloat;
class UChargeableComponent;
class USoundCue;
class ACollectable;
class UPerk;
class UStatusEffect;
class UFlashLightComponent;
class AActor;
class UStateMachine;
class UPlayerInteractionHandler;
class UMontagePlayer;
class UPerkManager;
class UDBDPlayerData;
class UAxisFlickMasher;
class UInputMasher;
class ADBDPlayer;
class UCharacterSnappingComponent;
class UCameraHandlerComponent;
class UPollableEventListener;
class UZoneDetectorComponent;
class UInteractionDefinition;
class UPrimitiveComponent;
class UAuthoritativeMovementComponent;
class UAnimMontage;
class ADBDPlayerController;
class UCameraComponent;
class UItemModifier;
class UItemAddon;
class UActorComponent;
class ADBDPlayerState;
class UCustomizedSkeletalMesh;
class ADBDPlayerCameraManager;
class UAnimInstance;
class ACamperPlayer;
class UGameplayModifierContainer;
class UInteractor;

UCLASS(Blueprintable, Abstract)
class ADBDPlayer : public ACharacter, public IPerkOwnerInterface, public IPerkManagerOwnerInterface, public IAnimTagProvider
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInteractable> Interactable;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FMovementInputDelegate OnMovementInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowInterrupting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinFallHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInterruptable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowNavigationInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowNavigationBackwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowTurningInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowPitchInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PitchLimitLower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PitchLimitUpper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StrafingOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PelvisHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PelvisHeightForPounceOnStandingCamper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PelvisHeightForPounceOnCrouchingCamper;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FAnimationStateDependencyUpdateDelegate OnAnimationStateDependencyUpdate;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnAttackedEvent OnAttackedDelegate;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnSensedDelegate OnSensed;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FAnimNotifyDelegate_Pickup OnAnimNotify_Pickup;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FAnimNotifyDelegate_Release OnAnimNotify_Release;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FSecondaryActionInputDelegate OnSecondaryActionInput;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnFirstPersonModeChanged OnFirstPersonModeChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USceneComponent* CameraAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForceDisableFootIK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForceSkillChecks;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FTransform IKLeftHandTransform;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FTransform IKRightHandTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraResetSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterClothes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterShoes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterAmbiance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugInteractionPriorityEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _emulatedYawInputDecay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _emulatedYawInputDecayWhenNoInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _emulatedYawInputReleaseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _emulatedYawInputDeadZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _emulatedYawMinimumInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _emulatedYawInputStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* BlinkFOVCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	float CurrentBlinkDistance;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool IsInBlinkingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AverageSpeedBufferTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AlmostCurrentSpeedBufferTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* GamepadYawCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* GamepadPitchCurve;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UChargeableComponent* BlindingChargeableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CarryJointName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackPounceCurves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackLungeCurves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackSlashCurves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackChainsawCurves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackBlinkCurves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackPounceCurves_StalkTier0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackPounceCurves_StalkTier1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCurveFloat*> AttackPounceCurves_StalkTier2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _pitchOffsetForInteractionPriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInteracting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClearPathTestRadiusMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClearPathTestHeightMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LocationClearTestRadiusMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LocationClearTestHeightMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClearPathTestStepHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GamePadTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GamePadLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseTurnSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseLookUpSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECamperState CurrentCamperState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CollectableAttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ACollectable*> Inventory;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, Instanced)
	TArray<UPerk*> _perks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<FOffering> _offerings;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, Instanced)
	TArray<UStatusEffect*> _statusEffects;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	TMap<AActor*, UFlashLightComponent*> _blindingEffectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStateMachine* _SM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UPlayerInteractionHandler* _interactionHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UMontagePlayer* _montagePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UPerkManager* _perkManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UDBDPlayerData* _playerData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UAxisFlickMasher* _wiggleAxisFlickMasher;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UInputMasher* _interactInputMasher;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	AInteractable* _interactable;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<ADBDPlayer*> _overlappingPlayers;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	ADBDPlayer* _guidingPlayer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	ADBDPlayer* _interactingPlayer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	EAttackSubstate _nextAttackSubstate;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TWeakObjectPtr<ADBDPlayer> _lockOnTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TWeakObjectPtr<ADBDPlayer> _damageTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UCharacterSnappingComponent* _characterSnappingComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UCameraHandlerComponent* _cameraHandlerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USceneComponent* _itemDropOffPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableProxy TunableDB;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDataTableProxy GlobalTunableDB;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<AActor*> ignoreActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectCameraTypeSettings> EffectCameraTypeSettings;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer _postChaseTimer;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UPollableEventListener* _eventListener;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UZoneDetectorComponent* _meatHookZoneDetector;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UInteractionDefinition* _bookmarkedInteraction;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TMap<EDetectionZone, UPrimitiveComponent*> _detectionZoneMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UAuthoritativeMovementComponent* _authoritativeMovementComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	float _blindnessRemainingTime;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _recentlyBlindedTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta=(AllowPrivateAccess=true))
	float _screenAspectRatio;

public:
	UFUNCTION(BlueprintCallable)
	void UseInventoryItem();

	UFUNCTION(BlueprintCallable)
	void UpdateLoadoutFromInventory();

	UFUNCTION(BlueprintCallable)
	bool TryInteractionType(EInputInteractionType interactionInputType, ADBDPlayer* Requester);

	UFUNCTION(BlueprintCallable)
	bool TryInteraction(UInteractionDefinition* Interaction, ADBDPlayer* Requester, bool Force);

	UFUNCTION(BlueprintCallable)
	bool TryFireScoreEvent(EDBDScoreTypes ScoreType, float percentToAward, bool redirectToGameEventSystem);

	UFUNCTION(BlueprintCallable)
	void TriggerTrapImmunity(float Duration);

	UFUNCTION(BlueprintCallable)
	void TriggerAnimNotify(EAnimNotifyType animNotifyType);

	UFUNCTION(BlueprintCallable)
	void ToggleGhost();

	UFUNCTION(BlueprintCallable)
	bool TeleportToBP(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck);

	UFUNCTION(BlueprintCallable)
	void StoreItem();

	UFUNCTION(BlueprintCallable)
	void StopSnap(bool snap);

	UFUNCTION(BlueprintCallable)
	void StopMontage();

	UFUNCTION(BlueprintCallable)
	void StopAllMovement();

	UFUNCTION(BlueprintCallable)
	void StartDrop(ACollectable* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	ACollectable* SpawnCollectedItem(FName ItemId);

	UFUNCTION(BlueprintCallable)
	void SnapCharacter(bool snapPosition, FVector Position, float stopSnapDistance, bool snapRotation, FRotator Rotation, float Time, bool useZCoord, bool sweepOnFinalSnap);

	UFUNCTION(BlueprintPure)
	bool ShouldFall() const;

	UFUNCTION(BlueprintCallable)
	void SetYawScale(float yawScale, float adjustTime);

	UFUNCTION(BlueprintCallable)
	void SetWorldCollisionResponse(ECollisionResponse Response);

	UFUNCTION(BlueprintCallable)
	void SetRightFootAudioSurfaceName(const FString& Name);

	UFUNCTION(BlueprintCallable)
	void SetReverseMontage(bool Reverse);

	UFUNCTION(BlueprintCallable)
	void SetPlayerDirection(FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void SetPitchScale(float yawScale, float adjustTime);

	UFUNCTION(BlueprintCallable)
	void SetPawnAuthoritativeMovement(bool authoritative);

	UFUNCTION(BlueprintCallable)
	void SetLeftFootAudioSurfaceName(const FString& Name);

protected:
	UFUNCTION(BlueprintCallable)
	void SetIsCloaked(bool IsCloaked, bool Forced);

public:
	UFUNCTION(BlueprintCallable)
	void SetInStalkMode(bool stalkMode, bool Forced);

	UFUNCTION(BlueprintCallable)
	void SetInParadise(bool inParadise);

	UFUNCTION(BlueprintCallable)
	void SetIgnoreActorCollision(AActor* ActorToIgnore, bool ignore);

	UFUNCTION(BlueprintCallable)
	void SetGuidingPlayer(ADBDPlayer* guidingPlayer);

	UFUNCTION(BlueprintCallable)
	void SetFirstPersonModelEnabled(bool Enabled, bool Force);

	UFUNCTION(BlueprintCallable)
	void SetFeetOnGround(bool feetOnGround);

	UFUNCTION(BlueprintCallable)
	void SetEnableServerPositionUpdates(bool enable);

	UFUNCTION(BlueprintCallable)
	void SetDetectionZoneEnabled(EDetectionZone detectionZoneID, bool Enabled);

	UFUNCTION(BlueprintCallable)
	void SetCurrentInteractionAnimation(EInteractionAnimation Animation);

	UFUNCTION(BlueprintCallable)
	void SetCharacterAudioSurfaceName(const FString& Name);

	UFUNCTION(BlueprintCallable)
	void SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag flagIndex, bool Enabled);

	UFUNCTION(BlueprintCallable)
	void SetAllowNavigation(bool enable);

	UFUNCTION(BlueprintCallable)
	void SetAllDetectionZonesEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerResetMeshRelativeOffSet();

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_UpdateScreenAspectRatio(const float Value);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_TeleportTo(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SpawnStatusEffect(FName statusEffectID);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SpawnPerk(FName perkID, int32 perkLevel);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SpawnItemAddon(FName addonID);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SpawnItem(FName ItemId);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetWindowFreeCam(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetSlasherAutoAttackOnCamperInteractFrameDelay(int32 frames);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetRunVaultEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetReverseTraverseEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetMaxWalkSpeed(float MaxWalkSpeed);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetForceAuthoritativeMovement(bool Value);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetEthereal(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetEnableServerPositionUpdates(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetEnableCapsuleDynamicResize(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetDebugSnapPoint(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetDebugFailAllInteractions(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetDebugCarry(bool Enabled);

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetCustomization(const TArray<FName>& customizationParts);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetContinuousPrintDebug(bool Enabled);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetAuthoritativeMovement(bool Enabled);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendSecondaryActionPressed();

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendItemUseInput(bool Pressed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendItemUse(bool Use);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendItemDropInput(bool Pressed);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendInteractionInput(bool Pressed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendFastInteractionInput(bool Pressed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_RemoveStatusEffects();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_RemoveStatusEffectByID(FName statusEffectID, bool bRemoveAllWithID);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_RemovePerks();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_PlayerReady();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_OnMovementInput(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_OnInsaneSkillCheck();

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_Ghost(bool Ghost);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_FailInterruption(ADBDPlayer* interruptee);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_Drop(ACollectable* Item, const FVector& Location, const FRotator& Rotation, bool onDeath);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_CheatDispatchOnPostItemAddonsCreation();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_Cheat();

	UFUNCTION(BlueprintCallable)
	void ResetYawScale(float adjustTime);

	UFUNCTION(BlueprintCallable)
	void ResetPitchScale(float adjustTime);

	UFUNCTION(BlueprintCallable)
	void ResetMeshRelativeRotation();

	UFUNCTION(BlueprintCallable)
	void ResetMeshRelativePosition();

	UFUNCTION(BlueprintCallable)
	void ResetCamera();

	UFUNCTION(BlueprintCallable)
	void RequestStun(EStunType stunType, ADBDPlayer* stunner);

	UFUNCTION(BlueprintCallable)
	void RemoveStatusEffect(UStatusEffect* StatusEffect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveFromInventory(ACollectable* Item);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* PlayMontage(FAnimationMontageDescriptor animMontageID, float PlayRate, bool isFollower);

	UFUNCTION(BlueprintCallable)
	void OnPickupStart(ADBDPlayer* target);

	UFUNCTION(BlueprintCallable)
	void OnPickupEnd(ADBDPlayer* target);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLocallyObservedChanged(bool IsLocallyObserved);

public:
	UFUNCTION(BlueprintCallable)
	void OnLevelReadyToPlay();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInterruptedStart();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInsaneSkillCheck();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHeadHidden(bool hidden);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEscapeDoorActivated();

private:
	UFUNCTION(BlueprintCallable)
	void OnCurrentMontageComplete(UAnimMontage* MontageAsset, bool interrupted);

	UFUNCTION(BlueprintCallable)
	void OnCurrentMontageBlendingOut(UAnimMontage* MontageAsset, bool interrupted);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnClientRestart();

protected:
	UFUNCTION(BlueprintCallable)
	void OnBlindChargeEmptied();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnAttackFinished(EAttackType attackType);

	UFUNCTION(BlueprintCallable)
	void OnAllPlayerLoaded();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_TriggerSprintWithParams(float Duration, UCurveFloat* Curve, FName effector);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_TriggerSprint();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetWindowFreeCam(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetRunVaultEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetReverseTraverseEnabled(bool Enabled);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetMaxWalkSpeed(float MaxWalkSpeed);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetInteractingPlayer(ADBDPlayer* interactingPlayer);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetForceAuthoritativeMovement(bool Value);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetEthereal(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetEnableServerPositionUpdates(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetEnableCapsuleDynamicResize(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetDebugSnapPoint(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetDebugCarry(bool Enabled);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetCustomization(const TArray<FName>& customizationParts);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetContinuousPrintDebug(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetAuthoritativeMovement(bool Enabled);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_ServerResetMeshRelativeOffSet();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_RemoveFromInventory(ACollectable* Item);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_PlayMontage(FAnimationMontageDescriptor animMontageID, float PlayRate);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_OnInsaneSkillCheck();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_LockOnTarget(ADBDPlayer* target);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_Ghost(bool Ghost);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_DamageTarget(ADBDPlayer* target, EAttackType attackType);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_ConfirmItemDrop(bool Pressed);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_ClearAndDestroyInventory();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_AttackSubstate(EAttackSubstate attackSubstate);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_AttackBegin(EAttackType attackType);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_AddToInventory(ACollectable* Item);

public:
	UFUNCTION(BlueprintCallable)
	void Local_RequestBlink(FTransform Destination);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Local_NotifyMatchEnded();

protected:
	UFUNCTION(BlueprintCallable)
	bool LineTraceSingleForFootIK(const FVector Start, const FVector End, bool traceComplex, FHitResult& OutHit);

public:
	UFUNCTION(BlueprintCallable)
	bool K2_SetActorLocationByBottomCapsule(FVector NewLocation, bool bSweep, FHitResult& SweepHitResult);

	UFUNCTION(BlueprintPure)
	bool IsUsingAimItem() const;

	UFUNCTION(BlueprintPure)
	bool IsUnintentionallyIncapacitated() const;

	UFUNCTION(BlueprintPure)
	bool IsStrafing() const;

	UFUNCTION(BlueprintPure)
	bool IsSprinting() const;

	UFUNCTION(BlueprintPure)
	bool IsSnappingRotation() const;

	UFUNCTION(BlueprintPure)
	bool IsSnappingPosition() const;

	UFUNCTION(BlueprintPure)
	bool IsSnapping() const;

	UFUNCTION(BlueprintPure)
	bool IsRunning() const;

	UFUNCTION(BlueprintPure)
	bool IsPlayingMontage(FAnimationMontageDescriptor animMontageID) const;

	UFUNCTION(BlueprintPure)
	bool IsPlayingAnyMontage() const;

	UFUNCTION(BlueprintPure)
	bool IsLocationClear(FVector targetPosition) const;

	UFUNCTION(BlueprintPure)
	bool IsLocallyObserved() const;

	UFUNCTION(BlueprintPure)
	bool IsInTerrorRadius() const;

	UFUNCTION(BlueprintPure)
	bool IsInteractionInputPressed(EInputInteractionType interactionType) const;

	UFUNCTION(BlueprintPure)
	bool IsInStalkMode() const;

	UFUNCTION(BlueprintPure)
	bool IsInParadise() const;

	UFUNCTION(BlueprintPure)
	bool IsInMeathookZone() const;

	UFUNCTION(BlueprintPure)
	bool IsInChaseCooldown() const;

	UFUNCTION(BlueprintPure)
	bool IsIncapacitated() const;

	UFUNCTION(BlueprintPure)
	bool IsHeadHidden() const;

	UFUNCTION(BlueprintPure)
	bool IsFacing(FVector Direction) const;

	UFUNCTION(BlueprintPure)
	bool IsExhausted() const;

	UFUNCTION(BlueprintPure)
	bool IsCrouching() const;

	UFUNCTION(BlueprintPure)
	bool IsCloaked() const;

	UFUNCTION(BlueprintPure)
	bool IsBlinded() const;

	UFUNCTION(BlueprintPure)
	bool IsAllowedNavigation() const;

	UFUNCTION(BlueprintCallable)
	void HideHead(bool hide);

	UFUNCTION(BlueprintPure)
	bool HasTrapImmunity() const;

	UFUNCTION(BlueprintPure)
	bool HasMoveInput() const;

	UFUNCTION(BlueprintPure)
	bool HasItem() const;

	UFUNCTION(BlueprintCallable)
	bool HasInInventory(TSubclassOf<ACollectable> Item);

	UFUNCTION(BlueprintPure)
	bool HasEquippedItem() const;

	UFUNCTION(BlueprintPure)
	bool HasDamageImmunity() const;

	UFUNCTION(BlueprintPure)
	bool HasClearPathToTargetWithIgnore(FVector targetPosition, const TArray<AActor*>& ignoreActorsNew) const;

	UFUNCTION(BlueprintPure)
	bool HasClearPathToTarget(FVector targetPosition) const;

	UFUNCTION(BlueprintPure)
	bool HasClearPathToItemDropOffPosition() const;

	UFUNCTION(BlueprintPure)
	bool HasAnimMontageEnded() const;

	UFUNCTION(BlueprintPure)
	bool GetWindowFreeCam() const;

	UFUNCTION(BlueprintPure)
	float GetTunableValue(FName Key, float DefaultValue, bool warnIfRowMissing) const;

	UFUNCTION(BlueprintPure)
	float GetStamina() const;

	UFUNCTION(BlueprintPure)
	FName GetSprintEffector() const;

	UFUNCTION(BlueprintPure)
	float GetSprintDurationTimerPercentage() const;

	UFUNCTION(BlueprintPure)
	bool GetRunVaultEnabled() const;

	UFUNCTION(BlueprintPure)
	FString GetRightFootAudioSurfaceName() const;

	UFUNCTION(BlueprintPure)
	bool GetReverseTraverseEnabled() const;

	UFUNCTION(BlueprintPure)
	FRotator GetRepControlRotation() const;

	UFUNCTION(BlueprintPure)
	float GetRemainingTrapImmunityDuration() const;

	UFUNCTION(BlueprintCallable)
	float GetProgressToEscapeCarry(ADBDPlayer* Player);

	UFUNCTION(BlueprintPure)
	EPlayerRole GetPlayerRole() const;

	UFUNCTION(BlueprintPure)
	UPlayerInteractionHandler* GetPlayerInteractionHandler() const;

	UFUNCTION(BlueprintPure)
	FRotator GetPlayerDirection() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintPure)
	UCameraComponent* GetPlayerCamera() const;

	UFUNCTION(BlueprintPure)
	UPerkManager* GetPerkManager() const;

	UFUNCTION(BlueprintPure)
	float GetPercentMovementSpeed() const;

	UFUNCTION(BlueprintPure)
	float GetPercentBlinded() const;

	UFUNCTION(BlueprintPure)
	FString GetNameDebugString() const;

	UFUNCTION(BlueprintPure)
	UMontagePlayer* GetMontagePlayer() const;

	UFUNCTION(BlueprintPure)
	float GetMontageLength(FAnimationMontageDescriptor animMontageID) const;

	UFUNCTION(BlueprintPure)
	UAnimMontage* GetMontage(FAnimationMontageDescriptor animMontageID) const;

	UFUNCTION(BlueprintPure)
	FVector GetMeshFeetPosition() const;

	UFUNCTION(BlueprintPure)
	float GetMaxSpeed() const;

	UFUNCTION(BlueprintPure)
	float GetLuck() const;

	UFUNCTION(BlueprintPure)
	FString GetLeftFootAudioSurfaceName() const;

	UFUNCTION(BlueprintPure)
	TArray<UItemModifier*> GetItemModifiers() const;

	UFUNCTION(BlueprintPure)
	FName GetItemID() const;

	UFUNCTION(BlueprintPure)
	USceneComponent* GetItemDropOffTransform() const;

	UFUNCTION(BlueprintPure)
	int32 GetItemCountByID(FName ItemId) const;

	UFUNCTION(BlueprintPure)
	int32 GetItemCount() const;

	UFUNCTION(BlueprintPure)
	TArray<UItemAddon*> GetItemAddons() const;

	UFUNCTION(BlueprintPure)
	ACollectable* GetItem() const;

	UFUNCTION(BlueprintPure)
	bool GetIsInterrupting() const;

	UFUNCTION(BlueprintPure)
	bool GetIsBeyondCrouchWalkSpeed() const;

	UFUNCTION(BlueprintCallable)
	FInteractionPlayerProperties GetInteractionPlayerProperties();

	UFUNCTION(BlueprintPure)
	float GetInteractingMaxSpeedPercent() const;

	UFUNCTION(BlueprintPure)
	AInteractable* GetInteractable() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetGuidingPlayer() const;

	UFUNCTION(BlueprintPure)
	UActorComponent* GetFirstComponentByClass(TSubclassOf<UActorComponent> ComponentClass) const;

	UFUNCTION(BlueprintPure)
	FVector GetFeetPositionAtTargetLocation(FVector Location) const;

	UFUNCTION(BlueprintPure)
	FVector GetFeetPosition() const;

	UFUNCTION(BlueprintPure)
	bool GetFeetOnGround() const;

	UFUNCTION(BlueprintPure)
	bool GetEnableServerPositionUpdates() const;

	UFUNCTION(BlueprintPure)
	UPrimitiveComponent* GetDetectionZone(EDetectionZone detectionZoneID) const;

	UFUNCTION(BlueprintPure)
	bool GetDebugFailAllInteractions() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayerState* GetDBDPlayerState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCustomizedSkeletalMesh* GetCustomizedSkeletalMesh();

	UFUNCTION(BlueprintPure)
	EInteractionAnimation GetCurrentInteractionAnimation() const;

	UFUNCTION(BlueprintPure)
	UInteractionDefinition* GetCurrentInteraction() const;

	UFUNCTION(BlueprintPure)
	bool GetContinuousPrintDebug() const;

	UFUNCTION(BlueprintPure)
	UCharacterSnappingComponent* GetCharacterSnappingComponent() const;

	UFUNCTION(BlueprintPure)
	FString GetCharacterAudioSurfaceName() const;

	UFUNCTION(BlueprintCallable)
	ADBDPlayerCameraManager* GetCameraManager();

	UFUNCTION(BlueprintPure)
	FVector GetCameraLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetCameraAnchorLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetBoneRelativeLocation(FName TargetBone, FName relativeBone) const;

	UFUNCTION(BlueprintPure)
	FVector GetBoneLocation(FName Name, EBoneSpaces::Type space) const;

	UFUNCTION(BlueprintPure)
	float GetBlinkTime() const;

	UFUNCTION(BlueprintPure)
	float GetBaseFOV() const;

	UFUNCTION(BlueprintPure)
	FVector GetAverageVelocity() const;

	UFUNCTION(BlueprintPure)
	EAttackType GetAttackMontageEvent() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	USceneComponent* GetAttachPoint(FName attachPointName) const;

	UFUNCTION(BlueprintPure)
	void GetAnimTags(TArray<FName>& outTags) const;

	UFUNCTION(BlueprintPure)
	UAnimInstance* GetAnimInstanceConst() const;

	UFUNCTION(BlueprintCallable)
	UAnimInstance* GetAnimInstance();

	UFUNCTION(BlueprintPure)
	FAnimData GetAnimData();

	UFUNCTION(BlueprintPure)
	FVector GetAlmostCurrentVelocity() const;

	UFUNCTION(BlueprintPure)
	FVector GetActorLocationFromFeetLocation(const FVector FeetLocation) const;

	UFUNCTION(BlueprintPure)
	FVector FindFurthestClearLocationBetweenPoints(FVector StartPosition, FVector targetPosition) const;

	UFUNCTION(BlueprintCallable)
	void EquipItem();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Drop(ACollectable* Item, const FVector& Location, const FRotator& Rotation, bool onDeath, bool adjustToGround);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Dissolve(bool Dissolve);

	UFUNCTION(BlueprintCallable)
	void DetachInteractor();

	UFUNCTION(BlueprintPure=false, BlueprintCallable)
	void DebugPrint(const FString& inString) const;

	UFUNCTION(BlueprintCallable)
	void DeactivateSkillChecks();

	UFUNCTION(BlueprintCallable)
	void DBDUnCrouch();

	UFUNCTION(BlueprintCallable)
	void DBDLog(const FString& LogText);

	UFUNCTION(BlueprintCallable)
	void DBDCrouch();

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_TogglePrintServerPositionUpdateEnabled();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ToggleForceAuthoritativeMovement();

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ToggleDebugInteractionInZone();

protected:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetMouseTurnSpeedMultiplier(float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetMouseLookUpSpeedMultiplier(float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetGamePadTurnRate(float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetGamePadLookUpRate(float Value);

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DebugStartInteraction();

	UFUNCTION(BlueprintCallable)
	void ConsumeStamina(float Stamina);

	UFUNCTION(BlueprintCallable)
	void Collect(ACollectable* Item);

protected:
	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation)
	void Client_TryInteractionType(EInputInteractionType interactionType, ADBDPlayer* Requester);

public:
	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation)
	void Client_SendCancelInteraction();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ClearAndDestroyInventory();

	UFUNCTION(BlueprintPure)
	bool CanSprint() const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInterrupt_BP() const;

public:
	UFUNCTION(BlueprintCallable)
	bool CanInteract();

	UFUNCTION(BlueprintPure)
	bool CanCollectExtraItem(ACollectable* Item) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanAttack_BP();

	UFUNCTION(BlueprintCallable)
	bool CanAttack();

public:
	UFUNCTION(BlueprintCallable)
	void CameraUpdated();

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_PlayMontage_Server(FAnimationMontageDescriptor animMontageID, float PlayRate);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_PlayMontage_Multicast(FAnimationMontageDescriptor animMontageID, float PlayRate);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_Multicast_BlinkDestination(FVector Position, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_LockOnTarget_Server(ADBDPlayer* target);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_LockOnTarget_Multicast(ADBDPlayer* target);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_DamageTarget_Server(ADBDPlayer* target, EAttackType attackType);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_DamageTarget_Multicast(ADBDPlayer* target, EAttackType attackType);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_BlinkDestination(FVector Position, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_AttackSubstate_Server(EAttackSubstate attackSubstate);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_AttackSubstate_Multicast(EAttackSubstate attackSubstate);

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AuthorityTriggerSprintWithParams(float Duration, float coolDown, UCurveFloat* Curve, FName effector);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AuthorityTriggerSprint();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AuthorityFlashlighted(ADBDPlayer* blinder);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AuthorityAttemptEscapeCarry(ACamperPlayer* Player);

	UFUNCTION(BlueprintCallable)
	bool Authority_TryForceEndOngoingScoreEvent(EDBDScoreTypes ScoreType);

	UFUNCTION(BlueprintCallable)
	bool Authority_TryForceCancelOngoingScoreEvent(EDBDScoreTypes ScoreType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_RemoveStatusEffectByID(FName statusEffectID, bool bRemoveAllWithID);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UStatusEffect* Authority_ImposeStatusEffect(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParam, UGameplayModifierContainer* originatingEffect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UStatusEffect* Authority_ImposeDynamicStatusEffect(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParam, UGameplayModifierContainer* originatingEffect);

	UFUNCTION(BlueprintCallable)
	void Authority_Blinded(EBlindType blindType, float coolDown, AActor* effector);

	UFUNCTION(BlueprintCallable)
	void Authority_ActivateAuthoritativeMovementOnDropped();

	UFUNCTION(BlueprintCallable)
	void AttachInteractor(UInteractor* Interactor);

	UFUNCTION(BlueprintPure)
	bool ArePlayersHandsFullOfBearTraps() const;

private:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddToInventory(ACollectable* Item);

public:
	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(UStatusEffect* StatusEffect);

	UFUNCTION(BlueprintCallable)
	void AddPerk(UPerk* Perk);

	UFUNCTION(BlueprintCallable)
	void AddDetectionZone(EDetectionZone detectionZoneID, UPrimitiveComponent* zone);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ADBDPlayer();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayer) { return 0; }
