#include "DBDBadgeRewardCondition.h"


FDBDBadgeRewardCondition::FDBDBadgeRewardCondition()
{
	this->ScoreType = EDBDScoreTypes::DBDCamperScore_SurviveHealthy;
	this->EqualityCondition = EDBDValueEqualityCondition::GreaterThanEqual;
	this->Value = 0.0f;
}
