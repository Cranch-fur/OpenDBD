#include "GameplayModifierData.h"


FGameplayModifierData::FGameplayModifierData()
{
	this->Conditions = TArray<TSubclassOf<UGameplayModifierCondition>>();
	this->Modifiers = TArray<FGamePlayModifier>();
	this->Flags = TArray<EGameplayModifierFlag>();
}
