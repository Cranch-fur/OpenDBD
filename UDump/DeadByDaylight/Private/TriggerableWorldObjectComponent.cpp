#include "TriggerableWorldObjectComponent.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"

class UPrimitiveComponent;
class AActor;
class ADBDPlayer;

void UTriggerableWorldObjectComponent::SetGuaranteedTriggerVolume(UPrimitiveComponent* Volume)
{

}

void UTriggerableWorldObjectComponent::SetConditionalTriggerVolume(UPrimitiveComponent* Volume)
{

}

void UTriggerableWorldObjectComponent::Multicast_OnTriggerReset_Implementation()
{

}

void UTriggerableWorldObjectComponent::Multicast_OnTriggered_Implementation(AActor* Instigator, FVector triggerLocation)
{

}

void UTriggerableWorldObjectComponent::GetConditionalModifiers_Implementation(ADBDPlayer* Player, float& outAdditive, float& outMultiplicative)
{

}

bool UTriggerableWorldObjectComponent::CanPlayerTriggerGuaranteed_Implementation(ADBDPlayer* Player)
{
	return false;
}

bool UTriggerableWorldObjectComponent::CanPlayerTriggerConditional_Implementation(ADBDPlayer* Player)
{
	return false;
}

void UTriggerableWorldObjectComponent::Authority_Trigger(AActor* Instigator, FVector triggerLocation)
{

}

void UTriggerableWorldObjectComponent::Authority_ResetTrigger()
{

}

void UTriggerableWorldObjectComponent::Authority_GuaranteedVolumeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{

}

UTriggerableWorldObjectComponent::UTriggerableWorldObjectComponent()
{
	this->_triggerResetTime = 0.000000;
	this->_randomChance = 0.000000;
	this->_rollFrequency = 0.000000;
}
