#include "Perk.h"
#include "EInventoryItemType.h"
#include "GameplayModifierData.h"
#include "Net/UnrealNetwork.h"

void UPerk::Multicast_InitializePerk_Implementation(FName perkID, int32 perkLevel)
{

}

bool UPerk::Multicast_InitializePerk_Validate(FName perkID, int32 perkLevel)
{
	return true;
}

int32 UPerk::GetPerkLevel() const
{
	return 0;
}

EInventoryItemType UPerk::GetInventoryItemType() const
{
	return EInventoryItemType::None;
}

FGameplayModifierData UPerk::GetGameplayModifierData()
{
	return FGameplayModifierData{};
}

FGameplayModifierData UPerk::GetConstGameplayModifierData() const
{
	return FGameplayModifierData{};
}

void UPerk::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPerk, PerkLevelData);
	DOREPLIFETIME(UPerk, IsUsable);
	DOREPLIFETIME(UPerk, _perkLevel);
}

UPerk::UPerk()
{
	this->IsUsable = true;
	this->_perkLevel = 0;
}
