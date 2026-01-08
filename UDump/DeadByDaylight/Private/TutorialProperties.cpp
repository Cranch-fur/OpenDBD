#include "TutorialProperties.h"


FTutorialProperties::FTutorialProperties()
{
	this->Name = NAME_None;
	this->Description = FText::GetEmpty();
	this->Type = ETutorialTypes::Survivor;
	this->Path = TEXT("");
}
