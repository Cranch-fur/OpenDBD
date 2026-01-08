#include "GameplayModifierCondition.h"
#include "Net/UnrealNetwork.h"

class UGameplayModifierContainer;
class ADBDPlayer;

void UGameplayModifierCondition::SetOwningGameplayModifier(UGameplayModifierContainer* OwningGameplayModifier)
{

}

ADBDPlayer* UGameplayModifierCondition::GetOwningPlayer() const
{
	return NULL;
}

UGameplayModifierContainer* UGameplayModifierCondition::GetOwningGameplayModifier() const
{
	return NULL;
}

void UGameplayModifierCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGameplayModifierCondition, _owningGameplayModifier);
}

UGameplayModifierCondition::UGameplayModifierCondition()
{

}
