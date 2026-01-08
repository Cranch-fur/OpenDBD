#include "PostGame_SurvivorAnalytics.h"


FPostGame_SurvivorAnalytics::FPostGame_SurvivorAnalytics()
{
	this->ClientId = TEXT("");
	this->MatchID = TEXT("");
	this->BadgeFirst = TEXT("");
	this->BadgeSecond = TEXT("");
	this->BloodPoints = 0;
	this->Objectives = 0;
	this->Survival = 0;
	this->Altruism = 0;
	this->Boldness = 0;
	this->Outcome = TEXT("");
	this->DamageState = ECamperDamageState::VE_Healthy;
	this->HookedCount = 0;
	this->GameDuration = 0.0f;
	this->Platform = TEXT("");
}
