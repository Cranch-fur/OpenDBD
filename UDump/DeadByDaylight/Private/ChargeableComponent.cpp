#include "ChargeableComponent.h"
#include "ESkillCheckCustomType.h"

class ADBDPlayer;
class AActor;

void UChargeableComponent::SetSecondsToCharge(float SecondsToCharge)
{

}

void UChargeableComponent::Reset()
{

}

void UChargeableComponent::Multicast_UpdateChargeAndSpeed_Implementation(float charge, float Speed)
{

}

void UChargeableComponent::Multicast_SkillCheckResponseAesthetic_Implementation(bool success, bool Bonus, ADBDPlayer* Player, bool TriggerLoudNoise, bool hadInput, ESkillCheckCustomType Type)
{

}

void UChargeableComponent::Multicast_OnEmptied_Implementation() const
{

}

void UChargeableComponent::Multicast_OnCompleteChanged_Implementation(bool COMPLETED) const
{

}

bool UChargeableComponent::IsSkillCheckTimePenaltyActive() const
{
	return false;
}

bool UChargeableComponent::HasPlayerDoneSkillCheck(int32 PlayerId) const
{
	return false;
}

float UChargeableComponent::GetTunableValue(FName TunableValueID) const
{
	return 0.0f;
}

float UChargeableComponent::GetSkillCheckChargePercent(ADBDPlayer* Player, bool success, bool Bonus) const
{
	return 0.0f;
}

float UChargeableComponent::GetSkillCheckCharge(ADBDPlayer* Player, bool success, bool Bonus) const
{
	return 0.0f;
}

float UChargeableComponent::GetPercentComplete() const
{
	return 0.0f;
}

float UChargeableComponent::GetMaxCharge() const
{
	return 0.0f;
}

float UChargeableComponent::GetCurrentCharge() const
{
	return 0.0f;
}

bool UChargeableComponent::GetComplete() const
{
	return false;
}

float UChargeableComponent::GetChargeRemaining() const
{
	return 0.0f;
}

void UChargeableComponent::AddChargePercent(float ChargePercent, AActor* Instigator)
{

}

void UChargeableComponent::AddCharge(float ChargeAmount, AActor* Instigator)
{

}

UChargeableComponent::UChargeableComponent()
{
	this->AllowSkillChecksAtFullCharge = false;
	this->ChargeableComponentID = TEXT("");
	this->DechargeRate = 0.000000;
	this->DechargeDelay = 0.000000;
	this->_isCharging = false;
}
