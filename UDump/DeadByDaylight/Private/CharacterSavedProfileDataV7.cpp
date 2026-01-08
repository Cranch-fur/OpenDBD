#include "CharacterSavedProfileDataV7.h"


FCharacterSavedProfileDataV7::FCharacterSavedProfileDataV7()
{
	this->BloodWebLevel = 0;
	this->PrestigeLevel = 0;
	this->TimesConfronted = 0;
	this->PrestigeDates = TArray<FString>();
	this->BloodWebData = FLegacySavedBloodWebPersistentData{};
	this->CharacterLoadoutData = FLegacySavedPlayerLoadoutData{};
	this->Inventory = TArray<FCharacterSavedInventoryDataV7>();
	this->CurrentCustomization = TArray<FName>();
}
