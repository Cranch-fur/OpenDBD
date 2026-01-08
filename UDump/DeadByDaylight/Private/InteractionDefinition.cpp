#include "InteractionDefinition.h"
#include "EInputInteractionType.h"
#include "AnimationMontageDescriptor.h"
#include "MontagePlaybackDefinition.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"

class ADBDPlayer;
class UInterruptionDefinition;
class UCurveFloat;
class UInteractor;
class AInteractable;
class AActor;
class UPrimitiveComponent;
class UInteractionDefinition;

bool UInteractionDefinition::IsSupportedCharacterType(ADBDPlayer* Player) const
{
	return false;
}

bool UInteractionDefinition::IsInterruptionPossible_Implementation(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee, const UInterruptionDefinition* interruption) const
{
	return false;
}

bool UInteractionDefinition::IsInteractionPossibleClient_Implementation(ADBDPlayer* Player, EInputInteractionType interactionType) const
{
	return false;
}

bool UInteractionDefinition::IsInteractionPossibleBP_Implementation(ADBDPlayer* Player, EInputInteractionType interactionType) const
{
	return false;
}

bool UInteractionDefinition::IsInteractionDone_Implementation(ADBDPlayer* Player, EInputInteractionType interactionType) const
{
	return false;
}

bool UInteractionDefinition::IsInputPressed(ADBDPlayer* Player) const
{
	return false;
}

bool UInteractionDefinition::IsCharacterFacingInteractorDirection(ADBDPlayer* Player) const
{
	return false;
}

bool UInteractionDefinition::IsCancelable_Implementation(ADBDPlayer* Player) const
{
	return false;
}

FAnimationMontageDescriptor UInteractionDefinition::GetUpdateMontage_Implementation() const
{
	return FAnimationMontageDescriptor{};
}

float UInteractionDefinition::GetUpdateAnimationPlayRate(ADBDPlayer* Player) const
{
	return 0.0f;
}

FMontagePlaybackDefinition UInteractionDefinition::GetUpdateAnimationPlayBackDefinition(ADBDPlayer* Player, bool Follower) const
{
	return FMontagePlaybackDefinition{};
}

float UInteractionDefinition::GetSnapTimeAtStart(ADBDPlayer* Player) const
{
	return 0.0f;
}

float UInteractionDefinition::GetSnapTime_Implementation(ADBDPlayer* Player) const
{
	return 0.0f;
}

FName UInteractionDefinition::GetSnapSocketName_Implementation(ADBDPlayer* Player) const
{
	return NAME_None;
}

FRotator UInteractionDefinition::GetSnapPointRotationAtStart(ADBDPlayer* Player) const
{
	return FRotator{};
}

FRotator UInteractionDefinition::GetSnapPointRotation_Implementation(ADBDPlayer* Player) const
{
	return FRotator{};
}

FVector UInteractionDefinition::GetSnapPointPositionAtStart(ADBDPlayer* Player) const
{
	return FVector{};
}

FVector UInteractionDefinition::GetSnapPointPosition_Implementation(ADBDPlayer* Player) const
{
	return FVector{};
}

float UInteractionDefinition::GetSnapDistanceAtStart(ADBDPlayer* Player) const
{
	return 0.0f;
}

float UInteractionDefinition::GetSnapDistance_Implementation(ADBDPlayer* Player) const
{
	return 0.0f;
}

ADBDPlayer* UInteractionDefinition::GetPlayerDependancy_Implementation(ADBDPlayer* interactingPlayer)
{
	return NULL;
}

UCurveFloat* UInteractionDefinition::GetNavigationSpeedCurveExit_Implementation() const
{
	return NULL;
}

TArray<UInterruptionDefinition*> UInteractionDefinition::GetInterruptionDefinitions() const
{
	return TArray<UInterruptionDefinition*>();
}

UInteractor* UInteractionDefinition::GetInteractor() const
{
	return NULL;
}

float UInteractionDefinition::GetInteractionTime(ADBDPlayer* Character) const
{
	return 0.0f;
}

FString UInteractionDefinition::GetInteractionText_Implementation(ADBDPlayer* Player)
{
	return TEXT("");
}

float UInteractionDefinition::GetInteractionSpeedModifier_Implementation(ADBDPlayer* Character) const
{
	return 0.0f;
}

float UInteractionDefinition::GetInteractionExitTime_Implementation(ADBDPlayer* Character) const
{
	return 0.0f;
}

FText UInteractionDefinition::GetInteractionDescriptionText() const
{
	return FText::GetEmpty();
}

AInteractable* UInteractionDefinition::GetInteractable() const
{
	return NULL;
}

FVector UInteractionDefinition::GetFocalPointPosition_Implementation(ADBDPlayer* Player) const
{
	return FVector{};
}

FAnimationMontageDescriptor UInteractionDefinition::GetExitMontage_Implementation() const
{
	return FAnimationMontageDescriptor{};
}

FMontagePlaybackDefinition UInteractionDefinition::GetExitAnimationPlayBackDefinition(ADBDPlayer* Player, bool Follower) const
{
	return FMontagePlaybackDefinition{};
}

FAnimationMontageDescriptor UInteractionDefinition::GetEnterMontage_Implementation() const
{
	return FAnimationMontageDescriptor{};
}

FMontagePlaybackDefinition UInteractionDefinition::GetEnterAnimationPlayBackDefinition(ADBDPlayer* Player, bool Follower) const
{
	return FMontagePlaybackDefinition{};
}

FVector UInteractionDefinition::GetActualSnapPointPositionAtStart(ADBDPlayer* Player) const
{
	return FVector{};
}

FVector UInteractionDefinition::GetActualSnapPointPosition(ADBDPlayer* Player) const
{
	return FVector{};
}

TArray<AActor*> UInteractionDefinition::GetActorsToIgnoreOverlap_Implementation() const
{
	return TArray<AActor*>();
}

float UInteractionDefinition::GetActorHeightDelta(ADBDPlayer* Player) const
{
	return 0.0f;
}

void UInteractionDefinition::EndOverlapCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

bool UInteractionDefinition::DoesPlayerHaveClearPath(ADBDPlayer* Player) const
{
	return false;
}

void UInteractionDefinition::CompleteCharge()
{

}

bool UInteractionDefinition::CanOverrideInteraction_Implementation(UInteractionDefinition* Interaction) const
{
	return false;
}

void UInteractionDefinition::BeginOverlapCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void UInteractionDefinition::AttachToZone(UPrimitiveComponent* zone)
{

}

void UInteractionDefinition::AttachToInteractor(UInteractor* Interactor)
{

}

void UInteractionDefinition::AddMutuallyExclusiveInteraction(UInteractionDefinition* Interaction)
{

}

UInteractionDefinition::UInteractionDefinition()
{
	this->InteractionID = TEXT("");
	this->interactionInputType = EInputInteractionType::VE_Interact;
	this->SnapTime = 0.100000;
	this->ResetCameraDuringSnap = false;
	this->AllowStartInteractionFromHeldInput = false;
	this->StopInteractionOnInputRelease = false;
	this->StopInteractionOnMontageComplete = false;
	this->StopInteractionOnMontageBlendingOut = false;
	this->StopInteractionOnTimerExpire = false;
	this->StopInteractionOnHeightDelta = false;
	this->StopEnterMontageOnExit = true;
	this->StopUpdateMontageOnExit = false;
	this->InteractionAnimation = EInteractionAnimation::VE_None;
	this->CamperCanInteract = false;
	this->SlasherCanInteract = false;
	this->snapPosition = false;
	this->snapRotation = false;
	this->SnapStopDistance = 0.000000;
	this->TestClearPathDuringSnap = true;
	this->TestPlayerOrientationOnSnap = true;
	this->ScaleEnterAnimationToTime = false;
	this->ScaleMainAnimationToTime = false;
	this->ScaleExitAnimationToTime = false;
	this->CanInteractWhileIncapacitated = false;
	this->CanInteractWhileCloaked = false;
	this->CanInteractWhileChainBlinking = false;
	this->CanInteractWhileAttacking = false;
	this->CanInteractWhileShocked = false;
	this->AllowNavigation = false;
	this->AllowRotation = false;
	this->UseCameraMontageMode = true;
	this->OwnerBlocksAttack = true;
	this->IsHighPriority = false;
	this->UseAuthoritativeMovement = true;
	this->AuthoritativeUnmoveable = false;
	this->DisableServerPositionUpdates = true;
	this->CancelOnHit = false;
	this->HideItem = true;
	this->AutoBindToParentInteractor = true;
	this->AutoBindToParentZone = true;
	this->CancelOnMoveInput = -1.000000;
	this->CanCrouch = false;
	this->AffectsStillness = true;
	this->DebugUnavailability = false;
	this->OwnershipUsability = EInteractionOwnership::AnyCanUse;
	this->InteractionTime = 0.000000;
	this->ExitInteractionTime = 0.000000;
	this->InteractionDescriptionText = FText::GetEmpty();
	this->InteractionHeightDeltaMax = 150.000000;
	this->SnapSocketName = NAME_None;
}
