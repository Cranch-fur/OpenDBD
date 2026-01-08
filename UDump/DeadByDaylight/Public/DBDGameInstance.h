#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Templates/SubclassOf.h"
#include "ParadiseData.h"
#include "EPlayerRole.h"
#include "BonusPointsEventData.h"
#include "EDisconnectErrors.h"
#include "FILE_NOT_FOUND"
#include "PlayerDisconnectDelegate.h"
#include "ELegalTermsStatus.h"
#include "UObject/NoExportTypes.h"
#include "DBDGameInstance.generated.h"

class UDBDDesignTunables;
class URegionFinder;
class UGameEventTracker;
class UDBDKeyDisplayInfo;
class UPlayerProfileDAL;
class AProceduralLevelBuilder;
class AOfferingSequenceManager;
class UGameFlowContextSystem;
class UDBDPersistentData;
class UPlayerDataStorageFacade;
class USequencer;
class ULightingHelper;
class UOfferingHandler;
class UActorKnowledgeCollection;
class UMapActorDB;
class UAkAudioBank;
class UInteractionProficiency;
class UDBDHud;
class ULoadAsyncBankData;
class UBloodwebManager;
class UDBDAuthentication;
class UDBDEasyAntiCheat;
class ADBDPlayer;
class UObject;
class AActor;
class APlayerController;
class ULocalPlayer;
class ULightingInterpolator;
class ABaseSky;
class ACamperPlayer;
class ASlasherPlayer;
class ADBDPlayerState;

UCLASS(Blueprintable, NonTransient)
class UDBDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear)
	TSubclassOf<UDBDDesignTunables> DesignTunableClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UDBDDesignTunables* designTunables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear)
	TSubclassOf<URegionFinder> RegionClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	URegionFinder* RegionFinder;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UGameEventTracker* GameEventTracker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDBDKeyDisplayInfo* KeyDisplayInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LeaveGameRequested;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LockOutTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UPlayerProfileDAL* PlayerProfileDAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AProceduralLevelBuilder> Builder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GenerationSeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PaperTileAssetName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TWeakObjectPtr<AOfferingSequenceManager> OfferingSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FParadiseData> ParadiseData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool PlayerProfileLoadFailed;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool SeasonEnded;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FString PendingReportSteamId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FString PendingReportPlayerName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	EPlayerRole PendingReportPlayerRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ReportedPlayers;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	int32 MatchRateValue;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<FBonusPointsEventData> BonusPointsEvents;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool ShouldRequestBonusPointsEvents;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	EDisconnectErrors DisconnectError;

	UPROPERTY(EditInstanceOnly, Transient)
	uint32 LastAuthOrEACErrorCode;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FString AuthOrEACErrorAdditionalInfo;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool HasServerLeftMatch;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	bool HasWrittenGameEndStats;

	UPROPERTY(EditAnywhere)
	FStreamableManager AssetLoader;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FPlayerDisconnectDelegate OnPlayerDisconnect;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UGameFlowContextSystem* _contextSystem;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDPersistentData* _persistentData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UPlayerDataStorageFacade* _playerDataFacade;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	USequencer* _sacrificeSequencer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ULightingHelper* _ligthingHelper;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UOfferingHandler* _offeringHandler;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UActorKnowledgeCollection*> _actorKnowledgeCollections;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UMapActorDB* _mapActorDB;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<UAkAudioBank*> _audioBanks;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<UInteractionProficiency*> _interactionProficiencies;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDHud* _theHud;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ULoadAsyncBankData*> _asyncAudioBanksPendingLoadData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UBloodwebManager* BloodwebManager;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDAuthentication* _auth;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDEasyAntiCheat* _eac;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ADBDPlayer*> _players;

	UPROPERTY(EditInstanceOnly, Transient)
	TArray<TWeakObjectPtr<UObject>> _execObjects;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _networkFailureDetected;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _networkConnectionMonitoringOnly;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _achievementStatsUploadEnabled;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isDisconnected;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isMirrorsSessionKilled;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isGamepadReconnected;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isProfileOffline;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isAppSuspended;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isAppResumed;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isInvitationRejected;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isAdditionalContentInstalled;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ELegalTermsStatus _legalTermsStatus;

public:
	UFUNCTION(BlueprintCallable)
	void StartOfferingSequence();

	UFUNCTION(BlueprintCallable)
	void SimulatePerk(TArray<FName> perks, int32 Level);

	UFUNCTION(BlueprintCallable)
	void SimulateOfferingSequence(const TArray<FName>& Offerings, bool playSequence);

	UFUNCTION(BlueprintCallable)
	void RemoveActorKnowledgeCollection(UActorKnowledgeCollection* collection);

	UFUNCTION(BlueprintPure)
	bool IsActorKnown(ADBDPlayer* knowledgePossessor, AActor* possiblyKnownActor) const;

	UFUNCTION(BlueprintPure)
	USequencer* GetSacrificeSequencer();

	UFUNCTION(BlueprintPure)
	int32 GetMaxSurvivorCount() const;

	UFUNCTION(BlueprintPure)
	UMapActorDB* GetMapActorDB() const;

	UFUNCTION(BlueprintPure)
	APlayerController* GetLocalPlayerController() const;

	UFUNCTION(BlueprintPure)
	ULocalPlayer* GetLocalPlayer() const;

	UFUNCTION(BlueprintPure)
	ULightingInterpolator* GetLightInterpolator();

	UFUNCTION(BlueprintPure)
	ULightingHelper* GetLightingHelper();

	UFUNCTION(BlueprintCallable)
	ABaseSky* GetLevelLighting();

	UFUNCTION(BlueprintPure)
	TArray<ACamperPlayer*> GetInGameSurvivors() const;

	UFUNCTION(BlueprintPure)
	TArray<ADBDPlayer*> GetInGamePlayers() const;

	UFUNCTION(BlueprintPure)
	TArray<ASlasherPlayer*> GetInGameKillers() const;

	UFUNCTION(BlueprintPure)
	int32 GetDeadOrDisconnectedCampersCount(ADBDPlayerState* exception) const;

	UFUNCTION(BlueprintCallable)
	void GenerateBloodWebForCurrentCharacter();

	UFUNCTION(BlueprintCallable)
	static void ForceCloseGame();

	UFUNCTION(BlueprintCallable)
	void FireSoundEvent(AActor* Instigator, const FVector& Location, bool shouldTrack, float audibleRange, ADBDPlayer* instigatingPlayer, bool isQuickAction);

	UFUNCTION(BlueprintCallable)
	void EndLoadingScreen();

	UFUNCTION(BlueprintCallable, Exec)
	void DBDToggleLightingLoaded();

	UFUNCTION(BlueprintCallable, Exec)
	void DBDLogFindSessions(int32 searchType, int32 Rank, bool ignoreVersion);

	UFUNCTION(BlueprintCallable, Exec)
	void DBDForceSave();

	UFUNCTION(BlueprintCallable, Exec)
	void DBDForceLoad();

	UFUNCTION(BlueprintCallable, Exec)
	void DBDDeleteLocalSaveFile();

	UFUNCTION(BlueprintCallable, Exec)
	void DBDApplyOfferingModification(float lightModifier, float fogModifier);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_UpdateSteamInventory();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ToggleOnlineRole();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_TestTickedDiceRoll(float BaseProbability, float ticks, float MultiplicativeModifier, float AdditiveModifier);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_TestSaveFile(const FString& Name);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_TestRegion();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_TestDiceRoll(float BaseProbability, float MultiplicativeModifier, float AdditiveModifier);

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_StartQuickPlay();

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetStatsUploadEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetCursorStickySpeed(float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetCursorStickIgnore(float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetCursorDefaultSpeed(float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetBonusEventsDate(const FString& dateString);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SendFriendInvite();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetInventory();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetCurrentUserStats(bool andUpload, bool andUpdateData);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetCurrentUserStat(FName StatName, bool andUpload);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetAllRituals();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_RemoveRitual(FName toRemove);

	UFUNCTION(BlueprintPure=false, BlueprintCallable, Exec)
	void DBD_PrintAudioConfig() const;

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_LogBloodWebDefinition();

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_LeaveMatch();

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_GotoSplashScreen(bool showDefaultDialogueOnSplashScreen);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_GetRegion();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_GenerateNewRitual(FName Name);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_GenerateNewRandomRitual();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_GenerateBloodWeb(int32 Level);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DumpSessions();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DisplayCurrentUserStat(FName StatName);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DestroySteamInventory();

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ClearInventory();

public:
	UFUNCTION(BlueprintPure=false, BlueprintCallable, Exec)
	void DBD_CheckForNewContent() const;

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ChangeSteamOverlayPosition(int32 Position, int32 VerticalOffset, int32 horizontalOffset);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AssignNewRitual();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AnalyticsTest();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AnalyticsReset();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddToStat(FName StatName, float amount);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddToRitual(int32 Index, float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddSlasherSkulls(int32 Count);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddSkulls(int32 camperSkullsCount, int32 slasherSkullsCount);

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddPerkToInventory(FName perkID);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddOfferingToInventory(FName ItemId);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddItemToInventory(FName ItemId);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddItemAddonToInventory(FName ItemAddonID);

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddFearTokens(int32 Count);

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddEverythingToInventory();

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddCamperSkulls(int32 Count);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddBloodPoints(int32 Count);

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddAllPerksToInventory(int32 amount);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddAllItemsAddonsOfferingsToInventory(int32 amount);

public:
	UFUNCTION(BlueprintCallable)
	void BeginLoadingScreen(const FString& MapName);

	UFUNCTION(BlueprintCallable)
	void AddActorKnowledgeCollection(UActorKnowledgeCollection* collection);

public:
	UDBDGameInstance();
};

FORCEINLINE uint32 GetTypeHash(const UDBDGameInstance) { return 0; }
