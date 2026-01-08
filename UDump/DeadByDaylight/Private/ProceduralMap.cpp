#include "ProceduralMap.h"


FProceduralMap::FProceduralMap()
{
	this->MapId = NAME_None;
	this->Name = FText::GetEmpty();
	this->ThemeName = FText::GetEmpty();
	this->Description = FText::GetEmpty();
	this->HookCountOffset = 0;
	this->BookShelvesMinCount = 0;
	this->BookShelvesMaxCount = 0;
	this->LivingWorldObjectsMinCount = 0;
	this->LivingWorldObjectsMaxCount = 0;
	this->ThumbnailPath = TEXT("");
	this->SortingIndex = 0;
	this->DlcIDString = TEXT("");
	this->DlcIDStringPS4 = TEXT("");
	this->DlcIDStringXB1 = TEXT("");
}
