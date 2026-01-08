#pragma once

#include "CoreMinimal.h"
#include "EnergyPoweredInterface.h"
#include "Components/ActorComponent.h"
#include "ChargeStateChangeDelegate.h"
#include "ChargePercentChangeDelegate.h"
#include "EEnergyTypeEnum.h"
#include "ChargerComponent.generated.h"

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UChargerComponent : public UActorComponent, public IEnergyPoweredInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChargerComponentID;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargeStateChangeDelegate OnChargeStateChange;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FChargePercentChangeDelegate OnChargePercentChange;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnergyTypeEnum EnergyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnergyUsePerSecond;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_EnergyLevel, meta=(AllowPrivateAccess=true))
	float _currentEnergyLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta=(AllowPrivateAccess=true))
	float _maxEnergyLevel;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UseEnergy(float seconds, float energyUseModifier);

	UFUNCTION(BlueprintCallable)
	void SetNormalizedEnergyLevel(float energyLevel);

	UFUNCTION(BlueprintCallable)
	void SetMaxEnergyLevel(float energyLevel);

	UFUNCTION(BlueprintCallable)
	void SetEnergyLevel(float energyLevel);

	UFUNCTION(BlueprintCallable)
	void Reset();

private:
	UFUNCTION(BlueprintCallable)
	void OnRep_EnergyLevel(float previousEnergyLevel);

public:
	UFUNCTION(BlueprintCallable)
	bool HasCharge();

	UFUNCTION(BlueprintPure)
	float GetNormalizedEnergyLevel() const;

	UFUNCTION(BlueprintPure)
	float GetMaxEnergyLevel() const;

	UFUNCTION(BlueprintPure)
	float GetEnergyLevel() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UChargerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UChargerComponent) { return 0; }
