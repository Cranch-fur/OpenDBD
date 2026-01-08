#include "ScoreAnalytics.h"


FScoreAnalytics::FScoreAnalytics()
{
	this->ClientId = TEXT("");
	this->MatchID = TEXT("");
	this->ScoreCategory = EDBDScoreTypes::DBDCamperScore_SurviveHealthy;
	this->ScoreCount = 0;
	this->ScoreExperience = 0.0f;
	this->Platform = TEXT("");
}
