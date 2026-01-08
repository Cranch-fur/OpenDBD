#include "PlayerSavedProfileDataShared.h"


FPlayerSavedProfileDataShared::FPlayerSavedProfileDataShared()
{
	this->PlayerName = TEXT("");
	this->SteamId = TEXT("");
	this->SelectedCamperIndex = 0;
	this->SelectedSlasherIndex = 0;
	this->SelectedCharacterCustomization = TArray<FName>();
	this->SlasherSkulls = 0;
	this->CamperSkulls = 0;
	this->CamperStreak = 0;
}
