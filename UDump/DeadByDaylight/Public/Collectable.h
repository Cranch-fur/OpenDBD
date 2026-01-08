#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "EItemHandPosition.h"
#include "ECollectableCategory.h"
#include "ECollectableState.h"
#include "UObject/NoExportTypes.h"
#include "EGameplayModifierFlag.h"
#include "EGameplayModifierType.h"
#include "Collectable.generated.h"

class UItemModifier;
class UInteractor;
class USkeletalMeshComponent;
class USceneComponent;
class ADBDPlayer;
class UItemAddon;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ACollectable : public AInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StrafeOnUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AimOnUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool VisibleWhenEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopRunningOnUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemHandPosition HandPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasUseInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasDropInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECollectableCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Draft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FromPlayerSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UItemModifier* BaseItemModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UItemModifier* ItemModifier1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UItemModifier* ItemModifier2;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BeingCollected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BeingDropped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UInteractor* _itemInteractor;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USceneComponent* _placementOrigin;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _collector;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	ECollectableState _state;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UItemAddon*> _itemAddons;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	int32 _itemCount;

	UPROPERTY(EditInstanceOnly, ReplicatedUsing=OnRep_BegunPlayFenceName, Transient)
	FGuid _begunPlayFenceName;

public:
	UFUNCTION(BlueprintCallable)
	void Use();

	UFUNCTION(BlueprintCallable)
	void SetItemInteractor(UInteractor* Interactor);

	UFUNCTION(BlueprintCallable)
	void SetCount(int32 Count);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUseReleased();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUse();

private:
	UFUNCTION(BlueprintCallable)
	void OnRep_BegunPlayFenceName();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnPostItemAddonsCreation(ADBDPlayer* Player);

private:
	UFUNCTION(BlueprintCallable)
	void OnFenceInitialized();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDropped();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCollected(ADBDPlayer* collector);

	UFUNCTION(BlueprintCallable)
	void NotifyUpdated();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_UseReleased();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_Use();

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_RemoveItemAddon(UItemAddon* addon);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_Dropped(ADBDPlayer* droppedBy, const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_Collected(ADBDPlayer* collector);

public:
	UFUNCTION(BlueprintPure)
	bool IsStored() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsRechargeable() const;

	UFUNCTION(BlueprintPure)
	bool IsOnGround() const;

	UFUNCTION(BlueprintPure)
	bool IsEquipped() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCountDisplayForced() const;

	UFUNCTION(BlueprintPure)
	bool IsCollected() const;

	UFUNCTION(BlueprintPure)
	bool HasGameplayModifierFlag(EGameplayModifierFlag flag) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetUsePercentLeft() const;

	UFUNCTION(BlueprintPure)
	float GetModifierSum(EGameplayModifierType Type, float DefaultValue) const;

	UFUNCTION(BlueprintPure)
	float GetModifierMax(EGameplayModifierType Type, float DefaultValue) const;

	UFUNCTION(BlueprintPure)
	UInteractor* GetItemInteractor() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	uint8 GetItemIconIndex() const;

	UFUNCTION(BlueprintPure)
	TArray<UItemAddon*> GetItemAddons();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	int32 GetCount() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetCollector() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DebugPrintStats();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanUse() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_RemoveItemAddon(UItemAddon* addon);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_AddItemAddon(UItemAddon* addon);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ACollectable();
};

FORCEINLINE uint32 GetTypeHash(const ACollectable) { return 0; }
