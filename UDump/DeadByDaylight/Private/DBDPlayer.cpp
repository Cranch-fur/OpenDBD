#include "DBDPlayer.h"
#include "EInputInteractionType.h"
#include "EDBDScoreTypes.h"
#include "EAnimNotifyType.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "EDetectionZone.h"
#include "EInteractionAnimation.h"
#include "EAuthoritativeMovementFlag.h"
#include "EStunType.h"
#include "AnimationMontageDescriptor.h"
#include "EAttackType.h"
#include "EAttackSubstate.h"
#include "Engine/HitResult.h"
#include "Templates/SubclassOf.h"
#include "EPlayerRole.h"
#include "InteractionPlayerProperties.h"
#include "Components/SkinnedMeshComponent.h"
#include "AnimData.h"
#include "EBlindType.h"
#include "Net/UnrealNetwork.h"
#include "ChargeableComponent.h"
#include "PlayerInteractionHandler.h"
#include "MontagePlayer.h"
#include "PerkManager.h"
#include "CharacterSnappingComponent.h"
#include "CameraHandlerComponent.h"
#include "Components/SceneComponent.h"
#include "PollableEventListener.h"
#include "ZoneDetectorComponent.h"
#include "AuthoritativeMovementComponent.h"

class ADBDPlayer;
class UInteractionDefinition;
class ACollectable;
class AActor;
class UStatusEffect;
class UAnimMontage;
class UCurveFloat;
class ADBDPlayerController;
class UCameraComponent;
class UItemModifier;
class UItemAddon;
class AInteractable;
class UActorComponent;
class UPrimitiveComponent;
class ADBDPlayerState;
class UCustomizedSkeletalMesh;
class ADBDPlayerCameraManager;
class UAnimInstance;
class ACamperPlayer;
class UGameplayModifierContainer;
class UInteractor;
class UPerk;

void ADBDPlayer::UseInventoryItem()
{

}

void ADBDPlayer::UpdateLoadoutFromInventory()
{

}

bool ADBDPlayer::TryInteractionType(EInputInteractionType interactionInputType, ADBDPlayer* Requester)
{
	return false;
}

bool ADBDPlayer::TryInteraction(UInteractionDefinition* Interaction, ADBDPlayer* Requester, bool Force)
{
	return false;
}

bool ADBDPlayer::TryFireScoreEvent(EDBDScoreTypes ScoreType, float percentToAward, bool redirectToGameEventSystem)
{
	return false;
}

void ADBDPlayer::TriggerTrapImmunity(float Duration)
{

}

void ADBDPlayer::TriggerAnimNotify(EAnimNotifyType animNotifyType)
{

}

void ADBDPlayer::ToggleGhost()
{

}

bool ADBDPlayer::TeleportToBP(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck)
{
	return false;
}

void ADBDPlayer::StoreItem()
{

}

void ADBDPlayer::StopSnap(bool snap)
{

}

void ADBDPlayer::StopMontage()
{

}

void ADBDPlayer::StopAllMovement()
{

}

void ADBDPlayer::StartDrop(ACollectable* Item)
{

}

ACollectable* ADBDPlayer::SpawnCollectedItem(FName ItemId)
{
	return NULL;
}

void ADBDPlayer::SnapCharacter(bool snapPosition, FVector Position, float stopSnapDistance, bool snapRotation, FRotator Rotation, float Time, bool useZCoord, bool sweepOnFinalSnap)
{

}

bool ADBDPlayer::ShouldFall() const
{
	return false;
}

void ADBDPlayer::SetYawScale(float yawScale, float adjustTime)
{

}

void ADBDPlayer::SetWorldCollisionResponse(ECollisionResponse Response)
{

}

void ADBDPlayer::SetRightFootAudioSurfaceName(const FString& Name)
{

}

void ADBDPlayer::SetReverseMontage(bool Reverse)
{

}

void ADBDPlayer::SetPlayerDirection(FRotator Rotation)
{

}

void ADBDPlayer::SetPitchScale(float yawScale, float adjustTime)
{

}

void ADBDPlayer::SetPawnAuthoritativeMovement(bool authoritative)
{

}

void ADBDPlayer::SetLeftFootAudioSurfaceName(const FString& Name)
{

}

void ADBDPlayer::SetIsCloaked(bool IsCloaked, bool Forced)
{

}

void ADBDPlayer::SetInStalkMode(bool stalkMode, bool Forced)
{

}

void ADBDPlayer::SetInParadise(bool inParadise)
{

}

void ADBDPlayer::SetIgnoreActorCollision(AActor* ActorToIgnore, bool ignore)
{

}

void ADBDPlayer::SetGuidingPlayer(ADBDPlayer* guidingPlayer)
{

}

void ADBDPlayer::SetFirstPersonModelEnabled(bool Enabled, bool Force)
{

}

void ADBDPlayer::SetFeetOnGround(bool feetOnGround)
{

}

void ADBDPlayer::SetEnableServerPositionUpdates(bool enable)
{

}

void ADBDPlayer::SetDetectionZoneEnabled(EDetectionZone detectionZoneID, bool Enabled)
{

}

void ADBDPlayer::SetCurrentInteractionAnimation(EInteractionAnimation Animation)
{

}

void ADBDPlayer::SetCharacterAudioSurfaceName(const FString& Name)
{

}

void ADBDPlayer::SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag flagIndex, bool Enabled)
{

}

void ADBDPlayer::SetAllowNavigation(bool enable)
{

}

void ADBDPlayer::SetAllDetectionZonesEnabled(bool Enabled)
{

}

void ADBDPlayer::ServerResetMeshRelativeOffSet_Implementation()
{

}

bool ADBDPlayer::ServerResetMeshRelativeOffSet_Validate()
{
	return true;
}

void ADBDPlayer::Server_UpdateScreenAspectRatio_Implementation(const float Value)
{

}

bool ADBDPlayer::Server_UpdateScreenAspectRatio_Validate(const float Value)
{
	return true;
}

void ADBDPlayer::Server_TeleportTo_Implementation(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck)
{

}

bool ADBDPlayer::Server_TeleportTo_Validate(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck)
{
	return true;
}

void ADBDPlayer::Server_SpawnStatusEffect_Implementation(FName statusEffectID)
{

}

bool ADBDPlayer::Server_SpawnStatusEffect_Validate(FName statusEffectID)
{
	return true;
}

void ADBDPlayer::Server_SpawnPerk_Implementation(FName perkID, int32 perkLevel)
{

}

bool ADBDPlayer::Server_SpawnPerk_Validate(FName perkID, int32 perkLevel)
{
	return true;
}

void ADBDPlayer::Server_SpawnItemAddon_Implementation(FName addonID)
{

}

bool ADBDPlayer::Server_SpawnItemAddon_Validate(FName addonID)
{
	return true;
}

void ADBDPlayer::Server_SpawnItem_Implementation(FName ItemId)
{

}

bool ADBDPlayer::Server_SpawnItem_Validate(FName ItemId)
{
	return true;
}

void ADBDPlayer::Server_SetWindowFreeCam_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetWindowFreeCam_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetSlasherAutoAttackOnCamperInteractFrameDelay_Implementation(int32 frames)
{

}

bool ADBDPlayer::Server_SetSlasherAutoAttackOnCamperInteractFrameDelay_Validate(int32 frames)
{
	return true;
}

void ADBDPlayer::Server_SetRunVaultEnabled_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetRunVaultEnabled_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetReverseTraverseEnabled_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetReverseTraverseEnabled_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetMaxWalkSpeed_Implementation(float MaxWalkSpeed)
{

}

bool ADBDPlayer::Server_SetMaxWalkSpeed_Validate(float MaxWalkSpeed)
{
	return true;
}

void ADBDPlayer::Server_SetForceAuthoritativeMovement_Implementation(bool Value)
{

}

bool ADBDPlayer::Server_SetForceAuthoritativeMovement_Validate(bool Value)
{
	return true;
}

void ADBDPlayer::Server_SetEthereal_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetEthereal_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetEnableServerPositionUpdates_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetEnableServerPositionUpdates_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetEnableCapsuleDynamicResize_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetEnableCapsuleDynamicResize_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetDebugSnapPoint_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetDebugSnapPoint_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetDebugFailAllInteractions_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetDebugFailAllInteractions_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetDebugCarry_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetDebugCarry_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetCustomization_Implementation(const TArray<FName>& customizationParts)
{

}

bool ADBDPlayer::Server_SetCustomization_Validate(const TArray<FName>& customizationParts)
{
	return true;
}

void ADBDPlayer::Server_SetContinuousPrintDebug_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetContinuousPrintDebug_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SetAuthoritativeMovement_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Server_SetAuthoritativeMovement_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Server_SendSecondaryActionPressed_Implementation()
{

}

bool ADBDPlayer::Server_SendSecondaryActionPressed_Validate()
{
	return true;
}

void ADBDPlayer::Server_SendItemUseInput_Implementation(bool Pressed)
{

}

bool ADBDPlayer::Server_SendItemUseInput_Validate(bool Pressed)
{
	return true;
}

void ADBDPlayer::Server_SendItemUse_Implementation(bool Use)
{

}

bool ADBDPlayer::Server_SendItemUse_Validate(bool Use)
{
	return true;
}

void ADBDPlayer::Server_SendItemDropInput_Implementation(bool Pressed)
{

}

bool ADBDPlayer::Server_SendItemDropInput_Validate(bool Pressed)
{
	return true;
}

void ADBDPlayer::Server_SendInteractionInput_Implementation(bool Pressed)
{

}

bool ADBDPlayer::Server_SendInteractionInput_Validate(bool Pressed)
{
	return true;
}

void ADBDPlayer::Server_SendFastInteractionInput_Implementation(bool Pressed)
{

}

bool ADBDPlayer::Server_SendFastInteractionInput_Validate(bool Pressed)
{
	return true;
}

void ADBDPlayer::Server_RemoveStatusEffects_Implementation()
{

}

bool ADBDPlayer::Server_RemoveStatusEffects_Validate()
{
	return true;
}

void ADBDPlayer::Server_RemoveStatusEffectByID_Implementation(FName statusEffectID, bool bRemoveAllWithID)
{

}

bool ADBDPlayer::Server_RemoveStatusEffectByID_Validate(FName statusEffectID, bool bRemoveAllWithID)
{
	return true;
}

void ADBDPlayer::Server_RemovePerks_Implementation()
{

}

bool ADBDPlayer::Server_RemovePerks_Validate()
{
	return true;
}

void ADBDPlayer::Server_PlayerReady_Implementation()
{

}

bool ADBDPlayer::Server_PlayerReady_Validate()
{
	return true;
}

void ADBDPlayer::Server_OnMovementInput_Implementation(ADBDPlayer* Player)
{

}

bool ADBDPlayer::Server_OnMovementInput_Validate(ADBDPlayer* Player)
{
	return true;
}

void ADBDPlayer::Server_OnInsaneSkillCheck_Implementation()
{

}

bool ADBDPlayer::Server_OnInsaneSkillCheck_Validate()
{
	return true;
}

void ADBDPlayer::Server_Ghost_Implementation(bool Ghost)
{

}

bool ADBDPlayer::Server_Ghost_Validate(bool Ghost)
{
	return true;
}

void ADBDPlayer::Server_FailInterruption_Implementation(ADBDPlayer* interruptee)
{

}

bool ADBDPlayer::Server_FailInterruption_Validate(ADBDPlayer* interruptee)
{
	return true;
}

void ADBDPlayer::Server_Drop_Implementation(ACollectable* Item, const FVector& Location, const FRotator& Rotation, bool onDeath)
{

}

bool ADBDPlayer::Server_Drop_Validate(ACollectable* Item, const FVector& Location, const FRotator& Rotation, bool onDeath)
{
	return true;
}

void ADBDPlayer::Server_CheatDispatchOnPostItemAddonsCreation_Implementation()
{

}

bool ADBDPlayer::Server_CheatDispatchOnPostItemAddonsCreation_Validate()
{
	return true;
}

void ADBDPlayer::Server_Cheat_Implementation()
{

}

bool ADBDPlayer::Server_Cheat_Validate()
{
	return true;
}

void ADBDPlayer::ResetYawScale(float adjustTime)
{

}

void ADBDPlayer::ResetPitchScale(float adjustTime)
{

}

void ADBDPlayer::ResetMeshRelativeRotation()
{

}

void ADBDPlayer::ResetMeshRelativePosition()
{

}

void ADBDPlayer::ResetCamera()
{

}

void ADBDPlayer::RequestStun(EStunType stunType, ADBDPlayer* stunner)
{

}

void ADBDPlayer::RemoveStatusEffect(UStatusEffect* StatusEffect)
{

}

void ADBDPlayer::RemoveFromInventory(ACollectable* Item)
{

}

UAnimMontage* ADBDPlayer::PlayMontage(FAnimationMontageDescriptor animMontageID, float PlayRate, bool isFollower)
{
	return NULL;
}

void ADBDPlayer::OnPickupStart(ADBDPlayer* target)
{

}

void ADBDPlayer::OnPickupEnd(ADBDPlayer* target)
{

}

void ADBDPlayer::OnLocallyObservedChanged_Implementation(bool IsLocallyObserved)
{

}

void ADBDPlayer::OnLevelReadyToPlay()
{

}

void ADBDPlayer::OnCurrentMontageComplete(UAnimMontage* MontageAsset, bool interrupted)
{

}

void ADBDPlayer::OnCurrentMontageBlendingOut(UAnimMontage* MontageAsset, bool interrupted)
{

}

void ADBDPlayer::OnBlindChargeEmptied()
{

}

void ADBDPlayer::OnAttackFinished_Implementation(EAttackType attackType)
{

}

void ADBDPlayer::OnAllPlayerLoaded()
{

}

void ADBDPlayer::Multicast_TriggerSprintWithParams_Implementation(float Duration, UCurveFloat* Curve, FName effector)
{

}

bool ADBDPlayer::Multicast_TriggerSprintWithParams_Validate(float Duration, UCurveFloat* Curve, FName effector)
{
	return true;
}

void ADBDPlayer::Multicast_TriggerSprint_Implementation()
{

}

bool ADBDPlayer::Multicast_TriggerSprint_Validate()
{
	return true;
}

void ADBDPlayer::Multicast_SetWindowFreeCam_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetWindowFreeCam_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetRunVaultEnabled_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetRunVaultEnabled_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetReverseTraverseEnabled_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetReverseTraverseEnabled_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetMaxWalkSpeed_Implementation(float MaxWalkSpeed)
{

}

bool ADBDPlayer::Multicast_SetMaxWalkSpeed_Validate(float MaxWalkSpeed)
{
	return true;
}

void ADBDPlayer::Multicast_SetInteractingPlayer_Implementation(ADBDPlayer* interactingPlayer)
{

}

void ADBDPlayer::Multicast_SetForceAuthoritativeMovement_Implementation(bool Value)
{

}

bool ADBDPlayer::Multicast_SetForceAuthoritativeMovement_Validate(bool Value)
{
	return true;
}

void ADBDPlayer::Multicast_SetEthereal_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetEthereal_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetEnableServerPositionUpdates_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetEnableServerPositionUpdates_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetEnableCapsuleDynamicResize_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetEnableCapsuleDynamicResize_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetDebugSnapPoint_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetDebugSnapPoint_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetDebugCarry_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetDebugCarry_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetCustomization_Implementation(const TArray<FName>& customizationParts)
{

}

bool ADBDPlayer::Multicast_SetCustomization_Validate(const TArray<FName>& customizationParts)
{
	return true;
}

void ADBDPlayer::Multicast_SetContinuousPrintDebug_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetContinuousPrintDebug_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_SetAuthoritativeMovement_Implementation(bool Enabled)
{

}

bool ADBDPlayer::Multicast_SetAuthoritativeMovement_Validate(bool Enabled)
{
	return true;
}

void ADBDPlayer::Multicast_ServerResetMeshRelativeOffSet_Implementation()
{

}

bool ADBDPlayer::Multicast_ServerResetMeshRelativeOffSet_Validate()
{
	return true;
}

void ADBDPlayer::Multicast_RemoveFromInventory_Implementation(ACollectable* Item)
{

}

void ADBDPlayer::Multicast_PlayMontage_Implementation(FAnimationMontageDescriptor animMontageID, float PlayRate)
{

}

bool ADBDPlayer::Multicast_PlayMontage_Validate(FAnimationMontageDescriptor animMontageID, float PlayRate)
{
	return true;
}

void ADBDPlayer::Multicast_OnInsaneSkillCheck_Implementation()
{

}

bool ADBDPlayer::Multicast_OnInsaneSkillCheck_Validate()
{
	return true;
}

void ADBDPlayer::Multicast_LockOnTarget_Implementation(ADBDPlayer* target)
{

}

bool ADBDPlayer::Multicast_LockOnTarget_Validate(ADBDPlayer* target)
{
	return true;
}

void ADBDPlayer::Multicast_Ghost_Implementation(bool Ghost)
{

}

bool ADBDPlayer::Multicast_Ghost_Validate(bool Ghost)
{
	return true;
}

void ADBDPlayer::Multicast_DamageTarget_Implementation(ADBDPlayer* target, EAttackType attackType)
{

}

bool ADBDPlayer::Multicast_DamageTarget_Validate(ADBDPlayer* target, EAttackType attackType)
{
	return true;
}

void ADBDPlayer::Multicast_ConfirmItemDrop_Implementation(bool Pressed)
{

}

bool ADBDPlayer::Multicast_ConfirmItemDrop_Validate(bool Pressed)
{
	return true;
}

void ADBDPlayer::Multicast_ClearAndDestroyInventory_Implementation()
{

}

void ADBDPlayer::Multicast_AttackSubstate_Implementation(EAttackSubstate attackSubstate)
{

}

bool ADBDPlayer::Multicast_AttackSubstate_Validate(EAttackSubstate attackSubstate)
{
	return true;
}

void ADBDPlayer::Multicast_AttackBegin_Implementation(EAttackType attackType)
{

}

bool ADBDPlayer::Multicast_AttackBegin_Validate(EAttackType attackType)
{
	return true;
}

void ADBDPlayer::Multicast_AddToInventory_Implementation(ACollectable* Item)
{

}

void ADBDPlayer::Local_RequestBlink(FTransform Destination)
{

}

void ADBDPlayer::Local_NotifyMatchEnded_Implementation()
{

}

bool ADBDPlayer::LineTraceSingleForFootIK(const FVector Start, const FVector End, bool traceComplex, FHitResult& OutHit)
{
	return false;
}

bool ADBDPlayer::K2_SetActorLocationByBottomCapsule(FVector NewLocation, bool bSweep, FHitResult& SweepHitResult)
{
	return false;
}

bool ADBDPlayer::IsUsingAimItem() const
{
	return false;
}

bool ADBDPlayer::IsUnintentionallyIncapacitated() const
{
	return false;
}

bool ADBDPlayer::IsStrafing() const
{
	return false;
}

bool ADBDPlayer::IsSprinting() const
{
	return false;
}

bool ADBDPlayer::IsSnappingRotation() const
{
	return false;
}

bool ADBDPlayer::IsSnappingPosition() const
{
	return false;
}

bool ADBDPlayer::IsSnapping() const
{
	return false;
}

bool ADBDPlayer::IsRunning() const
{
	return false;
}

bool ADBDPlayer::IsPlayingMontage(FAnimationMontageDescriptor animMontageID) const
{
	return false;
}

bool ADBDPlayer::IsPlayingAnyMontage() const
{
	return false;
}

bool ADBDPlayer::IsLocationClear(FVector targetPosition) const
{
	return false;
}

bool ADBDPlayer::IsLocallyObserved() const
{
	return false;
}

bool ADBDPlayer::IsInTerrorRadius() const
{
	return false;
}

bool ADBDPlayer::IsInteractionInputPressed(EInputInteractionType interactionType) const
{
	return false;
}

bool ADBDPlayer::IsInStalkMode() const
{
	return false;
}

bool ADBDPlayer::IsInParadise() const
{
	return false;
}

bool ADBDPlayer::IsInMeathookZone() const
{
	return false;
}

bool ADBDPlayer::IsInChaseCooldown() const
{
	return false;
}

bool ADBDPlayer::IsIncapacitated() const
{
	return false;
}

bool ADBDPlayer::IsHeadHidden() const
{
	return false;
}

bool ADBDPlayer::IsFacing(FVector Direction) const
{
	return false;
}

bool ADBDPlayer::IsExhausted() const
{
	return false;
}

bool ADBDPlayer::IsCrouching() const
{
	return false;
}

bool ADBDPlayer::IsCloaked() const
{
	return false;
}

bool ADBDPlayer::IsBlinded() const
{
	return false;
}

bool ADBDPlayer::IsAllowedNavigation() const
{
	return false;
}

void ADBDPlayer::HideHead(bool hide)
{

}

bool ADBDPlayer::HasTrapImmunity() const
{
	return false;
}

bool ADBDPlayer::HasMoveInput() const
{
	return false;
}

bool ADBDPlayer::HasItem() const
{
	return false;
}

bool ADBDPlayer::HasInInventory(TSubclassOf<ACollectable> Item)
{
	return false;
}

bool ADBDPlayer::HasEquippedItem() const
{
	return false;
}

bool ADBDPlayer::HasDamageImmunity() const
{
	return false;
}

bool ADBDPlayer::HasClearPathToTargetWithIgnore(FVector targetPosition, const TArray<AActor*>& ignoreActorsNew) const
{
	return false;
}

bool ADBDPlayer::HasClearPathToTarget(FVector targetPosition) const
{
	return false;
}

bool ADBDPlayer::HasClearPathToItemDropOffPosition() const
{
	return false;
}

bool ADBDPlayer::HasAnimMontageEnded() const
{
	return false;
}

bool ADBDPlayer::GetWindowFreeCam() const
{
	return false;
}

float ADBDPlayer::GetTunableValue(FName Key, float DefaultValue, bool warnIfRowMissing) const
{
	return 0.0f;
}

float ADBDPlayer::GetStamina() const
{
	return 0.0f;
}

FName ADBDPlayer::GetSprintEffector() const
{
	return NAME_None;
}

float ADBDPlayer::GetSprintDurationTimerPercentage() const
{
	return 0.0f;
}

bool ADBDPlayer::GetRunVaultEnabled() const
{
	return false;
}

FString ADBDPlayer::GetRightFootAudioSurfaceName() const
{
	return TEXT("");
}

bool ADBDPlayer::GetReverseTraverseEnabled() const
{
	return false;
}

FRotator ADBDPlayer::GetRepControlRotation() const
{
	return FRotator{};
}

float ADBDPlayer::GetRemainingTrapImmunityDuration() const
{
	return 0.0f;
}

float ADBDPlayer::GetProgressToEscapeCarry(ADBDPlayer* Player)
{
	return 0.0f;
}

EPlayerRole ADBDPlayer::GetPlayerRole() const
{
	return EPlayerRole::VE_None;
}

UPlayerInteractionHandler* ADBDPlayer::GetPlayerInteractionHandler() const
{
	return NULL;
}

FRotator ADBDPlayer::GetPlayerDirection() const
{
	return FRotator{};
}

ADBDPlayerController* ADBDPlayer::GetPlayerController() const
{
	return NULL;
}

UCameraComponent* ADBDPlayer::GetPlayerCamera() const
{
	return NULL;
}

UPerkManager* ADBDPlayer::GetPerkManager() const
{
	return NULL;
}

float ADBDPlayer::GetPercentMovementSpeed() const
{
	return 0.0f;
}

float ADBDPlayer::GetPercentBlinded() const
{
	return 0.0f;
}

FString ADBDPlayer::GetNameDebugString() const
{
	return TEXT("");
}

UMontagePlayer* ADBDPlayer::GetMontagePlayer() const
{
	return NULL;
}

float ADBDPlayer::GetMontageLength(FAnimationMontageDescriptor animMontageID) const
{
	return 0.0f;
}

UAnimMontage* ADBDPlayer::GetMontage(FAnimationMontageDescriptor animMontageID) const
{
	return NULL;
}

FVector ADBDPlayer::GetMeshFeetPosition() const
{
	return FVector{};
}

float ADBDPlayer::GetMaxSpeed() const
{
	return 0.0f;
}

float ADBDPlayer::GetLuck() const
{
	return 0.0f;
}

FString ADBDPlayer::GetLeftFootAudioSurfaceName() const
{
	return TEXT("");
}

TArray<UItemModifier*> ADBDPlayer::GetItemModifiers() const
{
	return TArray<UItemModifier*>();
}

FName ADBDPlayer::GetItemID() const
{
	return NAME_None;
}

USceneComponent* ADBDPlayer::GetItemDropOffTransform() const
{
	return NULL;
}

int32 ADBDPlayer::GetItemCountByID(FName ItemId) const
{
	return 0;
}

int32 ADBDPlayer::GetItemCount() const
{
	return 0;
}

TArray<UItemAddon*> ADBDPlayer::GetItemAddons() const
{
	return TArray<UItemAddon*>();
}

ACollectable* ADBDPlayer::GetItem() const
{
	return NULL;
}

bool ADBDPlayer::GetIsInterrupting() const
{
	return false;
}

bool ADBDPlayer::GetIsBeyondCrouchWalkSpeed() const
{
	return false;
}

FInteractionPlayerProperties ADBDPlayer::GetInteractionPlayerProperties()
{
	return FInteractionPlayerProperties{};
}

float ADBDPlayer::GetInteractingMaxSpeedPercent() const
{
	return 0.0f;
}

AInteractable* ADBDPlayer::GetInteractable() const
{
	return NULL;
}

ADBDPlayer* ADBDPlayer::GetGuidingPlayer() const
{
	return NULL;
}

UActorComponent* ADBDPlayer::GetFirstComponentByClass(TSubclassOf<UActorComponent> ComponentClass) const
{
	return NULL;
}

FVector ADBDPlayer::GetFeetPositionAtTargetLocation(FVector Location) const
{
	return FVector{};
}

FVector ADBDPlayer::GetFeetPosition() const
{
	return FVector{};
}

bool ADBDPlayer::GetFeetOnGround() const
{
	return false;
}

bool ADBDPlayer::GetEnableServerPositionUpdates() const
{
	return false;
}

UPrimitiveComponent* ADBDPlayer::GetDetectionZone(EDetectionZone detectionZoneID) const
{
	return NULL;
}

bool ADBDPlayer::GetDebugFailAllInteractions() const
{
	return false;
}

ADBDPlayerState* ADBDPlayer::GetDBDPlayerState() const
{
	return NULL;
}

UCustomizedSkeletalMesh* ADBDPlayer::GetCustomizedSkeletalMesh_Implementation()
{
	return NULL;
}

EInteractionAnimation ADBDPlayer::GetCurrentInteractionAnimation() const
{
	return EInteractionAnimation::VE_None;
}

UInteractionDefinition* ADBDPlayer::GetCurrentInteraction() const
{
	return NULL;
}

bool ADBDPlayer::GetContinuousPrintDebug() const
{
	return false;
}

UCharacterSnappingComponent* ADBDPlayer::GetCharacterSnappingComponent() const
{
	return NULL;
}

FString ADBDPlayer::GetCharacterAudioSurfaceName() const
{
	return TEXT("");
}

ADBDPlayerCameraManager* ADBDPlayer::GetCameraManager()
{
	return NULL;
}

FVector ADBDPlayer::GetCameraLocation() const
{
	return FVector{};
}

FVector ADBDPlayer::GetCameraAnchorLocation() const
{
	return FVector{};
}

FVector ADBDPlayer::GetBoneRelativeLocation(FName TargetBone, FName relativeBone) const
{
	return FVector{};
}

FVector ADBDPlayer::GetBoneLocation(FName Name, EBoneSpaces::Type space) const
{
	return FVector{};
}

float ADBDPlayer::GetBlinkTime() const
{
	return 0.0f;
}

float ADBDPlayer::GetBaseFOV() const
{
	return 0.0f;
}

FVector ADBDPlayer::GetAverageVelocity() const
{
	return FVector{};
}

EAttackType ADBDPlayer::GetAttackMontageEvent() const
{
	return EAttackType::VE_None;
}

USceneComponent* ADBDPlayer::GetAttachPoint_Implementation(FName attachPointName) const
{
	return NULL;
}

void ADBDPlayer::GetAnimTags(TArray<FName>& outTags) const
{

}

UAnimInstance* ADBDPlayer::GetAnimInstanceConst() const
{
	return NULL;
}

UAnimInstance* ADBDPlayer::GetAnimInstance()
{
	return NULL;
}

FAnimData ADBDPlayer::GetAnimData()
{
	return FAnimData{};
}

FVector ADBDPlayer::GetAlmostCurrentVelocity() const
{
	return FVector{};
}

FVector ADBDPlayer::GetActorLocationFromFeetLocation(const FVector FeetLocation) const
{
	return FVector{};
}

FVector ADBDPlayer::FindFurthestClearLocationBetweenPoints(FVector StartPosition, FVector targetPosition) const
{
	return FVector{};
}

void ADBDPlayer::EquipItem()
{

}

void ADBDPlayer::Drop(ACollectable* Item, const FVector& Location, const FRotator& Rotation, bool onDeath, bool adjustToGround)
{

}

void ADBDPlayer::DetachInteractor()
{

}

void ADBDPlayer::DebugPrint(const FString& inString) const
{

}

void ADBDPlayer::DeactivateSkillChecks()
{

}

void ADBDPlayer::DBDUnCrouch()
{

}

void ADBDPlayer::DBDLog(const FString& LogText)
{

}

void ADBDPlayer::DBDCrouch()
{

}

void ADBDPlayer::DBD_TogglePrintServerPositionUpdateEnabled()
{

}

void ADBDPlayer::DBD_ToggleForceAuthoritativeMovement()
{

}

void ADBDPlayer::DBD_ToggleDebugInteractionInZone()
{

}

void ADBDPlayer::DBD_SetMouseTurnSpeedMultiplier(float Value)
{

}

void ADBDPlayer::DBD_SetMouseLookUpSpeedMultiplier(float Value)
{

}

void ADBDPlayer::DBD_SetGamePadTurnRate(float Value)
{

}

void ADBDPlayer::DBD_SetGamePadLookUpRate(float Value)
{

}

void ADBDPlayer::DBD_DebugStartInteraction()
{

}

void ADBDPlayer::ConsumeStamina(float Stamina)
{

}

void ADBDPlayer::Collect(ACollectable* Item)
{

}

void ADBDPlayer::Client_TryInteractionType_Implementation(EInputInteractionType interactionType, ADBDPlayer* Requester)
{

}

bool ADBDPlayer::Client_TryInteractionType_Validate(EInputInteractionType interactionType, ADBDPlayer* Requester)
{
	return true;
}

void ADBDPlayer::Client_SendCancelInteraction_Implementation()
{

}

bool ADBDPlayer::Client_SendCancelInteraction_Validate()
{
	return true;
}

void ADBDPlayer::ClearAndDestroyInventory()
{

}

bool ADBDPlayer::CanSprint() const
{
	return false;
}

bool ADBDPlayer::CanInterrupt_BP_Implementation() const
{
	return false;
}

bool ADBDPlayer::CanInteract()
{
	return false;
}

bool ADBDPlayer::CanCollectExtraItem(ACollectable* Item) const
{
	return false;
}

bool ADBDPlayer::CanAttack_BP_Implementation()
{
	return false;
}

bool ADBDPlayer::CanAttack()
{
	return false;
}

void ADBDPlayer::CameraUpdated()
{

}

void ADBDPlayer::Broadcast_PlayMontage_Server_Implementation(FAnimationMontageDescriptor animMontageID, float PlayRate)
{

}

bool ADBDPlayer::Broadcast_PlayMontage_Server_Validate(FAnimationMontageDescriptor animMontageID, float PlayRate)
{
	return true;
}

void ADBDPlayer::Broadcast_PlayMontage_Multicast_Implementation(FAnimationMontageDescriptor animMontageID, float PlayRate)
{

}

bool ADBDPlayer::Broadcast_PlayMontage_Multicast_Validate(FAnimationMontageDescriptor animMontageID, float PlayRate)
{
	return true;
}

void ADBDPlayer::Broadcast_Multicast_BlinkDestination_Implementation(FVector Position, FRotator Rotation)
{

}

bool ADBDPlayer::Broadcast_Multicast_BlinkDestination_Validate(FVector Position, FRotator Rotation)
{
	return true;
}

void ADBDPlayer::Broadcast_LockOnTarget_Server_Implementation(ADBDPlayer* target)
{

}

bool ADBDPlayer::Broadcast_LockOnTarget_Server_Validate(ADBDPlayer* target)
{
	return true;
}

void ADBDPlayer::Broadcast_LockOnTarget_Multicast_Implementation(ADBDPlayer* target)
{

}

bool ADBDPlayer::Broadcast_LockOnTarget_Multicast_Validate(ADBDPlayer* target)
{
	return true;
}

void ADBDPlayer::Broadcast_DamageTarget_Server_Implementation(ADBDPlayer* target, EAttackType attackType)
{

}

bool ADBDPlayer::Broadcast_DamageTarget_Server_Validate(ADBDPlayer* target, EAttackType attackType)
{
	return true;
}

void ADBDPlayer::Broadcast_DamageTarget_Multicast_Implementation(ADBDPlayer* target, EAttackType attackType)
{

}

bool ADBDPlayer::Broadcast_DamageTarget_Multicast_Validate(ADBDPlayer* target, EAttackType attackType)
{
	return true;
}

void ADBDPlayer::Broadcast_BlinkDestination_Implementation(FVector Position, FRotator Rotation)
{

}

bool ADBDPlayer::Broadcast_BlinkDestination_Validate(FVector Position, FRotator Rotation)
{
	return true;
}

void ADBDPlayer::Broadcast_AttackSubstate_Server_Implementation(EAttackSubstate attackSubstate)
{

}

bool ADBDPlayer::Broadcast_AttackSubstate_Server_Validate(EAttackSubstate attackSubstate)
{
	return true;
}

void ADBDPlayer::Broadcast_AttackSubstate_Multicast_Implementation(EAttackSubstate attackSubstate)
{

}

bool ADBDPlayer::Broadcast_AttackSubstate_Multicast_Validate(EAttackSubstate attackSubstate)
{
	return true;
}

void ADBDPlayer::AuthorityTriggerSprintWithParams(float Duration, float coolDown, UCurveFloat* Curve, FName effector)
{

}

void ADBDPlayer::AuthorityTriggerSprint()
{

}

void ADBDPlayer::AuthorityFlashlighted(ADBDPlayer* blinder)
{

}

void ADBDPlayer::AuthorityAttemptEscapeCarry(ACamperPlayer* Player)
{

}

bool ADBDPlayer::Authority_TryForceEndOngoingScoreEvent(EDBDScoreTypes ScoreType)
{
	return false;
}

bool ADBDPlayer::Authority_TryForceCancelOngoingScoreEvent(EDBDScoreTypes ScoreType)
{
	return false;
}

void ADBDPlayer::Authority_RemoveStatusEffectByID(FName statusEffectID, bool bRemoveAllWithID)
{

}

UStatusEffect* ADBDPlayer::Authority_ImposeStatusEffect(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParam, UGameplayModifierContainer* originatingEffect)
{
	return NULL;
}

UStatusEffect* ADBDPlayer::Authority_ImposeDynamicStatusEffect(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParam, UGameplayModifierContainer* originatingEffect)
{
	return NULL;
}

void ADBDPlayer::Authority_Blinded(EBlindType blindType, float coolDown, AActor* effector)
{

}

void ADBDPlayer::Authority_ActivateAuthoritativeMovementOnDropped()
{

}

void ADBDPlayer::AttachInteractor(UInteractor* Interactor)
{

}

bool ADBDPlayer::ArePlayersHandsFullOfBearTraps() const
{
	return false;
}

void ADBDPlayer::AddToInventory(ACollectable* Item)
{

}

void ADBDPlayer::AddStatusEffect(UStatusEffect* StatusEffect)
{

}

void ADBDPlayer::AddPerk(UPerk* Perk)
{

}

void ADBDPlayer::AddDetectionZone(EDetectionZone detectionZoneID, UPrimitiveComponent* zone)
{

}

void ADBDPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADBDPlayer, _perks);
	DOREPLIFETIME(ADBDPlayer, _offerings);
	DOREPLIFETIME(ADBDPlayer, _statusEffects);
	DOREPLIFETIME(ADBDPlayer, _interactable);
	DOREPLIFETIME(ADBDPlayer, _blindnessRemainingTime);
	DOREPLIFETIME(ADBDPlayer, _screenAspectRatio);
}

ADBDPlayer::ADBDPlayer()
{
	this->Interactable = NULL;
	this->AllowInterrupting = false;
	this->MinFallHeight = 50.000000;
	this->IsInterruptable = false;
	this->IsStunned = false;
	this->IsAttacking = false;
	this->AllowNavigationInput = false;
	this->AllowNavigationBackwardInput = true;
	this->AllowTurningInput = false;
	this->AllowPitchInput = false;
	this->PitchLimitLower = -40.000000;
	this->PitchLimitUpper = 40.000000;
	this->PelvisHeight = 1.000000;
	this->PelvisHeightForPounceOnStandingCamper = 0.500000;
	this->PelvisHeightForPounceOnCrouchingCamper = 0.000000;
	this->ForceDisableFootIK = false;
	this->ForceSkillChecks = false;
	this->CameraResetSpeed = 2.250000;
	this->CharacterName = TEXT("");
	this->CharacterClothes = TEXT("");
	this->CharacterShoes = TEXT("");
	this->CharacterWeapon = TEXT("");
	this->CharacterAmbiance = TEXT("");
	this->CharacterState = TEXT("");
	this->DebugInteractionPriorityEnabled = false;
	this->_emulatedYawInputDecay = 0.200000;
	this->_emulatedYawInputDecayWhenNoInput = 7.000000;
	this->_emulatedYawInputReleaseTime = 0.750000;
	this->_emulatedYawInputDeadZone = 0.075000;
	this->_emulatedYawMinimumInput = 0.100000;
	this->_emulatedYawInputStrength = 0.200000;
	this->CurrentBlinkDistance = 0.000000;
	this->IsInBlinkingState = false;
	this->AverageSpeedBufferTime = 0.750000;
	this->AlmostCurrentSpeedBufferTime = 0.100000;
	this->BlindingChargeableComponent = CreateDefaultSubobject<UChargeableComponent>(TEXT("BlindnessChargeableComponent"));
	this->CarryJointName = TEXT("Camper_SocketLT");
	this->_pitchOffsetForInteractionPriority = 0.000000;
	this->IsInteracting = false;
	this->ClearPathTestRadiusMultiplier = 0.900000;
	this->ClearPathTestHeightMultiplier = 0.900000;
	this->LocationClearTestRadiusMultiplier = 0.900000;
	this->LocationClearTestHeightMultiplier = 0.900000;
	this->ClearPathTestStepHeight = 20.000000;
	this->GamePadTurnRate = 45.000000;
	this->GamePadLookUpRate = 45.000000;
	this->MouseTurnSpeedMultiplier = 1.000000;
	this->MouseLookUpSpeedMultiplier = 1.000000;
	this->CurrentCamperState = ECamperState::VE_None;
	this->CollectableAttachPoint = NAME_None;
	this->_interactionHandler = CreateDefaultSubobject<UPlayerInteractionHandler>(TEXT("InteractionHandler"));
	this->_montagePlayer = CreateDefaultSubobject<UMontagePlayer>(TEXT("MontagePlayer"));
	this->_perkManager = CreateDefaultSubobject<UPerkManager>(TEXT("PerkManager"));
	this->_nextAttackSubstate = EAttackSubstate::VE_None;
	this->_characterSnappingComponent = CreateDefaultSubobject<UCharacterSnappingComponent>(TEXT("SnappingComponent"));
	this->_cameraHandlerComponent = CreateDefaultSubobject<UCameraHandlerComponent>(TEXT("CameraHandlerComponent"));
	this->_itemDropOffPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ItemDropOffPosition"));
	this->_eventListener = CreateDefaultSubobject<UPollableEventListener>(TEXT("EventListener"));
	this->_meatHookZoneDetector = CreateDefaultSubobject<UZoneDetectorComponent>(TEXT("MeatHookZoneDetector"));
	this->_authoritativeMovementComponent = CreateDefaultSubobject<UAuthoritativeMovementComponent>(TEXT("AuthoritativeMovement"));
	this->_blindnessRemainingTime = 0.000000;
	this->_screenAspectRatio = 1.777778;
}
