#include "GamePersistentData.h"


FGamePersistentData::FGamePersistentData()
{
	this->PlayerCount = 0;
	this->SpectatorCount = 0;
	this->SessionInfos = FSessionInfos{};
	this->LastSessionInfos = FSessionInfos{};
	this->GamePresetData = FGamePresetData{};
}
