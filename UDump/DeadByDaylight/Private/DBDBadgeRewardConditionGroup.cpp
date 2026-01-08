#include "DBDBadgeRewardConditionGroup.h"


FDBDBadgeRewardConditionGroup::FDBDBadgeRewardConditionGroup()
{
	this->Operator = EDBDBadgeConditionGroupLogicalOperator::And;
	this->Conditions = TArray<FDBDBadgeRewardCondition>();
}
