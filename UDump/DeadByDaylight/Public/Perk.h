#pragma once

#include "CoreMinimal.h"
#include "GameplayModifierContainer.h"
#include "GameplayModifierData.h"
#include "EInventoryItemType.h"
#include "Perk.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UPerk : public UGameplayModifierContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, EditFixedSize, Replicated)
	FGameplayModifierData PerkLevelData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool IsUsable;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 _perkLevel;

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_InitializePerk(FName perkID, int32 perkLevel);

	UFUNCTION(BlueprintPure)
	int32 GetPerkLevel() const;

	UFUNCTION(BlueprintPure)
	EInventoryItemType GetInventoryItemType() const;

	UFUNCTION(BlueprintCallable)
	FGameplayModifierData GetGameplayModifierData();

	UFUNCTION(BlueprintCallable)
	FGameplayModifierData GetConstGameplayModifierData() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPerk();
};

FORCEINLINE uint32 GetTypeHash(const UPerk) { return 0; }
