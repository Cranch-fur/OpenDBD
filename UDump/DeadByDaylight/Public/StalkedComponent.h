#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StalkedAudioWarning.h"
#include "StalkedAudioWarningStopStart.h"
#include "StalkedAudioWarningVolume.h"
#include "StalkedComponent.generated.h"

class UChargeableComponent;
class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UStalkedComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FStalkedAudioWarning OnStalkedAudioWarning;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FStalkedAudioWarningStopStart OnStalkedAudioWarningStopStart;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FStalkedAudioWarningVolume OnStalkedAudioWarningVolume;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _percentForHightlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _rangeForHightlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _percentForOutline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _rangeForOutline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _percentForStalkWarningMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _percentForStalkWarningMax;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	float _maxStalkPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UChargeableComponent* _chargeableComponent;

public:
	UFUNCTION(BlueprintCallable)
	void SetChargeableComponent(UChargeableComponent* ChargeableComponent);

private:
	UFUNCTION(BlueprintCallable)
	void OnPlayerDisconnect(ADBDPlayer* Player);

public:
	UFUNCTION(BlueprintPure)
	bool IsStalkHighlightVisible() const;

	UFUNCTION(BlueprintPure)
	bool IsOutlineVisible() const;

	UFUNCTION(BlueprintPure)
	bool IsBeingStalked() const;

	UFUNCTION(BlueprintPure)
	float GetStalkPoints(ADBDPlayer* caller) const;

	UFUNCTION(BlueprintPure)
	float GetPercentStalkPoints(ADBDPlayer* caller) const;

	UFUNCTION(BlueprintPure)
	float GetOutlineVisibleRange() const;

	UFUNCTION(BlueprintPure)
	float GetMaxStalkPoints(ADBDPlayer* caller) const;

	UFUNCTION(BlueprintPure)
	UChargeableComponent* GetChargeableComponent() const;

	UFUNCTION(BlueprintPure)
	bool CanBeStandingKilled() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UStalkedComponent();
};

FORCEINLINE uint32 GetTypeHash(const UStalkedComponent) { return 0; }
