#include "ScoreValue.h"


FScoreValue::FScoreValue()
{
	this->ID = NAME_None;
	this->Type = EDBDScoreTypes::DBDCamperScore_SurviveHealthy;
	this->Category = EDBDScoreCategory::DBD_CamperScoreCat_Objectives;
	this->ExperienceValue = 0;
	this->MaxExperienceValue = 0;
	this->ExperienceDepreciationCurve = NULL;
	this->OngoingWaitTime = 0.0f;
	this->DisplayName = FText::GetEmpty();
}
