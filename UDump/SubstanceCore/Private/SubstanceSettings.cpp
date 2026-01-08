#include "SubstanceSettings.h"

USubstanceSettings::USubstanceSettings()
{
	this->MemoryBudgetMb = 512;
	this->CPUCores = 32;
	this->AsyncLoadMipClip = 3;
	this->DefaultGenerationMode = SGM_PlatformDefault;
	this->SubstanceEngine = SET_CPU;
}
