#include "BlinkerComponent.h"
#include "UObject/NoExportTypes.h"
#include "EBlinkerState.h"

class ADBDPlayer;

FVector UBlinkerComponent::UpdateTargetLocation(ADBDPlayer* Player, float blinkDistance, const TArray<float>& scanPercentLocations, float raycastStartHeight, float raycastEndHeight)
{
	return FVector{};
}

void UBlinkerComponent::SetBlinkerState(EBlinkerState blinkerState)
{

}

FVector UBlinkerComponent::LocalAddRandomToTargetLocation(ADBDPlayer* Player, FVector TargetLocation, float Distance)
{
	return FVector{};
}

bool UBlinkerComponent::IsReady() const
{
	return false;
}

EBlinkerState UBlinkerComponent::GetBlinkerState() const
{
	return EBlinkerState::VE_Ready;
}

float UBlinkerComponent::GetAutomaticBlinkDelay(int32 chainBlinkCount) const
{
	return 0.0f;
}

UBlinkerComponent::UBlinkerComponent()
{

}
