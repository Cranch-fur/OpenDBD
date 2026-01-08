#include "WorldRunawayMeshComponent.h"

void UWorldRunawayMeshComponent::SetShouldRunAwayWithRandomDelay(bool should, float delayRange)
{

}

void UWorldRunawayMeshComponent::SetShouldRunAwayWithDelay(bool should, float Delay)
{

}

void UWorldRunawayMeshComponent::SetShouldRunAway(bool should)
{

}

bool UWorldRunawayMeshComponent::GetShouldRunAway() const
{
	return false;
}

float UWorldRunawayMeshComponent::GetRunawayMustTickDuration_Implementation() const
{
	return 0.0f;
}

float UWorldRunawayMeshComponent::GetReturnMustTickDuration_Implementation() const
{
	return 0.0f;
}

UWorldRunawayMeshComponent::UWorldRunawayMeshComponent()
{
	this->ResetCooldown = 0.000000;
}
