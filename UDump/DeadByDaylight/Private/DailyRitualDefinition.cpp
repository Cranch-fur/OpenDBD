#include "DailyRitualDefinition.h"


FDailyRitualDefinition::FDailyRitualDefinition()
{
	this->DisplayName = FText::GetEmpty();
	this->Description = FText::GetEmpty();
	this->IconPath = TEXT("");
	this->PossibleCharacterIDs = TArray<int32>();
	this->PossibleRoles = TArray<EPlayerRole>();
	this->TrackedEvents = TArray<EDBDScoreTypes>();
	this->DifficultyLevels = TArray<FDailyRitualLevel>();
	this->Evaluator = NULL;
}
