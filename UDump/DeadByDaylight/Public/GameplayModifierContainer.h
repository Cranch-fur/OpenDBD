#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBDTimer.h"
#include "EGameplayModifierType.h"
#include "EGameplayModifierFlag.h"
#include "EInventoryItemType.h"
#include "GameplayModifierData.h"
#include "Templates/SubclassOf.h"
#include "GameplayModifierContainer.generated.h"

class UGameplayModifierCondition;
class UStatusEffect;
class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UGameplayModifierContainer : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseApplyFunction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BroadcastWhenApplicable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BroadcastOnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BroadcastAlways;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	FName ID;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer HUDIconTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDBDTimer CooldownTimer;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, Instanced)
	TArray<UGameplayModifierCondition*> _conditionsInner;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<FName> _tags;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateCooldownTimer(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void TriggerTimerActivePerkEvent(float Duration);

	UFUNCTION(BlueprintCallable)
	void SetModifierValue(EGameplayModifierType GameplayModifierType, float GameplayModifierValue);

	UFUNCTION(BlueprintCallable)
	void SetFlag(EGameplayModifierFlag GameplayModifierFlag, bool flagValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnStatusEffectInitialized(UStatusEffect* effect);

	UFUNCTION(BlueprintCallable)
	void OnInitialized_Internal();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInitialized();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnApplyEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnApplyBegin();

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_TriggerCooldownTimer(float cooldownTime);

public:
	UFUNCTION(BlueprintPure)
	bool IsApplicable() const;

	UFUNCTION(BlueprintCallable)
	void InstantiateGameplayModifierConditions();

	UFUNCTION(BlueprintPure)
	bool HasModifierOfType(EGameplayModifierType GameplayModifierType) const;

	UFUNCTION(BlueprintPure)
	bool HasFlag(EGameplayModifierFlag GameplayModifierFlag) const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetOwningPlayer() const;

	UFUNCTION(BlueprintPure)
	float GetModifierValue(EGameplayModifierType GameplayModifierType) const;

	UFUNCTION(BlueprintPure)
	int32 GetLevelToDisplay() const;

	UFUNCTION(BlueprintPure)
	EInventoryItemType GetInventoryItemType() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	int32 GetIconFilePathIndex() const;

	UFUNCTION(BlueprintCallable)
	FGameplayModifierData GetGameplayModifierData();

	UFUNCTION(BlueprintCallable)
	FGameplayModifierData GetConstGameplayModifierData() const;

	UFUNCTION(BlueprintPure)
	UGameplayModifierCondition* GetCondition(int32 Index) const;

	UFUNCTION(BlueprintPure=false, BlueprintCallable)
	void FireActivePerkEvent(float percentage, int32 chargeCount) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_TriggerCooldownTimer(float coolddownTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Apply(float deltaTime);

	UFUNCTION(BlueprintCallable)
	UGameplayModifierCondition* AddCondition(TSubclassOf<UGameplayModifierCondition> conditionType);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UGameplayModifierContainer();
};

FORCEINLINE uint32 GetTypeHash(const UGameplayModifierContainer) { return 0; }
