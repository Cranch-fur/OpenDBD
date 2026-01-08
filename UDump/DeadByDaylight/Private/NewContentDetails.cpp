#include "NewContentDetails.h"


FNewContentDetails::FNewContentDetails()
{
	this->Weight = 0;
	this->Type = ENewContentType::COMING_SOON;
	this->Title = TEXT("");
	this->Description = TEXT("");
	this->ImagePath = TEXT("");
	this->ImageHeight = 0;
	this->IsHidden = false;
	this->ContentTags = TArray<FName>();
}
