#include "SlasherPlayer.h"
#include "ESlasherGuidedAction.h"
#include "UObject/NoExportTypes.h"
#include "EKillerAbilities.h"
#include "EAttackZoneSet.h"
#include "EAttackType.h"
#include "EDetectionZone.h"
#include "EStunType.h"
#include "EBlindType.h"
#include "Net/UnrealNetwork.h"
#include "DBDPawnSensingComponent.h"
#include "StillnessTrackerComponent.h"
#include "FlashLightReceiverComponent.h"
#include "ChargeableComponent.h"

class ACamperPlayer;
class AActor;
class APawn;
class UItemAddon;
class UPhysicalMaterial;
class ADBDPlayer;

void ASlasherPlayer::UpdateSlasherStealthRatio_Implementation(float deltaTime)
{

}

void ASlasherPlayer::UpdateRageTierEffect_Implementation(int32 previousTier, int32 currentTier)
{

}

void ASlasherPlayer::SpawnSlasherPower_Implementation()
{

}

void ASlasherPlayer::SetStealthRatio(float stealthRatio)
{

}

void ASlasherPlayer::SetStalkTierWalkSpeedMultiplier(float stalkTierWalkSpeedMultiplier)
{

}

void ASlasherPlayer::SetLightIntensity(float Intensity)
{

}

void ASlasherPlayer::SetIsKilling(bool isKilling)
{

}

void ASlasherPlayer::SetIsInBlinkCooldown(bool IsInBlinkCooldown)
{

}

void ASlasherPlayer::SetIsChainBlinking(bool IsChainBlinking)
{

}

void ASlasherPlayer::SetInteractingCamperBlueprint(ACamperPlayer* camper)
{

}

void ASlasherPlayer::SetGuidedCamper(ACamperPlayer* camper)
{

}

void ASlasherPlayer::SetGuidedAction(ESlasherGuidedAction Action)
{

}

void ASlasherPlayer::SetChainsawSprinting(bool chainsawSprinting)
{

}

void ASlasherPlayer::SetBeingWiggled(bool Value)
{

}

void ASlasherPlayer::SetBaseTerrorRadius(float baseTerrorRadius)
{

}

void ASlasherPlayer::Server_SetLungeType_Implementation(int32 lungeType)
{

}

bool ASlasherPlayer::Server_SetLungeType_Validate(int32 lungeType)
{
	return true;
}

void ASlasherPlayer::Server_SendAttackInput_Implementation(bool Pressed)
{

}

bool ASlasherPlayer::Server_SendAttackInput_Validate(bool Pressed)
{
	return true;
}

void ASlasherPlayer::Server_AllowKillingAfterHooking_Implementation(int32 numKills)
{

}

bool ASlasherPlayer::Server_AllowKillingAfterHooking_Validate(int32 numKills)
{
	return true;
}

void ASlasherPlayer::Server_AllowKilling_Implementation(int32 numKills)
{

}

bool ASlasherPlayer::Server_AllowKilling_Validate(int32 numKills)
{
	return true;
}

void ASlasherPlayer::OnSurvivorsLeftChanged(int32 survivorRemaining)
{

}

void ASlasherPlayer::OnSurvivorKilled(ACamperPlayer* playerToKill, bool consumeKill)
{

}

void ASlasherPlayer::OnSoundTriggered(AActor* originator, FVector Location, bool shouldTrack, float audibleRange)
{

}

void ASlasherPlayer::OnLoudNoiseIndicatorDestroyed()
{

}

void ASlasherPlayer::OnKillerAbilityUpdateActivate_Implementation(EKillerAbilities KillerAbility, float percent)
{

}

void ASlasherPlayer::OnKillerAbilityEndDeactivate_Implementation(EKillerAbilities KillerAbility, bool Forced)
{

}

void ASlasherPlayer::OnKillerAbilityEndActivate_Implementation(EKillerAbilities KillerAbility)
{

}

void ASlasherPlayer::OnKillerAbilityBeginDeactivate_Implementation(EKillerAbilities KillerAbility, bool Forced)
{

}

void ASlasherPlayer::OnKillerAbilityBeginActivate_Implementation(EKillerAbilities KillerAbility)
{

}

void ASlasherPlayer::OnInvisibilityChargeEmptied()
{

}

void ASlasherPlayer::OnInvisibilityBurnoutChargeCompletionChanged(bool COMPLETED)
{

}

void ASlasherPlayer::OnChaseTargetFound(APawn* Pawn)
{

}

void ASlasherPlayer::OnBlindChargeEmptied()
{

}

void ASlasherPlayer::Multicast_SetLungeType_Implementation(int32 lungeType)
{

}

bool ASlasherPlayer::Multicast_SetLungeType_Validate(int32 lungeType)
{
	return true;
}

void ASlasherPlayer::Multicast_SetGuidedAction_Implementation(ESlasherGuidedAction Action)
{

}

bool ASlasherPlayer::Multicast_SetGuidedAction_Validate(ESlasherGuidedAction Action)
{
	return true;
}

void ASlasherPlayer::Multicast_SetAttackDetectionZoneSet_Implementation(EAttackZoneSet attackZoneSet)
{

}

void ASlasherPlayer::Multicast_OnChaseStart_Implementation(ACamperPlayer* camper)
{

}

void ASlasherPlayer::Multicast_OnChaseEnd_Implementation(ACamperPlayer* camper, float chaseTime)
{

}

void ASlasherPlayer::Multicast_DisplayAttackZones_Implementation(bool Display)
{

}

void ASlasherPlayer::Multicast_CamperHitAesthetic_Implementation(ACamperPlayer* camper, bool causedKO)
{

}

bool ASlasherPlayer::Multicast_CamperHitAesthetic_Validate(ACamperPlayer* camper, bool causedKO)
{
	return true;
}

void ASlasherPlayer::Multicast_AllowKillingAfterHooking_Implementation(int32 numKills)
{

}

bool ASlasherPlayer::Multicast_AllowKillingAfterHooking_Validate(int32 numKills)
{
	return true;
}

void ASlasherPlayer::Multicast_AllowKilling_Implementation(int32 numKills)
{

}

bool ASlasherPlayer::Multicast_AllowKilling_Validate(int32 numKills)
{
	return true;
}

void ASlasherPlayer::Local_RequestAttack(EAttackType attackType)
{

}

bool ASlasherPlayer::IsUncloaking_Implementation() const
{
	return false;
}

bool ASlasherPlayer::IsInChase() const
{
	return false;
}

bool ASlasherPlayer::IsInBlinkCooldown() const
{
	return false;
}

bool ASlasherPlayer::IsHooking() const
{
	return false;
}

bool ASlasherPlayer::IsCloaking_Implementation() const
{
	return false;
}

bool ASlasherPlayer::IsChainsawSprinting() const
{
	return false;
}

bool ASlasherPlayer::IsChainBlinking() const
{
	return false;
}

bool ASlasherPlayer::IsCarrying() const
{
	return false;
}

bool ASlasherPlayer::IsASurvivorInTerrorRadius() const
{
	return false;
}

bool ASlasherPlayer::IsAllowedToKill(ACamperPlayer* camper) const
{
	return false;
}

bool ASlasherPlayer::HasKillerAbility(EKillerAbilities KillerAbility) const
{
	return false;
}

float ASlasherPlayer::GetTerrorRadius() const
{
	return 0.0f;
}

float ASlasherPlayer::GetStealthRatio() const
{
	return 0.0f;
}

float ASlasherPlayer::GetStalkTierWalkSpeedMultiplier() const
{
	return 0.0f;
}

int32 ASlasherPlayer::GetRageTierFromRagePercent(float ragePercent) const
{
	return 0;
}

int32 ASlasherPlayer::GetRageTier() const
{
	return 0;
}

float ASlasherPlayer::GetRagePercent() const
{
	return 0.0f;
}

FVector ASlasherPlayer::GetPlayerDropOffPoint_Implementation() const
{
	return FVector{};
}

ACamperPlayer* ASlasherPlayer::GetObsessionTarget() const
{
	return NULL;
}

FRotator ASlasherPlayer::GetLookRotation() const
{
	return FRotator{};
}

float ASlasherPlayer::GetLongestChaseDuration() const
{
	return 0.0f;
}

float ASlasherPlayer::GetLightIntensity() const
{
	return 0.0f;
}

TArray<UItemAddon*> ASlasherPlayer::GetItemAddons() const
{
	return TArray<UItemAddon*>();
}

ACamperPlayer* ASlasherPlayer::GetInteractingCamper() const
{
	return NULL;
}

ACamperPlayer* ASlasherPlayer::GetGuidedCamper() const
{
	return NULL;
}

ESlasherGuidedAction ASlasherPlayer::GetGuidedAction() const
{
	return ESlasherGuidedAction::VE_None;
}

float ASlasherPlayer::GetBaseTerrorRadius() const
{
	return 0.0f;
}

float ASlasherPlayer::GetAnimDirection() const
{
	return 0.0f;
}

bool ASlasherPlayer::DetectObstruction(UPhysicalMaterial*& physicalMaterialOut, FVector& positionOut, FVector& normalOut, EDetectionZone detectionZoneID) const
{
	return false;
}

void ASlasherPlayer::DBD_SetAttackZones(EAttackZoneSet attackZoneSet)
{

}

void ASlasherPlayer::DBD_DisplayAttackZones(bool Display)
{

}

void ASlasherPlayer::DBD_AllowKilling()
{

}

bool ASlasherPlayer::CanPerformKillerAbility_Implementation(EKillerAbilities KillerAbility) const
{
	return false;
}

bool ASlasherPlayer::CanGainRage_Implementation() const
{
	return false;
}

bool ASlasherPlayer::CanEscapeCarry(ACamperPlayer* Player)
{
	return false;
}

bool ASlasherPlayer::CanBeBlinded() const
{
	return false;
}

void ASlasherPlayer::CameraUpdated()
{

}

void ASlasherPlayer::BroadcastOffensiveAction() const
{

}

void ASlasherPlayer::Authority_StopBlinding(AActor* effector)
{

}

void ASlasherPlayer::Authority_SetRagePercent(float ragePercent)
{

}

void ASlasherPlayer::Authority_RequestStun(EStunType stunType, ADBDPlayer* Requester)
{

}

void ASlasherPlayer::Authority_Blinded(EBlindType blindType, float coolDown, AActor* effector)
{

}

void ASlasherPlayer::Authority_ApplyDamageToTarget(ADBDPlayer* target, bool forceCauseKO)
{

}

void ASlasherPlayer::Authority_AllowKilling(int32 numKills)
{

}

void ASlasherPlayer::Authority_AddRagePercent(float ragePercent)
{

}

void ASlasherPlayer::Authority_AddInvisibilityBurnoutCharge(ADBDPlayer* blinder, float charge, AActor* effector)
{

}

void ASlasherPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASlasherPlayer, _powerAddons);
	DOREPLIFETIME(ASlasherPlayer, _ragePercent);
}

ASlasherPlayer::ASlasherPlayer()
{
	this->DefaultSlasherPower = NAME_None;
	this->LoudNoiseDisplayTime = 5.000000;
	this->TerrorRadius = 3200.000000;
	this->CamperExposer = NULL;
	this->KillerMoodInfluence = EKillerMoodInfluence::VE_None;
	this->ShowKillerPowerDebugInfo = false;
	this->TempUseRageMechanic = true;
	this->_attackAcceleration = 112500.000000;
	this->_slasherStealthRatio = 1.000000;
	this->_slasherLightIntensity = 1.000000;
	this->_camperSensor = CreateDefaultSubobject<UDBDPawnSensingComponent>(TEXT("CamperSensor"));
	this->_stillnessTracker = CreateDefaultSubobject<UStillnessTrackerComponent>(TEXT("StillnessTracker"));
	this->_stalkTierWalkSpeedMultiplier = 1.000000;
	this->_fullBodyFlashLightReceiver = CreateDefaultSubobject<UFlashLightReceiverComponent>(TEXT("FullBodyFlashLightReceiver"));
	this->_invisibilityBurnoutChargeableComponent = CreateDefaultSubobject<UChargeableComponent>(TEXT("InvisibilityBurnoutChargeableComponent"));
	this->_stealthDelay = 0.000000;
	this->_stealthIncreaseRate = 0.000000;
	this->_stealthDecreaseRate = 0.000000;
	this->_isKilling = false;
	this->_ragePercent = 0.000000;
}
