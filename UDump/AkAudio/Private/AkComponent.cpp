#include "AkComponent.h"

class UAkAudioEvent;

void UAkComponent::UseReverbVolumes(bool inUseReverbVolumes)
{

}

void UAkComponent::Stop()
{

}

void UAkComponent::SetSwitch(const FString& SwitchGroup, const FString& SwitchState)
{

}

void UAkComponent::SetStopWhenOwnerDestroyed(bool bStopWhenOwnerDestroyed)
{

}

void UAkComponent::SetRTPCValue(const FString& RTPC, float Value, int32 InterpolationTimeMs)
{

}

void UAkComponent::SetOutputBusVolume(float BusVolume)
{

}

void UAkComponent::SetAttenuationScalingFactor(float Value)
{

}

void UAkComponent::SetActiveListeners(int32 in_uListenerMask)
{

}

void UAkComponent::PostTrigger(const FString& Trigger)
{

}

int32 UAkComponent::PostAssociatedAkEvent()
{
	return 0;
}

int32 UAkComponent::PostAkEventByName(const FString& in_EventName)
{
	return 0;
}

int32 UAkComponent::PostAkEvent(UAkAudioEvent* AkEvent, const FString& in_EventName)
{
	return 0;
}

float UAkComponent::GetAttenuationRadius() const
{
	return 0.0f;
}

UAkComponent::UAkComponent()
{
	this->StopWhenOwnerDestroyed = true;
	this->AttenuationScalingFactor = 1.000000;
	this->OcclusionRefreshInterval = 0.200000;
	this->ShouldTransmitRelativeAngle = false;
	this->OcclusionAffect = 100.000000;
	this->EventName = TEXT("");
}
