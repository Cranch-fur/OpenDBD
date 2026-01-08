#include "CamperStillnessTrackerComponent.h"
#include "Engine/HitResult.h"

class UPrimitiveComponent;
class AActor;
class UExposerInteriorZoneComponent;

void UCamperStillnessTrackerComponent::OnOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void UCamperStillnessTrackerComponent::OnOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void UCamperStillnessTrackerComponent::ExitInteriorZone(UExposerInteriorZoneComponent* zone)
{

}

void UCamperStillnessTrackerComponent::EnterInteriorZone(UExposerInteriorZoneComponent* zone)
{

}

UCamperStillnessTrackerComponent::UCamperStillnessTrackerComponent()
{
	this->ChickenShitModifier = 0.000000;
	this->LoudNoiseTime = 0.000000;
	this->_exposerInstanceToSpawn = NULL;
}
