// starts at line 45
ADBDPlayerControllerBase::ADBDPlayerControllerBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) // Inherited from APlayerController
{
    // Initialize base boolean flags
    this->_playerDataRequested = false;
    this->_hasFinishedPlaying = false;
    this->_uploadStatsNeeded = false;

    // Initialize controller scale multipliers to 0.5 (represented as 0x3F000000 in assembly)
    this->_slasherGamepadScale = 0.5f;
    this->_slasherMouseScale = 0.5f;
    this->_camperGamepadScale = 0.5f;
    this->_camperMouseScale = 0.5f;

    // Assign the default cheat manager class.
    // The disassembly call to UDBDCheatManager::GetPrivateStaticClass is standard UE boilerplate 
    // and is replaced with the standard StaticClass() macro function.
    this->CheatClass = UDBDCheatManager::StaticClass();
}

// starts at line 50
void ADBDPlayerControllerBase::PostInitializeComponents()
{
    // Call the parent class implementation.
    Super::PostInitializeComponents();

    // Retrieve the Game Instance and cast it to UDBDGameInstance.
    // The disassembly shows Unreal Engine's fast cast implementation, which compares 
    // ClassTreeIndex against ClassTreeNumChildren to verify the class type.
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Create a new UFearMarketManager object with this controller as its Outer.
    // The disassembly shows the expanded inline code for the NewObject<T> template, 
    // which includes the FObjectInitializer::AssertIfInConstructor check and the 
    // core StaticConstructObject_Internal call.
    this->FearMarketManager = NewObject<UFearMarketManager>(this);

    // If the manager was successfully created, assign the Game Instance.
    if (IsValid(this->FearMarketManager))
    {
        this->FearMarketManager->_gameInstance = gameInstance;
    }
}

// starts at line 62
void ADBDPlayerControllerBase::BeginPlay()
{
    Super::BeginPlay();
    this->RefreshAxisScalingFromSettings();
}

// starts at line 68
void ADBDPlayerControllerBase::BeginDestroy()
{
    // Check if statistics upload is required before destroying the controller.
    if (this->_uploadStatsNeeded == true)
    {
        // Retrieve the Game Instance and cast it to UDBDGameInstance.
        UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        // The disassembly performs a null check and a GUObjectArray pending kill check (testing bit 29).
        // This is standard Unreal Engine boilerplate and is replaced with the IsValid() macro.
        if (IsValid(gameInstance) == true)
        {
            // Accessing an undefined boolean flag at offset 0x558 in UDBDGameInstance.
            if (gameInstance->_initializedFromCloud == true)
            {
                this->_uploadStatsNeeded = false;

                // Accessing an undefined UPlayerProfileDAL pointer at offset 0x120 in UDBDGameInstance.
                // The IDA pseudocode generated an obvious hallucination here as well.
                UPlayerProfileDAL* playerProfileDAL = gameInstance->PlayerProfileDAL;

                if (IsValid(playerProfileDAL) == true)
                {
                    // Call the upload function and proceed with destruction.
                    playerProfileDAL->UploadCurrentPlayerStats();
                    
                    // Proceed to destroy the base actor class.
                    Super::BeginDestroy();
                    return;
                }

                // Log category LogDBDGeneral with verbosity 5 translates to ELogVerbosity::Log.
                // Replaced the FMsg::Logf_Internal__VA disassembly boilerplate with the standard UE_LOG macro.
                UE_LOG(LogDBDGeneral, Log, TEXT("Failed to Upload Current Player Stats during the player controller destruction because the PlayerProfileDAL is invalid."));
            }
            else
            {
                // Replaced the FMsg::Logf_Internal__VA disassembly boilerplate with the standard UE_LOG macro.
                UE_LOG(LogDBDGeneral, Log, TEXT("Failed to Upload Current Player Stats during the player controller destruction because the Game Instance is invalid."));
            }
        }
    }

    // Proceed to destroy the base actor class if we didn't return early.
    Super::BeginDestroy();
}

// starts at line 84
ADBDPlayerState* ADBDPlayerControllerBase::GetDBDPlayerState()
{
    // The disassembly shows Unreal Engine's fast cast implementation, which compares 
    // ClassTreeIndex against ClassTreeNumChildren to safely cast the base object.
    // This is natively handled by the standard Cast<T> macro function.
    ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    if (dbdPlayerState != nullptr)
    {
        return dbdPlayerState;
    }
    else
    {
        return nullptr;
    }
}

// starts at line 97
void ADBDPlayerControllerBase::Reset()
{
    Super::Reset();
}

// starts at line 102
void ADBDPlayerControllerBase::InitPlayerState()
{
    /* Call the base class implementation of InitPlayerState */
    /* The disassembly explicitly calls AController::InitPlayerState */
    Super::InitPlayerState();

    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Verify if this is a local controller before proceeding with analytics initialization */
    if (this->IsLocalController() == true && IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
    {
        /* Load the OnlinePresence Analytics module */
        IOnlinePresencePlugin& OnlinePresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>("OnlinePresence");

        /* Retrieve string value from the module VTable at offset 0xE8 */
        /* Assign the retrieved string to the field in PlayerState located at offset 0x728 */
        DBDPlayerState->MirrorsId = OnlinePresenceModule.GetMirrorsId();
    }
}

// starts at line 112
bool ADBDPlayerControllerBase::Server_SendClientAuthentication_Validate(const FString& authClientTicket, uint64 SteamId, uint32 ticketLength)
{
    return true;
}

// starts at line 113
TSharedPtr<const FUniqueNetId> ADBDPlayerControllerBase::GetUniqueIDForLocalPlayer() const
{
    // A Player is accessed (from APlayerController) and cast to ULocalPlayer.
    // The disassembly shows standard boilerplate (GUObjectArray, ClassTreeIndex checks, 
    // and complex TSharedPtr memory management/reference counting).
    // All of this is cleanly handled by Unreal Engine's standard macros and templates.
    ULocalPlayer* localPlayer = Cast<ULocalPlayer>(this->Player);

    if (IsValid(localPlayer) == true)
    {
        // Return the preferred UniqueNetId directly. The compiler will handle the TSharedPtr copy semantics.
        return localPlayer->GetPreferredUniqueNetId();
    }

    // Return a null/empty TSharedPtr if the local player is invalid.
    return TSharedPtr<const FUniqueNetId>(nullptr);
}

// starts at line 124
void ADBDPlayerControllerBase::Server_BuildProceduralLevelFromMap_Implementation(const FString& map)
{
    // Get the Game Instance and cast it to UDBDGameInstance.
    // The disassembly shows Unreal Engine's fast cast implementation checking ClassTreeIndex.
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Call GetWorld() from the current object's VTable (offset 0x108).
    UWorld* world = this->GetWorld();

    if (world != nullptr)
    {
        // Access AuthorityGameMode (offset 0xF0 in UWorld). 
        // Using the standard Unreal Engine getter for safety and clarity.
        AGameMode* authorityGameMode = world->GetAuthGameMode();

        // The disassembly performs multiple checks on the GUObjectArray to verify that both 
        // the GameMode and the GameInstance are valid and not marked as pending kill. 
        // These checks are replaced with the standard Unreal Engine IsValid() macro.
        if (IsValid(authorityGameMode) == true && authorityGameMode->IsActorBeingDestroyed() == false && IsValid(gameInstance) == true)
        {
            // The disassembly calls TArray<ushort>::operator=, which corresponds to the internal 
            // array assignment of an FString (since TCHAR is ushort).
            // It assigns the 'map' string to a field at offset 0x188 within UDBDGameInstance.
            gameInstance->PaperTileAssetName = map;
        }
    }
}

// starts at line 138
void ADBDPlayerControllerBase::Server_BuildProceduralLevelFromSeed_Implementation(int32_t seed)
{
    // Retrieve the Game Instance and cast it to UDBDGameInstance.
    // The disassembly shows Unreal Engine's fast cast implementation checking ClassTreeIndex.
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Call GetWorld() from the current object's VTable.
    UWorld* world = this->GetWorld();

    if (world != nullptr)
    {
        // Access AuthorityGameMode (offset 0xF0 in UWorld). 
        // Using the standard Unreal Engine getter for safety and clarity.
        AGameMode* authorityGameMode = world->GetAuthGameMode();

        // The disassembly performs multiple checks on the GUObjectArray to verify that both 
        // the GameMode and the GameInstance are valid and not marked as pending kill (checking bit 29). 
        // These checks are replaced with the standard Unreal Engine IsValid() macro.
        if (IsValid(authorityGameMode) == true && authorityGameMode->IsActorBeingDestroyed() == false && IsValid(gameInstance) == true)
        {
            // The disassembly assigns the 'seed' integer to a field at offset 0x180 within UDBDGameInstance.
            gameInstance->GenerationSeed = seed;
        }
    }
}

// starts at line 152
void ADBDPlayerControllerBase::OnRep_PlayerState()
{
    // Call the base class implementation to handle standard player state replication
    Super::OnRep_PlayerState();

    // Check if the player data has already been requested
    if (this->_playerDataRequested == true)
    {
        // Attempt to send the player data locally since the player state is now available
        this->Local_TrySendPlayerData();
    }
}

// starts at line 162
void ADBDPlayerControllerBase::Client_RequestPlayerProfile_Implementation(bool fromOfflineLobby)
{
    // Set the internal flag indicating that player data has been requested
    this->_playerDataRequested = true;

    // Get the Game Instance and attempt to cast it to UDBDGameInstance.
    // The disassembly shows double calls to GetGameInstance and fast cast class checks, 
    // which are standardly simplified to a single Cast call.
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Verify the game instance is valid and not pending kill (standard IsValid check replaces GUObjectArray checks)
    if (IsValid(gameInstance) == true)
    {
        // Check if the player data needs to be reset based on the offline lobby state
        if (gameInstance->ShouldResetPlayerData(fromOfflineLobby) == true)
        {
            // Access the _playerDataFacade pointer at offset 0x3C0 in UDBDGameInstance.
            gameInstance->_playerDataFacade->ForceReloadProfile();
        }
    }

    // If any check fails, or if a reset is not needed, try to send the local player data.
    // The disassembly uses a tail call optimization (jmp) to this function at the end.
    this->Local_TrySendPlayerData();
}

// starts at line 177
void ADBDPlayerControllerBase::Local_TrySendPlayerData()
{
    /* Check if this controller is locally controlled before proceeding */
    if (this->IsLocalController() == false)
    {
        return;
    }

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true && IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
    {
        /* Retrieve the persistent data for the local player */
        FPlayerPersistentData* LocalPlayerPersistentData = GameInstance->GetLocalPlayerPersistentData();
        
        if (LocalPlayerPersistentData != nullptr)
        {
            /* Mark that the player data has been requested */
            this->_playerDataRequested = false;

            /* Load the OnlinePresence module */
            IOnlinePresencePlugin& OnlinePresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>("OnlinePresence");

            /* Retrieve a string value from the module VTable */
            /* Assign the retrieved string to a field in PlayerState located at offset 0x728 */
            DBDPlayerState->MirrorsId = OnlinePresenceModule.GetMirrorsId();

            /* Copy loadout and shared data from the persistent data */
            FPlayerLoadoutData PlayerLoadoutData(LocalPlayerPersistentData->CurrentLoadout);
            FPlayerSavedProfileDataShared PlayerSharedData(LocalPlayerPersistentData->SavedData.SharedData);

            /* Send the data to the server via the Server_ReceivePlayerProfile RPC */
            this->Server_ReceivePlayerProfile(OnlinePresenceModule.GetMirrorsId(), PlayerSharedData, PlayerLoadoutData);
        }
    }
}

// starts at line 196
void ADBDPlayerControllerBase::Server_ReceivePlayerProfile_Implementation(const FString& mirrorsId, FPlayerSavedProfileDataShared savedProfileData, FPlayerLoadoutData playerLoadout)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Create a copy of the PlayerState's UniqueNetId */
        FUniqueNetIdRepl UniqueNetIdCopy = this->PlayerState->UniqueId;

        /* Create copies of the profile and loadout data to pass to the GameInstance */
        FPlayerLoadoutData LoadoutDataCopy(playerLoadout);
        FPlayerSavedProfileDataShared ProfileDataCopy(savedProfileData);

        /* Pass the received data to the authority GameInstance */
        GameInstance->AuthorityReceivePlayerData(UniqueNetIdCopy, ProfileDataCopy, LoadoutDataCopy);
    }

    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    if (IsValid(DBDPlayerState) == true)
    {
        /* Assign the incoming mirrorsId string to a field in PlayerState located at offset 0x728 */
        DBDPlayerState->MirrorsId = mirrorsId;
    }
}

// starts at line 211
bool ADBDPlayerControllerBase::Server_ReceivePlayerProfile_Validate(const FString& mirrorsId, FPlayerSavedProfileDataShared savedProfileData, FPlayerLoadoutData playerLoadout)
{
    return true;
}

// starts at line 216
void ADBDPlayerControllerBase::Authority_FinishedPlaying()
{
    /* ROLE_Authority corresponds to 3 in ENetRole enum */
    if (this->Role == ROLE_Authority)
    {
        /* Use Unreal Engine Cast macro to safely cast the PlayerState */
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        TArray<FBadgeAnalyticEntry> analyticsBadgeData;

        /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
        /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
        if (IsValid(DBDPlayerState) == true)
        {
            /* Retrieve the badge evaluator component from the player state */
            UDBDBadgeEvaluatorComponent* BadgeEvaluator = DBDPlayerState->BadgeEvaluator;

            if (BadgeEvaluator != nullptr)
            {
                /* Get all badges managed by the evaluator */
                const TArray<UDBDBadge*>& AllBadges = BadgeEvaluator->Authority_GetAllBadges();

                /* Iterate over all badges to populate the analytics array */
                for (int32 i = 0; i < AllBadges.Num(); ++i)
                {
                    UDBDBadge* Badge = AllBadges[i];
                    
                    if (Badge != nullptr)
                    {
                        FBadgeAnalyticEntry entry;
                        
                        /* Accessing an FName property on the badge (offset + 28h from disassembly, probably BadgeID or Name) */
                        /* converting it to string to be used as a key in the analytic entry */
                        entry.Key = Badge->BadgeID.ToString();
                        entry.Value = Badge->GetProgress();
                        
                        /* Add the populated entry to our local array */
                        analyticsBadgeData.Add(entry);
                    }
                }
            }
        }
        
        /* Send the aggregated badge data to the client */
        this->Client_FinishedPlaying(analyticsBadgeData);
    }
}

// starts at line 243
void ADBDPlayerControllerBase::Client_FinishedPlaying_Implementation(const TArray<FBadgeAnalyticEntry>& badgeData)
{
    this->Local_FinishedPlaying_Internal(badgeData);
}

// starts at line 248
void ADBDPlayerControllerBase::Local_FinishedPlaying_Internal(const TArray<FBadgeAnalyticEntry>* badgeData)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        /* Explicitly call the function on the game instance */
        GameInstance->OnLeavingOnlineMultiplayer();

        /* Verify that this is a local controller and it hasn't already finished playing */
        if (this->_hasFinishedPlaying == false && this->IsLocalController() == true)
        {
            ADBDGameState* GameState = GameInstance->GetGameState();

            if (IsValid(GameState) == true && GameState->IsActorBeingDestroyed() == false)
            {
                /* Access the GameState session ID components to ensure it's valid */
                /* The disassembly checks if any of the components of the FGuid are non-zero */
                if (GameState->_sessionId.IsValid())
                {
                    /* Perform local stats writing */
                    this->Local_WriteGameEndStats();

                    ADBDPlayer* Player = Cast<ADBDPlayer>(this->Pawn);
                    if (IsValid(Player) == true && Player->IsActorBeingDestroyed() == false && Player->IsValidImpl())
                    {
                        Player->OnFinishedPlaying();
                    }

                    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
                    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
                    
                    if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
                    {
                        /* Flush any pending score analytics before processing final stats */
                        DBDPlayerState->FlushScoreAnalyticsToServer();
                        this->_hasFinishedPlaying = true;
                        
                        /* Calculate match duration and fetch match GUID string */
                        float Duration = GameInstance->_persistentData->_gamePersistentData.SessionInfos.GameSeconds;
                        FString MatchIdStr = GameState->_sessionId.ToString(EGuidFormats::Digits);
                        
                        /* Reconstruct the player role and game type based on the struct offsets and field analysis */
                        EPlayerRole PlayerRole = DBDPlayerState->GameRole;
                        EGameType GameType = GameInstance->_persistentData->_gamePersistentData.SessionInfos.GameType;

                        /* Initialize the base report with match details */
                        GameInstance->_analyticsManager.ReportAnalytics.InitializeReport(PlayerRole, GameType, MatchIdStr);

                        /* Use Unreal Engine Cast macro to safely cast the Character to CamperPlayer */
                        ACamperPlayer* CamperPlayer = Cast<ACamperPlayer>(this->Character);

                        /* Check if the player is a survivor (CamperPlayer) */
                        if (IsValid(CamperPlayer) == true && CamperPlayer->IsActorBeingDestroyed() == false && CamperPlayer->IsValidImpl())
                        {
                            /* Reconstruct the logic for fetching rewarded badges for the survivor */
                            UDBDBadgeEvaluatorComponent* BadgeEvaluator = DBDPlayerState->BadgeEvaluator;
                            
                            if (BadgeEvaluator != nullptr)
                            {
                                UDBDBadgeRewardData RewardBadgeData;
                                BadgeEvaluator->Local_GetRewardedBadges(RewardBadgeData);
                                
                                /* Reconstruct disconnect flag based on reference controller count from unique net id */
                                bool bDisconnected = DBDPlayerState->PlayerData._playerGameState == EGameState::VE_Disconnected ||
                                                     DBDPlayerState->PlayerData._playerGameState == EGameState::VE_ManuallyLeftMatch;
                                
                                /* Fetch stats without forcing an update */
                                const FUserGameStats& UserStats = DBDPlayerState->GetUserGameStats(false);
                                
                                FPostGame_SurvivorAnalytics SurvivorAnalytics(
                                    CamperPlayer, 
                                    bDisconnected, 
                                    UserStats, 
                                    RewardBadgeData.PrimaryBadge, 
                                    RewardBadgeData.SecondaryBadge, 
                                    Duration, 
                                    MatchIdStr);
                                
                                UBHVRAnalytics::AddTableRow<FPostGame_SurvivorAnalytics>(&SurvivorAnalytics);
                            }
                            
                            /* Send the analytics event */
                            GameInstance->_analyticsManager.GameplaySurvivorAnalytics.SendAnalytics(MatchIdStr);
                            
                            TArray<FAnalyticsEventAttribute> Attributes;
                            
                            /* Determine the end state string for the survivor based on their condition */
                            FString EndStateStr;
                            if (CamperPlayer->IsDrainTimerExpired() == true || CamperPlayer->IsHooked() == true)
                            {
                                EndStateStr = TEXT("Sacrificed");
                            }
                            else if (CamperPlayer->IsDyingTimerExpired() == true)
                            {
                                EndStateStr = TEXT("BledOut");
                            }
                            else if (CamperPlayer->IsEscaped() == true)
                            {
                                EndStateStr = TEXT("Escaped");
                            }
                            else
                            {
                                EndStateStr = TEXT("Murdered");
                            }
                            UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("SurvivorEndState"), EndStateStr);
                            
                            /* Include damage state */
                            ECamperDamageState DamageState = CamperPlayer->GetDamageState();
                            UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("DamageState"), Enum::ToString(DamageState));
                            
                            /* Include hooked count */
                            int32 HookCount = CamperPlayer->GetHookedCount();
                            UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("HookedCount"), HookCount);
                            
                            /* Attempt to determine which map tile the survivor ended on if they were inside a tile matrix */
                            /* Retrieve the tile matrix from the GameInstance builder */
                            UTileMatrix* TileMatrix = Cast<UTileMatrix>(GameInstance->Builder->_tileMatrix);
                            if (TileMatrix != nullptr)
                            {
                                UProceduralGenerationData* ProceduralGenerationData = GameInstance->Builder->ProceduralGenerationData;

                                USceneComponent* RootComp = CamperPlayer->GetRootComponent();
                                if (RootComp != nullptr)
                                {
                                    FVector Location = RootComp->GetComponentLocation();
                                    
                                    /* Normalize the location using tile size properties found in the disassembly offsets 236, 240 */
                                    int32 TileX = static_cast<int32>(Location.X / ProceduralGenerationData->ProceduralTileWidth);
                                    int32 TileY = static_cast<int32>(Location.Y / ProceduralGenerationData->ProceduralTileHeight);
                                    
                                    if (TileMatrix->ValidMatrixTile(TileX, TileY) == true)
                                    {
                                        ATile* EndedTile = TileMatrix->GetTile(TileX, TileY);
                                        if (IsValid(EndedTile) == true)
                                        {
                                            FString TileNameStr = EndedTile->GetFName().ToString();
                                            UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("SurvivorEndedOnTile"), TileNameStr);
                                        }
                                        else
                                        {
                                            UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("SurvivorEndedOnTile"), TEXT("OutsideOfGame"));
                                        }
                                    }
                                    else
                                    {
                                        UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("SurvivorEndedOnTile"), TEXT("OutsideOfGame"));
                                    }
                                }
                            }
                            
                            /* Retrieve and populate specific badge analytics attributes */
                            UDBDBadgeEvaluatorComponent* BadgeEvaluatorForAttr = DBDPlayerState->BadgeEvaluator;
                            if (BadgeEvaluatorForAttr != nullptr)
                            {
                                UDBDBadgeRewardData RewardBadgeDataAttr;
                                BadgeEvaluatorForAttr->Local_GetRewardedBadges(RewardBadgeDataAttr); /* UNDEFINED ELEMENT */
                                
                                if (RewardBadgeDataAttr.PrimaryBadge != nullptr)
                                {
                                    FString PrimaryBadgeStr = RewardBadgeDataAttr.PrimaryBadge->ID == FName("_EMPTY_") ? NAME_None : RewardBadgeDataAttr.PrimaryBadge->ID.ToString();
                                    UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("badge_first"), PrimaryBadgeStr);
                                }
                                
                                if (RewardBadgeDataAttr.SecondaryBadge != nullptr)
                                {
                                    FString SecondaryBadgeStr = RewardBadgeDataAttr.SecondaryBadge->ID == FName("_EMPTY_") ? NAME_None : RewardBadgeDataAttr.SecondaryBadge->ID.ToString();
                                    UBHVRAnalytics::AddAttributes<>(Attributes, TEXT("badge_second"), SecondaryBadgeStr);
                                }
                            }

                            /* Record progress for all badges provided in badgeData */
                            for (int32 i = 0; i < badgeData.Num(); ++i)
                            {
                                const FBadgeAnalyticEntry& Entry = badgeData[i];
                                
                                UBHVRAnalytics::AddAttributes<>(Attributes, *Entry.Key, Entry.Value);

                                /* Create the progression analytic event */
                                FBadgeProgressionAnalytics ProgressionEvent;
                                ProgressionEvent.MatchId = MatchIdStr;
                                ProgressionEvent.BadgeId = Entry.Key;
                                ProgressionEvent.BadgeValue = Entry.Value;
                                
                                /* Get the online presence plugin to fetch platform and client details */
                                IOnlinePresencePlugin& PresencePlugin = IOnlinePresencePlugin::Get();
                                ProgressionEvent.ClientId = PresencePlugin.GetPlatformAccountId(MatchIdStr);
                                ProgressionEvent.Platform = UDBDAnalytics::GetPlatformName();

                                UBHVRAnalytics::AddTableRow<FBadgeProgressionAnalytics>(&ProgressionEvent);
                            }

                            /* Finalize survivor analytics attributes and send GameEnded event */
                            DBDPlayerState->AddDataForAnalytics(Attributes);
                            UDBDAnalytics::RecordClientProgress(TEXT("GameEnded"), Attributes);
                            UDBDAnalytics::SetAsPostGame();
                        }
                        else if (PlayerRole == EPlayerRole::VE_Slasher) /* Check if player is a killer */
                        {
                            UDBDBadgeEvaluatorComponent* BadgeEvaluatorKiller = DBDPlayerState->BadgeEvaluator;
                            
                            if (BadgeEvaluatorKiller != nullptr)
                            {
                                UDBDBadgeRewardData RewardBadgeDataKiller;
                                BadgeEvaluatorKiller->Local_GetRewardedBadges(RewardBadgeDataKiller);
                                
                                /* Fetch stats without forcing an update */
                                const FUserGameStats& UserStatsKiller = DBDPlayerState->GetUserGameStats(false);
                                
                                FPostGame_KillerAnalytics KillerAnalytics(
                                    GameInstance->_persistentData->_cachedPlayerScoreMap, 
                                    UserStatsKiller, 
                                    RewardBadgeDataKiller.PrimaryBadge, 
                                    RewardBadgeDataKiller.SecondaryBadge, 
                                    Duration, 
                                    MatchIdStr);
                                    
                                UBHVRAnalytics::AddTableRow<FPostGame_KillerAnalytics>(&KillerAnalytics);
                            }
                            
                            /* Send the analytics event */
                            GameInstance->_analyticsManager.GameplayKillerAnalytics.SendAnalytics(Duration, MatchIdStr);
                        }
                    }
                }
            }
        }
    }
}

// starts at line 409
void ADBDPlayerControllerBase::Client_GameEnded_Implementation()
{
    this->Local_FinishedPlaying_Internal(nullptr);
}

// starts at line 415
void ADBDPlayerControllerBase::Server_UpdateReplicatedSkulls_Implementation(int32 skullsToAdd)
{
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(DBDPlayerState) == true)
    {
        /* Get the player role from the player state */
        EPlayerRole PlayerRole = DBDPlayerState->GameRole;

        /* If the player is a Killer (VE_Slasher = 1) */
        if (PlayerRole == EPlayerRole::VE_Slasher)
        {
            /* Add skulls to the killer's skull count */
            DBDPlayerState->SlasherData.SlasherSkulls += skullsToAdd;
        }
        /* If the player is a Survivor (VE_Camper = 2) */
        else if (PlayerRole == EPlayerRole::VE_Camper)
        {
            /* Add skulls to the survivor's skull count */
            DBDPlayerState->CamperData.CamperSkulls += skullsToAdd;
        }
    }
}

// starts at line 436
void ADBDPlayerControllerBase::SetUploadStatNeeded(bool value)
{
    this->_uploadStatsNeeded = value;
}

// starts at line 441
void ADBDPlayerControllerBase::Tick(float DeltaTime)
{
    /* Call the base class tick function */
    Super::Tick(DeltaTime);

    /* Check if stats upload is needed */
    if (this->_uploadStatsNeeded == false)
    {
        return;
    }

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Check if the game instance has been initialized from the cloud */
        if (GameInstance->_initializedFromCloud == true)
        {
            /* Get the local player controller and optionally cast it, though the cast result is unused in assembly */
            APlayerController* LocalPlayerController = GameInstance->GetLocalPlayerController();
            ADBDPlayerControllerBase* DBDLocalPlayerController = Cast<ADBDPlayerControllerBase>(LocalPlayerController);

            /* Check if any stat operation (upload, read, init) is already in progress */
            /* If handles are valid, an operation is ongoing */
            if (this->_statUploadHandle.IsValid() == false && this->_statReadHandle.IsValid() == false && this->_statInitHandle.IsValid() == false)
            {
                /* Reset the flag since we are initiating the upload */
                this->_uploadStatsNeeded = false;

                /* Bind the callback function for when the stat upload completes */
                /* The disassembly shows a call to AddUObject on a multicast delegate */
                this->_statUploadHandle = GameInstance->PlayerProfileDAL->OnUserStatUploadComplete.AddUObject(this, &ADBDPlayerControllerBase::Local_ProfileStatsPostUpload);

                /* Trigger the upload process on the DAL */
                GameInstance->PlayerProfileDAL->UploadCurrentPlayerStats();
            }
        }
    }
}

// starts at line 461
void ADBDPlayerControllerBase::SetupInputComponent()
{
    /* Call the base class implementation of SetupInputComponent */
    Super::SetupInputComponent();

    /* Load configuration for the PlayerInput component */
    this->PlayerInput->LoadConfig();

    /* Array to keep track of action mappings that are already supported/configured */
    TArray<FName> supportedMappings;

    /* Iterate through the existing ActionMappings on the PlayerInput */
    for (int32 i = 0; i < this->PlayerInput->ActionMappings.Num(); ++i)
    {
        const FInputActionKeyMapping& Mapping = this->PlayerInput->ActionMappings[i];
        
        /* If the action name isn't already in the supported list, add it */
        supportedMappings.AddUnique(Mapping.ActionName);
    }

    /* Get the Default Object for UInputSettings to access the engine's default input configuration */
    const UInputSettings* DefaultInputSettings = GetDefault<UInputSettings>();

    if (DefaultInputSettings != nullptr)
    {
        /* Iterate through the default engine action mappings */
        const TArray<FInputActionKeyMapping>& DefaultActionMappings = DefaultInputSettings->GetActionMappings();
        for (int32 i = 0; i < DefaultActionMappings.Num(); ++i)
        {
            const FInputActionKeyMapping& DefaultMapping = DefaultActionMappings[i];

            /* If the default mapping action name is not currently supported by the player's input */
            /* Add it to the PlayerInput to ensure all default actions are available */
            if (supportedMappings.Contains(DefaultMapping.ActionName) == false)
            {
                this->PlayerInput->AddActionMapping(DefaultMapping);
            }
        }
    }
}

// starts at line 488
void ADBDPlayerControllerBase::Local_InitMirrorCurrencies()
{
    /* Construct a delegate bound to the callback function for receiving currency balances */
    TBaseDelegate<void, bool, const FString&, int32> getCurrencyDelegate;
    getCurrencyDelegate.BindUObject(this, &ADBDPlayerControllerBase::Local_MirrorsGetCurrencyComplete);
    
    /* Store the handle of the delegate so it can be tracked or removed later */
    this->_getCurrencyHandle = getCurrencyDelegate.GetHandle();

    /* Access the Online Presence subsystem via the IOnlinePresencePlugin interface */
    TSharedRef<IDataStorePresenceSubsystem> DataStoreSubsystem = IOnlinePresencePlugin::DataStore();

    /* Request the current balance for Bloodpoints (DBDMirrorsSaveConstants::MIRRORS_CURRENCY_BLOODPOINTS) */
    /* The subsystem will invoke the bound delegate once the request completes */
    DataStoreSubsystem->GetCurrencyBalance(DBDMirrorsSaveConstants::MIRRORS_CURRENCY_BLOODPOINTS, getCurrencyDelegate);
}

// starts at line 496
void ADBDPlayerControllerBase::Local_MirrorsGetCurrencyComplete(bool success, const FString& responseMessage, int32 balanceValue)
{
    /* Invalidate the handle since the callback has been executed */
    this->_getCurrencyHandle.Invalidate();

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        if (success == false)
        {
            /* If the request failed, log a warning and broadcast failure */
            UE_LOG(GameFlow, Warning, TEXT("[GetCurrencyComplete] Failed to get currency from wallet: %s"), *responseMessage);
            
            this->OnMirrorCurrenciesLoadComplete.Broadcast(false);
            return;
        }

        if (balanceValue <= 0)
        {
            /* If balance is 0 or less, nothing to withdraw, broadcast success */
            this->OnMirrorCurrenciesLoadComplete.Broadcast(true);
            return;
        }

        /* If there are bloodpoints to withdraw, initiate the withdrawal */
        /* Set up the message describing the reason for withdrawal */
        FString ReasonMessage = TEXT("Available bloodpoints");

        /* Construct a delegate bound to the callback function for completing the withdrawal */
        /* The disassembly shows binding to Local_MirrorsWithdrawComplete */
        TBaseDelegate<void, bool, const FString&, int32> withdrawDelegate;
        withdrawDelegate.BindUObject(this, &ADBDPlayerControllerBase::Local_MirrorsWithdrawComplete, balanceValue);

        /* Access the Online Presence subsystem via the IOnlinePresencePlugin interface */
        TSharedRef<IDataStorePresenceSubsystem> DataStoreSubsystem = IOnlinePresencePlugin::DataStore();

        /* Execute the withdrawal request on the subsystem */
        DataStoreSubsystem->WithdrawCurrency(DBDMirrorsSaveConstants::MIRRORS_CURRENCY_BLOODPOINTS, balanceValue, ReasonMessage, withdrawDelegate);
    }
}

// starts at line 529
void ADBDPlayerControllerBase::Local_MirrorsWithdrawComplete(bool success, const FString& responseMessage, int32 balanceValue, int32 amountWithdrawn)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        if (success == true)
        {
            /* If the withdrawal was successful, fetch the local player's persistent data */
            FPlayerPersistentData* LocalPlayerPersistentData = GameInstance->GetLocalPlayerPersistentData();
            
            if (LocalPlayerPersistentData != nullptr)
            {
                /* Add the withdrawn amount (passed as amountWithdrawn parameter based on the stack frame analysis) */
                LocalPlayerPersistentData->AddBonusExperience(amountWithdrawn);
                
                /* Save the profile since currency/experience was modified */
                /* The false parameter indicates this is not a forced/priority save */
                GameInstance->_playerDataFacade->SaveFullProfile(false);
            }

            /* Log if there is still balance remaining after withdrawal */
            if (balanceValue > 0)
            {
                UE_LOG(GameFlow, Warning, TEXT("[WithdrawComplete] Balance still available from wallet."));
            }

            /* Broadcast success to any listeners */
            this->OnMirrorCurrenciesLoadComplete.Broadcast(true);
        }
        else
        {
            /* If the withdrawal failed, log the failure and the response message */
            UE_LOG(GameFlow, Warning, TEXT("[WithdrawComplete] Failed to withdraw from wallet: %s"), *responseMessage);

            /* Broadcast failure to any listeners */
            this->OnMirrorCurrenciesLoadComplete.Broadcast(false);
        }
    }
}

// starts at line 566
void ADBDPlayerControllerBase::DBD_SetFearMarketDate(FString dateString)
{
    /* Parse the ISO 8601 string into the FDateTime object */
    FDateTime date;
    FDateTime::ParseIso8601(*dateString, date);
    
    /* Bind the ReceivedFearMarketData callback to the FearMarketManager's delegate */
    /* The disassembly explicitly constructs the FName for the function using string manipulation */
    this->FearMarketManager->OnFearMarketDataReceived.AddUniqueDynamic(this, &ADBDPlayerControllerBase::ReceivedFearMarketData);
    
    /* Apply the parsed override date to the Fear Market Manager */
    this->FearMarketManager->_fearMarketOverrideDate = date;
    
    /* If the manager doesn't already have an active time handle, bind the generate function */
    if (this->FearMarketManager->_getTimeHandle.IsValid() == false)
    {
        /* Bind the GenerateNewFearMarket function to the global OnTimeReceivedDelegate */
        this->FearMarketManager->_getTimeHandle = UDBDTimeUtilities::OnTimeReceivedDelegate.AddUObject(this->FearMarketManager, &UFearMarketManager::GenerateNewFearMarket);
    }
    
    /* Trigger a network time update request to process the changes */
    UDBDTimeUtilities::GetNetDateTime();
}

// starts at line 576
void ADBDPlayerControllerBase::RegenerateFearMarketData()
{
    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(this->FearMarketManager) == true)
    {
        /* Bind the ReceivedFearMarketData callback to the FearMarketManager's delegate */
        /* The disassembly explicitly constructs the FName for the function using string manipulation */
        this->FearMarketManager->OnFearMarketDataReceived.AddUniqueDynamic(this, &ADBDPlayerControllerBase::ReceivedFearMarketData);
        
        /* If the manager doesn't already have an active time handle, bind the generate function */
        if (this->FearMarketManager->_getTimeHandle.IsValid() == false)
        {
            /* Bind the GenerateNewFearMarket function to the global OnTimeReceivedDelegate */
            this->FearMarketManager->_getTimeHandle = UDBDTimeUtilities::OnTimeReceivedDelegate.AddUObject(this->FearMarketManager, &UFearMarketManager::GenerateNewFearMarket);
        }
        
        /* Trigger a network time update request to process the changes */
        UDBDTimeUtilities::GetNetDateTime();
    }
}

// starts at line 585
void ADBDPlayerControllerBase::ReceivedFearMarketData(bool success, FFearMarketOfferingInstance fearMarketData)
{
    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(this->FearMarketManager) == true)
    {
        /* Remove the bound callback for receiving fear market data */
        /* The disassembly manually extracts the function name using string manipulation */
        this->FearMarketManager->OnFearMarketDataReceived.RemoveDynamic(this, &ADBDPlayerControllerBase::ReceivedFearMarketData);
    }

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        if (success == true)
        {
            /* Use Unreal Engine Cast macro to safely cast the PlayerState */
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

            if (IsValid(DBDPlayerState) == true)
            {
                /* Retrieve the player's persistent data using their UniqueNetId */
                /* Create a copy of the UniqueNetId to pass to the function */
                FUniqueNetIdRepl UniqueIdCopy = DBDPlayerState->GetUniqueId();
                
                /* Call GetPlayerPersistentData on the game instance's PersistentData property */
                FPlayerPersistentData* PersistentData = GameInstance->_persistentData->GetPlayerPersistentData(UniqueIdCopy);
                
                if (PersistentData != nullptr)
                {
                    /* Copy the received fear market data into the player's persistent local data */
                    PersistentData->SavedData.LocalData.FearMarket.CopyData(fearMarketData);
                }
            }
        }
    }

    /* Broadcast the success state to the UI delegate */
    /* The disassembly typo 'OnFearMerketDataRecievedUI' is preserved from the struct definition */
    this->OnFearMerketDataRecievedUI.Broadcast(success);
}

// starts at line 605
void ADBDPlayerControllerBase::Local_InitProfileStats()
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Access the PlayerProfileDAL from the GameInstance to bind the callback */
        UPlayerProfileDAL* ProfileDAL = GameInstance->PlayerProfileDAL;

        if (ProfileDAL != nullptr)
        {
            /* Bind the Local_ProfileStatsPostInitialization method to the OnUserStatInitializationComplete delegate */
            this->_statInitHandle = ProfileDAL->OnUserStatReadComplete.AddUObject(this, &ADBDPlayerControllerBase::Local_ProfileStatsPostInitialization);
        }
    }

    /* Chain the call to load the profile stats immediately after initialization setup */
    this->Local_LoadProfileStats();
}

// starts at line 616
void ADBDPlayerControllerBase::Local_LoadProfileStats()
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        /* Get the local player's unique ID as a shared pointer */
        TSharedPtr<const FUniqueNetId> localPlayerUniqueId = this->GetUniqueIDForLocalPlayer();

        if (localPlayerUniqueId.IsValid() == true)
        {
            /* Check if we haven't already bound the read callback */
            if (this->_statReadHandle.IsValid() == false)
            {
                /* Access the PlayerProfileDAL from the GameInstance to bind the callback */
                UPlayerProfileDAL* ProfileDAL = GameInstance->PlayerProfileDAL;

                if (ProfileDAL != nullptr)
                {
                    /* Bind the Local_LoadProfileStatsPostRead method to the OnUserStatReadComplete delegate */
                    this->_statReadHandle = ProfileDAL->OnUserStatReadComplete.AddUObject(this, &ADBDPlayerControllerBase::Local_LoadProfileStatsPostRead);
                }
            }

            /* Request the full player stats from the DAL using the unique ID */
            /* Convert the shared pointer to a shared ref, as expected by RequestFullPlayerStats */
            if (GameInstance->PlayerProfileDAL != nullptr)
            {
                GameInstance->PlayerProfileDAL->RequestFullPlayerStats(localPlayerUniqueId.ToSharedRef());
            }
            return;
        }
    }

    /* If we failed to get the game instance or the local player ID, broadcast failure */
    this->OnCloudLoadStatsComplete.Broadcast(false);
}

// starts at line 638
void ADBDPlayerControllerBase::Local_LoadProfileStatsPostRead(bool bWasSuccessful)
{
    bool bResultSuccess = false;

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        if (bWasSuccessful == true)
        {
            /* Get the local player's unique ID as a shared pointer */
            TSharedPtr<const FUniqueNetId> localPlayerUniqueId = this->GetUniqueIDForLocalPlayer();

            if (localPlayerUniqueId.IsValid() == true)
            {
                /* Access the PlayerProfileDAL and PersistentData from the GameInstance */
                /* Disassembly maps these to specific offsets within GameInstance or its members */
                UPlayerProfileDAL* ProfileDAL = GameInstance->PlayerProfileDAL;
                UDBDPersistentData* PersistentData = GameInstance->_persistentData;
                
                if (ProfileDAL != nullptr && PersistentData != nullptr)
                {
                    /* Get current stats for Killer and Camper skulls and store them in PersistentData */
                    /* Offset analysis from disassembly suggests these are stored at specific offsets inside PersistentData */
                    int32 KillerSkulls = 0;
                    ProfileDAL->GetCurrentPlayerStat(DBDProfileStats::DBD_KILLER_SKULLS, KillerSkulls);
                    PersistentData->_localPlayerPersistentData.SavedData.SharedData.SlasherSkulls = KillerSkulls;
                    
                    int32 CamperSkulls = 0;
                    ProfileDAL->GetCurrentPlayerStat(DBDProfileStats::DBD_CAMPER_SKULLS, CamperSkulls);
                    PersistentData->_localPlayerPersistentData.SavedData.SharedData.CamperSkulls = CamperSkulls;

                    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
                    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

                    /* Use UDBDPersistentData to copy local cache data to the player state */
                    /* The disassembly uses an offset of 0x168 for the source data within PersistentData */
                    UDBDPersistentData::CopyLocalCacheData(&PersistentData->_localPlayerPersistentData, DBDPlayerState);

                    /* Log the stat read action */
                    this->LogProfileStatRead(&PersistentData->_localPlayerPersistentData);

                    /* If the player state is valid, notify the persistent data system that saved data changed */
                    if (IsValid(DBDPlayerState) == true)
                    {
                        FUniqueNetIdRepl UniqueIdCopy = this->PlayerState->GetUniqueId();
                        PersistentData->OnSavedDataChanged(UniqueIdCopy, DBDPlayerState);
                    }

                    bResultSuccess = true;
                }
            }
        }

        /* Unbind the callback from the DAL */
        if (GameInstance->PlayerProfileDAL != nullptr)
        {
            GameInstance->PlayerProfileDAL->OnUserStatReadComplete.Remove(this->_statReadHandle);
            this->_statReadHandle.Invalidate();
        }
    }

    /* Broadcast the completion of the cloud stats load */
    this->OnCloudLoadStatsComplete.Broadcast(bResultSuccess);
}

// starts at line 647
void ADBDPlayerControllerBase::Local_SendCharacterData()
{
    return;
}

// starts at line 671
void ADBDPlayerControllerBase::Local_ProfileStatsPostInitialization(bool bWasSuccessful)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        /* Retrieve the achievement handler component from the player state */
        /* Use standard Unreal GetComponentByClass instead of manual lookup */
        UAchievementHandlerComponent* AchievementHandler = Cast<UAchievementHandlerComponent>(this->PlayerState->GetComponentByClass(UAchievementHandlerComponent::StaticClass()));

        if (IsValid(AchievementHandler) == true)
        {
            /* Unbind the initialization callback from the DAL */
            /* Based on previous functions, the DAL is accessed via GameInstance and the delegate is at offset 0x30 */
            UPlayerProfileDAL* ProfileDAL = GameInstance->PlayerProfileDAL;
            if (ProfileDAL != nullptr)
            {
                ProfileDAL->OnUserStatReadComplete.Remove(this->_statInitHandle);
                this->_statInitHandle.Invalidate();
            }

            /* Retrieve the player's persistent data using their UniqueNetId */
            UDBDPersistentData* PersistentData = GameInstance->_persistentData;
            
            if (PersistentData != nullptr && IsValid(this->PlayerState) == true)
            {
                /* Create a copy of the UniqueNetId to pass to the function */
                FUniqueNetIdRepl UniqueIdCopy = this->PlayerState->GetUniqueId();
                
                FPlayerPersistentData* PlayerPersistentData = PersistentData->GetPlayerPersistentData(UniqueIdCopy);

                /* If we successfully retrieved the persistent data, trigger the achievement check */
                if (PlayerPersistentData != nullptr)
                {
                    AchievementHandler->CheckAchievementsOnStartup(PlayerPersistentData);
                }
            }
        }
    }
}

// starts at line 688
void ADBDPlayerControllerBase::Local_ProfileStatsPostUpload(FName SessionName, bool bWasSuccessful)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        /* Access the PlayerProfileDAL from the GameInstance to unbind the callback */
        UPlayerProfileDAL* ProfileDAL = GameInstance->PlayerProfileDAL;

        if (ProfileDAL != nullptr)
        {
            /* Unbind the callback from the OnUserStatUploadComplete delegate */
            /* The disassembly uses an offset of 0xA0 from the DAL object for the delegate */
            ProfileDAL->OnUserStatUploadComplete.Remove(this->_statUploadHandle);
            
            /* Invalidate the handle since the callback has been executed and removed */
            this->_statUploadHandle.Invalidate();
        }
    }

    /* If another stats upload/operation was requested during the upload, trigger it now */
    if (this->_uploadStatsNeeded == true)
    {
        this->Local_LoadProfileStats();
    }
}

// starts at line 703
void ADBDPlayerControllerBase::ResetPreGameCachedValues()
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        /* Use Unreal Engine Cast macro to safely cast the PlayerState */
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        if (IsValid(DBDPlayerState) == true)
        {
            /* Retrieve the player's persistent data using their UniqueNetId */
            UDBDPersistentData* PersistentData = GameInstance->_persistentData;
            
            if (PersistentData != nullptr)
            {
                /* Create a copy of the UniqueNetId to pass to the function */
                FUniqueNetIdRepl UniqueIdCopy = DBDPlayerState->GetUniqueId();
                
                FPlayerPersistentData* PlayerPersistentData = PersistentData->GetPlayerPersistentData(UniqueIdCopy);

                /* If we successfully retrieved the persistent data, cache the pre-game values */
                if (PlayerPersistentData != nullptr)
                {
                    PlayerPersistentData->PreviousSlasherSkulls = PlayerPersistentData->SavedData.SharedData.SlasherSkulls;
                    PlayerPersistentData->PreviousCamperSkulls = PlayerPersistentData->SavedData.SharedData.CamperSkulls;
                    PlayerPersistentData->PreviousExperience = PlayerPersistentData->GetExperience(false);
                }
            }
        }
    }
}

// starts at line 717
void ADBDPlayerControllerBase::LogProfileStatWrite(const FOnlineLeaderboardWrite& writeObject) const
{
    /* Check if logging is enabled for this category before processing */
    if (UE_LOG_ACTIVE(GameFlow, Verbose))
    {
        /* Iterate over all the properties being written to the leaderboard/profile */
        for (FOnlineKeyValuePairs<FName, FVariantData>::TConstIterator It(writeObject.Properties); It; ++It)
        {
            const FName& StatName = It.Key();
            const FVariantData& StatData = It.Value();

            /* EOnlineKeyValuePairDataType::Int32 corresponds to 1 */
            if (StatData.GetType() == EOnlineKeyValuePairDataType::Int32)
            {
                int32 val;
                StatData.GetValue(val);

                UE_LOG(GameFlow, Verbose, TEXT("Writing Profile Stat %s: %i"), *StatName.ToString(), val);
            }
            /* EOnlineKeyValuePairDataType::Float corresponds to 7 */
            else if (StatData.GetType() == EOnlineKeyValuePairDataType::Float)
            {
                float OutData;
                StatData.GetValue(OutData);

                UE_LOG(GameFlow, Verbose, TEXT("Writing Profile Stat %s: %f"), *StatName.ToString(), OutData);
            }
            else
            {
                /* Fallback for unknown or unhandled variant types */
                UE_LOG(GameFlow, Verbose, TEXT("Writing Profile Stat %s: UNKNOWN VALUE TYPE"), *StatName.ToString());
            }
        }
    }
}

// starts at line 748
void ADBDPlayerControllerBase::LogProfileStatRead(const FPlayerPersistentData* persistentData) const
{
    /* Check if logging is enabled for the Verbose level before processing */
    if (UE_LOG_ACTIVE(GameFlow, Verbose))
    {
        /* Get string representation of Killer Skulls stat name */
        FString KillerStat = DBDProfileStats::DBD_KILLER_SKULLS.ToString();
        
        /* Log the killer skulls value */
        UE_LOG(GameFlow, Verbose, TEXT("Reading Profile Stat %s: %i"), *KillerStat, persistentData->SavedData.SharedData.SlasherSkulls);
        
        /* Second check is technically redundant in the disassembly, but preserving structure */
        if (UE_LOG_ACTIVE(GameFlow, Verbose))
        {
            /* Get string representation of Camper Skulls stat name */
            FString CamperStat = DBDProfileStats::DBD_CAMPER_SKULLS.ToString();
            
            /* Log the camper skulls value */
            UE_LOG(GameFlow, Verbose, TEXT("Reading Profile Stat %s: %i"), *CamperStat, persistentData->SavedData.SharedData.CamperSkulls);
        }
    }
}

// starts at line 754
void ADBDPlayerControllerBase::WritePlayerStats(FOnlineLeaderboardWrite& writeObject)
{
    /* Get the local player's unique ID as a shared pointer */
    TSharedPtr<const FUniqueNetId> localPlayerUniqueId = this->GetUniqueIDForLocalPlayer();

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (localPlayerUniqueId.IsValid() == true && IsValid(GameInstance) == true)
    {
        /* Check if the game instance is fully initialized/ready for writing stats */
        if (GameInstance->_initializedFromCloud == true)
        {
            /* Access the PlayerProfileDAL from the GameInstance to update player stats */
            UPlayerProfileDAL* ProfileDAL = GameInstance->PlayerProfileDAL;
            
            if (ProfileDAL != nullptr)
            {
                /* Convert the shared pointer to a shared ref, as expected by UpdatePlayerStats */
                ProfileDAL->UpdatePlayerStats(localPlayerUniqueId.ToSharedRef(), writeObject);
            }
        }
    }
}

// starts at line 764
int32 ADBDPlayerControllerBase::GetCamperIndex() const
{
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(DBDPlayerState) == true)
    {
        /* Retrieve the camper index from the validated player state */
        return DBDPlayerState->SelectedCamperIndex;
    }

    /* Return -1 (INDEX_NONE) if the player state is invalid */
    return INDEX_NONE;
}

// starts at line 775
int32 ADBDPlayerControllerBase::GetSlasherIndex() const
{
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(DBDPlayerState) == true)
    {
        /* Retrieve the slasher (killer) index from the validated player state */
        return DBDPlayerState->SelectedSlasherIndex;
    }

    /* Return -1 (INDEX_NONE) if the player state is invalid */
    return INDEX_NONE;
}

// starts at line 786
void ADBDPlayerControllerBase::Server_SelectCamper_Implementation(int32 camperIndex)
{
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
    {
        /* Set the camper index on the validated player state */
        DBDPlayerState->SelectedCamperIndex = camperIndex;
    }
}

// starts at line 800
void ADBDPlayerControllerBase::Server_SelectSlasher_Implementation(int32 slasherIndex)
{
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(DBDPlayerState) == true)
    {
        /* Set the slasher (killer) index on the validated player state */
        DBDPlayerState->SelectedSlasherIndex = slasherIndex;
    }
}

// starts at line 815
void ADBDPlayerControllerBase::SetCurrentCharacterPrestigeDates(FDateTime date)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        /* Use Unreal Engine Cast macro to safely cast the PlayerState */
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
        
        /* Retrieve the character's saved profile data from the Game Instance */
        /* The player state does not strictly need to be valid for this function call as it can take a nullptr, */
        /* but the cast ensures we pass the correct type if it is valid. */
        FCharacterSavedProfileData* CharacterData = GameInstance->GetCharacterSavedProfileDataForPlayerState(DBDPlayerState);

        if (CharacterData != nullptr)
        {
            /* Iterate through all the prestige earned dates and update them */
            for (int32 i = 0; i < CharacterData->PrestigeEarnedDates.Num(); ++i)
            {
                CharacterData->PrestigeEarnedDates[i] = date;
            }
        }
    }
}

// starts at line 831
void ADBDPlayerControllerBase::DBD_SetCurrentCharacterPrestigeDatesNow()
{
    this->SetCurrentCharacterPrestigeDates(FDateTime::UtcNow());
}

// starts at line 836
void ADBDPlayerControllerBase::DBD_SetCurrentCharacterPrestigeDatesPast()
{
    this->SetCurrentCharacterPrestigeDates(FDateTime::MinValue());
}

// starts at line 841
void ADBDPlayerControllerBase::DBD_SetCurrentCharacterPrestigeDatesFuture()
{
    this->SetCurrentCharacterPrestigeDates(FDateTime::MaxValue());
}

// starts at line 846
void ADBDPlayerControllerBase::DBD_FillAllScoreCategories()
{
    this->DBD_FillScoreCategory(0);
    this->DBD_FillScoreCategory(1);
    this->DBD_FillScoreCategory(2);
    this->DBD_FillScoreCategory(3);
}

// starts at line 854
void ADBDPlayerControllerBase::DBD_FillScoreCategory(int32 category)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(GameInstance) == true)
    {
        /* Retrieve the DesignTunables from the GameInstance */
        UDBDDesignTunables* DesignTunables = GameInstance->DesignTunables;
        
        if (DesignTunables != nullptr)
        {
            /* Get the tunable value for the score threshold without throwing a warning if missing (false flag) */
            float ThresholdValue = DesignTunables->GetTunableValue(TunableValues::SCORE_TO_SKULLREWARD_THRESHOLD, false);
            
            /* Fire a test score matching the threshold to fill the category */
            this->DBD_FireTestScore(category, ThresholdValue);
        }
    }
}

// starts at line 864
void ADBDPlayerControllerBase::DBD_FireTestScore(int32 category, float value)
{
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
    if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
    {
        EDBDScoreTypes ScoreTypeToFire;
        EDBDScoreTypes SlasherScoreFallback;

        /* Map the input category to the corresponding camper and slasher cheat score types */
        switch (category)
        {
            case 1:
                ScoreTypeToFire = EDBDScoreTypes::DBDCamperScore_CheatSurvival;
                SlasherScoreFallback = EDBDScoreTypes::DBDSlasherScore_CheatBrutality;
                break;
            case 2:
                ScoreTypeToFire = EDBDScoreTypes::DBDCamperScore_CheatAltruism;
                SlasherScoreFallback = EDBDScoreTypes::DBDSlasherScore_CheatDeviousness;
                break;
            case 3:
                ScoreTypeToFire = EDBDScoreTypes::DBDCamperScore_CheatBoldness;
                SlasherScoreFallback = EDBDScoreTypes::DBDSlasherScore_CheatHunter;
                break;
            default:
                ScoreTypeToFire = EDBDScoreTypes::DBDCamperScore_CheatObjectives;
                SlasherScoreFallback = EDBDScoreTypes::DBDSlasherScore_CheatSacrifice;
                break;
        }

        /* Check the player's role (Camper vs Slasher) */
        if (DBDPlayerState->GameRole == EPlayerRole::VE_Slasher)
        {
            ScoreTypeToFire = SlasherScoreFallback;
        }

        /* Dispatch the server RPC to fire the score event */
        this->Server_CheatFireScoreEvent(ScoreTypeToFire, value);
    }
}

// starts at line 901
void ADBDPlayerControllerBase::DBD_UpdateLocalStats()
{
    this->Local_LoadProfileStats();
}

// starts at line 906
void ADBDPlayerControllerBase::Server_CheatFireScoreEvent_Implementation(EDBDScoreTypes scoreType, float value)
{
    /* Use Unreal Engine Cast macro to safely cast the PlayerState */
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
    {
        /* Fire the score event on the player state */
        /* The final parameter (true) corresponds to 'bRedirectToGameEventSystem' based on the disassembly */
        DBDPlayerState->FireScoreEvent(scoreType, value, true);
    }
}

// starts at line 920
void ADBDPlayerControllerBase::SendAuthTicket(bool toClient, const FAuthenticationInfo& localAuth)
{
    /* Load the OnlinePresence module and check if we are running with a dedicated server */
    /* The disassembly uses LoadModuleChecked<FAnalyticsET> but the string literal is "OnlinePresence". */
    IOnlinePresencePlugin& PresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(FName("OnlinePresence"));
    
    if (PresenceModule.HasPlatformDataStore() == false)
    {
        /* Convert the authentication ticket to a string representation */
        FString authTicket = localAuth.TicketToString();
        uint64 steamID = localAuth.GetID();
        
        /* Calculate the actual ticket length (excluding null terminator if present) */
        uint32 ticketLength = (authTicket.Len() > 0) ? (authTicket.Len() - 1) : 0;

        /* Log the authentication details if verbosity is high enough */
        if (UE_LOG_ACTIVE(LogDBDAuthentication, VeryVerbose))
        {
            const TCHAR* DirectionStr = toClient ? TEXT("to client") : TEXT("to server");
            UE_LOG(LogDBDAuthentication, VeryVerbose, TEXT("sending auth ticket %s: steamID %llu, ticket %s, ticket length %u"), DirectionStr, steamID, *authTicket, ticketLength);
        }

        /* Dispatch the appropriate RPC based on the target destination */
        if (toClient == true)
        {
            this->Client_SendServerAuthentication(authTicket, steamID, ticketLength);
        }
        else
        {
            this->Server_SendClientAuthentication(authTicket, steamID, ticketLength);
        }
    }
}

// starts at line 943
void ADBDPlayerControllerBase::Client_SendServerAuthentication_Implementation(const FString& authServerTicket, uint64 steamID, uint32 ticketLength)
{
    this->AuthenticateRemoteTicket(authServerTicket, steamID, ticketLength);
}

// starts at line 953
void ADBDPlayerControllerBase::Server_SendClientAuthentication_Implementation(const FString& authClientTicket, uint64 steamID, uint32 ticketLength)
{
    /* Authenticate the incoming client ticket */
    if (this->AuthenticateRemoteTicket(authClientTicket, steamID, ticketLength) == true)
    {
        /* Use Unreal Engine Cast macro to safely cast the PlayerState to the Lobby specific player state */
        ADBDPlayerState_Lobby* LobbyPlayerState = Cast<ADBDPlayerState_Lobby>(this->PlayerState);

        /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
        /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
        if (IsValid(LobbyPlayerState) == true)
        {
            /* Trigger validation of the selected character on the authenticated player's lobby state */
            LobbyPlayerState->ValidateSelectedCharacter();
        }
    }
}

// starts at line 970
bool ADBDPlayerControllerBase::AuthenticateRemoteTicket(const FString& authServerTicket, uint64 steamID, uint32 ticketLength)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Retrieve the Authentication manager from the GameInstance */
        UDBDAuthentication* Authentication = GameInstance->GetAuthentication();
        
        if (IsValid(Authentication) == true)
        {
            /* Delegate the ticket authentication to the dedicated Authentication manager */
            return Authentication->AuthenticateRemoteTicket(authServerTicket, steamID, ticketLength, this);
        }
    }
    
    return false;
}

// starts at line 982
void ADBDPlayerControllerBase::Server_SendMessage_Implementation(int32 playerId, const FString& playerName, const FString& msg)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Check if the ChatManager weak pointer is valid */
        if (GameInstance->ChatManager.IsValid() == true)
        {
            /* Find the index of the player ID in the GameInstance's tracked player array */
            /* IDA identified the array start at offset 0x2E8 and element count at 0x2F0 */
            int32 playerIndex = INDEX_NONE;
            for (int32 i = 0; i < GameInstance->PlayerIds.Num(); ++i)
            {
                if (GameInstance->PlayerIds[i] == playerId)
                {
                    playerIndex = i;
                    break;
                }
            }

            /* Retrieve the actual ChatManager pointer and multicast the message */
            AChatManager* ChatManager = GameInstance->ChatManager.Get();
            if (ChatManager != nullptr)
            {
                ChatManager->Multicast_DisplayMessage(playerIndex, playerName, msg);
            }
        }
    }
}

// starts at line 997
void ADBDPlayerControllerBase::DBD_DebugSendChatMessage(FString msg)
{
    FString playerName;
    bool bSentMessage = false;

    /* Use Unreal Engine Cast macro to safely cast the Player to a LocalPlayer */
    ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(this->Player);

    if (IsValid(LocalPlayer) == true)
    {
        /* Get the player's nickname to use in the chat message */
        playerName = LocalPlayer->GetNickname();

        /* Use Unreal Engine Cast macro to safely cast the GameInstance */
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (IsValid(GameInstance) == true)
        {
            /* Check if the ChatManager weak pointer is valid */
            if (GameInstance->ChatManager.IsValid() == true)
            {
                /* Dispatch the server RPC to send the message using player ID 0 (likely representing local/debug user) */
                this->Server_SendMessage(0, playerName, msg);
                bSentMessage = true;
            }
        }
    }

    /* If we failed to send the message (e.g., not in the lobby), display an on-screen debug message */
    if (bSentMessage == false)
    {
        /* The disassembly maps the GEngine->AddOnScreenDebugMessage call to PostSerialize of a struct */
        if (GEngine != nullptr)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("You must be in the online lobby to use this debug function!"));
        }
    }
}

// starts at line 1014
void ADBDPlayerControllerBase::Server_EACMessageFromClient_Implementation(const TArray<uint8>& message, uint32 messageLength)
{
    /* Log the incoming message if verbosity is high enough */
    UE_LOG(LogDBDEasyAntiCheat, Verbose, TEXT("RPC Message from client"));

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Retrieve the EasyAntiCheat manager from the GameInstance */
        UDBDEasyAntiCheat* EACManager = GameInstance->GetEAC();
        
        if (IsValid(EACManager) == true)
        {
            /* Forward the incoming message to the EAC manager on the server */
            EACManager->Authority_HandleMessageFromClient(this, message, messageLength);
        }
    }
}

// starts at line 1029
void ADBDPlayerControllerBase::Client_EACMessageFromServer_Implementation(const TArray<uint8>& message, uint32 messageLength)
{
    /* Log the incoming message if verbosity is high enough */
    UE_LOG(LogDBDEasyAntiCheat, Verbose, TEXT("RPC Message from server"));

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Retrieve the EasyAntiCheat manager from the GameInstance */
        UDBDEasyAntiCheat* EACManager = GameInstance->GetEAC();
        
        if (IsValid(EACManager) == true)
        {
            /* Forward the incoming message to the EAC manager on the client (slave) */
            EACManager->Slave_HandleMessageFromServer(message, messageLength);
        }
    }
}

// starts at line 1038
void ADBDPlayerControllerBase::Client_ValidateServer_Implementation()
{
    /* Log the server validation RPC call if verbosity is high enough */
    UE_LOG(LogDBDEasyAntiCheat, Verbose, TEXT("RPC to validate server"));

    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Retrieve the EasyAntiCheat manager from the GameInstance */
        UDBDEasyAntiCheat* EACManager = GameInstance->GetEAC();
        
        if (IsValid(EACManager) == true)
        {
            /* Notify the EAC manager on the client (slave) that it has successfully connected/validated */
            EACManager->Slave_Connected();
        }
    }
}

// starts at line 1048
void ADBDPlayerControllerBase::ClientWasKickedImpl(EDisconnectErrors kickReason, uint32 errorCode, const FString& additionalInfo)
{
    /* Use Unreal Engine Cast macro to safely cast the GameInstance */
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
    if (IsValid(GameInstance) == true)
    {
        /* Register the disconnect error with the game instance */
        /* The final parameter (false) likely corresponds to 'bForceDisconnect' based on the disassembly naming */
        GameInstance->RegisterDisconnectError(kickReason, errorCode, additionalInfo, false);
    }
}

// starts at line 1057
bool ADBDPlayerControllerBase::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
    /* Check if there are any listeners bound to the custom key pressed event delegate */
    if (this->_onKeyPressedEvent.IsBound() == true)
    {
        /* If bound, broadcast the event with the pressed key */
        this->_onKeyPressedEvent.Broadcast(Key);
        
        /* Consume the input by returning false (or true depending on UE engine specifics, 
           but the disassembly explicitly xors al,al and returns 0 in this branch) */
        return false;
    }
    else
    {
        /* If no custom delegate is bound, route the input to the base APlayerController implementation */
        return Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
    }
}

// starts at line 1069
bool ADBDPlayerControllerBase::InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
    /* Check if there are any listeners bound to the custom key pressed event delegate */
    /* Even though this is an axis input, it seems to be suppressing axis inputs if a key event is actively being listened to */
    if (this->_onKeyPressedEvent.IsBound() == true)
    {
        /* Consume the input without processing the axis if the custom key delegate is bound */
        return false;
    }
    else
    {
        /* If no custom delegate is bound, route the axis input to the base APlayerController implementation */
        return Super::InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
    }
}

// starts at line 1080
void ADBDPlayerControllerBase::DBD_BuildFromMap(FString map)
{
    this->Server_BuildProceduralLevelFromMap(map);
}

// starts at line 1085
void ADBDPlayerControllerBase::DBD_BuildFromSeed(int seed)
{
    this->Server_BuildProceduralLevelFromSeed(seed);
}

// starts at line 1090
void ADBDPlayerControllerBase::RefreshAxisScalingFromSettings()
{
    /* Ensure GEngine is valid before attempting to access user settings */
    if (GEngine != nullptr)
    {
        /* Use Unreal Engine Cast macro to safely cast the base settings to DBD specific settings */
        UDBDGameUserSettings* GameUserSettings = Cast<UDBDGameUserSettings>(GEngine->GetGameUserSettings());

        /* Use standard Unreal Engine IsValid macro which handles pending kill and nullptr checks */
        /* This replaces the expanded internal GUObjectArray and flag verification from the disassembly */
        if (IsValid(GameUserSettings) == true)
        {
            /* Retrieve sensitivities and convert from a whole number (e.g., 0-100) to a float scalar (0.0f - 1.0f) */
            /* The float constant 0.0099999998 in the disassembly is the IEEE 754 standard representation of 0.01f */
            
            /* Offset 0x8B8 */
            this->_slasherMouseScale = GameUserSettings->GetKillerMouseSensitivity() * 0.01f;
            
            /* Offset 0x8B4 */
            this->_slasherGamepadScale = GameUserSettings->GetKillerControllerSensitivity() * 0.01f;
            
            /* Offset 0x8C0 */
            this->_camperMouseScale = GameUserSettings->GetSurvivorMouseSensitivity() * 0.01f;
            
            /* Offset 0x8BC */
            this->_camperGamepadScale = GameUserSettings->GetSurvivorControllerSensitivity() * 0.01f;
        }
    }
}

// starts at line 1124
void ADBDPlayerControllerBase::Server(const FString& commandLine)
{
    /* Check if this controller has authority (Role == ROLE_Authority, which is 3) */
    /* In Unreal Engine, ROLE_Authority means this instance is running on the server */
    if (this->Role == ENetRole::ROLE_Authority)
    {
        /* If we are the server, execute the console command directly via GEngine */
        if (GEngine != nullptr)
        {
            /* Get the current world context and output device redirector (for logging) */
            UWorld* CurrentWorld = this->GetWorld();
            FOutputDeviceRedirector* OutputRedirector = FOutputDeviceRedirector::Get();
            
            /* Execute the command */
            GEngine->Exec(CurrentWorld, *commandLine, OutputRedirector);
        }
    }
    else
    {
        /* If we are a client, forward the command to the server via an RPC */
        this->CallConsoleCmdOnServer_Server(commandLine);
    }
}

// starts at line 1137
void ADBDPlayerControllerBase::CallConsoleCmdOnServer_Server_Implementation(const FString& commandLine)
{
    /* Ensure GEngine is valid before attempting to execute a command */
    if (GEngine != nullptr)
    {
        /* Get the current world context and output device redirector (for logging) */
        UWorld* CurrentWorld = this->GetWorld();
        FOutputDeviceRedirector* OutputRedirector = FOutputDeviceRedirector::Get();
        
        /* Execute the command */
        GEngine->Exec(CurrentWorld, *commandLine, OutputRedirector);
    }
}
