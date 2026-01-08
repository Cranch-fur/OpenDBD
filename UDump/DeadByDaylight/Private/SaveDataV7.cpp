#include "SaveDataV7.h"


FSaveDataV7::FSaveDataV7()
{
	this->PlayerUID = TEXT("");
	this->SelectedCamperIndex = 0;
	this->SelectedSlasherIndex = 0;
	this->Offerings = TArray<FName>();
	this->FirstTimePlaying = false;
	this->CurrentSeasonTicks = 0;
	this->LastConnectedCharacterIndex = 0;
	this->DisconnectPenaltyTime = TEXT("");
	this->Experience = 0;
	this->BonusExperience = 0;
	this->FearTokens = 0;
	this->OngoingGameTime = TEXT("");
	this->CharacterData = TArray<FCharacterKeyTuple>();
	this->DailyRituals = FSavedDailyRitualContainerV7{};
	this->FearMarket = FSavedFearMarketOfferingInstanceV7{};
	this->LastLoadout = FLegacySavedPlayerLoadoutData{};
}
