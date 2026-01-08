#include "ConsoleUserSettings.h"


FConsoleUserSettings::FConsoleUserSettings()
{
	this->ActionMappings = TArray<FInputActionKeyMapping>();
	this->AxisMappings = TArray<FInputAxisKeyMapping>();
	this->MainVolume = 0;
	this->MenuMusicVolume = 0;
	this->UseHeadphones = false;
	this->MuteMusic = false;
	this->MuteOnFocusLost = false;
	this->KillerControllerSensitivity = 0;
	this->SurvivorControllerSensitivity = 0;
	this->InvertY = false;
}
