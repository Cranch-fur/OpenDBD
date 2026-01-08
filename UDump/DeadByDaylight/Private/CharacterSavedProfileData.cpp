#include "CharacterSavedProfileData.h"


FCharacterSavedProfileData::FCharacterSavedProfileData()
{
	this->BloodWebLevel = 0;
	this->PrestigeLevel = 0;
	this->TimesConfronted = 0;
	this->PrestigeEarnedDates = TArray<FDateTime>();
	this->BloodWebData = FBloodWebPersistentData{};
	this->CharacterLoadoutData = FPlayerLoadoutData{};
	this->InventoryData = TArray<FCharacterSavedInventoryData>();
	this->CurrentCustomization = TArray<FName>();
}
