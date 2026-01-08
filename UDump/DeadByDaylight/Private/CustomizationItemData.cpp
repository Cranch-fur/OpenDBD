#include "CustomizationItemData.h"


FCustomizationItemData::FCustomizationItemData()
{
	this->AssociatedCharacter = 0;
	this->PrestigeUlockLevel = 0;
	this->PrestigeUnlockDate = TEXT("");
	this->PartCategory = NAME_None;
	this->MaterialsMap = TArray<FMaterialReplacerData>();
	this->IsDefaultItem = false;
	this->IsLocked = false;
	this->AttachementSocketBlueprints = TArray<FBPAttachementSocketData>();
}
