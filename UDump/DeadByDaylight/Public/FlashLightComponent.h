#pragma once

#include "CoreMinimal.h"
#include "AimableInterface.h"
#include "Components/ActorComponent.h"
#include "UObject/ScriptInterface.h"
#include "FlashLightComponent.generated.h"

class IAimingStrategyInterface;
class USceneComponent;
class USpotLightComponent;
class UChargerComponent;
class AInteractable;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UFlashLightComponent : public UActorComponent, public IAimableInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlindingScoreCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<IAimingStrategyInterface> AimStrategy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USceneComponent* Tip;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USpotLightComponent* _spotlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UChargerComponent* _charger;

public:
	UFUNCTION(BlueprintPure)
	bool UsingCheapSpotLightOcclusion() const;

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_UpdateTargetLitCharge(AInteractable* target, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StopBlindCharge(AActor* effector);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_StartBlindCharge(AActor* effector);

public:
	UFUNCTION(BlueprintCallable)
	void OnUseReleased();

	UFUNCTION(BlueprintCallable)
	void OnUse();

	UFUNCTION(BlueprintPure)
	bool IsSlasherBlinded() const;

	UFUNCTION(BlueprintPure)
	bool IsInUse() const;

	UFUNCTION(BlueprintPure)
	float GetOcclusionDistance() const;

	UFUNCTION(BlueprintPure)
	float GetMaxAimingDistance() const;

	UFUNCTION(BlueprintPure)
	float GetEffectiveTimeToInvisibilityBurnoutModifier() const;

	UFUNCTION(BlueprintPure)
	float GetEffectiveTimeToBlindModifier() const;

	UFUNCTION(BlueprintPure)
	float GetEffectiveBlindnessDuration() const;

	UFUNCTION(BlueprintPure)
	float GetEffectiveBeamLength() const;

	UFUNCTION(BlueprintPure)
	float GetEffectiveBeamHalfAngle() const;

	UFUNCTION(BlueprintPure)
	float GetEffectiveAccuracy() const;

public:
	UFlashLightComponent();
};

FORCEINLINE uint32 GetTypeHash(const UFlashLightComponent) { return 0; }
