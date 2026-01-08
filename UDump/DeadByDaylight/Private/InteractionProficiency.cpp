#include "InteractionProficiency.h"
#include "EStatusEffectType.h"

class UChargeableInteractionDefinition;

float UInteractionProficiency::GetValue_Implementation(const UChargeableInteractionDefinition* chargeableInteraction) const
{
	return 0.0f;
}

EStatusEffectType UInteractionProficiency::GetType_Implementation(const UChargeableInteractionDefinition* chargeableInteraction) const
{
	return EStatusEffectType::None;
}

int32 UInteractionProficiency::GetLevel_Implementation(const UChargeableInteractionDefinition* chargeableInteraction) const
{
	return 0;
}

bool UInteractionProficiency::GetIsActive_Implementation(const UChargeableInteractionDefinition* chargeableInteraction) const
{
	return false;
}

UInteractionProficiency::UInteractionProficiency()
{

}
