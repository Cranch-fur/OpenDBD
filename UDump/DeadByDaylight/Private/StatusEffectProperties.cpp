#include "StatusEffectProperties.h"


FStatusEffectProperties::FStatusEffectProperties()
{
	this->Tags = TArray<FName>();
	this->StatusEffectBlueprint = NULL;
	this->LevelToDisplay = 0;
	this->StatusEffectType = EStatusEffectType::None;
}
