#include "OngoingScoreData.h"


FOngoingScoreData::FOngoingScoreData()
{
	this->ScoreType = EDBDScoreTypes::DBDCamperScore_SurviveHealthy;
	this->TimeSinceScore = 0.0f;
	this->ApplyScoreTimeout = 0.0f;
	this->AccumulatedPercent = 0.0f;
}
