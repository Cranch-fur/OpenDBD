#include "ScoreEvent.h"


FScoreEvent::FScoreEvent()
{
	this->ScoreType = EDBDScoreTypes::DBDCamperScore_SurviveHealthy;
	this->ExperienceAwarded = 0.0f;
	this->BonusPointModifier = 0.0f;
}
