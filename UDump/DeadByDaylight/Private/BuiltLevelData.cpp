#include "BuiltLevelData.h"


FBuiltLevelData::FBuiltLevelData()
{
	this->ThemeName = NAME_None;
	this->ThemeWeather = NAME_None;
	this->MapName = TEXT("");
	this->TileCount = 0;
	this->Dependencies = TArray<FDependency>();
}
