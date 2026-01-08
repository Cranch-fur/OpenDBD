#include "DBDCharacterMovementComponent.h"
#include "Engine/HitResult.h"

class UCapsuleComponent;
class UPrimitiveComponent;
class AActor;
class AMovementModifierActor;

void UDBDCharacterMovementComponent::SetRandomInputAmplitude(float Value)
{

}

void UDBDCharacterMovementComponent::SetMovementModifierDetector(UCapsuleComponent* movementModifierDetector)
{

}

void UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

bool UDBDCharacterMovementComponent::IsApplyingRandomInput() const
{
	return false;
}

AMovementModifierActor* UDBDCharacterMovementComponent::GetOverlappedMovementModifierActor() const
{
	return NULL;
}

UDBDCharacterMovementComponent::UDBDCharacterMovementComponent()
{
	this->ProjectedWallSlidingFactor = 1.000000;
	this->RandomInputFrontMultiplier = 0.800000;
	this->RandomInputSideMultiplier = 1.000000;
	this->RandomInputBackMultiplier = 0.200000;
	this->SprintEffector = NAME_None;
}
