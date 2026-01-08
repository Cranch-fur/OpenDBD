#include "ImpactInfo.h"


FImpactInfo::FImpactInfo()
{
	this->Position = FVector{};
	this->Actor = NULL;
	this->Component = NULL;
	this->SurfaceName = TEXT("");
}
