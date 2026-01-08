#include "FlashLightComponent.h"

class AInteractable;
class AActor;

bool UFlashLightComponent::UsingCheapSpotLightOcclusion() const
{
	return false;
}

void UFlashLightComponent::Server_UpdateTargetLitCharge_Implementation(AInteractable* target, float DeltaSeconds)
{

}

bool UFlashLightComponent::Server_UpdateTargetLitCharge_Validate(AInteractable* target, float DeltaSeconds)
{
	return true;
}

void UFlashLightComponent::Server_StopBlindCharge_Implementation(AActor* effector)
{

}

bool UFlashLightComponent::Server_StopBlindCharge_Validate(AActor* effector)
{
	return true;
}

void UFlashLightComponent::Server_StartBlindCharge_Implementation(AActor* effector)
{

}

bool UFlashLightComponent::Server_StartBlindCharge_Validate(AActor* effector)
{
	return true;
}

void UFlashLightComponent::OnUseReleased()
{

}

void UFlashLightComponent::OnUse()
{

}

bool UFlashLightComponent::IsSlasherBlinded() const
{
	return false;
}

bool UFlashLightComponent::IsInUse() const
{
	return false;
}

float UFlashLightComponent::GetOcclusionDistance() const
{
	return 0.0f;
}

float UFlashLightComponent::GetMaxAimingDistance() const
{
	return 0.0f;
}

float UFlashLightComponent::GetEffectiveTimeToInvisibilityBurnoutModifier() const
{
	return 0.0f;
}

float UFlashLightComponent::GetEffectiveTimeToBlindModifier() const
{
	return 0.0f;
}

float UFlashLightComponent::GetEffectiveBlindnessDuration() const
{
	return 0.0f;
}

float UFlashLightComponent::GetEffectiveBeamLength() const
{
	return 0.0f;
}

float UFlashLightComponent::GetEffectiveBeamHalfAngle() const
{
	return 0.0f;
}

float UFlashLightComponent::GetEffectiveAccuracy() const
{
	return 0.0f;
}

UFlashLightComponent::UFlashLightComponent()
{
	this->BlindingScoreCooldown = 5.000000;
}
