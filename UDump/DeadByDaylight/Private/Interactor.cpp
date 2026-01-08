#include "Interactor.h"
#include "EInputInteractionType.h"
#include "Net/UnrealNetwork.h"

class ADBDPlayer;
class UInteractionDefinition;
class UInterruptionDefinition;
class AInteractable;

void UInteractor::Multicast_ReservationStatus_Implementation(ADBDPlayer* Player, bool Lock)
{

}

bool UInteractor::Multicast_ReservationStatus_Validate(ADBDPlayer* Player, bool Lock)
{
	return true;
}

void UInteractor::Multicast_LockStatus_Implementation(ADBDPlayer* Player, UInteractionDefinition* currentInteraction, bool Lock)
{

}

bool UInteractor::Multicast_LockStatus_Validate(ADBDPlayer* Player, UInteractionDefinition* currentInteraction, bool Lock)
{
	return true;
}

bool UInteractor::IsInterruptionPossible(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee, const UInteractionDefinition* definition, const UInterruptionDefinition* interruption) const
{
	return false;
}

bool UInteractor::IsInteractionPossible(ADBDPlayer* Player, const UInteractionDefinition* definition, EInputInteractionType interactionType) const
{
	return false;
}

bool UInteractor::IsInteracting() const
{
	return false;
}

TArray<UInteractionDefinition*> UInteractor::GetInteractionDefinitions() const
{
	return TArray<UInteractionDefinition*>();
}

ADBDPlayer* UInteractor::GetInteractingPlayer() const
{
	return NULL;
}

AInteractable* UInteractor::GetInteractable() const
{
	return NULL;
}

UInteractionDefinition* UInteractor::GetCurrentInteraction() const
{
	return NULL;
}

bool UInteractor::CanPerformInteraction(ADBDPlayer* Player, const UInteractionDefinition* definition) const
{
	return false;
}

void UInteractor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInteractor, IsUsable);
}

UInteractor::UInteractor()
{
	this->InteractorName = TEXT("");
	this->IsUsable = true;
}
