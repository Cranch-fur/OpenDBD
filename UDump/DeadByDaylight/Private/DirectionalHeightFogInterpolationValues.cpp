#include "DirectionalHeightFogInterpolationValues.h"


FDirectionalHeightFogInterpolationValues::FDirectionalHeightFogInterpolationValues()
{
	this->FogDensity = 0.0f;
	this->FogInscatteringColor = FLinearColor{};
	this->DirectionalInscatteringExponent = 0.0f;
	this->DirectionalInscatteringStartDistance = 0.0f;
	this->DirectionalInscatteringColor = FLinearColor{};
	this->FogHeightFalloff = 0.0f;
	this->FogMaxOpacity = 0.0f;
	this->StartDistance = 0.0f;
	this->DistanceUntilFlatFog = 0.0f;
	this->DistanceUntilNoFog = 0.0f;
}
