#pragma once

#include "CoreMinimal.h"
#include "ItemModifier.h"
#include "Templates/SubclassOf.h"
#include "EInventoryItemType.h"
#include "ItemAddon.generated.h"

class ACollectable;
class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UItemAddon : public UItemModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACollectable> BaseItemType;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	EInventoryItemType InventoryItemType;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnPostItemAddonsCreation(ADBDPlayer* Player, ACollectable* Item);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetBaseItem(ACollectable* baseItem);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_InitializeItemAddon();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_AddAddonToItem(ACollectable* Item);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsReadyForInitialization() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InitializeAddon();

public:
	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetOwningPlayer() const;

	UFUNCTION(BlueprintPure)
	EInventoryItemType GetInventoryItemType() const;

	UFUNCTION(BlueprintPure)
	ACollectable* GetBaseItem() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Authority_OnCollectablePickedUp(ADBDPlayer* Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Authority_OnCollectableDropped(ADBDPlayer* Player);

private:
	UFUNCTION(BlueprintCallable)
	void ApplyMetaModifiers();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UItemAddon();
};

FORCEINLINE uint32 GetTypeHash(const UItemAddon) { return 0; }
