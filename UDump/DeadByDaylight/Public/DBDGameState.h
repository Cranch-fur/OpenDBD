#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EscapeDoorActivatedEvent.h"
#include "LevelReadyToPlayEvent.h"
#include "SurvivorsLeftChangedEvent.h"
#include "AllPlayerLoadedEvent.h"
#include "ActivatedGeneratorCountChangedDynamicEvent.h"
#include "BuiltLevelData.h"
#include "OfferingData.h"
#include "ServerJoiningData.h"
#include "UObject/NoExportTypes.h"
#include "GamePresetData.h"
#include "SelectedOffering.h"
#include "Dependency.h"
#include "GameFramework/OnlineReplStructs.h"
#include "DBDGameState.generated.h"

class ASlasherPlayer;
class AClipManager;
class ACamperPlayer;
class AInteractable;
class AActor;
class ADBDPlayerState_Menu;
class ADBDPlayerState;
class ADBDPlayer;
class APawn;

UCLASS(Blueprintable)
class ADBDGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Transient)
	FEscapeDoorActivatedEvent OnEscapeDoorActivated;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FLevelReadyToPlayEvent OnLevelReadyToPlay;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FSurvivorsLeftChangedEvent OnSurvivorsLeftChanged;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintAuthorityOnly)
	FAllPlayerLoadedEvent AuthorityOnAllPlayerLoaded;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FActivatedGeneratorCountChangedDynamicEvent OnActivatedGeneratorCountChangedDynamic;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	ASlasherPlayer* Slasher;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 GeneratorNeeded;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 CamperDeadCount;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 CamperInMeatLockerCount;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 CamperEscaped;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 WaitingForEscape;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 CamperCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LeaveSpectateRequested;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	bool IsGameEnded;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	bool PlayersReadyToStart;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	int32 SecondsLeftInLobby;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	uint8 HasServerLeftTheGame : 1;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_OnLevelLoaded, Transient)
	bool _gameLevelLoaded;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	FBuiltLevelData _builtLevelData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	FOfferingData _levelOfferings;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	AClipManager* _clipManager;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_ServerJoiningData, Transient, meta=(AllowPrivateAccess=true))
	FServerJoiningData _serverJoiningData;

	UPROPERTY(EditInstanceOnly, ReplicatedUsing=OnRep_SessionId, Transient)
	FGuid _sessionId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_ActivatedGeneratorCount, Transient, meta=(AllowPrivateAccess=true))
	int32 _activatedGeneratorCount;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_EscapeDoorActivated, Transient, meta=(AllowPrivateAccess=true))
	bool _escapeDoorActivated;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_HatchOpened, Transient, meta=(AllowPrivateAccess=true))
	bool _isHatchOpen;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_OnLevelReadyToPlay, Transient, meta=(AllowPrivateAccess=true))
	bool _levelReadyToPlay;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	bool _playerDistributionReady;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	bool _usingWeakenedMechanic;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_SurvivorLeft, Transient, meta=(AllowPrivateAccess=true))
	int32 _survivorLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _camperEscapedThroughHatch;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	ACamperPlayer* _obsessionTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _cachedObsessionEscaped;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_GamePresetData, Transient, meta=(AllowPrivateAccess=true))
	FGamePresetData _gamePresetData;

public:
	UFUNCTION(BlueprintCallable)
	void TravelToKillerServer();

	UFUNCTION(BlueprintCallable)
	void SortOfferings();

	UFUNCTION(BlueprintCallable)
	bool ShouldDisplayMapName() const;

	UFUNCTION(BlueprintCallable)
	void SetupGameLoaded();

	UFUNCTION(BlueprintCallable)
	void SetKillerIntroCompletedNormalized(float completedAmount);

	UFUNCTION(BlueprintCallable)
	void SetGameSelectedOffering(const TArray<FSelectedOffering>& Offerings);

	UFUNCTION(BlueprintCallable)
	void SetGameLevelLoaded();

	UFUNCTION(BlueprintCallable)
	void SetDisplayMapName(bool Display);

	UFUNCTION(BlueprintCallable)
	void SetBuiltLevelData(const FName& ThemeName, const FName& ThemeWeather, const FString& MapName, int32 TileCount, const TArray<FDependency>& LevelDependencies);

	UFUNCTION(BlueprintCallable)
	void Server_UpdateGameRole();

	UFUNCTION(BlueprintCallable)
	void ResetGameLevelLoaded();

	UFUNCTION(BlueprintCallable)
	void RemoveTrap(AInteractable* toRemove);

	UFUNCTION(BlueprintCallable)
	void RemoveHeartbeatEmitter(AActor* toRemove);

private:
	UFUNCTION(BlueprintCallable)
	void OnRep_SurvivorLeft(int32 OldValue);

	UFUNCTION()
	void OnRep_SessionId(FGuid OldValue);

	UFUNCTION(BlueprintCallable)
	void OnRep_ServerJoiningData();

	UFUNCTION(BlueprintCallable)
	void OnRep_OnLevelReadyToPlay(bool OldValue);

protected:
	UFUNCTION(BlueprintCallable)
	void OnRep_OnLevelLoaded(bool OldValue);

private:
	UFUNCTION(BlueprintCallable)
	void OnRep_HatchOpened(bool OldValue);

	UFUNCTION(BlueprintCallable)
	void OnRep_GamePresetData();

	UFUNCTION(BlueprintCallable)
	void OnRep_EscapeDoorActivated(bool OldValue);

	UFUNCTION(BlueprintCallable)
	void OnRep_ActivatedGeneratorCount(int32 OldValue);

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_UpdateCharacterFromGamePreset(ADBDPlayerState_Menu* playerState_Menu, FGamePresetData GamePresetData);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetServerLeftGame(bool hasServerLeftGame);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetKillerGoalPoints(FUniqueNetIdRepl PlayerId, int32 killerGoals);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetGameLevelLoaded();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetGameEnded(bool hasServerLeftGame);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetBuiltLevelData(const FName& ThemeName, const FName& ThemeWeather, const FString& MapName, int32 TileCount, const TArray<FDependency>& LevelDependencies);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_KillerServerSearchFailed();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_KillerSearchStarted();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_JoinKillerServerFailed();

	UFUNCTION(BlueprintCallable)
	void KillerServerFound(const FServerJoiningData& joinParams);

	UFUNCTION(BlueprintPure)
	bool IsPlayerDistributionReady() const;

	UFUNCTION(BlueprintCallable)
	bool IsOnePlayerLeft();

	UFUNCTION(BlueprintCallable)
	bool IsOfferingReceived() const;

	UFUNCTION(BlueprintCallable)
	bool IsObsessionTargetAliveInLevel();

	UFUNCTION(BlueprintCallable)
	bool IsObsessionTargetAlive();

	UFUNCTION(BlueprintPure)
	bool IsLightingGenerated() const;

	UFUNCTION(BlueprintCallable)
	bool IsLevelSetupDone() const;

	UFUNCTION(BlueprintPure)
	bool IsLevelReadyToPlay() const;

	UFUNCTION(BlueprintPure)
	bool IsIntroCompleted() const;

	UFUNCTION(BlueprintPure)
	bool IsHatchVisible() const;

	UFUNCTION(BlueprintPure)
	bool IsEscapeDoorOpen() const;

	UFUNCTION(BlueprintPure)
	bool IsEscapeDoorActivated() const;

	UFUNCTION(BlueprintCallable)
	void IntroCompleted();

	UFUNCTION(BlueprintCallable)
	int32 GetSurvivorLeft() const;

	UFUNCTION(BlueprintCallable)
	TArray<FSelectedOffering> GetSelectedOfferings() const;

	UFUNCTION(BlueprintCallable)
	bool GetSelectedOffering(int32 Index, FSelectedOffering& SelectedOffering) const;

	UFUNCTION(BlueprintCallable)
	int32 GetRemainingGeneratorsNeeded();

	UFUNCTION(BlueprintCallable)
	ADBDPlayerState* GetPlayerStateByIDString(const FString& ID) const;

	UFUNCTION(BlueprintPure)
	void GetPlayerRoleCounts(int32& survivorCount, int32& killerCount, int32& SpectatorCount) const;

	UFUNCTION(BlueprintPure)
	int32 GetNumberOfOtherActiveSurvivors(ACamperPlayer* exception) const;

	UFUNCTION(BlueprintPure)
	int32 GetNumberOfActiveSurvivors() const;

	UFUNCTION(BlueprintCallable)
	ADBDPlayerState* GetLocalPlayerState() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetLocalPlayerPawn() const;

	UFUNCTION(BlueprintPure)
	APawn* GetLocalPlayerBasePawn() const;

	UFUNCTION(BlueprintCallable)
	FBuiltLevelData GetLevelData() const;

	UFUNCTION(BlueprintCallable)
	float GetKillerIntroCompletedNormalized();

	UFUNCTION(BlueprintPure)
	int32 GetHeartbeatEmittersCount() const;

	UFUNCTION(BlueprintPure)
	bool GetGameLevelLoaded() const;

	UFUNCTION(BlueprintCallable)
	void Authority_SetSurvivorLeft(int32 survivorRemaining);

	UFUNCTION(BlueprintCallable)
	void Authority_SetServerJoiningData(const FServerJoiningData& joiningData);

	UFUNCTION(BlueprintCallable)
	void Authority_SetLevelReadyToPlay();

	UFUNCTION(BlueprintCallable)
	void Authority_SetHatchOpen(bool OPENED);

	UFUNCTION(BlueprintCallable)
	void Authority_SetAllPlayerLoaded();

	UFUNCTION(BlueprintCallable)
	void Authority_EvaluateObsessionTarget(ADBDPlayer* potentialTarget);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_EscapeThroughHatch();

	UFUNCTION(BlueprintCallable)
	void Authority_EnableObsession();

	UFUNCTION(BlueprintCallable)
	void AddTrap(AInteractable* toAdd);

	UFUNCTION(BlueprintCallable)
	void AddHeartbeatEmitter(AActor* toAdd);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ADBDGameState();
};

FORCEINLINE uint32 GetTypeHash(const ADBDGameState) { return 0; }
