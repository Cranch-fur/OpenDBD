#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EPlayerRole.h"
#include "CamperStateData.h"
#include "SlasherStateData.h"
#include "PlayerStateData.h"
#include "SkillCheckInteractionData.h"
#include "PlayerGameplayEventDelegate.h"
#include "ScoreEventArray.h"
#include "UserGameStats.h"
#include "OngoingScoreData.h"
#include "DBDRecentGameplayEvents.h"
#include "EGameState.h"
#include "EKillerJoiningState.h"
#include "ScoreEvent.h"
#include "EDBDScoreTypes.h"
#include "StatusViewSource.h"
#include "DBDPlayerState.generated.h"

class URitualHandlerComponent;
class UAchievementHandlerComponent;
class UActivityTrackerComponent;
class UDBDBadgeEvaluatorComponent;
class UGameplayNotificationManager;
class AActor;

UCLASS(Blueprintable)
class ADBDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	bool GameLevelLoaded;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	FString MirrorsId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData, Transient)
	FString SteamId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData, Transient)
	bool IsPlayerReady;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData, Transient)
	int32 SelectedCamperIndex;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData, Transient)
	EPlayerRole GameRole;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData, Transient)
	int32 SelectedSlasherIndex;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData, Transient)
	FCamperStateData CamperData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	URitualHandlerComponent* RitualHandler;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UAchievementHandlerComponent* AchievementHandler;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UActivityTrackerComponent* ActivityTracker;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UDBDBadgeEvaluatorComponent* BadgeEvaluator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData)
	FSlasherStateData SlasherData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData)
	FPlayerStateData PlayerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_DisplayData)
	FSkillCheckInteractionData SkillCheckInteractionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReadyToTravel;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FPlayerGameplayEventDelegate OnPlayerGameplayEvent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FScoreEventArray> _scoreEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUserGameStats _cachedUserGameStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOngoingScoreData> _ongoingScoreEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDBDRecentGameplayEvents _recentGameplayEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool _inParadise;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UGameplayNotificationManager* _gameplayNotificationManager;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void UpdateSkillCheckInteractionData(float Progress);

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateOngoingScores();

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetSkillCheckInteractionData(FSkillCheckInteractionData skillCheckData);

	UFUNCTION(BlueprintCallable)
	void SetPlayerGameState(EGameState newGameState);

	UFUNCTION(BlueprintCallable)
	void SetKillerJoiningState(EKillerJoiningState State);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetSelectedCharacterId(EPlayerRole forRole, int32 ID, bool updateDisplayData);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetSelectedCharacterCustomization(EPlayerRole forRole, int32 ID, const TArray<FName>& customizationItemIds, bool updateDisplayData);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetPlayerGameState(EGameState newGameState);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetGameRole(EPlayerRole newPlayerRole);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_KillerServerJoined(bool succeeded);

	UFUNCTION(BlueprintCallable)
	void OnRep_DisplayData();

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_UpdateSkillCheckInteractionData(float Progress);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetSkillCheckInteractionData(FSkillCheckInteractionData skillCheckInteraction);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetSelectedCharacterId(EPlayerRole forRole, int32 ID, bool updateDisplayData);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetSelectedCharacterCustomization(EPlayerRole forRole, int32 ID, const TArray<FName>& customizationItemIds, bool updateDisplayData);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetPlayerGameState(EGameState newGameState);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_SetInParadise(bool KILLED);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetEquipment(int32 SlotIndex, FName ItemId, bool callOnRep);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetAsLeftMatch();

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_ScoreEvent(FScoreEvent ScoreEvent);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_FireGameplayEvent(EDBDScoreTypes playerGameplayEventType, float amount, AActor* effector, AActor* target);

	UFUNCTION(BlueprintCallable)
	void LogCurrentLoadout();

	UFUNCTION(BlueprintCallable)
	EKillerJoiningState KillerJoiningState() const;

	UFUNCTION(BlueprintCallable)
	bool IsEscaped();

	UFUNCTION(BlueprintCallable)
	bool HasHappened(EDBDScoreTypes gameplayEventType, float maxSecondsAgo);

	UFUNCTION(BlueprintPure)
	UGameplayNotificationManager* GetGameplayNotificationManager() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentCharacterID() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void FireScoreEvent(EDBDScoreTypes ScoreType, float percentToAward, bool redirectToGameEventSystem);

	UFUNCTION(BlueprintCallable)
	void FireActiveStatusViewEvent(FName StatusViewID, FName uniqueSourceID, const FStatusViewSource StatusViewSource);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void FireActiveStatusEffectEvent(FName statusEffectID, float percentage, int32 iconFilePathIndex, int32 LevelToDisplay);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void FireActivePerkEvent(FName perkID, int32 LevelToDisplay, int32 iconFilePathIndex, float percentage, int32 chargeCount);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SetInParadise(bool KILLED);

protected:
	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation)
	void Client_SetGameRole(EPlayerRole newRole);

public:
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_RemoveItemFromInventory(FName toRemove, bool updateLoadout);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AuthorityFireGameplayEvent(EDBDScoreTypes playerGameplayEventType, float amount, AActor* effector, AActor* target);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_ForceEndOngoingScoreEvent(EDBDScoreTypes ScoreType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_ForceCancelOngoingScoreEvent(EDBDScoreTypes ScoreType);

	UFUNCTION(BlueprintCallable)
	void Authority_ApplyScoreEvent(EDBDScoreTypes ScoreType, float percentToAward, bool redirectToGameEventSystem);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ADBDPlayerState();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayerState) { return 0; }
