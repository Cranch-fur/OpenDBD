#include "PlayerDataSync.h"


FPlayerDataSync::FPlayerDataSync()
{
	this->PlayerData = FPlayerStateData{};
	this->CustomizationMesh = TArray<FName>();
	this->EquipedItemId = NAME_None;
	this->EquipedItemAddonIds = TArray<FName>();
}
