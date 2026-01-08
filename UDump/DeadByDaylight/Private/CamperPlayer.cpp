#include "CamperPlayer.h"
#include "ECamperImmobilizeState.h"
#include "ECamperGuidedAction.h"
#include "ECamperDamageState.h"
#include "Engine/HitResult.h"
#include "EAuthoritativeMovementFlag.h"
#include "UObject/NoExportTypes.h"
#include "Net/UnrealNetwork.h"
#include "FILE_NOT_FOUND"
#include "StalkedComponent.h"
#include "CamperStillnessTrackerComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "DBDPawnSensingComponent.h"
#include "ChargeableComponent.h"

class AActor;
class USceneComponent;
class ADBDPlayer;
class UPrimitiveComponent;
class APawn;
class UInteractionDefinition;
class UDBDSpringArmComponent;
class UAnimationMontageSlave;
class UDBDClipRegionComponent;
class UDBDCamperData;

void ACamperPlayer::UpdateSlasherProximityEvents(float DeltaSeconds)
{

}

void ACamperPlayer::UpdateHealingTimer(float deltaTime)
{

}

void ACamperPlayer::UpdateDrainTimer(float deltaTime)
{

}

void ACamperPlayer::TriggerWiggleSkillCheck()
{

}

void ACamperPlayer::TriggerAfflictionHUDFeedback()
{

}

void ACamperPlayer::StopHealingTimer()
{

}

void ACamperPlayer::StartHealingTimer(float Time)
{

}

void ACamperPlayer::SetOverlappingEscape(AActor* Escape)
{

}

void ACamperPlayer::SetIsSacrificed(bool sacrificed)
{

}

void ACamperPlayer::SetImmobilized(ECamperImmobilizeState State)
{

}

void ACamperPlayer::SetGuidedActionLocal(ECamperGuidedAction Action)
{

}

void ACamperPlayer::SetGuidedAction(ECamperGuidedAction Action)
{

}

void ACamperPlayer::SetEnableHookSlashableZone(bool enable)
{

}

void ACamperPlayer::SetDamageState(ECamperDamageState E, bool FireScoreEvent)
{

}

void ACamperPlayer::SetCameraAttachment(USceneComponent* Parent, FName SocketName)
{

}

void ACamperPlayer::SetBeingCarried(bool IsBeingCarried, ADBDPlayer* carrier)
{

}

void ACamperPlayer::Server_SetOnHookDrainToNextStage_Implementation(bool Enabled)
{

}

bool ACamperPlayer::Server_SetOnHookDrainToNextStage_Validate(bool Enabled)
{
	return true;
}

void ACamperPlayer::Server_SetItemNormalizedEnergyLevel_Implementation(float normalizedLevel)
{

}

bool ACamperPlayer::Server_SetItemNormalizedEnergyLevel_Validate(float normalizedLevel)
{
	return true;
}

void ACamperPlayer::Server_SetItemEnergyLevel_Implementation(float Level)
{

}

bool ACamperPlayer::Server_SetItemEnergyLevel_Validate(float Level)
{
	return true;
}

void ACamperPlayer::Server_SetImmobilized_Implementation(ECamperImmobilizeState State)
{

}

bool ACamperPlayer::Server_SetImmobilized_Validate(ECamperImmobilizeState State)
{
	return true;
}

void ACamperPlayer::Server_SetHookEscapeAutoSuccess_Implementation(bool Enabled)
{

}

bool ACamperPlayer::Server_SetHookEscapeAutoSuccess_Validate(bool Enabled)
{
	return true;
}

void ACamperPlayer::Server_SetEnableHookSlashableZone_Implementation(bool Enabled)
{

}

bool ACamperPlayer::Server_SetEnableHookSlashableZone_Validate(bool Enabled)
{
	return true;
}

void ACamperPlayer::Server_SendLeftRightMashedInput_Implementation(bool Pressed)
{

}

bool ACamperPlayer::Server_SendLeftRightMashedInput_Validate(bool Pressed)
{
	return true;
}

void ACamperPlayer::Server_SendInteractMashedInput_Implementation(bool Pressed)
{

}

bool ACamperPlayer::Server_SendInteractMashedInput_Validate(bool Pressed)
{
	return true;
}

void ACamperPlayer::Server_RemoveHealth_Implementation()
{

}

bool ACamperPlayer::Server_RemoveHealth_Validate()
{
	return true;
}

void ACamperPlayer::Server_MakeInvulnerable_Implementation()
{

}

bool ACamperPlayer::Server_MakeInvulnerable_Validate()
{
	return true;
}

void ACamperPlayer::Server_Kill_Implementation(bool sacrificed)
{

}

bool ACamperPlayer::Server_Kill_Validate(bool sacrificed)
{
	return true;
}

void ACamperPlayer::Server_IncreaseHealth_Implementation()
{

}

bool ACamperPlayer::Server_IncreaseHealth_Validate()
{
	return true;
}

void ACamperPlayer::Server_CheatEscaped_Implementation()
{

}

bool ACamperPlayer::Server_CheatEscaped_Validate()
{
	return true;
}

void ACamperPlayer::ResetPotentialSaviors()
{

}

void ACamperPlayer::ResetHideTimer()
{

}

void ACamperPlayer::ResetCameraAttachment()
{

}

void ACamperPlayer::OnUnhooked()
{

}

void ACamperPlayer::OnProximityEntered(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ACamperPlayer::OnPawnSensed(APawn* Pawn)
{

}

void ACamperPlayer::OnHealthBarCharged(bool complete)
{

}

void ACamperPlayer::OnDroppedStart()
{

}

void ACamperPlayer::OnDroppedEnd(bool clearLeader)
{

}

void ACamperPlayer::Multicast_SetOnHookDrainToNextStage_Implementation(bool Enabled)
{

}

bool ACamperPlayer::Multicast_SetOnHookDrainToNextStage_Validate(bool Enabled)
{
	return true;
}

void ACamperPlayer::Multicast_SetImmobilized_Implementation(ECamperImmobilizeState State)
{

}

bool ACamperPlayer::Multicast_SetImmobilized_Validate(ECamperImmobilizeState State)
{
	return true;
}

void ACamperPlayer::Multicast_SetHookEscapeAutoSuccess_Implementation(bool Enabled)
{

}

bool ACamperPlayer::Multicast_SetHookEscapeAutoSuccess_Validate(bool Enabled)
{
	return true;
}

void ACamperPlayer::Multicast_SetGuidedAction_Implementation(ECamperGuidedAction Action)
{

}

bool ACamperPlayer::Multicast_SetGuidedAction_Validate(ECamperGuidedAction Action)
{
	return true;
}

void ACamperPlayer::Multicast_SetEnableHookSlashableZone_Implementation(bool Enabled)
{

}

bool ACamperPlayer::Multicast_SetEnableHookSlashableZone_Validate(bool Enabled)
{
	return true;
}

void ACamperPlayer::Multicast_SetAuthoritativeMovementFlag_Implementation(EAuthoritativeMovementFlag flag, bool Value)
{

}

bool ACamperPlayer::Multicast_SetAuthoritativeMovementFlag_Validate(EAuthoritativeMovementFlag flag, bool Value)
{
	return true;
}

void ACamperPlayer::Multicast_OnKO_Implementation()
{

}

void ACamperPlayer::Multicast_OnHealed_Implementation()
{

}

bool ACamperPlayer::Multicast_OnHealed_Validate()
{
	return true;
}

void ACamperPlayer::Multicast_OnFailedStruggling_Implementation(UInteractionDefinition* Interaction)
{

}

bool ACamperPlayer::Multicast_OnFailedStruggling_Validate(UInteractionDefinition* Interaction)
{
	return true;
}

void ACamperPlayer::MoveToOwnerCarryJoint()
{

}

bool ACamperPlayer::IsUnintentionallyImmobilized() const
{
	return false;
}

bool ACamperPlayer::IsTrapped() const
{
	return false;
}

bool ACamperPlayer::IsTotallyInsane() const
{
	return false;
}

bool ACamperPlayer::IsSuccessfullyHidden() const
{
	return false;
}

bool ACamperPlayer::IsSacrificed() const
{
	return false;
}

bool ACamperPlayer::IsObsessionTarget() const
{
	return false;
}

bool ACamperPlayer::IsMovementGuided() const
{
	return false;
}

bool ACamperPlayer::IsKO() const
{
	return false;
}

bool ACamperPlayer::IsInsideCloset() const
{
	return false;
}

bool ACamperPlayer::IsInNeed() const
{
	return false;
}

bool ACamperPlayer::IsInjuredBleedoutTimerExpired() const
{
	return false;
}

bool ACamperPlayer::IsInjured() const
{
	return false;
}

bool ACamperPlayer::IsInInjuredBleedout() const
{
	return false;
}

bool ACamperPlayer::IsInBasement() const
{
	return false;
}

bool ACamperPlayer::IsImmobilized() const
{
	return false;
}

bool ACamperPlayer::IsHooked() const
{
	return false;
}

bool ACamperPlayer::IsHealthy() const
{
	return false;
}

bool ACamperPlayer::IsHealingTimerExpired() const
{
	return false;
}

bool ACamperPlayer::IsGuidedBySlasher() const
{
	return false;
}

bool ACamperPlayer::IsEscaped() const
{
	return false;
}

bool ACamperPlayer::IsDyingTimerExpired() const
{
	return false;
}

bool ACamperPlayer::IsDrainTimerExpired() const
{
	return false;
}

bool ACamperPlayer::IsDead() const
{
	return false;
}

bool ACamperPlayer::IsCrawling() const
{
	return false;
}

bool ACamperPlayer::IsCameraDetached() const
{
	return false;
}

bool ACamperPlayer::IsCameraAttached() const
{
	return false;
}

bool ACamperPlayer::IsBeingPutOnHook() const
{
	return false;
}

bool ACamperPlayer::IsBeingPutDown() const
{
	return false;
}

bool ACamperPlayer::IsBeingPickedUp() const
{
	return false;
}

bool ACamperPlayer::IsBeingKilled() const
{
	return false;
}

bool ACamperPlayer::IsBeingInteractedWith() const
{
	return false;
}

bool ACamperPlayer::IsBeingHealed() const
{
	return false;
}

bool ACamperPlayer::IsBeingChased() const
{
	return false;
}

bool ACamperPlayer::IsBeingCarried() const
{
	return false;
}

void ACamperPlayer::IncreaseHPSlotSize(float percent)
{

}

bool ACamperPlayer::HasHitEvents() const
{
	return false;
}

bool ACamperPlayer::HasGuidedAction() const
{
	return false;
}

float ACamperPlayer::GetWiggleFreeChargePercent() const
{
	return 0.0f;
}

FRotator ACamperPlayer::GetToAimPointRotation() const
{
	return FRotator{};
}

float ACamperPlayer::GetStillness() const
{
	return 0.0f;
}

UDBDSpringArmComponent* ACamperPlayer::GetSpringArmComponent_Implementation()
{
	return NULL;
}

float ACamperPlayer::GetPercentInjuredBleedoutTimer() const
{
	return 0.0f;
}

float ACamperPlayer::GetPercentHealingTimer() const
{
	return 0.0f;
}

float ACamperPlayer::GetPercentDyingTimer() const
{
	return 0.0f;
}

float ACamperPlayer::GetPercentDropStaggerCooldownTimer() const
{
	return 0.0f;
}

float ACamperPlayer::GetPercentDrainTimer() const
{
	return 0.0f;
}

AActor* ACamperPlayer::GetOverlappingEscape() const
{
	return NULL;
}

float ACamperPlayer::GetObsessionTargetWeight() const
{
	return 0.0f;
}

int32 ACamperPlayer::GetNumberOfEntityRenderRegions() const
{
	return 0;
}

UAnimationMontageSlave* ACamperPlayer::GetMontageFollower()
{
	return NULL;
}

int32 ACamperPlayer::GetMadnessTierBPNative_Implementation()
{
	return 0;
}

ECamperImmobilizeState ACamperPlayer::GetImmobilizedState() const
{
	return ECamperImmobilizeState::VE_None;
}

ECamperGuidedAction ACamperPlayer::GetGuidedAction() const
{
	return ECamperGuidedAction::VE_None;
}

UDBDClipRegionComponent* ACamperPlayer::GetEntityRenderRegionAtIndex(int32 InIndex) const
{
	return NULL;
}

float ACamperPlayer::GetDropStaggerTimeLeft() const
{
	return 0.0f;
}

float ACamperPlayer::GetDropStaggerPercentTimeElapsed() const
{
	return 0.0f;
}

ECamperDamageState ACamperPlayer::GetDamageState() const
{
	return ECamperDamageState::VE_Healthy;
}

float ACamperPlayer::GetCurrentTotalHealthBarPercent() const
{
	return 0.0f;
}

UChargeableComponent* ACamperPlayer::GetCurrentHPSlot()
{
	return NULL;
}

UCamperStillnessTrackerComponent* ACamperPlayer::GetCamperStillnessComponent()
{
	return NULL;
}

UDBDCamperData* ACamperPlayer::GetCamperData() const
{
	return NULL;
}

TArray<UDBDClipRegionComponent*> ACamperPlayer::GetAllEntityRenderRegions() const
{
	return TArray<UDBDClipRegionComponent*>();
}

FVector ACamperPlayer::GetAimPoint() const
{
	return FVector{};
}

void ACamperPlayer::Escape(AActor* Escape)
{

}

void ACamperPlayer::DetachFromGuidingPlayer()
{

}

void ACamperPlayer::DetachCamera()
{

}

void ACamperPlayer::DBDApplyDamage(bool& didDamage, bool& causedKO)
{

}

void ACamperPlayer::DBD_LeaveParadise()
{

}

void ACamperPlayer::DBD_ForceRun(bool Forced)
{

}

void ACamperPlayer::DBD_CheatEscaped()
{

}

void ACamperPlayer::DBD_AutoLeftRightMash(bool Enabled)
{

}

void ACamperPlayer::DBD_AutoInteractionMash(bool Enabled)
{

}

bool ACamperPlayer::ComputeHookEscapeResult()
{
	return false;
}

bool ACamperPlayer::ComputeBeartrapEscapeResult()
{
	return false;
}

void ACamperPlayer::Client_OnSlashedOutOfTrap_Implementation(ADBDPlayer* Requester)
{

}

bool ACamperPlayer::Client_OnSlashedOutOfTrap_Validate(ADBDPlayer* Requester)
{
	return true;
}

bool ACamperPlayer::CanStruggle() const
{
	return false;
}

bool ACamperPlayer::CanRushQuietly() const
{
	return false;
}

bool ACamperPlayer::CanInteractWithItems() const
{
	return false;
}

bool ACamperPlayer::CanHealSelf() const
{
	return false;
}

bool ACamperPlayer::CanGainInsanity() const
{
	return false;
}

bool ACamperPlayer::CanBeShocked() const
{
	return false;
}

bool ACamperPlayer::CanBePickedUp() const
{
	return false;
}

bool ACamperPlayer::CanBeKilled() const
{
	return false;
}

bool ACamperPlayer::CanBeImmediatelyKO() const
{
	return false;
}

bool ACamperPlayer::CanBeHealedFromInjured() const
{
	return false;
}

bool ACamperPlayer::CanBeHealed() const
{
	return false;
}

bool ACamperPlayer::CanBecomeHealthy() const
{
	return false;
}

bool ACamperPlayer::CanBeChased() const
{
	return false;
}

void ACamperPlayer::Broadcast_SendRunInput_Server_Implementation(bool Pressed)
{

}

bool ACamperPlayer::Broadcast_SendRunInput_Server_Validate(bool Pressed)
{
	return true;
}

void ACamperPlayer::Broadcast_SendRunInput_Multicast_Implementation(bool Pressed)
{

}

void ACamperPlayer::Broadcast_SendItemPreventRun_Server_Implementation(bool prevent)
{

}

bool ACamperPlayer::Broadcast_SendItemPreventRun_Server_Validate(bool prevent)
{
	return true;
}

void ACamperPlayer::Broadcast_SendItemPreventRun_Multicast_Implementation(bool prevent)
{

}

void ACamperPlayer::AuthoritySetHealth(ECamperDamageState health)
{

}

void ACamperPlayer::Authority_SetDrainTimerPercentLeft(float percentTime)
{

}

void ACamperPlayer::Authority_SetDrainTimerPercentElapsed(float percentTime)
{

}

void ACamperPlayer::Authority_ResetWiggleFreeCharge()
{

}

void ACamperPlayer::Authority_OnSlashed(ADBDPlayer* attacker)
{

}

void ACamperPlayer::Authority_OnFailedStruggling(UInteractionDefinition* Interaction)
{

}

void ACamperPlayer::Authority_ApplyDamageToState(bool& didDamage, bool& causedKO, ECamperDamageState targetState)
{

}

void ACamperPlayer::Authority_AddWiggleFreeChargePercentNoModifiers(float ChargePercent)
{

}

void ACamperPlayer::Authority_AddWiggleFreeCharge(float charge)
{

}

void ACamperPlayer::AttachToGuidingPlayer()
{

}

void ACamperPlayer::ApplyHeal()
{

}

void ACamperPlayer::ApplyFullHeal()
{

}

void ACamperPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACamperPlayer, _drainTimer);
	DOREPLIFETIME(ACamperPlayer, _dyingTimer);
	DOREPLIFETIME(ACamperPlayer, _injuredBleedoutTimer);
	DOREPLIFETIME(ACamperPlayer, _isBeingChased);
	DOREPLIFETIME(ACamperPlayer, _isInInjuredBleedout);
	DOREPLIFETIME(ACamperPlayer, _numHealthyHeals);
}

ACamperPlayer::ACamperPlayer()
{
	this->InputMashCount = 1.000000;
	this->InputMashDecayTime = 1.000000;
	this->CameraSocketName_Hooked = NAME_None;
	this->StruggleMashDelay = 0.400000;
	this->WiggleMashDelay = 0.400000;
	this->_pixelCounter = CreateDefaultSubobject<UBoxOcclusionQueryComponent>(TEXT("PixelCounter"));
	this->StalkedComponent = CreateDefaultSubobject<UStalkedComponent>(TEXT("StalkedComponent"));
	this->CameraResetToleranceYaw = 2.000000;
	this->CameraResetTolerancePitch = 2.000000;
	this->CameraRecenterOffsetYaw = 90.000000;
	this->CameraRecenterOffsetPitch = 30.000000;
	this->Gender = EGender::VE_Male;
	this->TimeforDeathWhileCrawling = 60.000000;
	this->TimeforDeathWhileHooked = 60.000000;
	this->SecondsUntilFootprintTrigger = 0.100000;
	this->TriggerAfflictionHUDIntro = false;
	this->_stillnessTracker = CreateDefaultSubobject<UCamperStillnessTrackerComponent>(TEXT("StillnessTracker"));
	this->PartiallyHiddenStillnessThreshold = 0.000000;
	this->ProximityZone = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityZone"));
	this->SlashableZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Slashable"));
	this->HookSlashableZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HookSlashable"));
	this->CrouchCapsuleHalfHeight = 0.000000;
	this->_camperSensor = CreateDefaultSubobject<UDBDPawnSensingComponent>(TEXT("CamperSensor"));
	this->_hpSlot01 = CreateDefaultSubobject<UChargeableComponent>(TEXT("HPSlot01"));
	this->_hpSlot02 = CreateDefaultSubobject<UChargeableComponent>(TEXT("HPSlot02"));
	this->_carryEscapeProgress = CreateDefaultSubobject<UChargeableComponent>(TEXT("CarryEscape"));
	this->_isBeingChased = false;
	this->_isInInjuredBleedout = false;
	this->_numHealthyHeals = 0;
	this->_hookedCount = 0;
}
