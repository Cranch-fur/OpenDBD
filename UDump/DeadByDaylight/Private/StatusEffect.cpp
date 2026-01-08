#include "StatusEffect.h"
#include "GameplayModifierData.h"
#include "StatusViewSource.h"
#include "Net/UnrealNetwork.h"

class ADBDPlayer;
class UGameplayModifierContainer;

void UStatusEffect::SetShouldDisplay(bool On)
{

}

void UStatusEffect::OnInitialized_Internal()
{

}

void UStatusEffect::Multicast_InitializeStatusEffect_Implementation(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParamNew, UGameplayModifierContainer* originatingEffect)
{

}

bool UStatusEffect::Multicast_InitializeStatusEffect_Validate(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParamNew, UGameplayModifierContainer* originatingEffect)
{
	return true;
}

bool UStatusEffect::GetShouldDisplay_Implementation() const
{
	return false;
}

float UStatusEffect::GetPercentageFill() const
{
	return 0.0f;
}

ADBDPlayer* UStatusEffect::GetOriginatingPlayer() const
{
	return NULL;
}

UGameplayModifierContainer* UStatusEffect::GetOriginatingEffect() const
{
	return NULL;
}

FGameplayModifierData UStatusEffect::GetGameplayModifierData()
{
	return FGameplayModifierData{};
}

FGameplayModifierData UStatusEffect::GetConstGameplayModifierData() const
{
	return FGameplayModifierData{};
}

void UStatusEffect::FireActiveStatusViewEvent(FName StatusViewID, FName uniqueSourceID, const FStatusViewSource StatusViewSource) const
{

}

void UStatusEffect::FireActiveStatusEffectEvent(float percentage, int32 LevelToDisplay) const
{

}

void UStatusEffect::Authority_RemoveSelf()
{

}

void UStatusEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatusEffect, ModifierData);
	DOREPLIFETIME(UStatusEffect, ShouldDisplay);
	DOREPLIFETIME(UStatusEffect, _originatingPlayer);
	DOREPLIFETIME(UStatusEffect, _originatingEffect);
}

UStatusEffect::UStatusEffect()
{
	this->BroadcastAsStatusEffectWhenApplicable = false;
	this->BroadcastAsStatusEffectAlways = false;
	this->customParam = 0.000000;
	this->RemainingLifeTime = 0.000000;
	this->ShouldDisplay = false;
}
