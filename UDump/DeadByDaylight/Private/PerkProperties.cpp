#include "PerkProperties.h"


FPerkProperties::FPerkProperties()
{
	this->Tags = TArray<FName>();
	this->AssociatedPlayerIndex = 0;
	this->MandatoryOnBloodweblevel = 0;
	this->TeachableOnBloodweblevel = 0;
	this->PerkBlueprint = NULL;
	this->PerkLevelRarity = TArray<EItemRarity>();
	this->PerkLevel1Description = FText::GetEmpty();
	this->PerkLevel2Description = FText::GetEmpty();
	this->PerkLevel3Description = FText::GetEmpty();
}
