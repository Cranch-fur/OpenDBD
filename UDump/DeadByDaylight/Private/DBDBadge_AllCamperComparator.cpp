#include "DBDBadge_AllCamperComparator.h"

UDBDBadge_AllCamperComparator::UDBDBadge_AllCamperComparator()
{
	this->MinimumValueForReward = 0.000000;
	this->ValueStrategy = EBadgeValueStrategy::Additive;
	this->_eventValueForOwner = 0.000000;
}
