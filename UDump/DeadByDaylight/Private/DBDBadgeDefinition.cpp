#include "DBDBadgeDefinition.h"


FDBDBadgeDefinition::FDBDBadgeDefinition()
{
	this->ID = NAME_None;
	this->Name = FText::GetEmpty();
	this->Description = FText::GetEmpty();
	this->Roles = TArray<EPlayerRole>();
	this->SpecificCharacterIndexes = TArray<int32>();
	this->BadgeGroup = EBadgeGroup::PrimaryGoal;
	this->Priority = 0;
	this->PipsToAward = 0;
	this->RewardConditionGroups = TArray<FDBDBadgeRewardConditionGroup>();
	this->CustomBadge = NULL;
	this->IconFilePaths = TArray<FString>();
	this->BadgeEvaluation = EBadgeEvaluation::Authority;
	this->Enabled = false;
}
