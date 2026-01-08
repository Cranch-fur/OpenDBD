#pragma once

#include "CoreMinimal.h"
#include "InteractionDefinition.h"
#include "ChargeableDefSkillCheckResponse.h"
#include "EChargeableInteractionBarType.h"
#include "ESkillCheckCustomType.h"
#include "ChargeableInteractionDefinition.generated.h"

class UChargeableComponent;
class ADBDPlayer;
class ACollectable;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UChargeableInteractionDefinition : public UInteractionDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargeableDefSkillCheckResponse SkillCheckResponseAestheticDelegate;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargeableDefSkillCheckResponse SkillCheckResponseAuthorityDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopInteractionOnChargeComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasProgressivelyHarderSkillChecks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProgressiveDifficultyModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EChargeableInteractionBarType ProgressBarType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ProgressPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HideProgressBar;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UChargeableComponent* _chargeableComponent;

public:
	UFUNCTION(BlueprintCallable)
	void SetChargeableComponent(UChargeableComponent* ChargeableComponent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSkillCheckResponseAuthorityBP(ADBDPlayer* Player, bool success, bool Bonus, bool hadInput, ESkillCheckCustomType Type);

private:
	UFUNCTION(BlueprintCallable)
	void OnSkillCheckResponseAuthority(bool success, bool Bonus, ADBDPlayer* Player, bool TriggerLoudNoise, bool hadInput, ESkillCheckCustomType Type);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSkillCheckResponseAestheticBP(ADBDPlayer* Player, bool success, bool Bonus, bool hadInput, ESkillCheckCustomType Type);

private:
	UFUNCTION(BlueprintCallable)
	void OnSkillCheckResponseAesthetic(bool success, bool Bonus, ADBDPlayer* Player, bool TriggerLoudNoise, bool hadInput, ESkillCheckCustomType Type);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInteractionCompletionStateChanged(ADBDPlayer* Player, bool complete);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnChargeableChangedBP(UChargeableComponent* oldChargeable, UChargeableComponent* newChargeable);

	UFUNCTION(BlueprintPure)
	bool IsChargeComplete() const;

	UFUNCTION(BlueprintPure)
	bool HasSkillCheckHappened() const;

	UFUNCTION(BlueprintPure)
	float GetTunableValue(FName TunableValueID, float DefaultValue) const;

	UFUNCTION(BlueprintPure)
	float GetTotalChargeModifier() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetSuccessProbability() const;

	UFUNCTION(BlueprintPure)
	float GetSkillCheckProbability() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetSkillCheckDifficulityModifier() const;

	UFUNCTION(BlueprintPure)
	int32 GetPipsPassed() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetPerkMultiplier() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetOwningDBDPlayer() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetMultiplicativeLuckBonus() const;

	UFUNCTION(BlueprintPure)
	float GetMaxCharge() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetItemEfficiencyModifier() const;

	UFUNCTION(BlueprintPure)
	float GetChargeAmountForTime(float DeltaSeconds) const;

	UFUNCTION(BlueprintPure)
	UChargeableComponent* GetChargeableComponent() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetAdditiveLuckBonus() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool Authority_ChargeWithItem(float DeltaSeconds, ACollectable* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool AddCharge(float DeltaSeconds);

public:
	UChargeableInteractionDefinition();
};

FORCEINLINE uint32 GetTypeHash(const UChargeableInteractionDefinition) { return 0; }
