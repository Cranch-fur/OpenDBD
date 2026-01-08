#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EInputInteractionType.h"
#include "EInteractionAnimation.h"
#include "EInteractionOwnership.h"
#include "AnimationMontageDescriptor.h"
#include "MontagePlaybackDefinition.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"
#include "InteractionDefinition.generated.h"

class UCurveFloat;
class UInterruptionDefinition;
class UInteractor;
class UInteractionDefinition;
class ADBDPlayer;
class AInteractable;
class AActor;
class UPrimitiveComponent;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UInteractionDefinition : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputInteractionType interactionInputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SnapTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ResetCameraDuringSnap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowStartInteractionFromHeldInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopInteractionOnInputRelease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopInteractionOnMontageComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopInteractionOnMontageBlendingOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopInteractionOnTimerExpire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopInteractionOnHeightDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopEnterMontageOnExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopUpdateMontageOnExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionAnimation InteractionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CamperCanInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SlasherCanInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool snapPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool snapRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SnapStopDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TestClearPathDuringSnap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TestPlayerOrientationOnSnap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ScaleEnterAnimationToTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ScaleMainAnimationToTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ScaleExitAnimationToTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanInteractWhileIncapacitated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanInteractWhileCloaked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanInteractWhileChainBlinking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanInteractWhileAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanInteractWhileShocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowNavigation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseCameraMontageMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* NavigationSpeedCurveEnter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* NavigationSpeedCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* NavigationSpeedCurveExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OwnerBlocksAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHighPriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseAuthoritativeMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AuthoritativeUnmoveable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DisableServerPositionUpdates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CancelOnHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HideItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoBindToParentInteractor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoBindToParentZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CancelOnMoveInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ProgressBasedSkillChecks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AffectsStillness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UInterruptionDefinition*> _interruptionDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugUnavailability;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionOwnership OwnershipUsability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExitInteractionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionDescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionHeightDeltaMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnimationMontageDescriptor EnterMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnimationMontageDescriptor UpdateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAnimationMontageDescriptor ExitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SnapSocketName;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UInteractor* _interactor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UInteractionDefinition*> _mutuallyExclusiveInteractionsClient;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionUpdateTick(ADBDPlayer* Player, float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionUpdateStart(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionUpdateEnd(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionInterruptStarted(ADBDPlayer* Player, UInterruptionDefinition* currentInterruption, ADBDPlayer* interruptingPlayer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionInterruptStart(ADBDPlayer* Player, UInterruptionDefinition* currentInterruption, ADBDPlayer* interruptingPlayer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionInterruptEnd(ADBDPlayer* Player, UInterruptionDefinition* currentInterruption, ADBDPlayer* interruptingPlayer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionInit(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionFinished(ADBDPlayer* Player, bool hasInteractionStarted);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionExitStart(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionExitEnd(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionEnterTick(ADBDPlayer* Player, float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionEnterStart(ADBDPlayer* Player, float actualSnapTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionEnterFailed(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionEnterEnd(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionCancelled(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionAnimNotifyEvent(FName NotifyId, ADBDPlayer* Player);

	UFUNCTION(BlueprintPure)
	bool IsSupportedCharacterType(ADBDPlayer* Player) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInterruptionPossible(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee, const UInterruptionDefinition* interruption) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInteractionPossibleClient(ADBDPlayer* Player, EInputInteractionType interactionType) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInteractionPossibleBP(ADBDPlayer* Player, EInputInteractionType interactionType) const;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInteractionDone(ADBDPlayer* Player, EInputInteractionType interactionType) const;

	UFUNCTION(BlueprintPure)
	bool IsInputPressed(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure)
	bool IsCharacterFacingInteractorDirection(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCancelable(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FAnimationMontageDescriptor GetUpdateMontage() const;

	UFUNCTION(BlueprintPure)
	float GetUpdateAnimationPlayRate(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure)
	FMontagePlaybackDefinition GetUpdateAnimationPlayBackDefinition(ADBDPlayer* Player, bool Follower) const;

	UFUNCTION(BlueprintPure)
	float GetSnapTimeAtStart(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetSnapTime(ADBDPlayer* Player) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetSnapSocketName(ADBDPlayer* Player) const;

public:
	UFUNCTION(BlueprintPure)
	FRotator GetSnapPointRotationAtStart(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FRotator GetSnapPointRotation(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure)
	FVector GetSnapPointPositionAtStart(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetSnapPointPosition(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure)
	float GetSnapDistanceAtStart(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetSnapDistance(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ADBDPlayer* GetPlayerDependancy(ADBDPlayer* interactingPlayer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCurveFloat* GetNavigationSpeedCurveExit() const;

	UFUNCTION(BlueprintPure)
	TArray<UInterruptionDefinition*> GetInterruptionDefinitions() const;

	UFUNCTION(BlueprintPure)
	UInteractor* GetInteractor() const;

	UFUNCTION(BlueprintPure)
	float GetInteractionTime(ADBDPlayer* Character) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetInteractionText(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetInteractionSpeedModifier(ADBDPlayer* Character) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetInteractionExitTime(ADBDPlayer* Character) const;

	UFUNCTION(BlueprintCallable)
	FText GetInteractionDescriptionText() const;

	UFUNCTION(BlueprintPure)
	AInteractable* GetInteractable() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetFocalPointPosition(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FAnimationMontageDescriptor GetExitMontage() const;

	UFUNCTION(BlueprintPure)
	FMontagePlaybackDefinition GetExitAnimationPlayBackDefinition(ADBDPlayer* Player, bool Follower) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FAnimationMontageDescriptor GetEnterMontage() const;

	UFUNCTION(BlueprintPure)
	FMontagePlaybackDefinition GetEnterAnimationPlayBackDefinition(ADBDPlayer* Player, bool Follower) const;

	UFUNCTION(BlueprintPure)
	FVector GetActualSnapPointPositionAtStart(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure)
	FVector GetActualSnapPointPosition(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AActor*> GetActorsToIgnoreOverlap() const;

	UFUNCTION(BlueprintPure)
	float GetActorHeightDelta(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintCallable)
	void EndOverlapCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintPure)
	bool DoesPlayerHaveClearPath(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void CompleteCharge();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanOverrideInteraction(UInteractionDefinition* Interaction) const;

	UFUNCTION()
	void BeginOverlapCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void AttachToZone(UPrimitiveComponent* zone);

	UFUNCTION(BlueprintCallable)
	void AttachToInteractor(UInteractor* Interactor);

	UFUNCTION(BlueprintCallable)
	void AddMutuallyExclusiveInteraction(UInteractionDefinition* Interaction);

public:
	UInteractionDefinition();
};

FORCEINLINE uint32 GetTypeHash(const UInteractionDefinition) { return 0; }
