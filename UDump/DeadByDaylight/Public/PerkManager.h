#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EGameplayModifierType.h"
#include "EGameplayModifierFlag.h"
#include "EGameplayModifierSource.h"
#include "PerkManager.generated.h"

class UStatusEffect;
class UGameplayModifierContainer;
class UPerk;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UPerkManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	bool HasPerkModifierOfType(EGameplayModifierType ModifierType) const;

	UFUNCTION(BlueprintPure)
	bool HasPerkFlag(EGameplayModifierFlag PerkFlag, EGameplayModifierSource Filter) const;

	UFUNCTION(BlueprintPure)
	bool HasAnyEffectWithID(FName ID) const;

	UFUNCTION(BlueprintPure)
	bool HasAnyAddonWithTag(FName Tag) const;

	UFUNCTION(BlueprintPure)
	bool HasAllCharacterPerksEquipped() const;

	UFUNCTION(BlueprintPure)
	UStatusEffect* GetStatusEffect(FName ID) const;

	UFUNCTION(BlueprintPure)
	UGameplayModifierContainer* GetSourceWithModifier(EGameplayModifierType ModifierType, EGameplayModifierSource Filter) const;

	UFUNCTION(BlueprintPure)
	UGameplayModifierContainer* GetSourceWithFlag(EGameplayModifierFlag PerkFlag, EGameplayModifierSource Filter) const;

	UFUNCTION(BlueprintPure)
	float GetPerkModifierMinValue(EGameplayModifierType ModifierType, float startValue) const;

	UFUNCTION(BlueprintPure)
	float GetPerkModifierMaxValue(EGameplayModifierType ModifierType) const;

	UFUNCTION(BlueprintPure)
	float GetPerkModifier(EGameplayModifierType ModifierType, EGameplayModifierSource Filter) const;

	UFUNCTION(BlueprintPure)
	UPerk* GetPerk(FName ID) const;

	UFUNCTION(BlueprintPure)
	float GetMultiplicativeModifierValueWithCompoundNegative(EGameplayModifierType ModifierType, EGameplayModifierSource Filter) const;

	UFUNCTION(BlueprintPure)
	float GetMultiplicativeModifierValue(EGameplayModifierType ModifierType) const;

public:
	UPerkManager();
};

FORCEINLINE uint32 GetTypeHash(const UPerkManager) { return 0; }
