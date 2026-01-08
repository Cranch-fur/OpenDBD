#include "InterruptionDefinition.h"
#include "UObject/NoExportTypes.h"

class ADBDPlayer;
class UInteractionDefinition;
class AInteractable;

void UInterruptionDefinition::SetInterruptorSnapPoint(const FTransform& Point)
{

}

void UInterruptionDefinition::SetInterrupteeSnapPoint(const FTransform& Point)
{

}

bool UInterruptionDefinition::IsUsingMontageFollower() const
{
	return false;
}

bool UInterruptionDefinition::IsUsingAttack() const
{
	return false;
}

bool UInterruptionDefinition::IsInterruptionPossible_Implementation(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee) const
{
	return false;
}

bool UInterruptionDefinition::InteractionCheckHeightDelta(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee) const
{
	return false;
}

FRotator UInterruptionDefinition::GetInterruptionSnapPointRotationForInterruptor_Implementation(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const
{
	return FRotator{};
}

FRotator UInterruptionDefinition::GetInterruptionSnapPointRotationForInterruptee_Implementation(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const
{
	return FRotator{};
}

FVector UInterruptionDefinition::GetInterruptionSnapPointPositionForInterruptor_Implementation(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const
{
	return FVector{};
}

FVector UInterruptionDefinition::GetInterruptionSnapPointPositionForInterruptee_Implementation(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const
{
	return FVector{};
}

UInteractionDefinition* UInterruptionDefinition::GetInteractionDefinition() const
{
	return NULL;
}

AInteractable* UInterruptionDefinition::GetInteractable() const
{
	return NULL;
}

bool UInterruptionDefinition::CanInterruptWhileCarrying() const
{
	return false;
}

UInterruptionDefinition::UInterruptionDefinition()
{
	this->InterruptionID = NAME_None;
	this->_usesWorldCoordinates = false;
	this->_snapInterruptor = true;
	this->_snapInterruptee = true;
	this->_pickUpOnInterrupt = false;
	this->_canInterruptWhileCarrying = false;
	this->_useMontageFollower = false;
	this->_maxHeightDeltaBetweenCharacters = 75.000000;
}
