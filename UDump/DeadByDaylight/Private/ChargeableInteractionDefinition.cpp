#include "ChargeableInteractionDefinition.h"
#include "ESkillCheckCustomType.h"

class UChargeableComponent;
class ADBDPlayer;
class ACollectable;

void UChargeableInteractionDefinition::SetChargeableComponent(UChargeableComponent* ChargeableComponent)
{

}

void UChargeableInteractionDefinition::OnSkillCheckResponseAuthority(bool success, bool Bonus, ADBDPlayer* Player, bool TriggerLoudNoise, bool hadInput, ESkillCheckCustomType Type)
{

}

void UChargeableInteractionDefinition::OnSkillCheckResponseAesthetic(bool success, bool Bonus, ADBDPlayer* Player, bool TriggerLoudNoise, bool hadInput, ESkillCheckCustomType Type)
{

}

bool UChargeableInteractionDefinition::IsChargeComplete() const
{
	return false;
}

bool UChargeableInteractionDefinition::HasSkillCheckHappened() const
{
	return false;
}

float UChargeableInteractionDefinition::GetTunableValue(FName TunableValueID, float DefaultValue) const
{
	return 0.0f;
}

float UChargeableInteractionDefinition::GetTotalChargeModifier() const
{
	return 0.0f;
}

float UChargeableInteractionDefinition::GetSuccessProbability_Implementation() const
{
	return 0.0f;
}

float UChargeableInteractionDefinition::GetSkillCheckProbability() const
{
	return 0.0f;
}

float UChargeableInteractionDefinition::GetSkillCheckDifficulityModifier_Implementation() const
{
	return 0.0f;
}

int32 UChargeableInteractionDefinition::GetPipsPassed() const
{
	return 0;
}

float UChargeableInteractionDefinition::GetPerkMultiplier_Implementation() const
{
	return 0.0f;
}

ADBDPlayer* UChargeableInteractionDefinition::GetOwningDBDPlayer() const
{
	return NULL;
}

float UChargeableInteractionDefinition::GetMultiplicativeLuckBonus_Implementation() const
{
	return 0.0f;
}

float UChargeableInteractionDefinition::GetMaxCharge() const
{
	return 0.0f;
}

float UChargeableInteractionDefinition::GetItemEfficiencyModifier_Implementation() const
{
	return 0.0f;
}

float UChargeableInteractionDefinition::GetChargeAmountForTime(float DeltaSeconds) const
{
	return 0.0f;
}

UChargeableComponent* UChargeableInteractionDefinition::GetChargeableComponent() const
{
	return NULL;
}

float UChargeableInteractionDefinition::GetAdditiveLuckBonus_Implementation() const
{
	return 0.0f;
}

bool UChargeableInteractionDefinition::Authority_ChargeWithItem(float DeltaSeconds, ACollectable* Item)
{
	return false;
}

bool UChargeableInteractionDefinition::AddCharge(float DeltaSeconds)
{
	return false;
}

UChargeableInteractionDefinition::UChargeableInteractionDefinition()
{
	this->StopInteractionOnChargeComplete = false;
	this->HasProgressivelyHarderSkillChecks = false;
	this->ProgressiveDifficultyModifier = 0.000000;
	this->ProgressBarType = EChargeableInteractionBarType::VE_Normal;
	this->HideProgressBar = false;
}
