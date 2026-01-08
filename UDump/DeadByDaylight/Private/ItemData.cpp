#include "ItemData.h"


FItemData::FItemData()
{
	this->ID = NAME_None;
	this->Type = EInventoryItemType::None;
	this->DisplayName = FText::GetEmpty();
	this->GenericDisplayName = FText::GetEmpty();
	this->Description = FText::GetEmpty();
	this->ItemBlueprint = NULL;
	this->ItemMeshV2 = NULL;
	this->ItemMesh = NULL;
	this->HandPosition = EItemHandPosition::None;
	this->IconFilePaths = TArray<FString>();
	this->Role = EPlayerRole::VE_None;
	this->Rarity = EItemRarity::Common;
	this->Inventory = false;
	this->Bloodweb = false;
	this->Chest = false;
	this->COMPLETED = false;
	this->Disabled = false;
	this->RequiredKillerAbility = EKillerAbilities::VE_None;
	this->CloudInventoryId = 0;
	this->CommunityId = TEXT("");
	this->DLCId = TEXT("");
	this->DLCIdPS4 = TEXT("");
	this->DLCIdXB1 = TEXT("");
	this->AntiDLC = false;
	this->AvailabledDLCId = TEXT("");
	this->AvailabledDLCIdPS4 = TEXT("");
	this->AvailabledDLCIdXB1 = TEXT("");
}
