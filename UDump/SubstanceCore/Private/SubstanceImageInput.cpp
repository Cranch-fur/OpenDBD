#include "SubstanceImageInput.h"

USubstanceImageInput::USubstanceImageInput()
{
	this->CompressionRGB = 0;
	this->CompressionAlpha = 0;
	this->SizeX = 0;
	this->SizeY = 0;
	this->NumComponents = 0;
	this->SourceFilePath = TEXT("");
	this->SourceFileTimestamp = TEXT("");
}
