#pragma once

#include "CoreMinimal.h"
#include "GameplayModifierContainer.h"
#include "GameplayModifierData.h"
#include "StatusViewSource.h"
#include "StatusEffect.generated.h"

class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UStatusEffect : public UGameplayModifierContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FGameplayModifierData ModifierData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BroadcastAsStatusEffectWhenApplicable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BroadcastAsStatusEffectAlways;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float customParam;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RemainingLifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool ShouldDisplay;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _originatingPlayer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UGameplayModifierContainer* _originatingEffect;

public:
	UFUNCTION(BlueprintCallable)
	void SetShouldDisplay(bool On);

	UFUNCTION(BlueprintCallable)
	void OnInitialized_Internal();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_InitializeStatusEffect(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParamNew, UGameplayModifierContainer* originatingEffect);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetShouldDisplay() const;

	UFUNCTION(BlueprintPure)
	float GetPercentageFill() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetOriginatingPlayer() const;

	UFUNCTION(BlueprintPure)
	UGameplayModifierContainer* GetOriginatingEffect() const;

	UFUNCTION(BlueprintCallable)
	FGameplayModifierData GetGameplayModifierData();

	UFUNCTION(BlueprintCallable)
	FGameplayModifierData GetConstGameplayModifierData() const;

	UFUNCTION(BlueprintPure=false, BlueprintCallable)
	void FireActiveStatusViewEvent(FName StatusViewID, FName uniqueSourceID, const FStatusViewSource StatusViewSource) const;

	UFUNCTION(BlueprintPure=false, BlueprintCallable)
	void FireActiveStatusEffectEvent(float percentage, int32 LevelToDisplay) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_RemoveSelf();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UStatusEffect();
};

FORCEINLINE uint32 GetTypeHash(const UStatusEffect) { return 0; }
