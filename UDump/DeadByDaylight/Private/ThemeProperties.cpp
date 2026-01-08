#include "ThemeProperties.h"


FThemeProperties::FThemeProperties()
{
	this->Name = NAME_None;
	this->Weather = NAME_None;
	this->TilePath = TEXT("");
	this->MapPath = TEXT("");
	this->ThemeNumber = 0;
	this->DisplayName = FText::GetEmpty();
	this->ThemeColorId = EThemeColorId::Invalid;
	this->ThemeSelectionOfferingEffectType = EOfferingEffectType::SlasherPointsAll;
	this->SubstitutionElements = TArray<FGameplayElementSubsitutions>();
	this->Weight = 0.0f;
	this->DlcIDString = TEXT("");
	this->DlcIDStringPS4 = TEXT("");
	this->DlcIDStringXB1 = TEXT("");
}
