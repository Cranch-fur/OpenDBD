#include "CharacterDescription.h"


FCharacterDescription::FCharacterDescription()
{
	this->CharacterIndex = 0;
	this->Role = EPlayerRole::VE_None;
	this->DisplayName = FText::GetEmpty();
	this->Subtitle = FText::GetEmpty();
	this->BackStory = FText::GetEmpty();
	this->Biography = FText::GetEmpty();
	this->IconFilePath = NAME_None;
	this->RequiredDlcIDString = TEXT("");
	this->RequiredDlcIDStringPS4 = TEXT("");
	this->RequiredDlcIDStringXB1 = TEXT("");
	this->IdName = NAME_None;
	this->GamePawn = NULL;
	this->MenuPawn = NULL;
	this->DefaultItem = NAME_None;
}
