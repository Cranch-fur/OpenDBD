#include "PlayerStateData.h"


FPlayerStateData::FPlayerStateData()
{
	this->CharacterLevel = 0;
	this->PrestigeLevel = 0;
	this->EquipedFavorId = NAME_None;
	this->EquipedPerkIds = TArray<FName>();
	this->EquipedPerkLevels = TArray<int32>();
	this->CustomizationMesh = TArray<FName>();
	this->IsLeavingMatch = false;
	this->_playerGameState = EGameState::VE_Active;
}
