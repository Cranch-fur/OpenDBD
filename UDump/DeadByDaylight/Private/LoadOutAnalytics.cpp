#include "LoadOutAnalytics.h"


FLoadOutAnalytics::FLoadOutAnalytics()
{
	this->LoadoutItem = TEXT("");
	this->LoadoutItemAddOn1 = TEXT("");
	this->LoadoutItemAddOn2 = TEXT("");
	this->LoadoutPerk1 = TEXT("");
	this->LoadoutPerk2 = TEXT("");
	this->LoadoutPerk3 = TEXT("");
	this->LoadoutPerk4 = TEXT("");
	this->LoadoutOffering = TEXT("");
	this->Role = EPlayerRole::VE_None;
	this->ClientId = TEXT("");
	this->MatchID = TEXT("");
	this->Platform = TEXT("");
}
