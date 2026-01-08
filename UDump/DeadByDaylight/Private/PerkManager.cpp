#include "PerkManager.h"
#include "EGameplayModifierType.h"
#include "EGameplayModifierFlag.h"
#include "EGameplayModifierSource.h"

class UStatusEffect;
class UGameplayModifierContainer;
class UPerk;

bool UPerkManager::HasPerkModifierOfType(EGameplayModifierType ModifierType) const
{
	return false;
}

bool UPerkManager::HasPerkFlag(EGameplayModifierFlag PerkFlag, EGameplayModifierSource Filter) const
{
	return false;
}

bool UPerkManager::HasAnyEffectWithID(FName ID) const
{
	return false;
}

bool UPerkManager::HasAnyAddonWithTag(FName Tag) const
{
	return false;
}

bool UPerkManager::HasAllCharacterPerksEquipped() const
{
	return false;
}

UStatusEffect* UPerkManager::GetStatusEffect(FName ID) const
{
	return NULL;
}

UGameplayModifierContainer* UPerkManager::GetSourceWithModifier(EGameplayModifierType ModifierType, EGameplayModifierSource Filter) const
{
	return NULL;
}

UGameplayModifierContainer* UPerkManager::GetSourceWithFlag(EGameplayModifierFlag PerkFlag, EGameplayModifierSource Filter) const
{
	return NULL;
}

float UPerkManager::GetPerkModifierMinValue(EGameplayModifierType ModifierType, float startValue) const
{
	return 0.0f;
}

float UPerkManager::GetPerkModifierMaxValue(EGameplayModifierType ModifierType) const
{
	return 0.0f;
}

float UPerkManager::GetPerkModifier(EGameplayModifierType ModifierType, EGameplayModifierSource Filter) const
{
	return 0.0f;
}

UPerk* UPerkManager::GetPerk(FName ID) const
{
	return NULL;
}

float UPerkManager::GetMultiplicativeModifierValueWithCompoundNegative(EGameplayModifierType ModifierType, EGameplayModifierSource Filter) const
{
	return 0.0f;
}

float UPerkManager::GetMultiplicativeModifierValue(EGameplayModifierType ModifierType) const
{
	return 0.0f;
}

UPerkManager::UPerkManager()
{

}
