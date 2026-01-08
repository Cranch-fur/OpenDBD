#include "SkillCheck.h"
#include "ESkillCheckCustomType.h"

class UChargeableInteractionDefinition;
class UInteractionDefinition;

bool USkillCheck::ShouldDeactivateSkillCheckOnInteractionEnd() const
{
	return false;
}

void USkillCheck::Server_SetInteraction_Implementation(UChargeableInteractionDefinition* Interaction)
{

}

bool USkillCheck::Server_SetInteraction_Validate(UChargeableInteractionDefinition* Interaction)
{
	return true;
}

void USkillCheck::Server_OnSkillCheckSuccess_Implementation(bool Bonus, ESkillCheckCustomType Type)
{

}

bool USkillCheck::Server_OnSkillCheckSuccess_Validate(bool Bonus, ESkillCheckCustomType Type)
{
	return true;
}

void USkillCheck::Server_OnSkillCheckFailure_Implementation(bool hadInput, ESkillCheckCustomType Type)
{

}

bool USkillCheck::Server_OnSkillCheckFailure_Validate(bool hadInput, ESkillCheckCustomType Type)
{
	return true;
}

void USkillCheck::Multicast_SkillCheckResponse_Implementation(bool hadInput, bool success, bool Bonus, ESkillCheckCustomType Type)
{

}

void USkillCheck::ActivateSkillCheck(UInteractionDefinition* Interaction, float Delay)
{

}

void USkillCheck::ActivateCustomSkillCheck(ESkillCheckCustomType Type, float Delay)
{

}

USkillCheck::USkillCheck()
{
	this->ProgressRate = 0.000000;
	this->SuccessZoneStart = 0.000000;
	this->SuccessZoneEnd = 0.000000;
	this->BonusZoneLength = 0.000000;
	this->SkillCheckCount = 0;
	this->xOffset = 0;
	this->xOffset_max = 0.250000;
	this->yOffset = 0;
	this->yOffset_max = 0.250000;
	this->InsaneSkillCheckJitterAmplitude = 100.000000;
	this->kSuccessZoneMinimum = 0.250000;
}
