#include "Gameplay_SurvivorAnalytics.h"


FGameplay_SurvivorAnalytics::FGameplay_SurvivorAnalytics()
{
	this->ClientId = TEXT("");
	this->MatchID = TEXT("");
	this->Speed = 0.0f;
	this->InjuredSpeed = 0.0f;
	this->InjuredDuration = 0.0f;
	this->HealthyDuration = 0.0f;
	this->KODuration = 0.0f;
	this->PalletSpawned = 0;
	this->PalletProcedural = 0;
	this->PalletGeneric = 0;
	this->PalletDrop = 0;
	this->PalletStun = 0;
	this->UnhookCount = 0;
	this->HealCount = 0;
	this->HealCountSuccess = 0;
	this->ClosetEnter = 0;
	this->Hatch = 0;
	this->SkillCheckCount = 0;
	this->SkillCheckCountGood = 0;
	this->SkillCheckCountGreat = 0;
	this->Platform = TEXT("");
}
