#include "PlayerSavedProfileDataLocal.h"


FPlayerSavedProfileDataLocal::FPlayerSavedProfileDataLocal()
{
	this->Tokens = 0;
	this->Offerings = TArray<FName>();
	this->FirstTimePlaying = false;
	this->Wins = 0;
	this->Losses = 0;
	this->CharacterData = TMap<int32, FCharacterSavedProfileData>();
	this->CurrentSeasonTicks = 0;
	this->DailyRituals = FDailyRitualContainer{};
	this->FearMarket = FFearMarketOfferingInstance{};
	this->LastConnectedLoadout = FPlayerLoadoutData{};
	this->LastConnectedCharacterIndex = 0;
	this->DisconnectPenaltyTime = FDateTime{};
	this->_experience = 0;
	this->_bonusExperience = 0;
	this->_fearTokens = 0;
	this->_ongoingGameTime = FDateTime{};
}
