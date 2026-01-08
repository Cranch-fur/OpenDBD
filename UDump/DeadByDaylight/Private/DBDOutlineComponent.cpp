#include "DBDOutlineComponent.h"

class UTexture;

void UDBDOutlineComponent::SetFadeTexture(UTexture* inFadeTexture)
{

}

void UDBDOutlineComponent::RefreshOutlineComponent()
{

}

UDBDOutlineComponent::UDBDOutlineComponent()
{
	this->InterpolationSpeed = 2.000000;
	this->ShouldBeAboveOutlines = false;
	this->LimitToCustomDepthObjects = false;
	this->OutlineName = TEXT("");
}
