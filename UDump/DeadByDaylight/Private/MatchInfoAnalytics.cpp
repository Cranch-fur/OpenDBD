#include "MatchInfoAnalytics.h"


FMatchInfoAnalytics::FMatchInfoAnalytics()
{
	this->GameMode = EGameType::Online;
	this->Role = EPlayerRole::VE_None;
	this->PartySize = 0;
	this->CharacterName = TEXT("");
	this->Rank = 0;
	this->Level = 0;
	this->Prestige = 0;
	this->Pips = 0;
	this->MapSeed = 0;
	this->MapName = TEXT("");
	this->ClientId = TEXT("");
	this->MatchID = TEXT("");
	this->Version = TEXT("");
	this->Platform = TEXT("");
}
