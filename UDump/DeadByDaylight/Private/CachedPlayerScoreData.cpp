#include "CachedPlayerScoreData.h"


FCachedPlayerScoreData::FCachedPlayerScoreData()
{
	this->PlayerId = TEXT("");
	this->SteamId = TEXT("");
	this->PlayerName = TEXT("");
	this->CharacterId = 0;
	this->PlayerScore = 0;
	this->PlayerRank = 0;
	this->PlayerGameState = EGameState::VE_Active;
	this->Role = EPlayerRole::VE_None;
	this->IsMyScore = 0;
	this->EquipedFavorId = NAME_None;
	this->EquipedPerks = TArray<FName>();
	this->EquipedPerkLevels = TArray<int32>();
	this->EquippedItemId = NAME_None;
	this->EquippedItemAddons = TArray<FName>();
	this->PlayerstateDataCache = FPlayerstateDataCache{};
	this->KillerGoalPoints = 0;
}
