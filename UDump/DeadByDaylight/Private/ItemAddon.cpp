#include "ItemAddon.h"
#include "EInventoryItemType.h"
#include "Net/UnrealNetwork.h"

class ACollectable;
class ADBDPlayer;

void UItemAddon::Multicast_SetBaseItem_Implementation(ACollectable* baseItem)
{

}

bool UItemAddon::Multicast_SetBaseItem_Validate(ACollectable* baseItem)
{
	return true;
}

void UItemAddon::Multicast_InitializeItemAddon_Implementation()
{

}

bool UItemAddon::Multicast_InitializeItemAddon_Validate()
{
	return true;
}

void UItemAddon::Multicast_AddAddonToItem_Implementation(ACollectable* Item)
{

}

bool UItemAddon::IsReadyForInitialization_Implementation() const
{
	return false;
}

ADBDPlayer* UItemAddon::GetOwningPlayer() const
{
	return NULL;
}

EInventoryItemType UItemAddon::GetInventoryItemType() const
{
	return EInventoryItemType::None;
}

ACollectable* UItemAddon::GetBaseItem() const
{
	return NULL;
}

void UItemAddon::ApplyMetaModifiers()
{

}

void UItemAddon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemAddon, InventoryItemType);
}

UItemAddon::UItemAddon()
{
	this->BaseItemType = NULL;
	this->InventoryItemType = EInventoryItemType::None;
}
