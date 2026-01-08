#include "GameplayModifierContainer.h"
#include "EGameplayModifierType.h"
#include "EGameplayModifierFlag.h"
#include "EInventoryItemType.h"
#include "GameplayModifierData.h"
#include "Templates/SubclassOf.h"
#include "Net/UnrealNetwork.h"

class ADBDPlayer;
class UGameplayModifierCondition;

void UGameplayModifierContainer::UpdateCooldownTimer(float deltaTime)
{

}

void UGameplayModifierContainer::TriggerTimerActivePerkEvent(float Duration)
{

}

void UGameplayModifierContainer::SetModifierValue(EGameplayModifierType GameplayModifierType, float GameplayModifierValue)
{

}

void UGameplayModifierContainer::SetFlag(EGameplayModifierFlag GameplayModifierFlag, bool flagValue)
{

}

void UGameplayModifierContainer::OnInitialized_Internal()
{

}

void UGameplayModifierContainer::Multicast_TriggerCooldownTimer_Implementation(float cooldownTime)
{

}

bool UGameplayModifierContainer::IsApplicable() const
{
	return false;
}

void UGameplayModifierContainer::InstantiateGameplayModifierConditions()
{

}

bool UGameplayModifierContainer::HasModifierOfType(EGameplayModifierType GameplayModifierType) const
{
	return false;
}

bool UGameplayModifierContainer::HasFlag(EGameplayModifierFlag GameplayModifierFlag) const
{
	return false;
}

ADBDPlayer* UGameplayModifierContainer::GetOwningPlayer() const
{
	return NULL;
}

float UGameplayModifierContainer::GetModifierValue(EGameplayModifierType GameplayModifierType) const
{
	return 0.0f;
}

int32 UGameplayModifierContainer::GetLevelToDisplay() const
{
	return 0;
}

EInventoryItemType UGameplayModifierContainer::GetInventoryItemType() const
{
	return EInventoryItemType::None;
}

int32 UGameplayModifierContainer::GetIconFilePathIndex_Implementation() const
{
	return 0;
}

FGameplayModifierData UGameplayModifierContainer::GetGameplayModifierData()
{
	return FGameplayModifierData{};
}

FGameplayModifierData UGameplayModifierContainer::GetConstGameplayModifierData() const
{
	return FGameplayModifierData{};
}

UGameplayModifierCondition* UGameplayModifierContainer::GetCondition(int32 Index) const
{
	return NULL;
}

void UGameplayModifierContainer::FireActivePerkEvent(float percentage, int32 chargeCount) const
{

}

void UGameplayModifierContainer::Authority_TriggerCooldownTimer(float coolddownTime)
{

}

UGameplayModifierCondition* UGameplayModifierContainer::AddCondition(TSubclassOf<UGameplayModifierCondition> conditionType)
{
	return NULL;
}

void UGameplayModifierContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGameplayModifierContainer, ID);
	DOREPLIFETIME(UGameplayModifierContainer, _conditionsInner);
}

UGameplayModifierContainer::UGameplayModifierContainer()
{
	this->UseApplyFunction = false;
	this->BroadcastWhenApplicable = false;
	this->BroadcastOnTimer = false;
	this->BroadcastAlways = false;
	this->ID = NAME_None;
}
