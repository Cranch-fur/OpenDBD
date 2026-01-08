#include "PlayerInteractionHandler.h"
#include "ESkillCheckCustomType.h"
#include "EInputInteractionType.h"
#include "InteractionPlayerProperties.h"

class ADBDPlayer;
class UInterruptionDefinition;
class UInteractionDefinition;
class AActor;

void UPlayerInteractionHandler::TriggerSkillCheck()
{

}

void UPlayerInteractionHandler::StartCustomSkillCheck(ESkillCheckCustomType Type, float Delay)
{

}

void UPlayerInteractionHandler::SortAvailableInteractions()
{

}

void UPlayerInteractionHandler::SetSkillCheckTimer(float Delay)
{

}

void UPlayerInteractionHandler::SetScanForInteractionsEnabled(bool Enabled)
{

}

void UPlayerInteractionHandler::Server_StoreInterruption_Interruptor_Implementation(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition)
{

}

bool UPlayerInteractionHandler::Server_StoreInterruption_Interruptor_Validate(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition)
{
	return true;
}

void UPlayerInteractionHandler::Server_StoreInteraction_Implementation(EInputInteractionType interactionType, UInteractionDefinition* Interaction)
{

}

bool UPlayerInteractionHandler::Server_StoreInteraction_Validate(EInputInteractionType interactionType, UInteractionDefinition* Interaction)
{
	return true;
}

void UPlayerInteractionHandler::Server_RequestInterruption_Interruptor_Implementation()
{

}

bool UPlayerInteractionHandler::Server_RequestInterruption_Interruptor_Validate()
{
	return true;
}

void UPlayerInteractionHandler::Server_RequestInteraction_Implementation()
{

}

bool UPlayerInteractionHandler::Server_RequestInteraction_Validate()
{
	return true;
}

void UPlayerInteractionHandler::Server_EndInteraction_Implementation(UInteractionDefinition* Interaction)
{

}

bool UPlayerInteractionHandler::Server_EndInteraction_Validate(UInteractionDefinition* Interaction)
{
	return true;
}

void UPlayerInteractionHandler::Server_DebugRequestInteraction_Implementation(const FString& interactionName, const FString& InteractorName, const FString& interactableName)
{

}

bool UPlayerInteractionHandler::Server_DebugRequestInteraction_Validate(const FString& interactionName, const FString& InteractorName, const FString& interactableName)
{
	return true;
}

void UPlayerInteractionHandler::Server_AnswerInterruption_Interruptee_Implementation(bool success)
{

}

bool UPlayerInteractionHandler::Server_AnswerInterruption_Interruptee_Validate(bool success)
{
	return true;
}

void UPlayerInteractionHandler::ResetSuccessiveSkillCheckCount()
{

}

void UPlayerInteractionHandler::RemoveInteraction(UInteractionDefinition* Interaction)
{

}

int32 UPlayerInteractionHandler::NumInteractionsInZone() const
{
	return 0;
}

void UPlayerInteractionHandler::Multicast_StoreInterruption_Interruptor_Implementation(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition)
{

}

bool UPlayerInteractionHandler::Multicast_StoreInterruption_Interruptor_Validate(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition)
{
	return true;
}

void UPlayerInteractionHandler::Multicast_StoreInteraction_Implementation(EInputInteractionType interactionType, UInteractionDefinition* Interaction)
{

}

bool UPlayerInteractionHandler::Multicast_StoreInteraction_Validate(EInputInteractionType interactionType, UInteractionDefinition* Interaction)
{
	return true;
}

void UPlayerInteractionHandler::Multicast_RequestInterruption_Interruptee_Implementation(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition)
{

}

bool UPlayerInteractionHandler::Multicast_RequestInterruption_Interruptee_Validate(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition)
{
	return true;
}

void UPlayerInteractionHandler::Multicast_InterruptionRequestResult_Interruptor_Implementation(bool success)
{

}

bool UPlayerInteractionHandler::Multicast_InterruptionRequestResult_Interruptor_Validate(bool success)
{
	return true;
}

void UPlayerInteractionHandler::Multicast_InteractionRequestResult_Implementation(bool success)
{

}

bool UPlayerInteractionHandler::Multicast_InteractionRequestResult_Validate(bool success)
{
	return true;
}

void UPlayerInteractionHandler::IncrementSuccessiveSkillCheckCount()
{

}

bool UPlayerInteractionHandler::HasAvailableInteraction(EInputInteractionType interactionType)
{
	return false;
}

bool UPlayerInteractionHandler::HasActiveSkillCheck() const
{
	return false;
}

float UPlayerInteractionHandler::GetMultiplicativeSkillCheckProbabilityModifier() const
{
	return 0.0f;
}

UInteractionDefinition* UPlayerInteractionHandler::GetCurrentInteraction() const
{
	return NULL;
}

UInteractionDefinition* UPlayerInteractionHandler::GetAvailableInteraction(EInputInteractionType interactionType) const
{
	return NULL;
}

float UPlayerInteractionHandler::GetAdditiveSkillCheckProbabilityModifier() const
{
	return 0.0f;
}

void UPlayerInteractionHandler::ForceBeginSkillCheckProgress()
{

}

void UPlayerInteractionHandler::Client_NotifyChargeCompleted_Implementation()
{

}

bool UPlayerInteractionHandler::Client_NotifyChargeCompleted_Validate()
{
	return true;
}

void UPlayerInteractionHandler::CleanInteractionArray(AActor* DestroyedActor)
{

}

bool UPlayerInteractionHandler::CanPerformInteraction(const FString& interactionName) const
{
	return false;
}

void UPlayerInteractionHandler::Broadcast_Multicast_ConfirmChargedCompleted_Implementation(bool chargeComplete)
{

}

bool UPlayerInteractionHandler::Broadcast_Multicast_ConfirmChargedCompleted_Validate(bool chargeComplete)
{
	return true;
}

void UPlayerInteractionHandler::Broadcast_InteractionPlayerProperties_Multicast_Implementation(FInteractionPlayerProperties playerProperties)
{

}

bool UPlayerInteractionHandler::Broadcast_InteractionPlayerProperties_Multicast_Validate(FInteractionPlayerProperties playerProperties)
{
	return true;
}

void UPlayerInteractionHandler::Broadcast_InteractionPlayerProperties_Implementation(FInteractionPlayerProperties playerProperties)
{

}

bool UPlayerInteractionHandler::Broadcast_InteractionPlayerProperties_Validate(FInteractionPlayerProperties playerProperties)
{
	return true;
}

void UPlayerInteractionHandler::Broadcast_ConfirmChargedCompleted_Implementation(bool chargeComplete)
{

}

bool UPlayerInteractionHandler::Broadcast_ConfirmChargedCompleted_Validate(bool chargeComplete)
{
	return true;
}

void UPlayerInteractionHandler::AddInteraction(UInteractionDefinition* Interaction)
{

}

UPlayerInteractionHandler::UPlayerInteractionHandler()
{
	this->PriorityConeAngle = 50.000000;
	this->InteractionLayer = EInteractionLayer::VE_Camper;
	this->_hasInteractionRequest = false;
	this->_hasInteractionRequestInBuffer = false;
	this->_requestedInteractionType = EInputInteractionType::VE_None;
	this->_requestedInteractionState = ERequestState::VE_None;
	this->_currentInteractionType = EInputInteractionType::VE_None;
	this->_requestedInterruptionIsInterruptor = false;
	this->_currentInterruptionIsInterruptor = false;
	this->_interactionInProgress = false;
}
