#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Lock.h"
#include "EInputInteractionType.h"
#include "Interactor.generated.h"

class UInteractionDefinition;
class ADBDPlayer;
class UInterruptionDefinition;
class AInteractable;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UInteractor : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool IsUsable;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FLock _lock;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FLock _reservation;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UInteractionDefinition* _currentInteraction;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UInteractionDefinition*> _interactionDefinitions;

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_ReservationStatus(ADBDPlayer* Player, bool Lock);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_LockStatus(ADBDPlayer* Player, UInteractionDefinition* currentInteraction, bool Lock);

public:
	UFUNCTION(BlueprintCallable)
	bool IsInterruptionPossible(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee, const UInteractionDefinition* definition, const UInterruptionDefinition* interruption) const;

	UFUNCTION(BlueprintCallable)
	bool IsInteractionPossible(ADBDPlayer* Player, const UInteractionDefinition* definition, EInputInteractionType interactionType) const;

	UFUNCTION(BlueprintPure)
	bool IsInteracting() const;

	UFUNCTION(BlueprintPure)
	TArray<UInteractionDefinition*> GetInteractionDefinitions() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetInteractingPlayer() const;

	UFUNCTION(BlueprintPure)
	AInteractable* GetInteractable() const;

	UFUNCTION(BlueprintPure)
	UInteractionDefinition* GetCurrentInteraction() const;

	UFUNCTION(BlueprintPure)
	bool CanPerformInteraction(ADBDPlayer* Player, const UInteractionDefinition* definition) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BPPostInitializeComponents();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UInteractor();
};

FORCEINLINE uint32 GetTypeHash(const UInteractor) { return 0; }
