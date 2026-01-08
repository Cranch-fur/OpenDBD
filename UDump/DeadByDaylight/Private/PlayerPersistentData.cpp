#include "PlayerPersistentData.h"


FPlayerPersistentData::FPlayerPersistentData()
{
	this->GameRole = EPlayerRole::VE_None;
	this->StartingLoadout = FPlayerLoadoutData{};
	this->CurrentLoadout = FPlayerLoadoutData{};
	this->CachedAvailableRitual = FDailyRitualInstance{};
	this->SavedData = FPlayerSavedProfileData{};
	this->PreviousSlasherSkulls = 0;
	this->PreviousCamperSkulls = 0;
	this->PreviousExperience = 0;
}
