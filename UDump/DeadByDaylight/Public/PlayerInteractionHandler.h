#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EInteractionLayer.h"
#include "OnDesactivateSkillCheck.h"
#include "EInputInteractionType.h"
#include "ERequestState.h"
#include "ESkillCheckCustomType.h"
#include "InteractionPlayerProperties.h"
#include "PlayerInteractionHandler.generated.h"

class UInteractionDefinition;
class ADBDPlayer;
class USkillCheck;
class UInterruptionDefinition;
class AActor;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UPlayerInteractionHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PriorityConeAngle;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	TArray<UInteractionDefinition*> InteractionsInZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionLayer InteractionLayer;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnDesactivateSkillCheck OnDesactivateSkillCheckDelegate;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _hasInteractionRequest;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _hasInteractionRequestInBuffer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UInteractionDefinition* _requestedInteraction;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	EInputInteractionType _requestedInteractionType;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ERequestState _requestedInteractionState;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UInteractionDefinition* _currentInteraction;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ADBDPlayer*> _currentPlayerDependencies;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	EInputInteractionType _currentInteractionType;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	USkillCheck* _skillCheck;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<float> _progressBasedSkillChecks;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _requestedInterruptionOtherParty;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UInterruptionDefinition* _requestedInterruptionDefinition;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _requestedInterruptionIsInterruptor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _currentInterruptionOtherParty;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UInterruptionDefinition* _currentInterruptionDefinition;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _currentInterruptionIsInterruptor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _interactionInProgress;

public:
	UFUNCTION(BlueprintCallable)
	void TriggerSkillCheck();

	UFUNCTION(BlueprintCallable)
	void StartCustomSkillCheck(ESkillCheckCustomType Type, float Delay);

	UFUNCTION(BlueprintCallable)
	void SortAvailableInteractions();

	UFUNCTION(BlueprintCallable)
	void SetSkillCheckTimer(float Delay);

	UFUNCTION(BlueprintCallable)
	void SetScanForInteractionsEnabled(bool Enabled);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StoreInterruption_Interruptor(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StoreInteraction(EInputInteractionType interactionType, UInteractionDefinition* Interaction);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_RequestInterruption_Interruptor();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_RequestInteraction();

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_EndInteraction(UInteractionDefinition* Interaction);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_DebugRequestInteraction(const FString& interactionName, const FString& InteractorName, const FString& interactableName);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_AnswerInterruption_Interruptee(bool success);

public:
	UFUNCTION(BlueprintCallable)
	void ResetSuccessiveSkillCheckCount();

	UFUNCTION(BlueprintCallable)
	void RemoveInteraction(UInteractionDefinition* Interaction);

	UFUNCTION(BlueprintPure)
	int32 NumInteractionsInZone() const;

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_StoreInterruption_Interruptor(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_StoreInteraction(EInputInteractionType interactionType, UInteractionDefinition* Interaction);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_RequestInterruption_Interruptee(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* InterruptionDefinition);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_InterruptionRequestResult_Interruptor(bool success);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_InteractionRequestResult(bool success);

public:
	UFUNCTION(BlueprintCallable)
	void IncrementSuccessiveSkillCheckCount();

	UFUNCTION(BlueprintCallable)
	bool HasAvailableInteraction(EInputInteractionType interactionType);

	UFUNCTION(BlueprintPure)
	bool HasActiveSkillCheck() const;

	UFUNCTION(BlueprintPure)
	float GetMultiplicativeSkillCheckProbabilityModifier() const;

	UFUNCTION(BlueprintPure)
	UInteractionDefinition* GetCurrentInteraction() const;

	UFUNCTION(BlueprintPure)
	UInteractionDefinition* GetAvailableInteraction(EInputInteractionType interactionType) const;

	UFUNCTION(BlueprintPure)
	float GetAdditiveSkillCheckProbabilityModifier() const;

	UFUNCTION(BlueprintCallable)
	void ForceBeginSkillCheckProgress();

	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation)
	void Client_NotifyChargeCompleted();

	UFUNCTION(BlueprintCallable)
	void CleanInteractionArray(AActor* DestroyedActor);

	UFUNCTION(BlueprintPure)
	bool CanPerformInteraction(const FString& interactionName) const;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_Multicast_ConfirmChargedCompleted(bool chargeComplete);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_InteractionPlayerProperties_Multicast(FInteractionPlayerProperties playerProperties);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_InteractionPlayerProperties(FInteractionPlayerProperties playerProperties);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_ConfirmChargedCompleted(bool chargeComplete);

	UFUNCTION(BlueprintCallable)
	void AddInteraction(UInteractionDefinition* Interaction);

public:
	UPlayerInteractionHandler();
};

FORCEINLINE uint32 GetTypeHash(const UPlayerInteractionHandler) { return 0; }
