#include "Interactable.h"
#include "EInputInteractionType.h"
#include "UObject/NoExportTypes.h"

class UInteractor;
class ADBDPlayer;
class UInteractionDefinition;
class UInterruptionDefinition;

void AInteractable::SetInteractorsUsable(bool usable, UInteractor* exception)
{

}

bool AInteractable::IsReadyForInteractableInit_Implementation()
{
	return false;
}

bool AInteractable::IsInterruptionPossible_Implementation(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee, const UInteractor* Interactor, const UInteractionDefinition* definition, const UInterruptionDefinition* interruption) const
{
	return false;
}

bool AInteractable::IsInteractionPossibleInternal_Implementation(ADBDPlayer* Player, const UInteractor* Interactor, const UInteractionDefinition* definition, EInputInteractionType interactionType) const
{
	return false;
}

bool AInteractable::IsInteractionDone_Implementation(ADBDPlayer* Player, const UInteractor* Interactor, const UInteractionDefinition* definition, EInputInteractionType interactionType) const
{
	return false;
}

bool AInteractable::InteractableInit_Implementation()
{
	return false;
}

bool AInteractable::HasInteractableInitializedBeenCalled() const
{
	return false;
}

bool AInteractable::HasAnOngoingInteraction() const
{
	return false;
}

ADBDPlayer* AInteractable::GetPlayerOwner() const
{
	return NULL;
}

TArray<UInteractor*> AInteractable::GetInteractors() const
{
	return TArray<UInteractor*>();
}

FVector AInteractable::GetFocalPointPosition_Implementation(ADBDPlayer* Player, const UInteractionDefinition* definition) const
{
	return FVector{};
}

void AInteractable::AuthorityRequestCancelOngoingInteractions(bool onlyOnHitCancelableInteractions)
{

}

AInteractable::AInteractable()
{
	this->TriggerSoundOnRushEnter = false;
}
