#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TierChangeDelegate.h"
#include "EDetectionZone.h"
#include "StalkerComponent.generated.h"

class UStalkedComponent;
class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UStalkerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FTierChangeDelegate OnTierChange;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UStalkedComponent*> _playerStalkedComponents;

public:
	UFUNCTION(BlueprintCallable)
	void Update(float deltaTime, ADBDPlayer* Player, EDetectionZone stalkerDetectionZone);

private:
	UFUNCTION(BlueprintCallable)
	void OnPlayerDisconnect(ADBDPlayer* Player);

public:
	UFUNCTION(BlueprintCallable)
	void NotifyKill(ADBDPlayer* killedPlayer);

	UFUNCTION(BlueprintPure)
	bool IsStalkingSomeone() const;

	UFUNCTION(BlueprintPure)
	bool IsAtLastStalkTier() const;

	UFUNCTION(BlueprintPure)
	bool HasMaxStalkPoints() const;

	UFUNCTION(BlueprintPure)
	float GetTotalStalkingPoints() const;

	UFUNCTION(BlueprintPure)
	float GetRequirementForTier(int32 tier) const;

	UFUNCTION(BlueprintPure)
	float GetPercentStalkingPointsInCurrentTier() const;

	UFUNCTION(BlueprintPure)
	float GetPercentStalkingPoints() const;

	UFUNCTION(BlueprintPure)
	int32 GetMaxStalkTier() const;

	UFUNCTION(BlueprintPure)
	float GetMaxStalkingPoints() const;

	UFUNCTION(BlueprintPure)
	float GetEvilWithinScoreMultiplier() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentTier() const;

	UFUNCTION(BlueprintPure)
	bool CanStandKill(ADBDPlayer* Player) const;

	UFUNCTION(BlueprintPure)
	bool CanStalk() const;

protected:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_Tier(int32 previousTier, int32 currentTier, bool IsFirstTime);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_SetStalkPoints(float stalkPoints);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_Multicast_Tier(int32 previousTier, int32 currentTier, bool IsFirstTime);

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable, WithValidation)
	void Broadcast_Multicast_SetStalkPoints(float stalkPoints);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_BeingStalked_Server(ADBDPlayer* Player, bool beingStalked);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_BeingStalked_Multicast(ADBDPlayer* Player, bool beingStalked);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_AddStalkPointsToPlayer_Server(ADBDPlayer* Player, float stalkPoints);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_AddStalkPointsToPlayer_Multicast(ADBDPlayer* Player, float stalkPoints);

public:
	UStalkerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UStalkerComponent) { return 0; }
