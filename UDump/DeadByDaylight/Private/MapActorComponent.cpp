#include "MapActorComponent.h"

class ADBDPlayer;

void UMapActorComponent::Multicast_SetDetectionEnabled_Implementation(bool Enabled)
{

}

bool UMapActorComponent::IsKnownBy(ADBDPlayer* Character) const
{
	return false;
}

bool UMapActorComponent::GetDetectionEnabled() const
{
	return false;
}

UMapActorComponent::UMapActorComponent()
{
	this->Category = EMapActorCategory::None;
	this->_detectionEnabled = true;
}
