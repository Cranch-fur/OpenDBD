void ADBDGameMode::AddToKillerGoalCounter(FName tunableValue, FUniqueNetIdRepl *id, KillerGoalCountStrategy type)
{
    // Retrieve the GameState and Cast it to the specific ADBDGameState class
    // In disassembly: Cast logic involving GetPrivateStaticClass and loop checks
    ADBDGameState *DBDGameState = Cast<ADBDGameState>(this->GameState);

    // Retrieve the GameInstance and Cast it to the specific UDBDGameInstance class
    UDBDGameInstance *DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Verify that both the GameInstance and GameState are valid (not nullptr and not pending kill)
    if (DBDGameInstance && DBDGameState)
    {
        // Get the tunable value (badge configuration) using the utility class
        float TunableFloatValue = UDBDUtilities::GetBadgeTunableValue(tunableValue);

        // Create a local copy of the UniqueNetIdRepl (handled via copy constructor in C++)
        FUniqueNetIdRepl PlayerIdCopy = *id;

        // Convert the tunable value to an integer for score calculation
        int32 PointsDelta = (int32)TunableFloatValue;

        // Access a member variable at offset 0x3b8 inside UDBDGameInstance.
        // This is likely a Manager or Persistent Data storage object.
        UDBDPersistentData *PersistentDataManager = DBDGameInstance->_persistentData;

        // Retrieve the cached player score data from the persistent data manager
        FCachedPlayerScoreData *ScoreData = PersistentDataManager->GetPlayerScoreCache(PlayerIdCopy);

        // Check if the score data was successfully retrieved
        if (ScoreData)
        {
            // Check the strategy type. In disassembly, it compares r15b (type) with 0x1.
            // Assuming 0x1 corresponds to a "Set" or "Replace" strategy.
            if (type == KillerGoalCountStrategy::Set)
            {
                // If the strategy is 'Set', we subtract the existing points from our target value
                // to calculate the difference (delta) needed to reach that new value.
                PointsDelta -= ScoreData->KillerGoalPoints;
            }

            // Apply the delta to the cached score (Add or Set logic combined)
            ScoreData->KillerGoalPoints += PointsDelta;

            // Multicast the new score to all clients via the GameState
            // Re-using the PlayerIdCopy and the updated score
            DBDGameState->Multicast_SetKillerGoalPoints(PlayerIdCopy, ScoreData->KillerGoalPoints);

            // Update the local GameMode's total killer goal points counter
            // The disassembly uses SSE instructions (cvtdq2ps) to accumulate the delta
            this->_killerGoalPoints += PointsDelta;
        }
    }
}




bool ADBDGameMode::CanSpectate_Implementation(APlayerController *Viewer, APlayerState *ViewTarget)
{
    // Attempt to cast the Viewer and ViewTarget to the game-specific classes.
    ADBDPlayerController *DbDViewerController = Cast<ADBDPlayerController>(Viewer);
    ADBDPlayerState *DbDViewTargetState = Cast<ADBDPlayerState>(ViewTarget);

    // If either cast fails (meaning the objects are not of the specific DeadByDaylight types),
    // we fall back to the parent class implementation (tail call in assembly).
    if (DbDViewerController == nullptr || DbDViewTargetState == nullptr)
    {
        return Super::CanSpectate_Implementation(Viewer, ViewTarget);
    }

    // Retrieve the PlayerState from the Viewer and cast it to ADBDPlayerState.
    ADBDPlayerState *DbDViewerState = Cast<ADBDPlayerState>(Viewer->PlayerState);

    // Ensure the Viewer's PlayerState is valid before accessing memory.
    if (DbDViewerState)
    {
        // If the Viewer is a Slasher, they cannot spectate.
        EPlayerRole ViewerRole = DbDViewerState->GameRole;
        if (ViewerRole == VE_Slasher)
        {
            return false;
        }

        EPlayerRole TargetRole = DbDViewTargetState->GameRole;

        // If the Target is a Slasher, we need to check if the Viewer has special privileges.
        if (TargetRole == VE_Slasher)
        {
            // Retrieve the Viewer's state again via a static helper function.
            ADBDPlayerState *ViewerStateCheck = DbDViewerController->GetDBDPlayerState();

            // Check if this retrieved state is valid.
            if (ViewerStateCheck)
            {
                EPlayerRole ViewerCheckRole = ViewerStateCheck->GameRole;
                if (ViewerCheckRole != VE_Observer)
                {
                    return false;
                }
            }
            else
            {
                // If we couldn't get the state, deny spectating.
                return false;
            }
        }

        // If the Target's role is Spectator, they cannot be spectated.
        if (TargetRole == VE_Observer)
        {
            return false;
        }

        // Check a boolean flag at offset 0x7f9 in the ViewTarget.
        // If this flag is set (not 0), spectating is forbidden.
        // This could be "IsDisconnected", "IsCloaked", or "PreventSpectating".
        EGameState TargetBlockSpectate = DbDViewTargetState->PlayerData->_playerGameState; /* UNDEFINED ELEMENT */
        if (TargetBlockSpectate != VE_Active)
        {
            return false;
        }

        // If all checks pass, allow spectating.
        return true;
    }

    // Default return false if the ViewerState was null (though flow usually implies validity).
    return false;
}




void ADBDGameMode::CheckGameEnded()
{
    // Retrieve the GameState and cast to ADBDGameState
    ADBDGameState *DbdGameState = Cast<ADBDGameState>(this->GameState);

    // Retrieve the GameInstance and cast to UDBDGameInstance
    UDBDGameInstance *DbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Basic validity checks for critical objects.
    // The disassembly performs manual InternalIndex checks to ensure objects aren't pending kill.
    if (DbdGameState == nullptr || DbdGameInstance == nullptr)
    {
        return;
    }

    UWorld *World = GetWorld();

    if (World == nullptr)
    {
        return;
    }

    // Check specific flags before proceeding with logic

    // Check a byte at GameState + 0x611. This could be a "MatchEnded" or "ProcessingEnd" flag.
    if (DbdGameState->IsGameEnded) /* UNDEFINED ELEMENT */
    {
        return;
    }

    // Check a status byte on the helper object (offset 0x840). If 3, we abort checks.
    if (World->WorldType == PIE) /* UNDEFINED ELEMENT */
    {
        return;
    }

    // Check if the "IgnoreGameEndConditions" console variable is set
    static IConsoleVariable *CVarIgnoreGameEndConditions = IConsoleManager::Get().FindConsoleVariable(TEXT("IgnoreGameEndConditions"));
    if (CVarIgnoreGameEndConditions && CVarIgnoreGameEndConditions->GetInt() != 0)
    {
        return;
    }

    // Initialize local variables for the loop
    bool bShouldEndGame = true; // Maps to 'Parameters' and 'rsi' in assembly (Default to true/1)
    int32 KillerCount = 0;      // Maps to 'rbx_1'
    int32 SurvivorCount = 0;    // Maps to 'Parameters_1' / 'r13'

    // Get the OnlinePresencePlugin to check player validity
    IOnlinePresencePlugin *PresencePlugin = IOnlinePresencePlugin::Get();

    // Verify Plugin availability (VTable + 0xA0)
    bool bPluginAvailable = false;
    if (PresencePlugin)
    {
        // 0xA0 is likely "IsAvailable()" or similar
        bPluginAvailable = PresencePlugin->IsConnected();
    }

    if (bPluginAvailable == false)
    {
        // If plugin is missing, force end game (rsi = 1)
        bShouldEndGame = true;
    }
    else
    {
        // Iterate through the PlayerArray in GameState
        for (APlayerState *PlayerState : DbdGameState->PlayerArray)
        {
            ADBDPlayerState *DbdPlayerState = Cast<ADBDPlayerState>(PlayerState);

            if (DbdPlayerState == nullptr)
            {
                continue;
            }

            // Perform Online Presence check for the specific player
            // VTable + 0x148 on PresencePlugin, passing player unique ID data
            // logic simplified: if presence check fails, we treat player as invalid
            TSharedPtr<IMatchmakingPresenceSubsystem> Matchmaking = PresencePlugin->GetMatchmaking();
            bool bPresenceValid = Matchmaking->IsPlayerInSession(DbdPlayerState->SessionName, DbdPlayerState->UniqueId);

            if (bPresenceValid == false)
            {
                continue;
            }

            // Check if player is considered "Active" in the match
            bool bIsPlayerActive = true;

            // Flag at 0x7f9 (likely "IsDisconnected" or "HasEscaped")
            uint8 IsDisconnectedFlag = DbdPlayerState->PlayerData->_playerGameState; /* UNDEFINED ELEMENT */

            // Check if player is disconnected AND the level has finished loading
            if (IsDisconnectedFlag != VE_Active && this->_currentLevelLoadingStep >= ELevelLoadingSteps::LoadingCompleted)
            {
                bIsPlayerActive = false;
            }

            // Check flags at 0x3ac (Bitwise check 0x10) - Likely "LeftMatch" or similar
            if (DbdPlayerState->bIsInactive)
            {
                bIsPlayerActive = false;
            }

            // If the player is active, we check their role to determine if game continues
            if (bIsPlayerActive == true)
            {
                // Get Player Role (0x750)
                EPlayerRole PlayerRole = DbdPlayerState->GameRole; /* UNDEFINED ELEMENT */

                if (PlayerRole == VE_Camper) // Role 2: Survivor (Camper)
                {
                    SurvivorCount++;
                    // If there is at least one active survivor, the game should NOT end
                    bShouldEndGame = false;
                }
                else if (PlayerRole == VE_Slasher) // Role 1: Killer (Slasher)
                {
                    KillerCount++;
                }
            }
        }

        // Final check: If there are no Killers present, the game must end,
        // regardless of survivor count.
        if (KillerCount == 0)
        {
            bShouldEndGame = true;
        }
    }

    // If this is the server (Role == 3), update the survivor count and broadcast delegate
    if (DbdGameState->Role == ROLE_Authority)
    {
        DbdGameState->_survivorLeft = SurvivorCount;
        DbdGameState->OnSurvivorsLeftChanged.Broadcast(SurvivorCount);
    }

    DbdGameState->IsGameEnded = bShouldEndGame;

    // If the game has ended, handle the logic
    if (bShouldEndGame == true)
    {
        this->HandleMatchEnded(false);
    }
}




void ADBDGameMode::CheckLoadoutAndThemeStatus(UDBDGameInstance *instance)
{
    // Retrieve GameState and cast to specific DBD class
    ADBDGameState *DbdGameState = Cast<ADBDGameState>(this->GameState);

    // Perform validity check (Disassembly does manual class tree index check)
    if (DbdGameState == nullptr)
    {
        return;
    }

    // Calculate the target number of players expected to play (Total Players - Spectators)
    int32 TotalPlayerArrayNum = DbdGameState->PlayerArray.Num();
    int32 ObserverCount = DbdGameState->GetObserverCount();
    int32 ExpectedPlayerCount = TotalPlayerArrayNum - ObserverCount;

    // Get the number of players who have finished loading/validating their loadout
    int32 ReadyPlayerCount = this->GetPlayerReadyCount(instance);

    // Logic: Wait here until either the Timer expires OR all expected players are ready.
    // _gameLevelLoadingTimeOutTimer is at offset 0x478
    bool bIsTimerDone = this->GameLevelLoadingTimeOutTimer.IsDone();

    if (!bIsTimerDone && ReadyPlayerCount < ExpectedPlayerCount)
    {
        // Still waiting for players...
        return;
    }

    // --- Transition to Intro Wait State ---

    // Set loading step to 3 (WaitingForIntroToBeDone)
    this->SetCurrentLoadingStep(ELevelLoadingSteps::WaitingForIntroToBeDone);

    // Mark the level as loaded in the GameState
    ADBDGameState *CurrentGameState = instance->GetGameState();
    if (CurrentGameState)
    {
        CurrentGameState->SetGameLevelLoaded();
    }

    // Check if we need to adjust player counts (e.g. someone disconnected during load)
    this->CheckPlayerCountAdjustement(instance);

    // Record the Match Start Time (UtcNow)
    this->_matchStartTime = FDateTime::UtcNow();

    // Reset the Network Check Failure Timer to 1.0 seconds
    this->NetworkCheckFailTimer.Reset(1.0f);

    // --- Server Authority Logic: Update Survivor Counts ---
    // Note: Re-fetching GameState via Helper as per disassembly
    CurrentGameState = instance->GetGameState();

    if (CurrentGameState && CurrentGameState->Role == ROLE_Authority)
    {
        // Calculate survivors: (Total - Observers) - 1.
        // The "- 1" accounts for the Killer, who is in the player array but not a survivor.
        int32 ActualSurvivors = ExpectedPlayerCount - 1;

        // Update internal variable (Offset 0x6f4)
        CurrentGameState->SurvivorsLeft = ActualSurvivors;

        // Broadcast the change to listeners (UI, Audio, etc.)
        CurrentGameState->OnSurvivorsLeftChanged.Broadcast(ActualSurvivors);
    }

    // --- Transition to Completion State ---

    // Set loading step to 4 (LoadingCompleted)
    this->SetCurrentLoadingStep(ELevelLoadingSteps::LoadingCompleted);

    // --- Server Authority Logic: Level Ready ---
    CurrentGameState = instance->GetGameState();

    if (CurrentGameState && CurrentGameState->Role == ROLE_Authority)
    {
        // Set flag that level is ready to play (Offset 0x6ee)
        CurrentGameState->LevelReadyToPlay = true;

        // Broadcast Level Ready event (Offset 0x540)
        CurrentGameState->OnLevelReadyToPlay.Broadcast();

        // execute final pre-start logic
        CurrentGameState->OnBeforeLevelStarts();
    }

    // Final check to see if the match should end immediately (e.g. not enough players left)
    this->CheckGameEnded();
}




void ADBDGameMode::CheckMapLoadingStatus(float deltaSeconds)
{
    // Retrieve the GameInstance using the standard AActor method
    UGameInstance *GameInstance = this->GetGameInstance();
    UDBDGameInstance *DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    ADBDGameState *DBDGameState = nullptr;
    bool bIsStateValid = false;

    // Proceed only if the generic GameInstance exists and is successfully cast to UDBDGameInstance
    if (DBDGameInstance)
    {
        World *WorldContext = GetWorld();

        if (WorldContext)
        {
            // Accessing GameState from the WorldContext (likely UWorld) at offset 0x58
            AGameState *RawGameState = WorldContext->GameState;

            if (RawGameState)
            {
                DBDGameState = Cast<ADBDGameState>(RawGameState);
            }
        }

        // Validate that we have a valid ADBDGameState
        if (DBDGameState)
        {
            bool bFlagIsSet = DBDGameState->bActorIsBeingDestroyed;

            if (bFlagIsSet == false)
            {
                bIsStateValid = true;
            }
        }
    }

    // --- MAIN LOGIC FLOW ---

    // If GameInstance, GameState are valid and flags are clear
    if (bIsStateValid == true)
    {
        // 1. Check validity of a WeakObjectPtr at offset 0x178 in GameInstance
        // Likely checking if a session or specific manager is still valid
        /* UNDEFINED ELEMENT: Member at offset 0x178 in UDBDGameInstance (FWeakObjectPtr) */
        TWeakObjectPtr<AProceduralLevelBuilder> UnknownWeakPtr = DBDGameInstance->Builder;

        // Disassembly performs IsValid check and then Get check against nullptr/pending kill logic
        bool bWeakPtrPointsToValidObject = false;
        if (UnknownWeakPtr.IsValid())
        {
            AProceduralLevelBuilder *WeakPtrTarget = UnknownWeakPtr.Get();
            if (WeakPtrTarget)
            {
                if (WeakPtrTarget->_buildingDone)
                {
                    bWeakPtrPointsToValidObject = true;
                }
            }
        }

        // Only proceed if the weak pointer check passed (or more accurately, if it didn't fail the specific check)
        // However, looking at ASM flow (0x1402484f6), if the byte at 0x6b8 is FALSE, we continue.
        // If it is TRUE, we jump to the "Wait" block.
        if (bWeakPtrPointsToValidObject == true)
        {
            // 2. Update the Loading Timeout Timer
            this->_gameLevelLoadingTimeOutTimer->Update(deltaSeconds);

            // 3. Check if Timer is Done and event hasn't been sent yet
            if (this->_gameLevelLoadingTimeOutTimer->IsDone() && this->_levelLoadingTimeoutSent == false)
            {
                // Record "GameLevelLoadingTimeOut" event
                // The disassembly constructs a TArray manually, here we use the UE4 wrapper
                UBHVRAnalytics::RecordEvent(TEXT("GameLevelLoadingTimeOut"), TArray<FAnalyticsEventAttribute>());

                this->_levelLoadingTimeoutSent = true;
            }

            // 4. State Machine for Loading Steps
            // _currentLevelLoadingStep is at offset 0x472 (byte)
            ELevelLoadingSteps CurrentStep = this->_currentLevelLoadingStep;

            if (CurrentStep == WaitingForPlayersToBeSpawned)
            {
                this->CheckPlayerLoadingStatus(DBDGameInstance);
            }
            else if (CurrentStep == WaitingForLoadoutAndTheme)
            {
                this->CheckLoadoutAndThemeStatus(DBDGameInstance);

                // Check if step changed to 4 (LoadingCompleted) after the previous call
                if (this->_currentLevelLoadingStep == LoadingCompleted)
                {
                    this->CheckSurvivorDisconnectBeforeMatchStart();
                }
            }
            // Return to avoid hitting the "Waiting" block below
            return;
        }
    }

    // --- WAITING / LOGGING LOGIC (The "Goto" Target) ---
    // This block is reached if:
    // 1. GameInstance is null
    // 2. GameState is null or invalid class
    // 3. Flags in GameState (0x140) were set
    // 4. The WeakPtr check at 0x178 failed/indicated waiting

    // We need to re-verify existence of GameInstance and the WeakPtr at 0x178 to perform logging
    if (DBDGameInstance)
    {
        /* UNDEFINED ELEMENT: Member at offset 0x178 in UDBDGameInstance (FWeakObjectPtr) */
        TWeakObjectPtr<AProceduralLevelBuilder> UnknownWeakPtr = DBDGameInstance->Builder;

        if (UnknownWeakPtr.IsValid())
        {
            AProceduralLevelBuilder *WeakPtrTarget = UnknownWeakPtr.Get();

            bool bIsBuilt = WeakPtrTarget->_buildingDone;

            // If not built (false), log the status
            if (bIsBuilt == false)
            {
                const TCHAR *StatusString = TEXT("");

                if (WeakPtrTarget->_generatedData.GenerationLogs == nullptr)
                {
                    StatusString = TEXT(""); // Disassembly implies empty or specific default
                }
                else
                {
                    StatusString = WeakPtrTarget->_generatedData.GenerationLogs;
                }

                FString LogMessage = FString::Printf(TEXT("LoadingStatus -> WaitingForLevelToBeBuilt %s"), StatusString);

                // Log to DBDAnalytics
                // Source file path and line number (0xd4 = 212) derived from disassembly args
                UDBDAnalytics::Log(LogMessage, TEXT("D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\DBDGame\\DBDGameMode.cpp"), 212);
            }
        }
    }
}




void ADBDGameMode::CheckPlayerCountAdjustement(UDBDGameInstance *instance)
{
    // Access the Builder WeakPtr at offset 0x178 in UDBDGameInstance
    /* UNDEFINED ELEMENT: Member at offset 0x178 in UDBDGameInstance (TWeakObjectPtr<AProceduralLevelBuilder>) */
    TWeakObjectPtr<AProceduralLevelBuilder> &BuilderWeakPtr = instance->Builder;

    // Only proceed if the Builder pointer is valid
    if (BuilderWeakPtr.IsValid())
    {
        ADBDGameState *GameState = instance->GetGameState();

        int32 SurvivorCount = 0;
        int32 KillerCount = 0;

        // Iterate through the PlayerArray to count specific roles based on internal flags
        // Accessing GameState->PlayerArray which is a TArray<APlayerState*>
        for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
        {
            APlayerState *PlayerState = GameState->PlayerArray[i];

            if (PlayerState)
            {
                ADBDPlayerState *DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);

                // Ensure the cast was successful and check class hierarchy depth/flags if needed (handled by Cast)
                if (DBDPlayerState)
                {
                    // Check the custom Role byte at offset 0x750 in ADBDPlayerState
                    // 1 = Survivor, 2 = Killer
                    /* UNDEFINED ELEMENT: Member at offset 0x750 in ADBDPlayerState (uint8) */
                    EPlayerRole GameRole = DBDPlayerState->GameRole;

                    if (GameRole == VE_Slasher)
                    {
                        KillerCount++;
                    }
                    else if (GameRole == VE_Camper)
                    {
                        SurvivorCount++;
                    }
                }
            }
        }

        int32 CurrentPlayerCount = KillerCount + SurvivorCount;

        // Access the persistent data object at offset 0x3b8 in UDBDGameInstance
        /* UNDEFINED ELEMENT: Member at offset 0x3b8 in UDBDGameInstance (UDBDPersistentData*) */
        UDBDOersistentData *PersistentData = instance->_persistentData;

        // Access the PlayerCount integer at offset 0x28 within the persistent data
        /* UNDEFINED ELEMENT: Member at offset 0x28 in UDBDPersistentData (int32) */
        int32 StoredPlayerCount = PersistentData->_gamePersistentData.PlayerCount;

        // If the calculated current player count is less than the stored persistent count, adjustment is needed
        if (CurrentPlayerCount < StoredPlayerCount)
        {
            AProceduralLevelBuilder *Builder = BuilderWeakPtr.Get();

            // Adjust the generator count in the builder based on the new player count
            Builder->AdjustGeneratorCount(CurrentPlayerCount);

            // Update the stored persistent count to match the current count
            StoredPlayerCount = CurrentPlayerCount;

            // Update the GameMode's internal generator requirement
            this->UpdateGeneratorNeeded();

            ADBDGameState *CurrentGameState = UDBDGameInstance::GetGameState(instance);

            // Check if the current instance has Authority (ROLE_Authority == 3). Role is commonly at offset 0x110.
            /* UNDEFINED ELEMENT: Role property at offset 0x110 in AActor (ENetRole) */
            if (CurrentGameState->Role == ROLE_Authority)
            {
                // Access _activatedGeneratorCount at offset 0x6e8
                /* UNDEFINED ELEMENT: Member at offset 0x6e8 in ADBDGameState (int32) */
                int32 ActivatedGenCount = CurrentGameState->_activatedGeneratorCount;
                int32 PreviousCount = ActivatedGenCount;

                // Reset activated generator count to 0
                ActivatedGenCount = 0;

                // If the count was previously non-zero, broadcast the change
                if (PreviousCount != 0)
                {
                    // Broadcast C++ Multicast Delegate at offset 0x570
                    /* UNDEFINED ELEMENT: FMulticastDelegate at offset 0x570 in ADBDGameState */
                    // TBaseMulticastDelegate<void, int32>::Broadcast(0)
                    FOnActivatedGeneratorCountChanged &DelegateRef = CurrentGameState->OnActivatedGeneratorCountChanged;
                    DelegateRef.Broadcast(0);

                    // Broadcast Dynamic Multicast Delegate at offset 0x5e0 (Blueprint accessible)
                    /* UNDEFINED ELEMENT: FMulticastScriptDelegate at offset 0x5e0 in ADBDGameState */
                    // In assembly: constructs parameters on stack and calls ProcessMulticastDelegate
                    FActivatedGeneratorCountChangedDynamicEvent &OnGenCountChangedDynamic = CurrentGameState->OnActivatedGeneratorCountChangedDynamic;
                    OnGenCountChangedDynamic.Broadcast(0);
                }
            }
        }

        UWorld *World = this->GetWorld();

        if (World)
        {
            for (TActorIterator<AInteractable> It(World); It; ++It)
            {
                AInteractable *Interactable = *It;

                // Check validity and pending kill status
                if (Interactable && Interactable->IsPendingKill() == false)
                {
                    Interactable->ProceduralLevelDependencyItemAdjusted();
                }
            }
        }
    }
}




void ADBDGameMode::CheckPlayerLoadingStatus(UDBDGameInstance *instance)
{
    // Retrieve GameState.
    // In ASM: Access via offset 0x418 on 'this', verified against ADBDGameState class.
    /* UNDEFINED ELEMENT: Member at offset 0x418 in ADBDGameMode (Likely cached GameState pointer) */
    ADBDGameState *DBDGameState = this->GetGameState<ADBDGameState>();

    if (DBDGameState)
    {
        // 1. Check Player Readiness
        // Calculate expected player count (Total - Observers)
        int32 TotalPlayers = DBDGameState->PlayerArray.Num();
        int32 ObserverCount = DBDGameState->GetObserverCount();
        int32 ExpectedPlayers = TotalPlayers - ObserverCount;

        // Get count of players currently reporting "Ready"
        int32 ReadyPlayers = this->GetPlayerReadyCount(instance);

        // Check Loading Timer
        // If Timer is NOT done AND ReadyPlayers < ExpectedPlayers, we wait (return).
        bool bTimerDone = this->_gameLevelLoadingTimeOutTimer->IsDone();

        if (!bTimerDone && ReadyPlayers < ExpectedPlayers)
        {
            return;
        }
    }

    // 2. Prepare Level Generation Data defaults
    FName ThemeName = FName("Default");

    // ASM refers to global data_14361ea80 for default weather.
    /* UNDEFINED ELEMENT: Global/Static FName for default weather */
    FName ThemeWeather = FName("DefaultWeather");

    FString MapNameData; // 'Original_1' in pseudo
    int32 TileCount = 0;
    TArray<FDependency> LevelDependencies;

    // 3. Retrieve Data from Builder (if valid)
    /* UNDEFINED ELEMENT: Builder WeakPtr at offset 0x178 in UDBDGameInstance */
    TWeakObjectPtr<AProceduralLevelBuilder> &WeakBuilder = instance->Builder;

    if (WeakBuilder.IsValid())
    {
        AProceduralLevelBuilder *Builder = WeakBuilder.Get();

        // Access Theme Data at offset 0x4d8.
        // 0x4d8 is likely a pointer to a UObject or Struct containing theme info.
        // Inside that object: Offset 0x28 = Name, Offset 0x30 = Weather.
        /* UNDEFINED ELEMENT: Member at offset 0x4d8 in Builder (Theme Info Ptr) */
        UMapData *ThemeInfo = Builder->_mapData;
        if (ThemeInfo)
        {
            ThemeName = ThemeInfo->ThemeName;
            ThemeWeather = ThemeInfo->ThemeWeather;
        }

        // Access MapName array at offset 0x3f8 in Builder
        /* UNDEFINED ELEMENT: Member at offset 0x3f8 in Builder (TArray<uint16>) */
        MapNameData = Builder->_generatedData.UsedPaperTileMap;

        // Access TileCount at offset 0x648 in Builder
        /* UNDEFINED ELEMENT: Member at offset 0x648 in Builder (int32) */
        TileCount = Builder->_tileSpawnCount;

        // Access LevelDependencies array at offset 0x480 in Builder
        /* UNDEFINED ELEMENT: Member at offset 0x480 in Builder (TArray<FDependency>) */
        TArray<FDependency> BuilderDependencies = Builder->_generatedData.LevelDependencies;

        // ASM performs explicit check to ensure we aren't copying to self, then copies
        if (BuilderDependencies != LevelDependencies)
        {
            LevelDependencies = BuilderDependencies;
        }
    }

    // 4. Update GameState with Gathered Data
    // Re-acquire GameState from Instance (standard pattern in this function's ASM)
    ADBDGameState *TargetGameState = Cast<ADBDGameState>(instance->GetGameState());

    if (TargetGameState)
    {
        // Only update if we have Authority
        if (TargetGameState->Role == ROLE_Authority)
        {
            // Update _builtLevelData struct in GameState
            /* UNDEFINED ELEMENT: _builtLevelData struct at offset 0x628 in ADBDGameState */
            // Struct Layout inferred:
            // 0x00 (0x628): ThemeName
            // 0x08 (0x630): ThemeWeather
            // 0x10 (0x638): MapName (Array)
            // 0x20 (0x648): TileCount
            // 0x28 (0x650): Dependencies (Array)

            FBuiltLevelData &BuiltData = TargetGameState->_builtLevelData;

            BuiltData.ThemeName = ThemeName;
            BuiltData.ThemeWeather = ThemeWeather;
            BuiltData.MapName = MapNameData;
            BuiltData.TileCount = TileCount;
            BuiltData.Dependencies = LevelDependencies;

            // Call Multicast function to replicate data to clients
            // Signature inferred from ASM arguments: (ThemeName, ThemeWeather, MapName, TileCount, Dependencies)
            /* UNDEFINED ELEMENT: Multicast_SetBuiltLevelData function */
            TargetGameState->Multicast_SetBuiltLevelData(ThemeName, ThemeWeather, MapNameData, TileCount, LevelDependencies);
        }
    }

    // 5. Final State Transition
    // Re-acquire GameState again (ASM does this repetitively)
    ADBDGameState *FinalGameState = Cast<ADBDGameState>(instance->GetGameState());

    if (FinalGameState)
    {
        if (FinalGameState->Role == ROLE_Authority)
        {
            // Broadcast "AuthorityOnAllPlayerLoaded" delegate
            // Offset 0x560 contains the delegate
            /* UNDEFINED ELEMENT: FMulticastScriptDelegate at offset 0x560 in ADBDGameState */
            FAllPlayerLoadedEvent &AllPlayersLoadedDelegate = FinalGameState->AuthorityOnAllPlayerLoaded;

            // Invoke the delegate
            if (AllPlayersLoadedDelegate->IsBound())
            {
                AllPlayersLoadedDelegate->Broadcast();
            }
        }
    }

    // Advance Loading Step
    // 2 corresponds to enum value "WaitingForLoadoutAndTheme" based on context
    /* UNDEFINED ELEMENT: Enum/Constant for Loading Step 2 */
    this->SetCurrentLoadingStep(ELevelLoadingSteps::WaitingForLoadoutAndTheme);

    // Create Loadouts
    this->CreatePlayersLoadout();
}




void ADBDGameMode::CheckSurvivorDisconnectBeforeMatchStart()
{
    UDBDGameInstance *DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Access Persistent Data stored in GameInstance at offset 0x3B8.
    // This object contains the TSet of players expected to be in the match.
    /* UNDEFINED ELEMENT: Member at offset 0x3b8 in UDBDGameInstance (UDBDPersistentData*) */
    UDBDPersistentData *PersistentData = DBDGameInstance->_persistentData;

    if (PersistentData == nullptr)
    {
        return;
    }

    // Access the Map using the specific definition provided
    // Offset 0x4C0 is derived from the assembly relative to PersistentData
    /* UNDEFINED ELEMENT: _cachedPlayerScoreMap at offset 0x4c0 */
    TMap<FString, FCachedPlayerScoreData> &CachedPlayerMap = PersistentData->_cachedPlayerScoreMap;

    // Iterate over the cached players using an iterator
    // The assembly performs a bit-scan loop here, which corresponds to TMap/TSet iteration
    for (auto It = CachedPlayerMap.CreateIterator(); It; ++It)
    {
        // The Key is the Player ID as a string
        FString CachedPlayerIdString = It.Key();

        // Convert the String Key to FUniqueNetIdRepl for comparison with PlayerStates
        // The ASM shows construction of FUniqueNetIdRepl wrapper around the string
        FUniqueNetIdRepl CachedNetId;
        CachedNetId.SetUniqueNetId(MakeShareable(new FUniqueNetIdString(CachedPlayerIdString)));

        bool bIsPlayerFoundInMatch = false;

        // Access World and GameState to check active players
        /* UNDEFINED VTABLE: Calling GetWorld() via vtable index [0x108] */
        UWorld *World = this->GetWorld();

        if (World)
        {
            AGameState *GameState = World->GetGameState();

            if (GameState)
            {
                // Iterate through the actual connected players in the match
                for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
                {
                    APlayerState *CurrentPlayerState = GameState->PlayerArray[i];

                    if (CurrentPlayerState)
                    {
                        // Compare current player's UniqueId with the cached one
                        if (CurrentPlayerState->UniqueId == CachedNetId)
                        {
                            bIsPlayerFoundInMatch = true;
                            break;
                        }
                    }
                }
            }
        }

        // If the player from the cache is NOT found in the current match (Disconnected)
        if (bIsPlayerFoundInMatch == false)
        {
            // Prepare the Objective ID
            FName ScoreTypeId = FName(TEXT("Killer_ObjectivePoints_SurvivorDisconnectBeforeMatchStart"));

            // Award points to the Killer
            // Params: ScoreTypeId, VictimId (The disconnected player), Count (1)
            /* UNDEFINED ELEMENT: Function ADBDGameMode::AddToKillerGoalCounter */
            this->AddToKillerGoalCounter(ScoreTypeId, CachedNetId, 1);
        }
    }

    // The assembly ends with a logic block checking the HashSize of the Map and calling Rehash/Compact if needed.
    // This happens at address 0x140249557 (TSet::Rehash).
    if (CachedPlayerMap.Num() > 0)
    {
        CachedPlayerMap.CompactStable();
    }
}




AActor *ADBDGameMode::ChoosePlayerStart_Implementation(AController *Player)
{
    // Check if the player already has a specifically assigned spawn point
    /* UNDEFINED ELEMENT */
    APlayerStart *AssignedStart = this->GetAssignedSpawnPoint(Player);

    if (AssignedStart && AssignedStart->IsPendingKill() == false)
    {
        return AssignedStart;
    }

    // Check if the Game Mode is running in Procedural Generation Mode
    if (this->_proceduralMode == true)
    {
        /* UNDEFINED ELEMENT */
        APlayerStart *KillerSpawn = this->GetKillerSpawnPoint();

        // Cast the Controller's PlayerState to the custom DBD class
        ADBDPlayerState *DBDPlayerState = Cast<ADBDPlayerState>(Player->PlayerState);

        // Get the Game Instance and cast to custom DBD class
        UDBDGameInstance *DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        // Validate necessary objects are present and valid
        if (DBDPlayerState && DBDGameInstance && KillerSpawn && KillerSpawn->IsPendingKill() == false)
        {
            // Accessing the 'GameRole' byte at offset 0x750 in ADBDPlayerState.
            // 0x1 = Killer, 0x3 = Observer (Spectator), Other = Survivor.
            // Using pointer arithmetic to access the specific offset from disassembly.
            EPlayerRole GameRole = DBDPlayerState->GameRole;

            if (GameRole == VE_Slasher) // Killer
            {
                return KillerSpawn;
            }
            else if (GameRole == VE_Observer) // Observer
            {
                /* UNDEFINED ELEMENT */
                return this->GetObserverSpawnPoint();
            }
            else // Survivor
            {
                // Check if a specific Offering (Type 0x16 / 22) is active in the Game Instance
                // 0x16 likely corresponds to an offering like "Shroud of Binding" or "Shroud of Separation"
                /* UNDEFINED ELEMENT */
                bool bHasShroudOffering = DBDGameInstance->HasOfferingOfType(FarKiller);

                if (bHasShroudOffering == true)
                {
                    /* UNDEFINED ELEMENT */
                    return this->GetAvailableStartPoint();
                }
                else
                {
                    // Check if the player has a specific "Far" offering active (Argument 0x4 in bitfield checks usually implies type)
                    // The disassembly passes the PlayerId to a specific overload of HasOfferingOfType
                    /* UNDEFINED ELEMENT */
                    bool bHasFarOffering = DBDGameInstance->HasOfferingOfType(FarKiller, DBDPlayerState->PlayerId);

                    if (bHasFarOffering == true)
                    {
                        // Spawn furthest from the Killer
                        /* UNDEFINED ELEMENT */
                        return this->GetFurthestAvailableStartPoint(KillerSpawn);
                    }
                    else
                    {
                        // Spawn closest to the Killer (default behavior if no Far offering)
                        /* UNDEFINED ELEMENT */
                        return this->GetClosestAvailableStartPoint(KillerSpawn);
                    }
                }
            }
        }

        // If procedural generation objects are invalid, fall through to return nullptr
        return nullptr;
    }
    else
    {
        // Standard / Play In Editor (PIE) Logic
        // Iterate over all APlayerStart actors in the world
        for (TActorIterator<APlayerStart> It(this->GetWorld()); It; ++It)
        {
            APlayerStart *CurrentStart = *It;

            if (this->IsSpawnpointAllowed(CurrentStart, Player))
            {
                return CurrentStart;
            }
        }

        return nullptr;
    }
}




void ADBDGameMode::ClearKillerGoalPoints()
{
    this->_killerGoalPoints = 0;
}




void ADBDGameMode::CompleteEscapeRequirements()
{
    // Retrieve the GameState pointer from the parent class (AGameMode)
    // Offset 0x418 matches AGameMode::GameState in the provided structure
    AGameState* CurrentGameState = this->GameState;

    // Standard null check
    if (CurrentGameState == nullptr)
    {
        return;
    }

    // The disassembly compares ClassTreeIndex against ADBDGameState::GetPrivateStaticClass.
    // In C++, we use Safe Cast to ensure the GameState is of type ADBDGameState.
    ADBDGameState* DBDGameState = Cast<ADBDGameState>(CurrentGameState);

    // If the cast fails (CurrentGameState is not an ADBDGameState), return.
    if (DBDGameState == nullptr)
    {
        return;
    }

    // The disassembly checks the Global Object Array (GUObjectArray) and the IsPendingKill flag (0x8 >> 0x1d & 1).
    // In Unreal Engine, IsValid() handles both the null check, garbage collection validity, and pending kill status.
    if (IsValid(DBDGameState) == false)
    {
        return;
    }

    // The disassembly explicitly checks byte [rbx+0x140] bit 0x4.
    // According to the structure provided (_S), __bitfield140 contains bActorIsBeingDestroyed.
    // 0x4 (binary 100) corresponds to the 3rd bit in that union, which is bActorIsBeingDestroyed.
    if (DBDGameState->IsActorBeingDestroyed() == true)
    {
        return;
    }

    // Call the specific function on the DBDGameState instance.
    // The disassembly uses a tail jump (jmp), effectively transferring control to this function.
    DBDGameState->Authority_SignalEscapeDoorActivated(); /* UNDEFINED ELEMENT */
}




void ADBDGameMode::CreatePlayersLoadout()
{
    // Retrieve the World pointer via virtual function (VTable offset 0x108 corresponds to GetWorld() in AActor)
    UWorld* World = this->GetWorld();

    // Iterate through all controllers in the world
    for (FConstControllerIterator Iterator = World->GetControllerIterator(); Iterator; ++Iterator)
    {
        // Retrieve the controller from the iterator
        AController* Controller = Iterator->Get();

        // Safety check to ensure the controller is valid
        if (Controller == nullptr)
        {
            continue;
        }

        // Retrieve the Pawn controlled by this controller (Offset 0x388 in AController)
        APawn* ControlledPawn = Controller->GetPawn();

        // Attempt to cast the Pawn to ADBDPlayer
        ADBDPlayer* DBDPlayer = Cast<ADBDPlayer>(ControlledPawn);

        // If the pawn is not an ADBDPlayer or is null, skip to the next controller
        if (DBDPlayer == nullptr)
        {
            continue;
        }

        // Standard engine validity check (IsPendingKill, etc.) derived from GUObjectArray checks in disassembly
        if (IsValid(DBDPlayer) == false)
        {
            continue;
        }

        // Specific check for the 'bActorIsBeingDestroyed' flag.
        // In the disassembly (14024c7d8), this checks byte at offset 0x140, bit 0x4.
        if (DBDPlayer->IsActorBeingDestroyed() == true)
        {
            continue;
        }

        // Call an undefined virtual function at offset 0x1070.
        // The disassembly checks the boolean result (test al, al).
        // This likely checks if the loadout is ready to be created or required.
        bool bShouldCreate = DBDPlayer-> /* UNDEFINED VTABLE [Offset 0x1070] */ ();

        if (bShouldCreate == true)
        {
            // Call an undefined virtual function at offset 0xF00.
            // This presumably performs the actual creation of the loadout.
            DBDPlayer->AuthorityHandleLoadoutOnSpawn();
        }
    }
}




APlayerStart* ADBDGameMode::CreatePlayerStart(const USceneComponent* spawnPoint, FName tag)
{
    // Setup Spawn Parameters
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = this->Instigator;
    
    // 0x1 corresponds to ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Logic from 14024c5f2 - 14024c64c:
    // The disassembly manually validates the spawnPoint's internal WorldRotationCache.
    // In functional C++, this is equivalent to calling GetComponentRotation(), which handles the cache update.
    FRotator Rotation = spawnPoint->GetComponentRotation();

    // Logic from 14024c652 - 14024c6ca:
    // Calculates location. It takes the Component location and adds (UpVector * 100.0f).
    // 0x42c80000 is 100.0f in IEEE 754 float.
    FVector Location = spawnPoint->GetComponentLocation() + (FVector::UpVector * 100.0f);

    // Retrieve World via VTable (Offset 0x108 is GetWorld())
    UWorld* World = this->GetWorld();

    class ADBDPlayerStart* NewPlayerStart = nullptr;

    if (World)
    {
        // Call SpawnActor using the ADBDPlayerStart class.
        // The disassembly explicitly gets the private static class for "/Script/DeadByDaylight.DBDPlayerStart".
        NewPlayerStart = World->SpawnActor<ADBDPlayerStart>(ADBDPlayerStart::StaticClass(), Location, Rotation, SpawnParameters);
    }

    if (NewPlayerStart)
    {
        // Offset 0x3a8 typically corresponds to PlayerStartTag in APlayerStart
        NewPlayerStart->PlayerStartTag = tag;

        // Offset 0x3b0 is set to 0. 
        // Without the ADBDPlayerStart structure, the specific member name is unknown.
        NewPlayerStart->UsageCount = 0;
    }

    return NewPlayerStart;
}




void ADBDGameMode::Escaped(class APlayerState* EscapedPlayer)
{
    // Check if the provided pointer is valid before attempting logic
    if (EscapedPlayer)
    {
        // Attempt to cast the generic APlayerState to the specific ADBDPlayerState
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(EscapedPlayer);

        // If the cast is successful, proceed with validity checks
        if (DBDPlayerState)
        {
            // The disassembly performs deep validity checks:
            // 1. Checks GUObjectArray flags (handled by IsValid in UE4).
            // 2. Checks bit 0x4 at offset 0x140, which is bActorIsBeingDestroyed.
            if (IsValid(DBDPlayerState) && DBDPlayerState->IsActorBeingDestroyed() == false)
            {
                // Call a virtual function on the PlayerState.
                // The disassembly sets the second argument (rdx) to 0.
                DBDPlayerState->AuthoritySetInParadise(false);
            }
        }
    }

    // The function ends with a tail jump to CheckGameEnded.
    // This ensures game end conditions are checked regardless of the player's validity.
    this->CheckGameEnded();
}




APlayerStart* ADBDGameMode::GetAssignedSpawnPoint(AController* NewPlayer) const
{
    // Define an array to store the potential spawn points.
    // In the disassembly, this structure is located at [rsp+0x20].
    TArray<APlayerStart*> SurvivorSpawnPoints;
    const_cast<ADBDGameMode*>(this)->GetSurvivorSpawnPoints(SurvivorSpawnPoints);

    // Iterate through the collected spawn points.
    for (int32 i = 0; i < SurvivorSpawnPoints.Num(); ++i)
    {
        class APlayerStart* SpawnPoint = SurvivorSpawnPoints[i];

        // Basic null check (1402556c3)
        if (SpawnPoint == nullptr)
        {
            continue;
        }

        // Attempt to cast to the specific DBD player start class (1402556cf).
        class ADBDPlayerStart* DBDSpawnPoint = Cast<ADBDPlayerStart>(SpawnPoint);

        // If cast fails, skip (1402556e4)
        if (DBDSpawnPoint == nullptr)
        {
            continue;
        }

        // Retrieve the PlayerState from the Controller (Offset 0x3a0)
        // And retrieve the PlayerId from the PlayerState (Offset 0x3a8)
        // The disassembly assumes PlayerState is valid here.
        int32 NewPlayerId = NewPlayer->PlayerState->PlayerId;

        // Compare the PlayerId with a specific member in ADBDPlayerStart at offset 0x3b4.
        // This likely corresponds to an 'AssignedPlayerId' member variable.
        // Since the structure is not provided, we access the offset directly.
        int32 AssignedId = DBDSpawnPoint->AssociatedPlayerId;

        // If the IDs match, we found the assigned spawn point (1402556f9).
        if (AssignedId == NewPlayerId)
        {
            // The TArray destructor handles the memory cleanup (FMemory::Free in 140255723).
            return SpawnPoint;
        }
    }

    // Return nullptr if no matching spawn point is found.
    return nullptr;
}




APlayerStart* ADBDGameMode::GetAvailableStartPoint() const
{
    // Allocate a local array to store the results from the helper function.
    // The disassembly manages this memory manually (allocating stack space and calling FMemory::Free).
    TArray<APlayerStart*> SurvivorSpawnPoints;

    // Call the helper function to populate the array.
    // We use const_cast because 'this' is const in the current function signature,
    // but GetSurvivorSpawnPoints typically implies modification or might not be marked const in this version.
    const_cast<ADBDGameMode*>(this)->GetSurvivorSpawnPoints(SurvivorSpawnPoints);

    // Iterate through all retrieved spawn points.
    for (int32 i = 0; i < SurvivorSpawnPoints.Num(); ++i)
    {
        class APlayerStart* CandidateStart = SurvivorSpawnPoints[i];

        // Basic null check before processing.
        if (CandidateStart == nullptr)
        {
            continue;
        }

        // Attempt to cast the generic APlayerStart to ADBDPlayerStart.
        // The disassembly explicitly checks the class tree index.
        class ADBDPlayerStart* DBDStart = Cast<ADBDPlayerStart>(CandidateStart);

        // If the cast fails, skip to the next item.
        if (DBDStart == nullptr)
        {
            continue;
        }

        // Check a specific integer member at offset 0x3b4.
        // The disassembly compares it to 0xFFFFFFFF (-1).
        // Based on context from previous functions, offset 0x3b4 is likely the 'AssignedPlayerId'.
        // A value of -1 typically implies the spawn point is currently unassigned/available.
        int32 AssignedPlayerId = DBDStart->AssociatedPlayerId; /* UNDEFINED ELEMENT */

        if (AssignedPlayerId == -1)
        {
            // Found a specifically available (unassigned) point. Return it immediately.
            return CandidateStart;
        }
    }

    // Fallback logic:
    // If we iterated through the whole list and didn't find an explicit match (value -1),
    // we return the first spawn point in the list, provided the list is not empty.
    if (SurvivorSpawnPoints.Num() > 0)
    {
        return SurvivorSpawnPoints[0];
    }

    // If the list is empty, return null.
    return nullptr;
}




APlayerStart* ADBDGameMode::GetClosestAvailableStartPoint(APlayerStart* spawnPoint) const
{
    // Initialize the minimum distance to the maximum possible float value.
    // 0x7f7fffff corresponds to FLT_MAX in IEEE 754.
    float MinDistanceSquared = FLT_MAX;

    // Result pointer initialization.
    class APlayerStart* ClosestStartPoint = nullptr;

    // Allocate array for results. Memory is managed manually in disassembly (stack + FMemory::Free),
    // but TArray handles this automatically in C++.
    TArray<APlayerStart*> SurvivorSpawnPoints;

    // Call helper function. We use const_cast because the current function is const,
    // but GetSurvivorSpawnPoints likely modifies internal caching or isn't marked const.
    const_cast<ADBDGameMode*>(this)->GetSurvivorSpawnPoints(SurvivorSpawnPoints);

    // Determine the location of the reference spawnPoint.
    // The disassembly explicitly checks the RootComponent (offset 0x160 in this build).
    FVector GoalLocation = FVector::ZeroVector;

    // Check if spawnPoint is valid.
    if (spawnPoint)
    {
        // In the disassembly (140256003), it accesses the RootComponent directly.
        // If it exists, it copies the Translation (Location).
        USceneComponent* GoalRoot = spawnPoint->GetRootComponent();
        if (GoalRoot)
        {
            GoalLocation = GoalRoot->GetComponentLocation();
        }
    }

    // Iterate through all potential survivor spawn points.
    for (int32 i = 0; i < SurvivorSpawnPoints.Num(); ++i)
    {
        class APlayerStart* CandidatePoint = SurvivorSpawnPoints[i];

        // Basic null check (140255fca).
        if (CandidatePoint == nullptr)
        {
            continue;
        }

        // Cast to specific ADBDPlayerStart class (140255fd6).
        class ADBDPlayerStart* DBDCandidate = Cast<ADBDPlayerStart>(CandidatePoint);

        // If cast fails, skip.
        if (DBDCandidate == nullptr)
        {
            continue;
        }

        // Check availability.
        // The disassembly (140255ff6) compares the int32 at offset 0x3b4 to -1 (0xffffffff).
        // If it is NOT -1, we jump (skip). Therefore, we are looking for -1 (Available/Unassigned).
        // Since the structure is not provided, we access via pointer arithmetic.
        int32 AssignedID = DBDCandidate->AssociatedPlayerId; /* UNDEFINED ELEMENT */

        if (AssignedID != -1)
        {
            continue;
        }

        // Determine the location of the candidate point.
        FVector CandidateLocation = FVector::ZeroVector;
        USceneComponent* CandidateRoot = DBDCandidate->GetRootComponent();

        if (CandidateRoot)
        {
            CandidateLocation = CandidateRoot->GetComponentLocation();
        }

        // Calculate Squared Euclidean Distance (1402560b4 - 1402560db).
        // SSE instructions in disassembly perform: (Goal - Candidate)^2.
        float CurrentDistanceSquared = FVector::DistSquared(GoalLocation, CandidateLocation);

        // Compare with current minimum (1402560df).
        if (CurrentDistanceSquared < MinDistanceSquared)
        {
            MinDistanceSquared = CurrentDistanceSquared;
            ClosestStartPoint = CandidatePoint;
        }
    }

    // TArray destructor handles memory cleanup (FMemory::Free equivalent).
    return ClosestStartPoint;
}




UClass* ADBDGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    // Retrieve the GameInstance and cast it to the custom DBD class
    UGameInstance* GameInstance = GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    ADBDPlayerControllerBase* CastedController = Cast<ADBDPlayerControllerBase>(InController);

    // Validate Controller and GameInstance
    if (CastedController == nullptr || DBDGameInstance == nullptr)
    {
        // Check global logging verbosity (GameFlow). 
        // In disassembly: cmp byte [rel GameFlow], 0x2
        /* UNDEFINED ELEMENT: External Global Variable 'GameFlow' access */
        bool bShouldLog = false; // Placeholder for: GameFlow.Verbosity >= 2

        if (bShouldLog)
        {
            // Log error message: "GetDefaultPawnClassForController called with null controller, or non ADBDPlayer"
            // Disassembly uses FMsg::Logf_Internal__VA
            UE_LOG(LogTemp, Warning, TEXT("GetDefaultPawnClassForController called with null controller, or non ADBDPlayer"));
        }
        return nullptr;
    }

    // Retrieve PlayerState from the Controller
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(InController->PlayerState);

    // Check for Survivor Role (0x2 corresponds to VE_Camper in DBD context)
    // Offset 0x750 is GameRole
    if (DBDPlayerState && DBDPlayerState->GameRole == VE_Camper)
    {
        int32 CamperIndex = CastedController->GetCamperIndex();
        
        // Attempt to get the character class
        UClass* CamperClass = DBDGameInstance->GetGameCharacterSynchronous(CamperIndex);

        // Standard IsValid check (handles the GUObjectArray checks in disassembly)
        if (IsValid(CamperClass))
        {
            return CamperClass;
        }

        // Fallback to random camper if invalid
        int32 RandomCamperIndex = DBDGameInstance->GetRandomCamperIndex();
        return DBDGameInstance->GetGameCharacterSynchronous(RandomCamperIndex);
    }

    // Logic for Non-Survivors
    ADBDPlayerState* ControllerDBDState = CastedController->GetDBDPlayerState();

    // Check for Observer/Spectator Role (0x3)
    if (ControllerDBDState && ControllerDBDState->GameRole == VE_Observer)
    {
        // 0xFFFFFFFF (-1) indicates a default/generic character for observers
        return DBDGameInstance->GetGameCharacterSynchronous(-1);
    }

    // Slasher (Killer) Logic
    int32 SlasherIndex = InController->GetSlasherIndex();

    UWorld* World = this->GetWorld();
    
    if (World->WorldType == PIE && SlasherIndex == -1)
    {
        SlasherIndex = 0x10000000;
    }

    // Attempt to get the Slasher class
    UClass* SlasherClass = DBDGameInstance->GetGameCharacterSynchronous(SlasherIndex);

    if (IsValid(SlasherClass))
    {
        return SlasherClass;
    }

    // Fallback to random slasher
    int32 RandomSlasherIndex = DBDGameInstance->GetRandomSlasherIndex();
    return DBDGameInstance->GetGameCharacterSynchronous(RandomSlasherIndex);
}




FTimespan ADBDGameMode::GetElapsedMatchTime() const
{
    // Get the current UTC time.
    // FDateTime::UtcNow() is a standard Unreal Engine static function.
    FDateTime CurrentTime = FDateTime::UtcNow();

    // Retrieve the stored Match Start Time from the class member.
    // Offset 0x4b8 corresponds to _matchStartTime in the provided structure.
    FDateTime StartTime = this->_matchStartTime;

    // Calculate the elapsed time by subtracting the start time from the current time.
    // In Unreal Engine, the '-' operator for FDateTime returns an FTimespan.
    // Disassembly logic: Result.Ticks = CurrentTime.Ticks - StartTime.Ticks
    FTimespan ElapsedTime = CurrentTime - StartTime;

    return ElapsedTime;
}




APlayerStart* ADBDGameMode::GetFurthestAvailableStartPoint(class APlayerStart* spawnPoint) const
{
    // Initialize the maximum distance to -1.0f (0xbf800000 in IEEE 754).
    // Since squared distances are always non-negative, the first valid candidate will always update this.
    float MaxDistanceSquared = -1.0f;

    class APlayerStart* FurthestStartPoint = nullptr;

    // Allocate array for results.
    // The disassembly handles stack allocation and FMemory::Free manually, which maps to TArray in C++.
    TArray<APlayerStart*> SurvivorSpawnPoints;

    // Call helper function. const_cast is required because 'this' is const,
    // but the helper likely isn't marked const or modifies cached data.
    const_cast<ADBDGameMode*>(this)->GetSurvivorSpawnPoints(SurvivorSpawnPoints);

    // Determine the location of the reference spawnPoint (the origin of the distance check).
    FVector OriginLocation = FVector::ZeroVector;

    // Check if the input spawnPoint is valid.
    if (spawnPoint)
    {
        // Disassembly (140257033) explicitly checks the RootComponent (offset 0x160).
        USceneComponent* OriginRoot = spawnPoint->GetRootComponent();
        if (OriginRoot)
        {
            OriginLocation = OriginRoot->GetComponentLocation();
        }
    }

    // Iterate through all potential survivor spawn points.
    for (int32 i = 0; i < SurvivorSpawnPoints.Num(); ++i)
    {
        class APlayerStart* CandidatePoint = SurvivorSpawnPoints[i];

        // Basic null check.
        if (CandidatePoint == nullptr)
        {
            continue;
        }

        // Cast to specific ADBDPlayerStart class.
        // Disassembly uses GetPrivateStaticClass comparisons.
        class ADBDPlayerStart* DBDCandidate = Cast<ADBDPlayerStart>(CandidatePoint);

        if (DBDCandidate == nullptr)
        {
            continue;
        }

        // Check availability.
        // The disassembly (140257026) compares the int32 at offset 0x3b4 to -1 (0xffffffff).
        // If it is NOT -1, we skip. This implies -1 represents an "Available" or "Unassigned" state.
        int32 AssignedID = DBDCandidate->AssociatedPlayerId; /* UNDEFINED ELEMENT */

        if (AssignedID != -1)
        {
            continue;
        }

        // Determine the location of the candidate point.
        FVector CandidateLocation = FVector::ZeroVector;
        USceneComponent* CandidateRoot = DBDCandidate->GetRootComponent();

        if (CandidateRoot)
        {
            CandidateLocation = CandidateRoot->GetComponentLocation();
        }

        // Calculate Squared Euclidean Distance (140257079 - 14025710b).
        // SSE instructions perform: (Origin - Candidate)^2.
        float CurrentDistanceSquared = FVector::DistSquared(OriginLocation, CandidateLocation);

        // Compare with current maximum (14025710f).
        // jbe (Jump if Below or Equal) skips the update. So we update if Current > Max.
        if (CurrentDistanceSquared > MaxDistanceSquared)
        {
            MaxDistanceSquared = CurrentDistanceSquared;
            FurthestStartPoint = CandidatePoint;
        }
    }

    // TArray destructor handles memory cleanup (FMemory::Free in 140257154).
    return FurthestStartPoint;
}




int32_t ADBDGameMode::GetKillerGoalPoints()
{
    return this->_killerGoalPoints;
}




APlayerStart* ADBDGameMode::GetKillerSpawnPoint() const
{
    // Retrieve the World pointer via virtual function (VTable offset 0x108)
    UWorld* World = this->GetWorld();

    // Iterate through all APlayerStart actors in the world.
    // The disassembly manually constructs FActorIteratorState, which is the underlying mechanism of TActorIterator.
    for (TActorIterator<APlayerStart> It(World); It; ++It)
    {
        class APlayerStart* SpawnPoint = *It;

        // Basic validity check (handled by IsValid in UE4)
        if (IsValid(SpawnPoint) == false)
        {
            continue;
        }

        // Explicit check for bit 0x4 at offset 0x140 (bActorIsBeingDestroyed) seen in disassembly.
        if (SpawnPoint->IsActorBeingDestroyed() == true)
        {
            continue;
        }

        // Retrieve the PlayerStartTag (Offset 0x3a8 in APlayerStart)
        FName PlayerStartTag = SpawnPoint->PlayerStartTag;

        // Logic roughly equivalent to FName::SplitNameWithCheck with "Slasher"
        // We convert to string to perform the analysis seen in the disassembly.
        FString TagString = PlayerStartTag.ToString();
        FString BaseName = TEXT("Slasher");

        if (TagString.StartsWith(BaseName) == true)
        {
            // Extract the suffix part of the name
            FString Suffix = TagString.RightChop(BaseName.Len());

            // Ensure the suffix is numeric to match the number extraction logic
            if (Suffix.IsNumeric() == true)
            {
                // Convert suffix to integer
                int32 ExtractedNumber = FCString::Atoi(*Suffix);

                // The disassembly increments the extracted number by 1 before comparison
                // 14025822f: inc r14d
                int32 CalculatedIndex = ExtractedNumber + 1;

                // Compare against the integer at offset 0x3ac.
                // Since the specific class definition for ADBDPlayerStart is not provided,
                // and standard APlayerStart does not have a member at 0x3ac, we access it via pointer arithmetic.
                int32 StoredIndex = SpawnPoint->PlayerStartTag.Number; /* UNDEFINED ELEMENT */

                if (CalculatedIndex == StoredIndex)
                {
                    // If the Calculated Index (Suffix + 1) matches the Stored Index, we found the correct point.
                    return SpawnPoint;
                }
            }
        }
    }

    // Return nullptr if no matching spawn point is found.
    return nullptr;
}




APlayerStart* ADBDGameMode::GetObserverSpawnPoint() const
{
    // Retrieve the World pointer via virtual function (VTable offset 0x108)
    UWorld* World = this->GetWorld();

    // Iterate through all APlayerStart actors in the world.
    // The disassembly explicitly loads the class for "/Script/Engine.PlayerStart".
    for (TActorIterator<APlayerStart> It(World); It; ++It)
    {
        class APlayerStart* SpawnPoint = *It;

        // Basic validity check (handled by IsValid in UE4)
        if (IsValid(SpawnPoint) == false)
        {
            continue;
        }

        // Explicit check for bit 0x4 at offset 0x140 (bActorIsBeingDestroyed)
        if (SpawnPoint->IsActorBeingDestroyed() == true)
        {
            continue;
        }

        // Retrieve the PlayerStartTag (Offset 0x3a8 in APlayerStart)
        FName PlayerStartTag = SpawnPoint->PlayerStartTag;

        // Logic roughly equivalent to FName::SplitNameWithCheck with "Observer"
        // We simulate the string analysis performed in the disassembly.
        FString TagString = PlayerStartTag.ToString();
        FString BaseName = TEXT("Observer");

        // Check if the tag starts with "Observer"
        if (TagString.StartsWith(BaseName) == true)
        {
            // Extract the suffix part of the name
            FString Suffix = TagString.RightChop(BaseName.Len());

            // Ensure the suffix is numeric to extract the number
            if (Suffix.IsNumeric() == true)
            {
                // Convert suffix to integer
                int32 ExtractedNumber = FCString::Atoi(*Suffix);

                // The disassembly increments the extracted number by 1 before comparison
                // 14025b2df: inc r14d
                int32 CalculatedIndex = ExtractedNumber + 1;

                // Compare against the integer at offset 0x3ac.
                // This member is not part of standard APlayerStart, accessed via pointer arithmetic.
                int32 StoredIndex = SpawnPoint->PlayerStartTag.Number; /* UNDEFINED ELEMENT */

                if (CalculatedIndex == StoredIndex)
                {
                    // If the match is confirmed, return this spawn point.
                    return SpawnPoint;
                }
            }
        }
    }

    // Return nullptr if no matching spawn point is found.
    return nullptr;
}




int32 ADBDGameMode::GetPlayerReadyCount(UDBDGameInstance* instance)
{
    // Clear the debug string that tracks player readiness state
    // Offset 0x4A0 corresponds to _readyDebugString (FString)
    this->_readyDebugString.Empty();

    int32 ReadyCount = 0;

    // Retrieve the World from the VTable (Offset 0x108)
    UWorld* World = this->GetWorld();

    // Iterate over all controllers in the world
    for (FConstControllerIterator Iterator = World->GetControllerIterator(); Iterator; ++Iterator)
    {
        AController* Controller = Iterator->Get();

        // Safety check for controller validity
        if (Controller == nullptr)
        {
            continue;
        }

        // Initialize a temporary string key for this specific controller's status (e.g., "1" or "0")
        FString PlayerStatusKey = TEXT("");

        // Attempt to get PlayerData.
        // Disassembly passes the object at offset 0x388 of the Controller. 
        // In UE4.13 AController, 0x388 is typically the Pawn.
        APawn* ControlledPawn = Controller->GetPawn(); 
        UDBDPlayerData* PlayerData = UDBDPlayerData::GetPlayerData(ControlledPawn);

        // Standard validity check (IsPendingKill check at 0x140 bit 4 seen in disassembly)
        bool bIsPlayerDataValid = false;
        if (PlayerData)
        {
            // Check IsPendingKill (GUObjectArray check in disassembly) and bActorIsBeingDestroyed
            if (IsValid(PlayerData) == true && PlayerData->IsActorBeingDestroyed() == false)
            {
                bIsPlayerDataValid = true;
            }
        }

        // --- Branch: PlayerData is INVALID ---
        // This usually handles Observers or players without a valid pawn/character yet
        if (bIsPlayerDataValid == false)
        {
            // Access PlayerState at offset 0x3A0
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(Controller->PlayerState);

            // Validate PlayerState and check IsPendingKill
            bool bIsPlayerStateValid = false;
            if (DBDPlayerState)
            {
                if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
                {
                    bIsPlayerStateValid = true;
                }
            }

            // Check if the player is an Observer (GameRole == 3 at offset 0x750)
            if (bIsPlayerStateValid == true && DBDPlayerState->GameRole == VE_Observer) /* UNDEFINED ELEMENT: VE_Observer */
            {
                // Append "Observer" to the main debug string
                this->_readyDebugString += TEXT("Observer");
            }
            else
            {
                // Append error message for invalid pawn
                this->_readyDebugString += TEXT("Invalid Pawn (Check CharacterDB)");
            }
        }
        // --- Branch: PlayerData is VALID ---
        else
        {
            // Check the current loading step of the GameMode
            // Offset 0x472 corresponds to _currentLevelLoadingStep enum
            ELevelLoadingSteps CurrentStep = this->_currentLevelLoadingStep;
            
            bool bIsReady = false;

            // Logic for Step 1: Level Loading
            if (CurrentStep == WaitingForPlayersToBeSpawned)
            {
                // Check Verbosity for LogLoading category (Global variable check in disassembly)
                /* UNDEFINED ELEMENT: if (LogLoading.Verbosity >= 5) */
                {
                    // Access _gameLevelLoaded at offset 0xF1 in UDBDPlayerData
                    // Logs: "User: %s %s: %i"
                    UE_LOG(LogLoading, Verbose, TEXT("User: %s %s: %i"), 
                        *ControllerExt::GetPlayerName(Controller), 
                        *Enum::ToString(this->_currentLevelLoadingStep), 
                        PlayerData->_gameLevelLoaded);
                }

                if (PlayerData->_gameLevelLoaded != 0)
                {
                    bIsReady = true;
                }
            }
            // Logic for Step 2: Spawning (Loadout & Light)
            else if (CurrentStep == 2)
            {
                /* UNDEFINED ELEMENT: if (LogLoading.Verbosity >= 5) */
                {
                    // Logs: "User: %s %s: (IsLoadoutSpawned: %i - IsLightSpawned: %i)"
                    UE_LOG(LogLoading, Verbose, TEXT("User: %s %s: (IsLoadoutSpawned: %i - IsLightSpawned: %i)"), 
                        *ControllerExt::GetPlayerName(Controller), 
                        *Enum::ToString(this->_currentLevelLoadingStep), 
                        UDBDPlayerData::IsLoadoutSpawned(PlayerData),
                        UDBDPlayerData::IsLightSpawned(PlayerData));
                }

                if (UDBDPlayerData::IsLoadoutSpawned(PlayerData) == true && 
                    UDBDPlayerData::IsLightSpawned(PlayerData) == true)
                {
                    bIsReady = true;
                }
            }
            // Logic for Step 3: Intro Completion
            else if (CurrentStep == WaitingForIntroToBeDone)
            {
                /* UNDEFINED ELEMENT: if (LogLoading.Verbosity >= 5) */
                {
                    // Logs: "User: %s %s: %i"
                    UE_LOG(LogLoading, Verbose, TEXT("User: %s %s: %i"), 
                        *ControllerExt::GetPlayerName(Controller), 
                        *Enum::ToString(this->_currentLevelLoadingStep), 
                        UDBDPlayerData::IsIntroCompleted(PlayerData));
                }

                if (UDBDPlayerData::IsIntroCompleted(PlayerData))
                {
                    bIsReady = true;
                }
            }

            // Update ready count and status string
            if (bIsReady == true)
            {
                ReadyCount = ReadyCount + 1;
                PlayerStatusKey.AppendInt(1);
            }
            else
            {
                PlayerStatusKey.AppendInt(0);
            }
        }

        // Separator logic: If the debug string is not empty, append a separator
        if (this->_readyDebugString.IsEmpty() == false)
        {
            // The disassembly loads a string that contains " : " or " | "
            this->_readyDebugString += TEXT(" | ");
        }

        // Append the specific status key for this controller
        if (PlayerStatusKey.IsEmpty() == false)
        {
            this->_readyDebugString += PlayerStatusKey;
        }
    }

    // Get total number of players in the game state
    // Offset 0x418 is GameState, 0x3B8 is PlayerArray.Num()
    int32 TotalPlayers = 0;
    if (this->GameState)
    {
        TotalPlayers = this->GameState->PlayerArray.Num();
    }

    // Final Log construction
    FString StepString = Enum::ToString(this->_currentLevelLoadingStep);
    
    // Log formatted string
    // "LoadingStatus -> %s current client ready count : %d / %d : %s"
    FString LogMessage = FString::Printf(TEXT("LoadingStatus -> %s current client ready count : %d / %d : %s"), 
        *StepString, 
        ReadyCount, 
        TotalPlayers, 
        *this->_readyDebugString);

    // Send to Analytics
    UDBDAnalytics::Log(TEXT("LoadingStatus"), TEXT("ReadyState"), LogMessage);

    return ReadyCount;
}




AProceduralLevelBuilder* ADBDGameMode::GetProceduralLevelBuilder()
{
    // Retrieve the GameInstance associated with this GameMode.
    // In the disassembly: call AActor::GetGameInstance
    UGameInstance* GameInstance = this->GetGameInstance();

    // Attempt to cast the generic GameInstance to the specific UDBDGameInstance.
    // The disassembly explicitly compares the class tree against "/Script/DeadByDaylight.DBDGameInstance".
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // If the cast fails or the instance is null, return nullptr.
    if (DBDGameInstance == nullptr)
    {
        return nullptr;
    }

    // The disassembly performs a specific check against the Global Object Array (GUObjectArray).
    // It checks if a specific flag bit (likely PendingKill) is set.
    // In Unreal Engine C++, IsValid() encapsulates null checks and PendingKill checks.
    if (IsValid(DBDGameInstance) == false)
    {
        return nullptr;
    }

    // The function calls FWeakObjectPtr::Get() on a member located at offset 0x178 of the DBDGameInstance.
    // Since the structure of UDBDGameInstance is not provided, we access this member using pointer arithmetic.
    // The member is treated as a TWeakObjectPtr based on the function call 'FWeakObjectPtr::Get'.
    
    /* UNDEFINED ELEMENT: Accessing unknown member at offset 0x178 of UDBDGameInstance */

    TWeakObjectPtr<AProceduralLevelBuilder> ProceduralLevelBuilderWeakPtr = DBDGameInstance->Builder;

    // Retrieve the object from the weak pointer.
    // The disassembly uses a tail jump (jmp) to FWeakObjectPtr::Get, returning the UObject*.
    // We cast it to the expected return type AProceduralLevelBuilder*.
    UObject* BuilderObject = ProceduralLevelBuilderWeakPtr->Get();

    return static_cast<AProceduralLevelBuilder*>(BuilderObject);
}




TArray<APlayerStart*> ADBDGameMode::GetSurvivorSpawnPoints() const
{
    TArray<APlayerStart*> Result;

    // Retrieve the World pointer via virtual function (VTable offset 0x108)
    UWorld* World = this->GetWorld();

    // Iterate through all APlayerStart actors in the world.
    // The disassembly loads the class for "/Script/Engine.PlayerStart".
    for (TActorIterator<APlayerStart> It(World); It; ++It)
    {
        class APlayerStart* SpawnPoint = *It;

        // Basic validity check (handled by IsValid in UE4)
        if (IsValid(SpawnPoint) == false)
        {
            continue;
        }

        // Explicit check for bit 0x4 at offset 0x140 (bActorIsBeingDestroyed)
        if (SpawnPoint->IsActorBeingDestroyed() == true)
        {
            continue;
        }

        // Retrieve the PlayerStartTag (Offset 0x3a8 in APlayerStart)
        FName PlayerStartTag = SpawnPoint->PlayerStartTag;

        // Logic roughly equivalent to FName::SplitNameWithCheck with "Camper"
        // We simulate the logic derived from the assembly:
        // 1. Check if name starts with "Camper".
        // 2. Extract number.
        // 3. Compare with internal ID at 0x3ac.
        
        FString TagString = PlayerStartTag.ToString();
        FString BaseName = TEXT("Camper");
        int32 CalculatedIndex = 0;
        bool bMatchesBaseName = false;

        // Check if the tag is exactly "Camper"
        if (TagString.Equals(BaseName, ESearchCase::IgnoreCase))
        {
            CalculatedIndex = 0;
            bMatchesBaseName = true;
        }
        // Check if it is "Camper" followed by a number (e.g. "Camper1")
        else if (TagString.StartsWith(BaseName, ESearchCase::IgnoreCase))
        {
            FString Suffix = TagString.RightChop(BaseName.Len());
            if (Suffix.IsNumeric())
            {
                // Disassembly behavior: If split succeeds, index = Number + 1.
                // Example: "Camper1" -> 1 -> Index 2. 
                CalculatedIndex = FCString::Atoi(*Suffix) + 1;
                bMatchesBaseName = true;
            }
        }

        if (bMatchesBaseName)
        {
            // Compare against the integer at offset 0x3ac.
            // This member is not part of standard APlayerStart, likely 'int32 ID' in ADBDPlayerStart.
            // Accessed via pointer arithmetic as the structure is not provided.
            int32 StoredIndex = SpawnPoint.PlayerStartTag.Number; /* UNDEFINED ELEMENT */

            if (CalculatedIndex == StoredIndex)
            {
                Result.Add(SpawnPoint);
            }
        }
    }

    return Result;
}




void ADBDGameMode::GuidedActionModified()
{
    // The disassembly shows a direct jump to CheckGameEnded.
    // This is a tail call optimization. The function effectively just calls CheckGameEnded.
    this->CheckGameEnded();
}




void ADBDGameMode::HandleMatchEnded(uint8 serverHasLeftMatch)
{
    // Update the session joinability (FALSE/0 passed in disassembly 14025e857)
    this->UpdateJoinability(false);

    // Notify all players that the game has ended
    this->SetPlayersGameEnded((bool)serverHasLeftMatch);

    // Retrieve the GameInstance
    UGameInstance* GameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // Ensure GameInstance is valid before proceeding with LocalPlayer logic
    if (DBDGameInstance)
    {
        // Retrieve the first LocalPlayer
        ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(0);

        if (LocalPlayer)
        {
            // Create a LocalPlayerContext to safely retrieve the Pawn
            FLocalPlayerContext PlayerContext(LocalPlayer, nullptr); // Disassembly creates this on stack
            APawn* LocalPawn = PlayerContext.GetPawn();

            // Cast to ADBDPlayer
            ADBDPlayer* DBDPlayer = Cast<ADBDPlayer>(LocalPawn);

            // Validity check: IsValid + explicit check for 'bActorIsBeingDestroyed' (bit 0x4 at offset 0x140)
            if (IsValid(DBDPlayer) && !DBDPlayer->IsActorBeingDestroyed())
            {
                // Call two undefined virtual functions. If both return true, notify match ended locally.
                // 1. VTable offset 0x1070
                // 2. VTable offset 0x630
                /* UNDEFINED VTABLE */
                bool bCheck1 = DBDPlayer->IsValidImpl();
                bool bCheck2 = DBDPlayer->IsLocallyControlled();

                if (bCheck1 && bCheck2)
                {
                    DBDPlayer->Local_NotifyMatchEnded();
                }
            }
        }
    }

    // Refresh GameInstance pointer (disassembly re-acquires it or restores from stack)
    // Validate DBDGameInstance again
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Check for a specific Offering/Modifier.
    // The pseudo-code suggests "KillerSelectionModifier".
    // 0xFFFFFFFF passed as the second int arg.
    /* UNDEFINED ELEMENT: Variable 'KillerSelectionModifier' assumed from context */
    FName OfferingValue = DBDGameInstance->GetOfferingsValue("KillerSelectionModifier", -1);

    // If the offering value is "None" (Index 0, Number 0), return early.
    if (OfferingValue.IsNone())
    {
        return;
    }

    // Check LocalPlayer again. If null, return.
    ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(0);
    if (LocalPlayer == nullptr)
    {
        return;
    }

    // Retrieve PlayerState from the LocalPlayer's Controller
    APlayerController* PC = LocalPlayer->PlayerController;
    if (PC == nullptr || PC->PlayerState == nullptr)
    {
        return;
    }

    APlayerState* PlayerState = PC->PlayerState;

    // Validate PlayerState (IsValid + bActorIsBeingDestroyed check)
    if (!IsValid(PlayerState) || PlayerState->IsActorBeingDestroyed())
    {
        return;
    }

    // Retrieve the PlayerPersistentData.
    // The function UDBDPersistentData::GetPlayerPersistentData takes an unknown object 
    // located at DBDGameInstance + 0x3b8 and the Player's UniqueId.
    /* UNDEFINED ELEMENT: Unknown member at 0x3b8 in UDBDGameInstance */
    UDBDPersistentData* PersistentDataManager = DBDGameInstance->_persistentData;
    
    // Pass the UniqueNetId from the PlayerState
    FUniqueNetIdRepl UniqueId = PlayerState->UniqueId;

    FPlayerPersistentData* PersistentData = PersistentDataManager->GetPlayerPersistentData(UniqueId);

    // If data was retrieved successfully, update the SelectedSlasherIndex.
    if (PersistentData)
    {
        // Offset 0x18c corresponds to SavedData.SharedData.SelectedSlasherIndex in the provided structure hints.
        // Value 0x10000000 (268435456) is assigned.
        PersistentData->SavedData.SharedData.SelectedSlasherIndex = 0x10000000;
    }
}




void ADBDGameMode::HostGame()
{
    // Verify that this instance has authority (ROLE_Authority == 3).
    // Offset 0x110 corresponds to AActor::Role.
    if (this->Role == ROLE_Authority)
    {
        // Retrieve the number of players currently in the GameState.
        // Offset 0x418 is GameState. Offset 0x3B8 in AGameState is PlayerArray.
        int32 PlayerCount = 0;
        if (this->GameState)
        {
            PlayerCount = this->GameState->PlayerArray.Num();
        }

        // Retrieve the GameInstance.
        UGameInstance* GameInstance = this->GetGameInstance();
        
        // Attempt to cast to the specific UDBDGameInstance.
        // Disassembly performs GetPrivateStaticClass check.
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

        if (DBDGameInstance)
        {
            // The disassembly accesses a pointer at offset 0x3B8 of UDBDGameInstance,
            // then writes the PlayerCount to offset 0x28 of that internal object.
            // Since the specific structure of UDBDGameInstance is not provided, pointer arithmetic is used.
            
            /* UNDEFINED ELEMENT: Updating unknown internal session/state member in UDBDGameInstance */
            UDBDPersistentData* PersistentData = DBDGameInstance->_persistentData;
            if (PersistentData)
            {
                PersistentData->_gamePersistentData.PlayerCount = PlayerCount;
            }
        }

        // Retrieve the World.
        UWorld* World = this->GetWorld();

        if (World)
        {
            // Construct the URL for ServerTravel.
            // The disassembly references global constants `UIConstants::MAPS_ROOT_0` and `UIConstants::MAP_NAME_PARADISE_0`.
            // It manually concatenates them into an FString (which wraps TArray<TCHAR>).
            
            /* UNDEFINED ELEMENT: Global constants UIConstants::MAPS_ROOT_0 / MAP_NAME_PARADISE_0 */
            
            // Logic equivalent to: FString MapURL = UIConstants::MAPS_ROOT_0 + UIConstants::MAP_NAME_PARADISE_0;
            // Assuming these constants form a valid map path (e.g., "/Game/Maps/Paradise").
            FString MapURL = UIConstants::MAPS_ROOT_0 + UIConstants::MAP_NAME_PARADISE_0;
            
            // Note: The disassembly shows manual array resizing and memcpy operations here 
            // to build this string from the static data blocks. 
            // For functional reconstruction, standard string concatenation is assumed.
            
            // Call ServerTravel on the World.
            // VTable offset 0x1F8 in UWorld corresponds to ServerTravel.
            // Arguments: URL, bAbsolute (false), bShouldSkipGameNotify (false).
            World->ServerTravel(MapURL, false, false);
        }
    }
}




void ADBDGameMode::InitGameState()
{
    // Call the parent implementation first
    Super::InitGameState();

    // Reset the network fence, likely for replication synchronization
    /* UNDEFINED ELEMENT: ANetworkFenceActor::Authority_ResetFence() */
    ANetworkFenceActor::Authority_ResetFence();

    // Retrieve and cast the GameState
    AGameState* CurrentGameState = this->GameState;
    ADBDGameState* DBDGameState = Cast<ADBDGameState>(CurrentGameState);

    // Validate GameState (Check null, GUObjectArray validity, and PendingKill/Destroyed flags)
    if (DBDGameState && IsValid(DBDGameState) && !DBDGameState->IsActorBeingDestroyed())
    {
        // Generate a new Session ID
        FGuid NewSessionId = FGuid::NewGuid();

        // Store the Session ID in the GameState at offset 0x6d8
        // Since the ADBDGameState structure is not provided, we use pointer arithmetic.
        DBDGameState->_sessionId = NewSessionId;

        // Update the global analytics session ID
        UDBDAnalytics::_sessionId = NewSessionId;
    }

    // Retrieve GameInstance
    UGameInstance* GameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // Validate GameInstance
    if (DBDGameInstance && IsValid(DBDGameInstance))
    {
        // Retrieve the GameEventTracker from the GameInstance (Offset 0x108)
        UGameEventTracker* EventTracker = DBDGameInstance->GameEventTracker;

        if (EventTracker)
        {
            // Bind 'OnGameplayEvent' to the dynamic delegate in the EventTracker.
            // Offset 0x28 in UGameEventTracker corresponds to OnGameplayEvent.
            // The disassembly uses string manipulation to find the function name, which is standard behavior
            // for the AddDynamic macro in UE4.
            
            EventTracker->OnGameplayEvent.AddDynamic(this, &ADBDGameMode::OnGameplayEvent);
        }
    }
}




FString ADBDGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    // Retrieve the GameInstance and cast it to the custom DBD type
    UGameInstance* GameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // Retrieve the PlayerState from the new controller and cast it to the custom DBD type
    APlayerState* PlayerState = NewPlayerController->PlayerState;
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);

    // Check if both the GameInstance and PlayerState are valid and cast successfully
    if (DBDGameInstance)
    {
        if (DBDPlayerState)
        {
            // Parse the "Role" option from the login options string
            // "LoginOptionsKeys::ROLE" is likely a static const FString key defined elsewhere
            /* UNDEFINED ELEMENT */
            FString RoleOptionString = UGameplayStatics::ParseOption(Options, LoginOptionsKeys::ROLE);
            
            int32 RoleOptionValue = 0;
            if (RoleOptionString.IsEmpty() == false)
            {
                RoleOptionValue = FCString::Atoi(*RoleOptionString);
            }

            // Parse the "Name" option from the login options string
            FString NameOptionString = UGameplayStatics::ParseOption(Options, TEXT("Name"));

            // If the name is missing, fallback to a default value (source indicates a global variable in_pszStateGroup)
            if (NameOptionString.IsEmpty() == true)
            {
                /* UNDEFINED ELEMENT */
                NameOptionString = FString(in_pszStateGroup); //Wwise thing
            }

            // Truncate the name to 20 characters if it exceeds the limit
            if (NameOptionString.Len() > 20)
            {
                NameOptionString = NameOptionString.Left(20);
            }

            // Note: The disassembly performs a copy of the name to a local buffer here, 
            // but effectively only the Role logic proceeds to use external calls in this block.

            // If the requested role index is greater than 1, attempt to load persistent data
            if (RoleOptionValue > 1)
            {
                // Create a copy of the UniqueId to pass to the persistent data manager
                FUniqueNetIdRepl PlayerUniqueId = DBDPlayerState->UniqueId;

                // Retrieve persistent data for this player
                // UDBDPersistentData is a custom class, GetPlayerPersistentData is a static method
                /* UNDEFINED ELEMENT */
                FPlayerPersistentData* PersistentData = UDBDPersistentData::GetPlayerPersistentData(PlayerUniqueId);

                if (PersistentData)
                {
                    // Check if the controller is local to determine how data is copied
                    bool bIsLocalController = NewPlayerController->IsLocalController();

                    // Copy data from persistent storage to the PlayerState
                    /* UNDEFINED ELEMENT */
                    UDBDPersistentData::CopyData(PersistentData, DBDPlayerState, bIsLocalController);

                    // Set the GameRole on the PlayerState based on the loaded persistent data
                    // 0 = false (likely bForce or similar boolean flag)
                    /* UNDEFINED ELEMENT */
                    DBDPlayerState->SetGameRole(PersistentData->GameRole, false);
                }
            }
        }
    }

    // Fallback logic for assigning a Role if one hasn't been assigned yet
    if (DBDPlayerState)
    {
        // Check if the GameRole is currently 0 (VE_None). 
        // 0x750 offset in disassembly likely corresponds to the GameRole property or a replicated component holding it.
        if (DBDPlayerState->GameRole == EPlayerRole::VE_None)
        {
            // Default role is set to 2 (Likely VE_Camper/Survivor based on context)
            EPlayerRole NewRole = EPlayerRole::VE_Camper;

            // Check if the game mode is configured to start with a Slasher
            if (this->StartWithSlasher == true)
            {
                // Retrieve the GameState to check the player count
                AGameState* CurrentGameState = this->GetGameState();

                // If this is the first player joining (PlayerArray.Num() == 1), assign them as Slasher (1)
                // 1 = Likely VE_Slasher/Killer
                if (CurrentGameState)
                {
                    if (CurrentGameState->PlayerArray.Num() == 1)
                    {
                        NewRole = EPlayerRole::VE_Slasher;
                    }
                }
            }

            // Apply the determined role to the PlayerState
            DBDPlayerState->SetGameRole(NewRole, false);
        }
    }

    // Call the base class implementation of InitNewPlayer
    return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}




void ADBDGameMode::InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer)
{
    // Check if the StartSpot pointer is null before proceeding
    if (StartSpot == nullptr)
    {
        return;
    }

    // Attempt to cast the generic AActor to the specific ADBDPlayerStart class
    // The disassembly explicitly retrieves and checks against ADBDPlayerStart::GetPrivateStaticClass
    ADBDPlayerStart* DBDPlayerStart = Cast<ADBDPlayerStart>(StartSpot);

    // If the cast is successful, proceed with logic
    if (DBDPlayerStart)
    {
        // The disassembly performs manual checks on GUObjectArray, InternalIndex, and ObjectFlags (0x20000000 and 0x4)
        // These checks correspond to validating that the object is not PendingKill or Unreachable.
        // We use the standard UE4 IsValid() function which encapsulates these checks.
        if (IsValid(DBDPlayerStart) == true)
        {
            // Similarly check if the NewPlayer controller is valid based on the flags check in disassembly
            if (IsValid(NewPlayer) == true)
            {
                // Access the PlayerState from the Controller (Offset 0x3A0 in AController)
                APlayerState* PlayerState = NewPlayer->PlayerState;

                if (PlayerState)
                {
                    // Retrieve the PlayerId (Offset 0x3A8 in APlayerState)
                    int32 PlayerId = PlayerState->PlayerId;

                    // Store the PlayerId into the ADBDPlayerStart instance
                    // The disassembly writes the PlayerId (ecx) to offset 0x3B4 of the StartSpot (rbx)
                    // We assume the member variable at this offset is named PlayerId or similar
                    /* UNDEFINED ELEMENT */
                    DBDPlayerStart->AssociatedPlayerId = PlayerId;
                }
            }
        }
    }
}




bool ADBDGameMode::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player)
{
    // Retrieve the PlayerState from the provided Controller
    // Disassembly accesses offset 0x3A0 of AController (standard Engine offset for PlayerState)
    APlayerState* GenericPlayerState = Player->PlayerState;

    // Attempt to cast the generic PlayerState to the specific ADBDPlayerState
    // Disassembly calls ADBDPlayerState::GetPrivateStaticClass to verify the type
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(GenericPlayerState);

    // If the PlayerState is invalid or casting failed, return false immediately
    if (DBDPlayerState == nullptr)
    {
        return false;
    }

    // Retrieve the player's role from the ADBDPlayerState
    // Disassembly accesses a byte at offset 0x750. Based on logic (1, 2, 3), this represents the GameRole.
    /* UNDEFINED ELEMENT */
    EPlayerRole GameRole = DBDPlayerState->GameRole;

    // Check logic for Role 1 (Slasher)
    if (GameRole == VE_Slasher)
    {
        // Check if the SpawnPoint's tag matches "Slasher"
        // Disassembly calls FName::operator== with "Slasher"
        if (SpawnPoint->PlayerStartTag == FName("Slasher"))
        {
            return true;
        }
    }
    // Check logic for Role 2 (Camper/Survivor)
    else if (GameRole == VE_Camper)
    {
        // Check if the SpawnPoint's tag matches "Camper"
        // Disassembly calls FName::operator== with "Camper"
        if (SpawnPoint->PlayerStartTag == FName("Camper"))
        {
            return true;
        }
    }
    // Check logic for Role 3 (Observer/Spectator)
    else if (GameRole == VE_Observer)
    {
        // Check if the SpawnPoint's tag matches "Observer"
        // Disassembly calls FName::operator== with "Observer"
        if (SpawnPoint->PlayerStartTag == FName("Observer"))
        {
            return true;
        }
    }

    // If no role matched the specific spawn tag requirements, return false
    return false;
}




void ADBDGameMode::Killed(APlayerState* KilledPlayer, bool shouldCheckGameEnded)
{
    // Attempt to cast the generic PlayerState to the specific ADBDPlayerState
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(KilledPlayer);

    // Validate the pointer and ensure the object is not pending kill
    // The disassembly performs checks on Flags (0x20000000) and InternalIndex to ensure validity
    if (IsValid(DBDPlayerState) == true)
    {
        // Call a virtual function on the PlayerState.
        // The disassembly loads the VTable (mov rax, [rbx]) and calls the function at offset 0x750.
        // The argument passed is 1 (mov dl, 1), which likely represents a state (e.g., Dead).
        /* UNDEFINED VTABLE */
        DBDPlayerState->AuthoritySetInParadise(true);
    }

    // Check if we should verify the game ending condition
    if (shouldCheckGameEnded == true)
    {
        // Call the protected/internal function to check if the game has ended
        /* UNDEFINED ELEMENT */
        this->CheckGameEnded();
    }
}




void ADBDGameMode::LevelLoaded(const FString& levelName)
{
    // The disassembly loads the VTable and jumps to the function at offset 0x640.
    // According to the provided VTable, offset 0x640 corresponds to the RestartGame function.
    // The use of 'jmp' indicates a tail call optimization. 
    // While 'levelName' is passed in registers, RestartGame does not take parameters 
    // in the provided context, so the argument is effectively ignored.
    this->RestartGame();
}




APlayerController* ADBDGameMode::Login(UPlayer* NewPlayer, ENetRole RemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    // Retrieve the current network mode of the actor
    // 2 corresponds to NM_ListenServer in UE4 ENetMode enum
    ENetMode NetMode = this->GetNetMode();

    // Retrieve the World from the new player object
    UWorld* World = NewPlayer->GetWorld();

    // Check if the game is running as a Listen Server or if the World is in PlayInEditor (PIE) mode.
    // 3 corresponds to EWorldType::PIE
    bool bIsListenServerOrPIE = false;
    
    if (NetMode == ENetMode::NM_ListenServer)
    {
        bIsListenServerOrPIE = true;
    }
    else
    {
        if (World)
        {
            if (World->WorldType == EWorldType::PIE)
            {
                bIsListenServerOrPIE = true;
            }
        }
    }

    // If it is NOT a Listen Server and NOT PlayInEditor mode, override the default classes.
    // This logic ensures specific classes are used in Standalone or Dedicated Server environments 
    // likely to enforce standard behavior before the specific game logic takes over.
    if (bIsListenServerOrPIE == false)
    {
        // Set the HUD Class to the base AHUD class
        // Disassembly: AHUD::GetPrivateStaticClass(L"/Script/Engine")
        this->HUDClass = AHUD::StaticClass();

        // Set the PlayerState Class to the custom ADBDPlayerState class
        // Disassembly: ADBDPlayerState::GetPrivateStaticClass(L"/Script/DeadByDaylight")
        this->PlayerStateClass = ADBDPlayerState::StaticClass();
    }

    // Call the base AGameMode::Login function to process the login and create the PlayerController
    APlayerController* NewPlayerController = Super::Login(NewPlayer, RemoteRole, Portal, Options, UniqueId, ErrorMessage);

    // Validate that the PlayerController was created successfully
    // The disassembly performs manual IsValid checks (checking GUObjectArray and flags)
    if (NewPlayerController)
    {
        if (IsValid(NewPlayerController) == true)
        {
            // Force the HUD class on the client to match the GameMode's current HUDClass
            // This ensures the client gets the correct HUD even if overrides happened above
            NewPlayerController->ClientSetHUD(this->HUDClass);
        }
    }

    return NewPlayerController;
}




void ADBDGameMode::OnGameplayEvent(EDBDScoreTypes gameplayEventType, float amount, AActor* instigator, AActor* target)
{
    // The logic attempts to find a relevant "Subject" actor. 
    // It prefers the instigator, but falls back to the target if the instigator is not a valid ACamperPlayer.
    AActor* SubjectActor = nullptr;

    // Check if instigator is valid and is of type ACamperPlayer
    // The disassembly explicitly checks against ACamperPlayer::GetPrivateStaticClass() and validates internal object flags (IsValid)
    if (instigator && instigator->IsA(ACamperPlayer::StaticClass()) && IsValid(instigator) == true)
    {
        SubjectActor = instigator;
    }
    // If instigator was not valid/applicable, check if the target is a valid ACamperPlayer
    else if (target && target->IsA(ACamperPlayer::StaticClass()) && IsValid(target) == true)
    {
        SubjectActor = target;
    }

    // If we found a valid Camper actor to process
    if (SubjectActor)
    {
        // Cast the generic actor to the specific ADBDPlayer class to access custom functions
        /* UNDEFINED ELEMENT - Assuming ACamperPlayer inherits from ADBDPlayer */
        ADBDPlayer* DBDPlayer = Cast<ADBDPlayer>(SubjectActor);

        if (DBDPlayer)
        {
            // Retrieve the custom player controller. 
            // Disassembly calls ?GetPlayerController@ADBDPlayer@@ which is likely a custom helper method.
            /* UNDEFINED ELEMENT */
            ADBDPlayerController* PlayerController = DBDPlayer->GetPlayerController();

            // Validate the controller. Disassembly performs IsValid checks (GUObjectArray bounds and flags).
            if (PlayerController && IsValid(PlayerController) == true)
            {
                APlayerState* PlayerState = PlayerController->PlayerState;

                // Validate the PlayerState
                if (PlayerState && IsValid(PlayerState) == true)
                {
                    FName ObjectiveTag;
                    bool bShouldProcessEvent = false;
                    
                    // Strategy 0 = Add, 1 = Set. Based on assembly: "xor r9d, r9d" vs "mov r9b, 1"
                    /* UNDEFINED ELEMENT */
                    KillerGoalCountStrategy GoalStrategy = KillerGoalCountStrategy::Add; 

                    // Map the specific Score Type to a Killer Objective Tag
                    switch (gameplayEventType)
                    {
                        case EDBDScoreTypes::DBDSlasherScore_Kill:
                            ObjectiveTag = FName(TEXT("Killer_ObjectivePoints_Killed"));
                            bShouldProcessEvent = true;
                            break;

                        case EDBDScoreTypes::DBDSlasherScore_BleedOut:
                            ObjectiveTag = FName(TEXT("Killer_ObjectivePoints_BleedOut"));
                            bShouldProcessEvent = true;
                            break;

                        case EDBDScoreTypes::DBDSlasherScore_SacrificeSuccess:
                            ObjectiveTag = FName(TEXT("Killer_ObjectivePoints_Sacrificed"));
                            bShouldProcessEvent = true;
                            break;

                        case EDBDScoreTypes::DBDSlasherScore_CamperHookedFirstTime:
                            ObjectiveTag = FName(TEXT("Killer_ObjectivePoints_HookedFirstTime"));
                            bShouldProcessEvent = true;
                            break;

                        case EDBDScoreTypes::DBDSlasherScore_CamperEnterHookStrugglePhase:
                            ObjectiveTag = FName(TEXT("Killer_ObjectivePoints_HookedStrugglePhase"));
                            bShouldProcessEvent = true;
                            break;

                        case EDBDScoreTypes::DBDSlasherScore_CamperDisconnectedDuringMatch:
                            ObjectiveTag = FName(TEXT("Killer_ObjectivePoints_SurvivorDisconnect"));
                            GoalStrategy = KillerGoalCountStrategy::Set; // Strategy 1 in assembly
                            bShouldProcessEvent = true;
                            break;

                        default:
                            break;
                    }

                    if (bShouldProcessEvent == true)
                    {
                        // Create a copy of the Player's UniqueNetId. 
                        // The assembly shows explicit ref-counting manipulation (SharedReferenceCount), 
                        // which is handled by the copy constructor of FUniqueNetIdRepl in C++.
                        FUniqueNetIdRepl PlayerUniqueId = PlayerState->UniqueId;

                        // Call the internal function to update the killer's goal progress
                        /* UNDEFINED ELEMENT */
                        this->AddToKillerGoalCounter(ObjectiveTag, PlayerUniqueId, GoalStrategy);
                    }
                }
            }
        }
    }
}




void ADBDGameMode::ProceduralLevelBuilt()
{
    // Set the current loading step to 1 (Scanning/Building phase complete)
    this->_currentLevelLoadingStep = WaitingForPlayersToBeSpawned;

    // Retrieve the builder responsible for the procedural generation
    /* UNDEFINED ELEMENT */
    AProceduralLevelBuilder* ProceduralLevelBuilder = this->GetProceduralLevelBuilder();

    // Only proceed if the builder exists and is valid
    if (ProceduralLevelBuilder)
    {
        // Mark the game mode as operating in procedural mode
        this->_proceduralMode = true;

        // Access the GameInstance to clear specific procedural data (FParadiseData)
        UGameInstance* GameInstance = this->GetGameInstance();
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

        if (DBDGameInstance)
        {
            // The disassembly accesses an array at offset 0x1C0 (GameInstance + 28 in pseudo-code pointer arithmetic)
            // It clears this array.
            /* UNDEFINED ELEMENT */
            DBDGameInstance->ParadiseData.Empty(); 
        }

        // --- Spawn Slasher (Killer) Start ---
        // Retrieve the transform component for the killer from the builder
        const USceneComponent* KillerSpawnPoint = ProceduralLevelBuilder->_generatedData.KillerSpawnPoint;
        
        // Create the PlayerStart actor with the "Slasher" tag
        /* UNDEFINED ELEMENT */
        this->CreatePlayerStart(KillerSpawnPoint, FName("Slasher"));

        // --- Spawn Survivor (Camper) Starts ---
        // Iterate through the array of survivor spawn points generated by the builder
        TArray<const USceneComponent*>& SurvivorSpawnPoints = ProceduralLevelBuilder->_generatedData.SurvivorSpawnPoints;
        
        for (int32 i = 0; i < SurvivorSpawnPoints.Num(); i++)
        {
            const USceneComponent* SurvivorPoint = SurvivorSpawnPoints[i];
            if (SurvivorPoint)
            {
                // Create PlayerStart actors with the "Camper" tag
                /* UNDEFINED ELEMENT */
                this->CreatePlayerStart(SurvivorPoint, FName("Camper"));
            }
        }

        // --- Spawn Observer (Spectator) Start ---
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.Instigator = this->Instigator;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        UClass* PlayerStartClass = ADBDPlayerStart::StaticClass();
        UWorld* World = this->GetWorld();

        // Spawn the observer start at ZeroVector/ZeroRotator
        APlayerStart* ObserverStart = World->SpawnActor<APlayerStart>(PlayerStartClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
        
        if (ObserverStart)
        {
            ObserverStart->PlayerStartTag = FName("Observer");
        }

        // --- Handle Match State ---
        if (this->MatchState == MatchState::InProgress)
        {
            this->RestartGame();
        }
        else
        {
            this->SetMatchState(MatchState::EnteringMap);

            // Clear a specific bit flag (Bit 3/Value 8) at offset 0x388 before starting play
            // This is likely related to level transition or seamless travel flags.
            /* UNDEFINED ELEMENT */
            this->bUseSeamlessTravel = false;

            this->StartPlay();
        }

        // --- Initialize Interactables ---
        // Iterate through all actors of type AInteractable in the world
        // The disassembly checks IsA(AInteractable::StaticClass()) explicitly inside the iterator setup
        for (TActorIterator<AInteractable> It(this->GetWorld()); It; ++It)
        {
            AInteractable* Interactable = *It;
            if (Interactable)
            {
                // Call a virtual function at vtable index 188 (Offset 0x5E0)
                // This likely initializes the interactable after procedural placement
                /* UNDEFINED VTABLE */
                Interactable->ProceduralLevelBuilt();
            }
        }

        // --- Finalize Mechanics ---
        // Recalculate how many generators are needed based on the generated level
        /* UNDEFINED ELEMENT */
        this->UpdateGeneratorNeeded();
    }
}




void ADBDGameMode::RestartGame()
{
    // Notify subsystems that the game is in a state where joinability might change (e.g., restarting)
    // 1 = true (likely bUpdateOnlineSession or similar boolean)
    /* UNDEFINED ELEMENT */
    this->UpdateJoinability(true);

    // Reset the EscapeOpened flag to false as we are restarting
    this->EscapeOpened = false;

    // Check if the match has officially started before attempting restart logic
    if (this->HasMatchStarted() == true)
    {
        // Iterate over all PlayerControllers in the world
        // UE4 uses TIndexedContainerIterator for sparse arrays like the controller list
        for (FConstPlayerControllerIterator Iterator = this->GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            // Retrieve the controller from the weak pointer in the iterator
            AController* Controller = Iterator->Get();

            // Validate the controller and its PlayerState
            if (Controller && Controller->PlayerState)
            {
                // Check if the player is NOT a spectator.
                // 2 corresponds to the bit flag for bIsSpectator in APlayerState.
                // If (bIsSpectator & 2) == 0, it means they are not a spectator (or at least that specific flag is unset).
                if (Controller->PlayerState->bIsSpectator == false)
                {
                    // Ensure the PlayerStateClass is set to the correct custom class
                    // Disassembly: ADBDPlayerState::GetPrivateStaticClass(L"/Script/DeadByDaylight")
                    // It seems to be retrieving the class from the existing player state to potentially re-use or ensure consistency
                    this->PlayerStateClass = Controller->PlayerState->GetClass();

                    // Destroy the existing PlayerState
                    // AActor::Destroy(bool bNetForce, bool bShouldModifyLevel) -> 0, 1 based on disassembly
                    Controller->PlayerState->Destroy(false, true);

                    // Handle the Pawn
                    APawn* ControlledPawn = Controller->GetPawn();
                    if (ControlledPawn)
                    {
                        // Check if the Pawn is of type ADBDPlayer (or ACamperPlayer/ASlasherPlayer base)
                        // Disassembly checks against ADBDPlayer::GetPrivateStaticClass
                        if (ControlledPawn->IsA(ADBDPlayer::StaticClass()))
                        {
                            // Unpossess and destroy the pawn
                            Controller->UnPossess();
                            ControlledPawn->Destroy(false, true);
                        }
                    }

                    // Re-initialize the PlayerState for the controller.
                    // This creates a new PlayerState instance since the old one was destroyed.
                    Controller->InitPlayerState();

                    // Restart the player (spawn pawn, etc.) using the GameMode's logic
                    this->RestartPlayer(Controller);
                }
            }
        }

        // Mark the game restart flag as true
        this->_gameRestarted = true;
    }
}




void ADBDGameMode::SetCurrentLoadingStep(ELevelLoadingSteps inStep)
{
    // Check the verbosity level of the "LogLoading" category. 
    // In UE4, verbosity level 5 usually corresponds to ELogVerbosity::VeryVerbose.
    // The assembly compares the current verbosity against 5.
    if (UE_GET_LOG_VERBOSITY(LogLoading) >= ELogVerbosity::VeryVerbose)
    {
        // Convert the enum value to a string description.
        // "Enum::ToString" is a helper function identified in the disassembly.
        /* UNDEFINED ELEMENT */
        FString StepStringLog = Enum::ToString(inStep);

        // Log the current loading step.
        // Disassembly calls FMsg::Logf_Internal__VA with the format "Current Loading Step : %s"
        UE_LOG(LogLoading, VeryVerbose, TEXT("Current Loading Step : %s"), *StepStringLog);
    }

    // Update the internal state variable.
    // The struct defines _currentLevelLoadingStep as a 1-byte array at offset 0x472.
    this->_currentLevelLoadingStep = inStep;

    // Prepare the event name for analytics.
    // The assembly constructs a string by concatenating "LoadingStep/" with the string representation of the enum.
    /* UNDEFINED ELEMENT */
    FString StepName = Enum::ToString(inStep);
    FString AnalyticsEventName = FString(TEXT("LoadingStep/")) + StepName;

    // Prepare an empty array of attributes for the analytics event.
    // The assembly initializes this array but does not add elements to it in this function.
    /* UNDEFINED ELEMENT */
    TArray<FAnalyticsEventAttribute> Attributes;

    // Send the event to the custom analytics system.
    // Disassembly calls UBHVRAnalytics::RecordEvent.
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::RecordEvent(AnalyticsEventName, Attributes);
}




void ADBDGameMode::SetPlayersGameEnded(bool serverHasLeftMatch)
{
    // Iterate over all player controllers in the world to update their game end status
    for (FConstPlayerControllerIterator Iterator = this->GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        // Get the controller from the weak object pointer
        // The disassembly calls FWeakObjectPtr::Get
        APlayerController* Controller = Iterator->Get();

        // Check if the controller is valid
        if (Controller)
        {
            // Cast to ADBDPlayerController to access specific functionality
            // Disassembly checks against ADBDPlayerController::GetPrivateStaticClass
            ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(Controller);

            // Ensure the controller is valid (not pending kill, etc.)
            // The disassembly explicitly checks flags (0x20000000 = RF_PendingKill/RF_Unreachable) and InternalIndex
            if (IsValid(DBDController) == true)
            {
                // Access the PlayerState
                APlayerState* PlayerState = DBDController->PlayerState;

                if (PlayerState)
                {
                    // Verify it is an ADBDPlayerState
                    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);

                    if (IsValid(DBDPlayerState) == true)
                    {
                        // Call the authority function to evaluate the internal game end state for this controller
                        // This likely calculates scores, updates localized states, etc.
                        /* UNDEFINED ELEMENT */
                        DBDController->AuthorityEvaluateGameEndInternalState();
                    }
                }
            }
        }
    }

    // Handle GameState update if the server has left
    AGameState* CurrentGameState = this->GameState;
    if (CurrentGameState)
    {
        ADBDGameState* DBDGameState = Cast<ADBDGameState>(CurrentGameState);
        
        // Check if GameState is valid and not marked for destruction
        if (IsValid(DBDGameState) == true)
        {
            if (serverHasLeftMatch == true)
            {
                // Notify clients via multicast that the game ended because the server left
                /* UNDEFINED ELEMENT */
                DBDGameState->Multicast_SetGameEnded(serverHasLeftMatch);
            }
        }
    }

    // Iterate through controllers again to finalize scoring events and clean up references
    for (FConstPlayerControllerIterator Iterator = this->GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* Controller = Iterator->Get();
        if (Controller)
        {
            ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(Controller);

            if (IsValid(DBDController) == true)
            {
                // Evaluate score events for the end of the game
                /* UNDEFINED ELEMENT */
                DBDController->AuthorityEvaluateGameEndScoreEvents();

                // Access the PlayerState again for cleanup logic
                ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(DBDController->PlayerState);

                if (IsValid(DBDPlayerState) == true)
                {
                    // Check specific game-end conditions on the PlayerState
                    // Offsets 1872 and 2041 correspond to specific byte flags in the disassembly.
                    // The logic checks if byte at 1872 is 1 OR byte at 2041 is 0 or 5.
                    // This likely corresponds to connection status or game outcome states (e.g. Escaped, Dead).
                    /* UNDEFINED ELEMENT */
                    bool bCondition1 = (DBDPlayerState->GameRole == VE_Slasher);
                    /* UNDEFINED ELEMENT */
                    EGameState StatusByte = DBDPlayerState->PlayerData._playerGameState;
                    bool bCondition2 = (StatusByte == VE_Active || StatusByte == VE_None);

                    if (bCondition1 || bCondition2)
                    {
                        DBDPlayerState->AuthoritySetInParadise(false);
                    }
                }
            }
        }
    }
}




APawn* ADBDGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
    // First, attempt to spawn using the base GameMode implementation
    APawn* ResultPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

    // If the base implementation succeeded, return the pawn immediately
    if (ResultPawn)
    {
        return ResultPawn;
    }

    // If spawning failed, enter a fallback retry loop
    int32 AttemptCount = 1;
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = this->Instigator;

    // Loop until a pawn is successfully spawned or we exceed 10 attempts
    do
    {
        if (AttemptCount > 10)
        {
            // If we've exhausted retry attempts, jump to the forced spawn logic
            goto ForceSpawn;
        }

        // Logging logic for procedural level generation debugging
        // Checks verbosity level 5 (VeryVerbose) for the log category "ProceduralLevelGeneration"
        if (UE_GET_LOG_VERBOSITY(ProceduralLevelGeneration) >= ELogVerbosity::VeryVerbose)
        {
            FString ClassName = TEXT("None");
            
            // Try to resolve the class name of the DefaultPawnClass
            UClass* PawnClassToSpawn = this->DefaultPawnClass;
            if (PawnClassToSpawn)
            {
                // Verify inheritance from APawn to be safe, although DefaultPawnClass usually enforces this
                if (PawnClassToSpawn->IsChildOf(APawn::StaticClass()))
                {
                    ClassName = PawnClassToSpawn->GetName();
                }
            }

            // Log the spawn attempt number and class name
            // "Spawn attempt %s of %d"
            UE_LOG(ProceduralLevelGeneration, VeryVerbose, TEXT("Spawn attempt %s of %d"), *ClassName, AttemptCount);
        }

        // Prepare rotation and location for the spawn
        FRotator StartRotation(ForceInit);
        FVector StartLocation(ForceInit);

        if (StartSpot && StartSpot->GetRootComponent())
        {
            USceneComponent* StartRoot = StartSpot->GetRootComponent();
            
            // Retrieve rotation. The disassembly involves FQuat and caching logic similar to GetComponentRotation()
            // but explicitly updates the WorldRotationCache if needed.
            FQuat ComponentQuat = StartRoot->GetComponentQuat();
            StartRotation = ComponentQuat.Rotator();

            // Explicit cache update seen in disassembly:
            // This suggests manual manipulation or an inlined version of a specific engine function.
            // In standard UE4 code, StartRoot->GetComponentRotation() usually suffices.
            
            StartLocation = StartRoot->GetComponentLocation();
        }
        else
        {
            StartRotation = FRotator::ZeroRotator;
            StartLocation = FVector::ZeroVector;
        }

        // Apply a Z-offset based on the attempt count to avoid collision overlaps
        // This moves the spawn point up by 100 units for each retry
        StartLocation.Z += (float)(AttemptCount * 100);

        // Determine the class to spawn for this controller
        UClass* PawnClass = this->GetDefaultPawnClassForController(NewPlayer);

        // Attempt to spawn the actor again
        AttemptCount++;
        ResultPawn = this->GetWorld()->SpawnActor<APawn>(PawnClass, StartLocation, StartRotation, SpawnInfo);

    } while (ResultPawn == nullptr); // Continue loop if spawn failed

    // If we successfully spawned a pawn, verify its validity
    if (ResultPawn)
    {
        // Standard IsValid checks (not pending kill) logic seen in disassembly
        if (IsValid(ResultPawn))
        {
            return ResultPawn;
        }
    }

ForceSpawn:
    // Fallback strategy: Force spawn with collision adjustments allowed
    // 4 correspond to bNoCollisionFail or similar flags being OR'd into the spawn settings
    /* UNDEFINED ELEMENT */
    SpawnInfo.bNoFail = true; 
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FRotator ForcedRotation = FRotator::ZeroRotator;
    FVector ForcedLocation = FVector::ZeroVector;

    if (StartSpot && StartSpot->GetRootComponent())
    {
        USceneComponent* StartRoot = StartSpot->GetRootComponent();
        
        // Similar rotation logic as above
        FQuat ComponentQuat = StartRoot->GetComponentQuat();
        ForcedRotation = ComponentQuat.Rotator();
        
        ForcedLocation = StartRoot->GetComponentLocation();
    }

    // Apply the offset again based on current attempt count
    ForcedLocation.Z += (float)(AttemptCount * 100);

    // Get class and spawn
    UClass* ForcedPawnClass = this->GetDefaultPawnClassForController(NewPlayer);
    ResultPawn = this->GetWorld()->SpawnActor<APawn>(ForcedPawnClass, ForcedLocation, ForcedRotation, SpawnInfo);

    // Log the forced spawn if verbosity allows
    if (UE_GET_LOG_VERBOSITY(ProceduralLevelGeneration) >= ELogVerbosity::VeryVerbose)
    {
        FString ClassName = TEXT("None");
        if (this->DefaultPawnClass && this->DefaultPawnClass->IsChildOf(APawn::StaticClass()))
        {
            ClassName = this->DefaultPawnClass->GetName();
        }

        // "Fall back forced Spawn for %s"
        UE_LOG(ProceduralLevelGeneration, VeryVerbose, TEXT("Fall back forced Spawn for %s"), *ClassName);
    }

    return ResultPawn;
}




void ADBDGameMode::StartMatch()
{
   Super::StartMatch();
   this->UpdateJoinability(true);
}




void ADBDGameMode::StartNewPlayer(APlayerController* NewPlayer)
{
    // Retrieve the GameInstance and cast it to the custom DBD type
    UGameInstance* GameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // Retrieve the PlayerState from the new controller and cast it to the custom DBD type
    APlayerState* PlayerState = NewPlayer->PlayerState;
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);

    // If both the custom GameInstance and PlayerState are valid, attempt to load persistent data
    if (DBDGameInstance)
    {
        if (DBDPlayerState)
        {
            // Prepare the unique net ID for persistent data retrieval
            FUniqueNetIdRepl PlayerUniqueId = DBDPlayerState->UniqueId;

            // Retrieve persistent data for this player
            // UDBDPersistentData is a custom class
            /* UNDEFINED ELEMENT */
            FPlayerPersistentData* PersistentData = UDBDPersistentData::GetPlayerPersistentData(PlayerUniqueId);

            if (PersistentData)
            {
                // Check if the controller is local to determine how data is copied
                bool bIsLocalController = NewPlayer->IsLocalController();

                // Copy data from persistent storage to the PlayerState
                /* UNDEFINED ELEMENT */
                UDBDPersistentData::CopyData(PersistentData, DBDPlayerState, bIsLocalController);

                // Set the GameRole on the PlayerState based on the loaded persistent data
                // 0 likely corresponds to a boolean flag (e.g., bForce)
                /* UNDEFINED ELEMENT */
                DBDPlayerState->SetGameRole(PersistentData->GameRole, false);
            }
        }
    }

    // Fallback logic for assigning a Role if the player state is valid but role is likely uninitialized (VE_None)
    if (DBDPlayerState)
    {
        // Check if role is VE_None (0)
        // offset 0x750 in disassembly corresponds to the GameRole property
        if (DBDPlayerState->GameRole == EPlayerRole::VE_None)
        {
            // Default role is Camper (2)
            EPlayerRole NewRole = EPlayerRole::VE_Camper;

            AGameState* CurrentGameState = this->GameState;

            // Check if the game mode is configured to start with a Slasher
            if (this->StartWithSlasher == true)
            {
                // If this is the first player joining (PlayerArray.Num() == 1), assign them as Slasher (1)
                if (CurrentGameState && CurrentGameState->PlayerArray.Num() == 1)
                {
                    NewRole = EPlayerRole::VE_Slasher;
                }
            }

            // Apply the determined role to the PlayerState
            /* UNDEFINED ELEMENT */
            DBDPlayerState->SetGameRole(NewRole, false);
        }
    }

    // Call the base class implementation of StartNewPlayer
    Super::StartNewPlayer(NewPlayer);
}




void ADBDGameMode::StartPlay()
{
    // Call the base class implementation first
    Super::StartPlay();

    // Retrieve the GameInstance
    UGameInstance* GameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // If the custom GameInstance is valid
    if (DBDGameInstance)
    {
        // Retrieve the SacrificeSequencer from the GameInstance.
        // Disassembly: UDBDGameInstance::GetSacrificeSequencer()
        /* UNDEFINED ELEMENT */
        USequencer* SacrificeSequencer = DBDGameInstance->GetSacrificeSequencer();

        // Reset the sequencer.
        // Disassembly explicitly calls USequencer::Reset().
        /* UNDEFINED ELEMENT */
        if (SacrificeSequencer)
        {
            SacrificeSequencer->Reset();
        }
    }
}




void ADBDGameMode::StartTravel()
{
    // Iterate over all player controllers to validate readiness for travel
    for (FConstPlayerControllerIterator Iterator = this->GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        // Get the controller from the weak object pointer
        APlayerController* Controller = Iterator->Get();

        // If the controller is valid
        if (Controller)
        {
            // Access the PlayerState
            APlayerState* PlayerState = Controller->PlayerState;
            if (PlayerState)
            {
                // Verify the PlayerState class
                // Disassembly checks ADBDPlayerState::GetPrivateStaticClass()
                if (PlayerState->IsA(ADBDPlayerState::StaticClass()))
                {
                    // Access a specific member in ADBDPlayerState (offset 0x820)
                    // Based on context ("StartTravel"), this likely checks if the player is ready or has acknowledged the travel.
                    // Let's assume it's a boolean flag named bIsReadyForTravel or similar.
                    /* UNDEFINED ELEMENT */
                    bool bIsReady = PlayerState->IsReadyToTravel; // 0x820 offset in ADBDPlayerState

                    // If any player is NOT ready (byte at 0x820 is false/0), abort the travel initiation.
                    if (bIsReady == false)
                    {
                        return;
                    }
                }
            }
        }
    }

    // Prepare travel URL/Options
    // The disassembly accesses a global or static struct UIConstants::MAPS_ROOT_0
    // It copies a string from this source.
    /* UNDEFINED ELEMENT */
    FString TravelURL = UIConstants::MAPS_ROOT_0;

    // Append the map name or offline lobby name if available
    // Disassembly logic: if UIConstants::MAP_NAME_OFFLINE_LOBBY_0 has data, append it to TravelURL
    /* UNDEFINED ELEMENT */
    FString OfflineLobbyName = UIConstants::MAP_NAME_OFFLINE_LOBBY_0;
    
    if (OfflineLobbyName.IsEmpty() == false)
    {
        // Concatenate the string. The assembly logic essentially does: TravelURL += OfflineLobbyName;
        // It handles null-termination explicitly.
        TravelURL += OfflineLobbyName;
    }

    // Execute the ServerTravel on the world
    // Arguments: URL, bAbsolute, bShouldSkipGameNotify
    // 0, 0 in assembly corresponds to false, false
    this->GetWorld()->ServerTravel(TravelURL, false, false);
}




void ADBDGameMode::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);
  this->CheckMapLoadingStatus(DeltaSeconds);
}




void ADBDGameMode::UpdateGeneratorNeeded()
{
    // Retrieve the GameState from the GameMode
    // Offset 0x418 typically corresponds to the GameState member in this engine version's AGameMode
    AGameState* LocalGameState = this->GameState;

    // Check if GameState is valid before proceeding
    if (LocalGameState == nullptr)
    {
        return;
    }

    // Attempt to cast the generic GameState to the specific ADBDGameState
    // The disassembly performs a manual IsA check using ClassTreeIndex
    ADBDGameState* DbdGameState = Cast<ADBDGameState>(LocalGameState);

    // If the cast fails (nullptr), we stop
    if (DbdGameState == nullptr)
    {
        return;
    }

    // Verify the Object Flags to ensure the GameState is not pending kill
    // 0x20000000 corresponds to RF_PendingKill or RF_BeginDestroyed in UE4.13
    if (DbdGameState->IsPendingKill() == true)
    {
        return;
    }

    // Check the bAutoDestroyWhenFinished flag on the Actor
    // Disassembly checks byte at offset 0x140, bit 4 (0x04)
    if (DbdGameState->bAutoDestroyWhenFinished == true)
    {
        return;
    }

    // Retrieve the GameInstance from the Actor
    UGameInstance* LocalGameInstance = this->GetGameInstance();

    // Attempt to cast to UDBDGameInstance
    // The disassembly performs a manual IsA check here. If it fails, the pointer becomes null.
    UDBDGameInstance* DbdGameInstance = Cast<UDBDGameInstance>(LocalGameInstance);

    // If we have a valid UDBDGameInstance
    if (DbdGameInstance)
    {
        /* UNDEFINED ELEMENT */
        // The disassembly accesses a pointer at offset 0x3B8 within UDBDGameInstance.
        // We do not have the definition for UDBDGameInstance to name this member.
        // We retrieve the pointer value manually to maintain functionality.
        UDBDPersistentData* PersistentData = DbdGameInstance->_persistentData;

        // Ensure the retrieved pointer is valid
        if (PersistentData)
        {
            /* UNDEFINED ELEMENT */
            // The disassembly reads a 32-bit integer (DWORD) from offset 0x28 of the object retrieved above.
            int32 ValueToCopy = PersistentData->_gamePersistentData.PlayerCount;

            /* UNDEFINED ELEMENT */
            // The disassembly writes this value to offset 0x5F8 within ADBDGameState.
            // We do not have the definition for ADBDGameState to name this member.
            DbdGameState->GeneratorNeeded = ValueToCopy;
        }
    }
}




void ADBDGameMode::UpdateJoinability(bool inGame)
{
    // Retrieve the World from the GameMode
    UWorld* World = this->GetWorld();

    // Verify World existence
    if (World == nullptr)
    {
        return;
    }

    // Verify the World object is not pending kill (Flags check 0x20000000 in ASM)
    if (World->IsPendingKill() == true)
    {
        return;
    }

    // Ensure we are not running in a Play-In-Editor (PIE) session
    if (World->IsPlayInEditor() == true)
    {
        return;
    }

    // Retrieve the GameInstance
    UGameInstance* GameInstance = this->GetGameInstance();

    if (GameInstance == nullptr)
    {
        return;
    }

    // Attempt to cast to the specific UDBDGameInstance
    // The disassembly performs a manual class tree index check here
    UDBDGameInstance* DbdGameInstance = Cast<UDBDGameInstance>(GameInstance);

    if (DbdGameInstance == nullptr)
    {
        return;
    }

    // Check if the DbdGameInstance is pending kill
    if (DbdGameInstance->IsPendingKill() == true)
    {
        return;
    }

    /* UNDEFINED ELEMENT */
    // The disassembly accesses a UObject pointer at offset 0x3B0 within UDBDGameInstance.
    // mov rdx, [rdi+3B0h]
    UGameFlowContextSystem* ContextSystem = DbdGameInstance->_contextSystem;

    if (ContextSystem == nullptr)
    {
        return;
    }

    // Verify the unknown object is valid (InternalIndex check in ASM)
    if (ContextSystem->IsPendingKill() == true)
    {
        return;
    }

    /* UNDEFINED ELEMENT */
    // The disassembly accesses a pointer at offset 0x90 within the previous object.
    // This pointer is then used to find the ServerInstance.
    // mov r8, [rdx+90h]
    UOnlineSystemHandler* OnlineSystemHandler = ContextSystem->m_OnlineSystemHandler;

    if (OnlineSystemHandler == nullptr)
    {
        return;
    }

    /* UNDEFINED ELEMENT */
    // The disassembly retrieves the UDBDServerInstance from offset 0x5F0 of the struct found above.
    // mov r8, [r8+5F0h]
    UDBDServerInstance* ServerInstance = OnlineSystemHandler->_serverInstance;

    if (ServerInstance == nullptr)
    {
        return;
    }

    // Verify the ServerInstance is valid (InternalIndex check in ASM)
    if (ServerInstance->IsPendingKill() == true)
    {
        return;
    }

    // Determine the session state string based on the inGame boolean
    // The disassembly loads addresses of global strings (likely static const)
    const FString* SessionState = nullptr;

    if (inGame == true)
    {
        /* UNDEFINED ELEMENT */
        // Referenced as SessionState__GAME_0 in disassembly
        static const FString SessionStateGame = TEXT("GAME");
        SessionState = &SessionStateGame;
    }
    else
    {
        /* UNDEFINED ELEMENT */
        // Referenced as SessionState__NONE_0 in disassembly
        static const FString SessionStateNone = TEXT("NONE");
        SessionState = &SessionStateNone;
    }

    // Update the host settings on the server instance
    ServerInstance->UpdateHostSettings(*SessionState);
}