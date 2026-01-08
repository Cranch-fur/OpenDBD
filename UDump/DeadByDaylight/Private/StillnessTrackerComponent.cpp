#include "StillnessTrackerComponent.h"

void UStillnessTrackerComponent::StartTracking()
{

}

UStillnessTrackerComponent::UStillnessTrackerComponent()
{
	this->SpeedThreshold = 300.000000;
	this->DistanceThreshold = 400.000000;
	this->DistanceDecay = 1.000000;
	this->DecayWhileMoving = false;
	this->StillnessThreshhold = 0.700000;
	this->StillnessTimerLimit = 120.000000;
	this->DecaySpeedMultiplier = 2.000000;
}
