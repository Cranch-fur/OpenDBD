#include "UserGameStats.h"


FUserGameStats::FUserGameStats()
{
	this->Disconnected = false;
	this->FearTokens = 0;
	this->Skulls = 0;
	this->BonusExperience = TMap<EDBDScoreCategory, int32>();
}
