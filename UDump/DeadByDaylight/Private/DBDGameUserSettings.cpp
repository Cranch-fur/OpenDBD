#include "DBDGameUserSettings.h"

void UDBDGameUserSettings::SaveSettings()
{

}

UDBDGameUserSettings::UDBDGameUserSettings()
{
	this->ScreenScaleForWindowedMode = 100;
	this->ScalabilityLevel = 3;
	this->AutoAdjust = false;
	this->ScreenResolution = 100;
	this->FullScreen = true;
	this->MainVolume = 80;
	this->MenuMusicVolume = 100;
	this->UseHeadphones = true;
	this->MuteMusic = false;
	this->MuteOnFocusLost = true;
	this->KillerMuseSensitivity = 50;
	this->SurvivorMuseSensitivity = 50;
	this->KillerControllerSensitivity = 50;
	this->SurvivorControllerSensitivity = 50;
	this->InvertY = false;
	this->Language = TEXT("en");
	this->HighestWeightSeenNews = 0;
	this->ChatDefaultVisibility = true;
	this->LastPanelContextId = 0;
}
