#include "InteractionPlayerProperties.h"


FInteractionPlayerProperties::FInteractionPlayerProperties()
{
	this->AverageVelocityAtStart = FVector{};
	this->StaminaAtStart = 0.0f;
	this->PlayerPositionAtStart = FVector{};
	this->Requester = NULL;
	this->SnapPositionAtStart = FVector{};
	this->SnapRotationAtStart = FRotator{};
	this->SnapDistanceAtStart = 0.0f;
	this->SnapTimeAtStart = 0.0f;
}
