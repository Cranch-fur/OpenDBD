#include "PostGame_KillerAnalytics.h"


FPostGame_KillerAnalytics::FPostGame_KillerAnalytics()
{
	this->ClientId = TEXT("");
	this->MatchID = TEXT("");
	this->BadgeFirst = TEXT("");
	this->BadgeSecond = TEXT("");
	this->BloodPoints = 0;
	this->Brutality = 0;
	this->Deviousness = 0;
	this->Hunter = 0;
	this->Sacrifice = 0;
	this->GameDuration = 0.0f;
	this->EscapeesCount = 0;
	this->SacrificedCount = 0;
	this->KilledCount = 0;
	this->DisconnectCount = 0;
	this->Platform = TEXT("");
}
