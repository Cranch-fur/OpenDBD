#include "HintData.h"


FHintData::FHintData()
{
	this->Category = EHintCategory::Lore;
	this->Title = FText::GetEmpty();
	this->Description = FText::GetEmpty();
	this->IconPath = TEXT("");
	this->levelMin = 0;
	this->levelMax = 0;
	this->relevantCharacterID = 0;
	this->DLCId = TEXT("");
	this->DLCIdPS4 = TEXT("");
	this->DLCIdXB1 = TEXT("");
}
