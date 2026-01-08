#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChargeableEmptiedDelegate.h"
#include "ChargeableSkillCheckResponse.h"
#include "ChargeApplied.h"
#include "DBDTimer.h"
#include "ChargeData.h"
#include "ESkillCheckCustomType.h"
#include "ChargeableComponent.generated.h"

class AActor;
class ADBDPlayer;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UChargeableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargeableEmptiedDelegate OnInteractionEmptiedEvent;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargeableSkillCheckResponse SkillCheckResponseAestheticDelegate;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargeableSkillCheckResponse SkillCheckResponseAuthorityDelegate;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargeApplied ChargeAppliedAuthorityDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowSkillChecksAtFullCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChargeableComponentID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DechargeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DechargeDelay;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _skillCheckFailurePenaltyTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _dechargeTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FChargeData> _chargesThisFrame;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<AActor*> _instigatorsSinceEmpty;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<FName, float> _tunableValueMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isCharging;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<int32> _playersWhoDidSkillCheck;

public:
	UFUNCTION(BlueprintCallable)
	void SetSecondsToCharge(float SecondsToCharge);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Reset();

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_UpdateChargeAndSpeed(float charge, float Speed);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SkillCheckResponseAesthetic(bool success, bool Bonus, ADBDPlayer* Player, bool TriggerLoudNoise, bool hadInput, ESkillCheckCustomType Type);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_OnEmptied() const;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_OnCompleteChanged(bool COMPLETED) const;

public:
	UFUNCTION(BlueprintPure)
	bool IsSkillCheckTimePenaltyActive() const;

	UFUNCTION(BlueprintPure)
	bool HasPlayerDoneSkillCheck(int32 PlayerId) const;

	UFUNCTION(BlueprintPure)
	float GetTunableValue(FName TunableValueID) const;

	UFUNCTION(BlueprintPure)
	float GetSkillCheckChargePercent(ADBDPlayer* Player, bool success, bool Bonus) const;

	UFUNCTION(BlueprintPure)
	float GetSkillCheckCharge(ADBDPlayer* Player, bool success, bool Bonus) const;

	UFUNCTION(BlueprintPure)
	float GetPercentComplete() const;

	UFUNCTION(BlueprintPure)
	float GetMaxCharge() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentCharge() const;

	UFUNCTION(BlueprintPure)
	bool GetComplete() const;

	UFUNCTION(BlueprintPure)
	float GetChargeRemaining() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddChargePercent(float ChargePercent, AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddCharge(float ChargeAmount, AActor* Instigator);

public:
	UChargeableComponent();
};

FORCEINLINE uint32 GetTypeHash(const UChargeableComponent) { return 0; }
