#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EInputInteractionType.h"
#include "UObject/NoExportTypes.h"
#include "Interactable.generated.h"

class UInteractor;
class ADBDPlayer;
class UInterruptionDefinition;
class UInteractionDefinition;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TriggerSoundOnRushEnter;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UInteractor*> _interactors;

public:
	UFUNCTION(BlueprintCallable)
	void SetInteractorsUsable(bool usable, UInteractor* exception);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PreInterruptionEvent(ADBDPlayer* interruptor, ADBDPlayer* interruptee, UInterruptionDefinition* interruption);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsReadyForInteractableInit();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInterruptionPossible(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee, const UInteractor* Interactor, const UInteractionDefinition* definition, const UInterruptionDefinition* interruption) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInteractionPossibleInternal(ADBDPlayer* Player, const UInteractor* Interactor, const UInteractionDefinition* definition, EInputInteractionType interactionType) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInteractionDone(ADBDPlayer* Player, const UInteractor* Interactor, const UInteractionDefinition* definition, EInputInteractionType interactionType) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool InteractableInit();

public:
	UFUNCTION(BlueprintPure)
	bool HasInteractableInitializedBeenCalled() const;

	UFUNCTION(BlueprintPure)
	bool HasAnOngoingInteraction() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetPlayerOwner() const;

	UFUNCTION(BlueprintPure)
	TArray<UInteractor*> GetInteractors() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetFocalPointPosition(ADBDPlayer* Player, const UInteractionDefinition* definition) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AuthorityRequestCancelOngoingInteractions(bool onlyOnHitCancelableInteractions);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Authority_UpdateLit(float DeltaSeconds, AActor* effector);

public:
	AInteractable();
};

FORCEINLINE uint32 GetTypeHash(const AInteractable) { return 0; }
