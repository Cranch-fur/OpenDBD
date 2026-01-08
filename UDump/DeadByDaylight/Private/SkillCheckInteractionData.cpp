#include "SkillCheckInteractionData.h"


FSkillCheckInteractionData::FSkillCheckInteractionData()
{
	this->IsActive = false;
	this->HitAreaStart = 0.0f;
	this->HitAreaLength = 0.0f;
	this->BonusAreaLength = 0.0f;
	this->Progress = 0.0f;
	this->IsHexed = false;
	this->xOffset = 0;
	this->yOffset = 0;
}
