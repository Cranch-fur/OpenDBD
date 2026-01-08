#include "FlashLightReceiverComponent.h"

class UFlashLightComponent;
class AActor;

void UFlashLightReceiverComponent::RemoveFlashLight(UFlashLightComponent* flashLight)
{

}

bool UFlashLightReceiverComponent::IsInAnyFlashLightZone() const
{
	return false;
}

bool UFlashLightReceiverComponent::IsActorInAnyFlashLightZone(const AActor* target)
{
	return false;
}

void UFlashLightReceiverComponent::AddFlashLight(UFlashLightComponent* flashLight)
{

}

UFlashLightReceiverComponent::UFlashLightReceiverComponent()
{

}
