// starts at line 97
ADBDPlayerState::ADBDPlayerState(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize standard primitive states
    this->GameLevelLoaded = false;
    this->IsPlayerReady = false;
    this->SelectedCamperIndex = -1;
    this->GameRole = EPlayerRole::VE_None;
    this->SelectedSlasherIndex = -1;

    // Initialize Camper specific data
    this->CamperData.CamperSkulls = -1;
    this->CamperData.EquipedItemId = NAME_None;
    
    // Initialize Slasher specific data
    this->SlasherData.SlasherSkulls = -1;
    this->SlasherData.EquipedPowerId = NAME_None;

    // Initialize generic Player data
    this->PlayerData.CharacterLevel = 0;
    this->PlayerData.EquipedFavorId = NAME_None;
    
    // IDA identifies a 16-bit assignment of 1280 (0x0500) to PlayerData.IsLeavingMatch.
    // This is likely setting two adjacent 8-bit variables (e.g., IsLeavingMatch and a padding byte or next bool)
    this->PlayerData.IsLeavingMatch = false;
    this->PlayerData._playerGameState = EGameState::VE_None;

    // Initialize Skill Check interaction data
    this->SkillCheckInteractionData.IsActive = false;
    this->SkillCheckInteractionData.HitAreaStart = 0;
    this->SkillCheckInteractionData.HitAreaLength = 0;
    this->SkillCheckInteractionData.BonusAreaLength = 0;
    this->SkillCheckInteractionData.Progress = 0;
    this->SkillCheckInteractionData.IsHexed = false;
    this->SkillCheckInteractionData.xOffset = 0;
    this->SkillCheckInteractionData.yOffset = 0;

    // Initialize cached game stats
    this->_cachedUserGameStats.Disconnected = false;
    this->_cachedUserGameStats.FearTokens = 0;
    this->_cachedUserGameStats.Skulls = 0;

    // Initialize networking and inventory flags
    this->_postGameInventoryHandled = false;
    this->_inParadise = false;
    this->_killerJoiningState = EKillerJoiningState::Disconnected;

    // Create default subobject components using standard Unreal Engine template functions.
    // This abstracts the low-level UObject::CreateDefaultSubobject calls seen in the disassembly.
    this->RitualHandler = ObjectInitializer.CreateDefaultSubobject<URitualHandlerComponent>(this, TEXT("RitualHandler"));
    this->AchievementHandler = ObjectInitializer.CreateDefaultSubobject<UAchievementHandlerComponent>(this, TEXT("AchievementHandler"));
    this->ActivityTracker = ObjectInitializer.CreateDefaultSubobject<UActivityTrackerComponent>(this, TEXT("ActivityTracker"));
    this->BadgeEvaluator = ObjectInitializer.CreateDefaultSubobject<UDBDBadgeEvaluatorComponent>(this, TEXT("BadgeEvaluator"));
    this->_gameplayNotificationManager = ObjectInitializer.CreateDefaultSubobject<UGameplayNotificationManager>(this, TEXT("GameplayNotificationManager"));
}

// starts at line 106
void ADBDPlayerState::PostInitializeComponents()
{
    // Call the parent class PostInitializeComponents
    Super::PostInitializeComponents();

    UWorld* world = this->GetWorld();
    if (world != nullptr)
    {
        // The disassembly shows a complex inline allocation for the timer delegate, 
        // which in standard UE4 is handled by the template expansion of SetTimer.
        // Replaced the decompiled memory management with the standard UE4 SetTimer implementation.
        world->GetTimerManager().SetTimer(this->_ongoingScoreTimerHandle, this, &ADBDPlayerState::UpdateOngoingScores, 0.25f, true, -1.0f);
    }

    // Check if the GameInstance is of type UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance))
    {
        // Bind RitualHandler and AchievementHandler to the local OnPlayerGameplayEvent delegate
        // Replaced the decompiled __Internal_AddDynamic calls with standard AddDynamic macro
        if (this->RitualHandler != nullptr)
        {
            this->OnPlayerGameplayEvent.AddDynamic(this->RitualHandler, &URitualHandlerComponent::ReceiveGameEvent);
        }
        
        if (this->AchievementHandler != nullptr)
        {
            this->OnPlayerGameplayEvent.AddDynamic(this->AchievementHandler, &UAchievementHandlerComponent::ReceiveGameEvent);
        }

        // Bind handlers to the UDBDGameInstance delegate
        if (this->AchievementHandler != nullptr)
        {
            dbdGameInstance->GameEventTracker.OnGameflowEventNative.AddUObject(this->AchievementHandler, &UAchievementHandlerComponent::ReceiveGameflowEvent);
        }

        if (this->RitualHandler != nullptr)
        {
            dbdGameInstance->GameEventTracker.OnGameflowEventNative.AddUObject(this->RitualHandler, &URitualHandlerComponent::ReceiveGameflowEvent);
        }
    }
}

// starts at line 112
bool ADBDPlayerState::Multicast_UpdateSkillCheckInteractionData_Validate(float)
{
    return true;
}

// starts at line 126
void ADBDPlayerState::Destroyed()
{
    UWorld* world = this->GetWorld();
    
    // Check if the world exists
    if (world != nullptr)
    {
        // Use the standard Unreal Engine timer clearing mechanism
        world->GetTimerManager().ClearTimer(this->_ongoingScoreTimerHandle);
        
        // Manually invalidate the handle by setting it to 0 as seen in the disassembly
        this->_ongoingScoreTimerHandle.Invalidate(); 
    }

    // Call the parent class Destroyed
    Super::Destroyed();
}

// starts at line 136
void ADBDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Call the base class implementation first
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPSTRUCT macro is the standard Unreal Engine way to register properties for replication.
    // The decompiled code shows the expansion of this macro for multiple properties, 
    // including thread-safe static initialization of property pointers and array iteration.
    
    DOREPLIFETIME(ADBDPlayerState, GameLevelLoaded);
    DOREPLIFETIME(ADBDPlayerState, IsPlayerReady);
    DOREPLIFETIME(ADBDPlayerState, CamperData);
    DOREPLIFETIME(ADBDPlayerState, SlasherData);
    DOREPLIFETIME(ADBDPlayerState, PlayerData);
    DOREPLIFETIME(ADBDPlayerState, GameRole);
    DOREPLIFETIME(ADBDPlayerState, _inParadise);
    DOREPLIFETIME(ADBDPlayerState, SelectedCamperIndex);
    DOREPLIFETIME(ADBDPlayerState, SelectedSlasherIndex);
    DOREPLIFETIME(ADBDPlayerState, SteamId);
    DOREPLIFETIME(ADBDPlayerState, MirrorsId);
    DOREPLIFETIME(ADBDPlayerState, SkillCheckInteractionData);
}

// starts at line 155
void ADBDPlayerState::CopyDataFromCache(FPlayerstateDataCache cache)
{
    // The decompiled code shows a direct assignment to the internal _scoreEvents map.
    this->_scoreEvents = cache.ScoreEvents;

    // Reset the ActivityTracker with inactiveTime from cache.
    // The assembly reads inactiveTime from offset 0x50 and passes activeTime as 0.0f.
    if (this->ActivityTracker != nullptr)
    {
        this->ActivityTracker->Reset(cache.TimeInactive, 0.0f);
    }
}

// starts at line 161
void ADBDPlayerState::CopyProperties(APlayerState* PlayerState)
{
    // Call the parent class CopyProperties to handle base properties
    Super::CopyProperties(PlayerState);

    // Safely check if the provided PlayerState is valid
    if (PlayerState == nullptr)
    {
        return;
    }

    // Cast the target PlayerState to our specific class.
    // The disassembly shows the internal ClassTreeIndex comparison, which is 
    // the expanded inline implementation of Unreal Engine's Cast<T> template.
    ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(PlayerState);
    
    if (dbdPlayerState != nullptr)
    {
        // Copy primitive properties
        dbdPlayerState->SetGameRole(this->GameRole, false);
        
        dbdPlayerState->SelectedCamperIndex = this->SelectedCamperIndex;
        dbdPlayerState->SelectedSlasherIndex = this->SelectedSlasherIndex;

        // Copy complex containers and structs.
        dbdPlayerState->_scoreEvents = this->_scoreEvents;        
        dbdPlayerState->PlayerData = this->PlayerData;
        dbdPlayerState->SlasherData = this->SlasherData;
        dbdPlayerState->CamperData = this->CamperData;

        // FString copies natively wrap the TArray assignments seen in the pseudo-code
        dbdPlayerState->SteamId = this->SteamId;
        dbdPlayerState->MirrorsId = this->MirrorsId;
    }
}

// starts at line 180
void ADBDPlayerState::OnPlayerPawnBecomingActive()
{
    // Check if the current game state is NOT already 6 or 7.
    if (this->PlayerData._playerGameState != EGameState::VE_Disconnected && 
        this->PlayerData._playerGameState != EGameState::VE_ManuallyLeftMatch)
    {
        // Set the player game state to 0 (VE_Active)
        this->SetPlayerGameState(EGameState::VE_Active);
        
        // Check if the BadgeEvaluator component is valid and not marked as unreachable/pending kill
        if (IsValid(this->BadgeEvaluator))
        {
            this->BadgeEvaluator->InitializeBadges();
        }
    }
}

// starts at line 193
void ADBDPlayerState::SetUniqueId(const TSharedPtr<const FUniqueNetId>& InUniqueId)
{
    // Call the parent implementation to correctly set the base state's UniqueId
    Super::SetUniqueId(InUniqueId);

    // Cast to our specific DBDGameInstance.
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance))
    {
        // Prepare a copy of the assigned UniqueId for lookup
        // The assembly passes this via a temporary FUniqueNetIdRepl struct.
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

        // The disassembly calls UDBDPersistentData::GetPlayerPersistentData.
        // The pointer to UDBDPersistentData is read from [rdi+3B8h], where rdi is the GameInstance.
        UDBDPersistentData* persistentData = dbdGameInstance->_persistentData;
        
        if (persistentData != nullptr)
        {
            FPlayerPersistentData* playerData = persistentData->GetPlayerPersistentData(netIdRepl);
            
            // Ensure data was found and the SteamId array has more than 1 element
            if (playerData != nullptr && playerData->SavedData.SharedData.SteamId.Len() > 1)
            {
                // Copy the SteamId string.
                this->SteamId = playerData->SavedData.SharedData.SteamId;
            }
        }
    }
}

// starts at line 216
void ADBDPlayerState::Server_KillerServerJoined_Implementation(bool succeeded)
{
    // Create an array to hold the analytics event attributes
    TArray<FAnalyticsEventAttribute> attr;

    // Add the result of the server join to the attributes
    UBHVRAnalytics::AddAttributes(attr, FString(TEXT("SWF_KillerServerJoined_Result_ini")), succeeded);

    // Add the player's Steam ID to the attributes
    UBHVRAnalytics::AddAttributes(attr, FString(TEXT("SWF_KillerServerJoined_Id_szni")), this->SteamId);

    // Record the Survive With Friend event with the populated attributes
    UBHVRAnalytics::RecordEvent(FString(TEXT("SurviveWithFriend")), attr);

    // Update the killer joining state based on the succeeded flag.
    this->_killerJoiningState = succeeded == true ? EKillerJoiningState::Connected : EKillerJoiningState::ConnectionFailed;
}

// starts at line 225
void ADBDPlayerState::Server_SetGameRole_Implementation(EPlayerRole newPlayerRole)
{
    this->SetGameRole(newPlayerRole, false);
}

// starts at line 235
void ADBDPlayerState::SetGameRole(EPlayerRole role, bool ignoreGameType)
{
    // Check if the current network role is ROLE_Authority (3)
    if (this->Role == ROLE_Authority)
    {
        // On server: Set the role via client RPC and change it locally
        this->Client_SetGameRole(role);
        this->ChangeRole(role);

        UWorld* world = this->GetWorld();
        if (IsValid(world) == true)
        {
            // Cast the GameStateBase to ADBDGameState
            ADBDGameState* dbdGameState = Cast<ADBDGameState>(world->GetGameState());
            
            if (IsValid(dbdGameState) == true && dbdGameState->IsActorBeingDestroyed() == false)
            {
                dbdGameState->Server_UpdateGameRole();
            }
        }
    }
    else
    {
        // On client: Attempt to change the role
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (IsValid(dbdGameInstance) == true)
        {
            // The disassembly accesses a boolean at offset 0x30 of an object 
            // located at offset 0x3B8 of the UGameInstance.
            UDBDPersistentData* persistentData = dbdGameInstance->_persistentData;

            // Clients can only set their role if explicitly ignoring the game type or if in party mode
            if (ignoreGameType || persistentData->_gamePersistentData.SessionInfos.GameType == EGameType::PartyMode)
            {
                this->Server_SetGameRole(role);
                this->ChangeRole(role);
            }
            else
            {
                // If not allowed, log a warning
                UE_LOG(LogGameFlow, Warning, TEXT("[ADBDPlayerState::SetGameRole] Client trying to set the game role. Only the server is allowed to do that for modes other than Party Mode (unless when specified -> debug purposes only)"));
            }
        }
    }
}

// starts at line 249
void ADBDPlayerState::Authority_SetMirrorsId(const FString& mirrorsId)
{
    if (this->Role.Value == ROLE_Authority)
    {
        this->MirrorsId = mirrorsId;
    }
}

// starts at line 269
void ADBDPlayerState::ChangeStartingGameRole(EPlayerRole gameRole)
{
    // Check if the returned GameInstance is of type UDBDGameInstance.
    // The disassembly uses the low-level ClassTreeIndex check which is equivalent to Cast<T>.
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    
    // Ensure the game instance is valid and the requested role is different from the current one.
    // The disassembly checks: `cmp [rbx+750h], sil` (where 750h is GameRole and sil is the new gameRole)
    if (IsValid(dbdGameInstance) == true && this->GameRole != gameRole)
    {
        // Update the game role on the player state itself.
        this->SetGameRole(gameRole, false);

        // Fetch local persistent data from the GameInstance.
        FPlayerPersistentData* localPersistentData = dbdGameInstance->GetLocalPlayerPersistentData();
        if (localPersistentData != nullptr)
        {
            // The disassembly shows setting a byte at offset 0 of the returned struct.
            localPersistentData->GameRole = gameRole;

            // Call Local_UpdateToSelectedCharacter on the persistent data.
            localPersistentData->Local_UpdateToSelectedCharacter(this, true);
        }
    }
}

// starts at line 285
void ADBDPlayerState::Client_SetGameRole_Implementation(EPlayerRole newRole)
{
    // Apply the role change locally
    this->ChangeRole(newRole);
    this->GameRole = newRole;

    // Verify the game instance is a UDBDGameInstance using the standard Cast
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // The assembly passes the UniqueId via a temporary FUniqueNetIdRepl struct.
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

        // Retrieve the persistent data object from the game instance.
        FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
        
        if (playerData != nullptr)
        {
            // Update the role in the persistent data
            playerData->GameRole = this->GameRole;

            if (this->GameRole == EPlayerRole::VE_Slasher)
            {
                if (playerData->SavedData.SharedData.SelectedSlasherIndex < 0)
                {
                    playerData->SavedData.SharedData.SelectedSlasherIndex = 0;
                }
            }
            else if (this->GameRole == EPlayerRole::VE_Camper)
            {
                if (playerData->SavedData.SharedData.SelectedCamperIndex < 0)
                {
                    playerData->SavedData.SharedData.SelectedCamperIndex = 0;
                }
            }
        }
    }
}

// starts at line 315
void ADBDPlayerState::ChangeRole(EPlayerRole role)
{
    // Update the local game role
    this->GameRole = role;

    // Verify the game instance type using the standard Cast
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Prepare a temporary copy of the unique net ID, as done in other functions.
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

        // Retrieve the persistent data object from the GameInstance
        FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
        
        if (playerData != nullptr)
        {
            // Sync the persistent data role with the newly set role
            playerData->GameRole = this->GameRole;

            if (this->GameRole == EPlayerRole::VE_Slasher)
            {
                if (playerData->SavedData.SharedData.SelectedSlasherIndex < 0)
                {
                    playerData->SavedData.SharedData.SelectedSlasherIndex = 0x10000000;
                }
            }
            else if (this->GameRole == EPlayerRole::VE_Camper)
            {
                if (playerData->SavedData.SharedData.SelectedCamperIndex < 0)
                {
                    playerData->SavedData.SharedData.SelectedCamperIndex = 0;
                }
            }

            // Finally, call Local_UpdateToSelectedCharacter to apply changes locally.
            playerData->Local_UpdateToSelectedCharacter(this, true);
        }
    }
}

// starts at line 341
FPlayerPersistentData* ADBDPlayerState::GetPlayerPersistentData() const
{
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Prepare a temporary copy of the unique net ID, as done in other functions.
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

        // Fetch and return the player's persistent data using their unique net ID
        return dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
    }

    // Return nullptr if any check fails
    return nullptr;
}

// starts at line 351
int ADBDPlayerState::GetCurrentPlayerRankForRoleWithDiff(EPlayerRole role, int newSkulls) const
{
    // Standard check to ensure the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Fetch the design tunables object from the game instance.
        // The assembly uses offset 0xF0 from the game instance. 
        UDBDDesignTunables* tunables = dbdGameInstance->DesignTunables;
        
        if (tunables != nullptr)
        {
            // The disassembly checks the 'role' byte.
            if (role == EPlayerRole::VE_Slasher)
            {
                // Add the newSkulls diff to the current slasher skulls and calculate rank
                return tunables->GetRankForSkulls(this->SlasherData.SlasherSkulls + newSkulls);
            }
            else if (role == EPlayerRole::VE_Camper)
            {
                // Add the newSkulls diff to the current camper skulls and calculate rank
                return tunables->GetRankForSkulls(this->CamperData.CamperSkulls + newSkulls);
            }
        }
    }

    // Default to rank 0 if checks fail or role is not handled
    return 0;
}

// starts at line 390
void ADBDPlayerState::SetPlayerGameState(EGameState newGameState)
{
    // The disassembly shows setting the byte at [this+7F9h].
    this->PlayerData._playerGameState = newGameState;

    // Send an RPC to the server to synchronize the new game state
    this->Server_SetPlayerGameState(newGameState);

    // Re-evaluate the local state after calling the server RPC.
    if (this->PlayerData._playerGameState != EGameState::VE_Active && 
        this->PlayerData._playerGameState != EGameState::VE_None)
    {
        // Warning: The game flow log expects the old state and the new state strings.
        // It checks if the string conversion returned a valid array. If not, it uses a fallback string,
        // which IDA identifies as 'in_pszStateGroup', likely an empty string macro like TEXT("").
        UE_LOG(LogGameFlow, Warning, TEXT("[LOCAL] Trying to set a new gamestate to an already finished gamestate. Old: %s New: %s"),
               *Enum::ToString(this->PlayerData._playerGameState),
               *Enum::ToString(newGameState));
    }
}

// starts at line 407
void ADBDPlayerState::Server_SetPlayerGameState_Implementation(EGameState newGameState)
{
    // Multicast the new game state to all clients to synchronize the state across the network
    this->Multicast_SetPlayerGameState(newGameState);

    // Only proceed with the warning if the state is not 0, not 5, and the new state is different.
    if (this->PlayerData._playerGameState != EGameState::VE_Active && 
        this->PlayerData._playerGameState != EGameState::VE_None &&
        this->PlayerData._playerGameState != newGameState)
    {
        // Log a warning that the server is trying to modify a gamestate that is considered finished.
        // The manual string array checks from the disassembly are implicitly handled by FString's operator*
        UE_LOG(LogGameFlow, Warning, TEXT("[SERVER] Trying to set a new gamestate to an already finished gamestate. Old: %s New: %s"),
               *Enum::ToString(this->PlayerData._playerGameState),
               *Enum::ToString(newGameState));
    }
}

// starts at line 426
void ADBDPlayerState::Multicast_SetPlayerGameState_Implementation(EGameState newGameState)
{
    this->PlayerData._playerGameState = newGameState;
}

// starts at line 432
void ADBDPlayerState::Authority_HandleEnterParadiseScoreEvents()
{
    // Check if the server has authority or if we are in standalone mode
    if (this->Role == ROLE_Authority || this->GetNetMode() == NM_Standalone)
    {
        // Verify the owner is an AController
        AController* controller = Cast<AController>(this->GetOwner());
        
        // The disassembly explicitly checks the validity of the actor using IsValidImpl.
        // In Unreal, this usually corresponds to checking if the pointer is valid and the actor is not pending kill.
        if (IsValid(controller) == true && controller->IsActorBeingDestroyed() == false)
        {
            // Get the controlled pawn and cast to ACamperPlayer
            // The disassembly checks offset 0x398 on AController, which corresponds to the Pawn pointer.
            ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(controller->Character);
            
            if (IsValid(camperPlayer) == true && camperPlayer->IsActorBeingDestroyed() == false && camperPlayer->IsValidImpl() == true)
            {
                // Get the damage state of the camper
                ECamperDamageState damageState = camperPlayer->GetDamageState();
                
                EDBDScoreTypes scoreTypeToFire = EDBDScoreTypes::DBDCamperScore_SurviveHealthy;
                bool shouldFireDamageScore = true;

                // Evaluate the damage state and determine the appropriate score event.
                if (damageState == ECamperDamageState::VE_Healthy)
                {
                    scoreTypeToFire = EDBDScoreTypes::DBDCamperScore_SurviveHealthy;
                }
                else if (damageState == ECamperDamageState::VE_Injured)
                {
                    scoreTypeToFire = EDBDScoreTypes::DBDCamperScore_SurviveWounded;
                }
                else if (damageState == ECamperDamageState::VE_KO)
                {
                    scoreTypeToFire = EDBDScoreTypes::DBDCamperScore_SurviveKO;
                }
                else
                {
                    // Any other state does not trigger a damage-based survive score
                    shouldFireDamageScore = false;
                }

                // Fire the score event related to survival damage state
                if (shouldFireDamageScore)
                {
                    this->FireScoreEvent(scoreTypeToFire, 1.0f, true);
                }

                // Check if the camper was being chased when they escaped
                // The disassembly checks a byte at offset 0x1578 on ACamperPlayer, mapping to _isBeingChased
                if (camperPlayer->_isBeingChased)
                {
                    // 0x45 (69) corresponds to DBDCamperScore_EscapeWhileChased
                    this->FireScoreEvent(EDBDScoreTypes::DBDCamperScore_EscapeWhileChased, 1.0f, true);
                }
            }
        }

        // Regardless of camper state, fire the EnterParadise event
        // 0x9D (157) corresponds to DBDPlayerScore_EnterParadise
        this->FireScoreEvent(EDBDScoreTypes::DBDPlayerScore_EnterParadise, 1.0f, true);
    }
}

// starts at line 473
void ADBDPlayerState::AuthoritySetInParadise(bool killed)
{
    // Check if we are running on the server (Authority or Standalone)
    if (this->Role == ROLE_Authority || this->GetNetMode() == NM_Standalone)
    {
        // Only proceed if not already in paradise
        if (this->_inParadise == false)
        {
            // Set the state locally
            this->_inParadise = true;

            // Handle score events for entering paradise
            this->Authority_HandleEnterParadiseScoreEvents();

            // Notify the specific client that owns this PlayerState
            this->Client_SetInParadise(killed);

            // Get the owner and check if it's a valid DBDPlayerControllerBase
            ADBDPlayerControllerBase* playerController = Cast<ADBDPlayerControllerBase>(this->GetOwner());
            
            if (IsValid(playerController) == true && playerController->IsActorBeingDestroyed() == false)
            {
                // Notify the controller that the player has finished playing
                playerController->Authority_FinishedPlaying();
            }

            // Finally, multicast this state change to all clients
            this->Multicast_SetInParadise(killed);
        }
    }
    else
    {
        // We are on a client trying to call an Authority-only function
        UE_LOG(LogGameFlow, Warning, TEXT("[ADBDPlayerState::SetInParadise] Client trying to se the player as In paradise. Only the server is allowed to do that."));
    }
}

// starts at line 499
void ADBDPlayerState::Local_SetInParadiseOnDisconnectFromServer(bool killed)
{
    // Check if the player is not already in paradise
    if (this->_inParadise == false)
    {
        // Mark the player as being in paradise locally
        this->_inParadise = true;

        // Notify the client that the player has been set to paradise
        this->Client_SetInParadise(killed);

        // Cast the owner actor to a DBDPlayerControllerBase
        ADBDPlayerControllerBase* playerController = Cast<ADBDPlayerControllerBase>(this->GetOwner());

        if (IsValid(playerController) == true && playerController->IsActorBeingDestroyed() == false)
        {
            // Notify the local client controller that the game has ended
            playerController->Client_GameEnded_Implementation();
        }

        // Multicast the paradise state change to all other clients
        this->Multicast_SetInParadise(killed);
    }
}

// starts at line 518
void ADBDPlayerState::Client_SetInParadise_Implementation(bool killed)
{
    this->Local_HandlePostGameInventory(killed);
}

// starts at line 524
void ADBDPlayerState::FlushScoreAnalyticsToServer()
{
    // Iterate through all score analytics data to add them as table rows for analytics
    for (TPair<EDBDScoreTypes, FScoreAnalytics>& scoreAnalyticsPair : this->_playerScoreAnalyticsData)
    {
        UBHVRAnalytics::AddTableRow<FScoreAnalytics>(&scoreAnalyticsPair.Value);
    }

    // Empty the map and clear its hash data.
    // The disassembly shows the inline expansion of the TMap Empty and memory freeing operations.
    this->_playerScoreAnalyticsData.Empty();
}

// starts at line 540
void ADBDPlayerState::Local_HandlePostGameInventory(bool killed)
{
    if (this->_postGameInventoryHandled)
    {
        return;
    }

    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == false)
    {
        this->LogCurrentLoadout();
        return;
    }

    // Set up unique net id lookup
    FUniqueNetIdRepl netIdRepl;
    netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

    // Fetch persistent data via GameInstance. 
    FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
    FCharacterSavedProfileData* localProfileData = dbdGameInstance->GetLocalCharacterSavedProfileData();
    
    if (playerData == nullptr || localProfileData == nullptr)
    {
        this->_postGameInventoryHandled = true;
        this->LogCurrentLoadout();
        return;
    }

    // Check if the current game type affects progression. 
    if (dbdGameInstance->GetCurrentGametypeAffectsProgression() == false)
    {
        // If it doesn't affect progression, jump to the end validation and update logic
        playerData->ValidateData(true);
        playerData->Local_UpdateToSelectedCharacter(this, false);
        this->_postGameInventoryHandled = true;
        this->LogCurrentLoadout();
        return;
    }

    // Try to get a valid CamperPlayer or SlasherPlayer from the Owner's pawn
    ACamperPlayer* camperPlayer = nullptr;
    ASlasherPlayer* slasherPlayer = nullptr;

    // Validate Owner
    ADBDPlayerControllerBase* playerController = Cast<ADBDPlayerControllerBase>(this->GetOwner());
    
    if (IsValid(playerController) == true && playerController->IsActorBeingDestroyed() == false)
    {
        ACharacter* pawn = playerController->GetCharacter();
        if (pawn != nullptr)
        {
            camperPlayer = Cast<ACamperPlayer>(pawn);
            slasherPlayer = Cast<ASlasherPlayer>(pawn);
        }
    }

    // --- Camper Inventory Handling ---
    if (IsValid(camperPlayer) == true && camperPlayer->IsActorBeingDestroyed() == false && camperPlayer->IsValidImpl())
    {
        bool isDeadOrSacrificed = (this->PlayerData._playerGameState == VE_Dead || this->PlayerData._playerGameState == VE_Sacrificed);
        
        if (!isDeadOrSacrificed)
        {
            // Give post game achievements if the camper survived and wasn't dead/sacrificed
            this->Local_HandlePostGameInventoryAchievements();
        }

        // Check for CamperItemLostPrevention offering (Type 10 / 0x0A based on IDA)
        bool hasItemLostPrevention = false;
        
        // The disassembly accesses player state at offset 928 (0x3A0) on the CamperPlayer.
        APlayerState* camperPlayerState = camperPlayer->PlayerState;
        if (IsValid(camperPlayerState) == true && camperPlayerState->IsActorBeingDestroyed() == false)
        {
            // It fetches the ID from offset 0x3A8 on the player state
            int32 playerIdForOffering = camperPlayerState->PlayerId;
            hasItemLostPrevention = dbdGameInstance->HasOfferingOfType(EOfferingEffectType::CamperItemLostPrevention, playerIdForOffering);
        }

        // If the player died and didn't have the prevention offering, they lose their items.
        if (isDeadOrSacrificed && !hasItemLostPrevention)
        {
            playerData->ValidateData(true);
            playerData->Local_UpdateToSelectedCharacter(this, false);
            this->_postGameInventoryHandled = true;
            this->LogCurrentLoadout();
            return;
        }

        // Process the camper's inventory to save remaining items
        TArray<ACollectable*> playerInventory = camperPlayer->GetPlayerInventory();
        for (ACollectable* item : playerInventory)
        {
            if (IsValid(item) == true && item->IsActorBeingDestroyed() == false)
            {
                // Check if there is enough usage percent left to keep the item.
                float usageLeft = item->GetUsePercentLeft();
                float threshold = dbdGameInstance->DesignTunables->GetTunableValue(TunableValues::ITEM_PERSIST_USAGE_THRESHOLD, false);
                
                if (usageLeft >= threshold)
                {
                    // Item is kept, save it to the profile
                    localProfileData->AddNamedItemToInventory(item->ItemID);
                    
                    // Also save its addons
                    TArray<UItemAddon*>& addons = item->GetItemAddons();
                    for (UItemAddon* addon : addons)
                    {
                        if (addon != nullptr)
                        {
                            // The disassembly fetches the FName of the addon at offset 30 (0x1E)
                            localProfileData->AddNamedItemToInventory(addon->ID);
                        }
                    }
                }
            }
        }
        
        // Finalize state update
        playerData->ValidateData(true);
        playerData->Local_UpdateToSelectedCharacter(this, false);
        this->_postGameInventoryHandled = true;
        this->LogCurrentLoadout();
        return;
    }

    // --- Slasher Inventory Handling ---
    if (IsValid(slasherPlayer) == true && slasherPlayer->IsActorBeingDestroyed() == false && slasherPlayer->IsValidImpl())
    {
        // Check for KillerItemLostPrevention offering (Type 11 / 0x0B based on IDA)
        if (dbdGameInstance->HasOfferingOfType(EOfferingEffectType::KillerItemLostPrevention, this->PlayerId))
        {
            // If they have the offering, refund their power addons
            TArray<FName> powerAddonsToKeep = playerData->CurrentLoadout.PowerAddOns;
            for (const FName& addonName : powerAddonsToKeep)
            {
                localProfileData->AddNamedItemToInventory(addonName);
            }
        }
    }

    // Fallback validation and update
    playerData->ValidateData(true);
    playerData->Local_UpdateToSelectedCharacter(this, false);
    this->_postGameInventoryHandled = true;
    this->LogCurrentLoadout();
}

// starts at line 615
void ADBDPlayerState::Local_HandlePostGameInventoryAchievements()
{
    // Verify it's a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == false)
    {
        return;
    }

    // Only process achievements if the game type affects progression
    if (dbdGameInstance->GetCurrentGametypeAffectsProgression() == false)
    {
        return;
    }

    // Create a local UniqueNetId copy for the persistent data lookup
    FUniqueNetIdRepl netIdRepl;
    netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

    // Fetch the player's persistent data
    FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
    
    if (playerData != nullptr)
    {
        UDBDDesignTunables* tunables = dbdGameInstance->DesignTunables;

        // --- Check for New Item Achievement ---
        // If the item in the current loadout is different from the starting loadout, 
        // and it's not empty, it means they picked up a new item.
        FName currentItem = playerData->CurrentLoadout.Item;
        FName startingItem = playerData->StartingLoadout.Item;
        
        // The disassembly checks against a specific empty constant value.
        if (currentItem != startingItem &&
            currentItem != LoadoutSlotDefaultValue::EMPTY)
        {
            // 0x47 (71) corresponds to DBDCamperScore_NewItem based on IDA
            if (dbdGameInstance->GameEventTracker != nullptr)
            {
                dbdGameInstance->GameEventTracker->FireGameflowEvent(EDBDScoreTypes::DBDCamperScore_NewItem, 1.0f, this);
            }
        }

        // --- Setup Data for Ultra Rare Keep Achievement ---
        FItemData* itemData = nullptr;
        FItemAddonProperties* firstAddonData = nullptr;
        FItemAddonProperties* secondAddonData = nullptr;

        if (tunables != nullptr)
        {
            itemData = tunables->GetItemData(startingItem, true);

            if (playerData->StartingLoadout.ItemAddOns.Num() >= 1)
            {
                firstAddonData = tunables->GetItemAddonData(playerData->StartingLoadout.ItemAddOns[0]);
            }

            if (playerData->StartingLoadout.ItemAddOns.Num() >= 2)
            {
                secondAddonData = tunables->GetItemAddonData(playerData->StartingLoadout.ItemAddOns[1]);
            }
        }

        // --- Check for Ward Offering ---
        bool hasWardOffering = false;
        FOfferingProperties* offeringData = nullptr;
        if (tunables != nullptr)
        {
            offeringData = tunables->GetOfferingData(playerData->StartingLoadout.CamperFavor, true);
        }

        if (offeringData != nullptr)
        {
            for (const FName& tag : offeringData->Tags)
            {
                // The disassembly checks against an internal constant string for the ward tag
                if (tag == OfferingTags::OFFERING_TAG_WARD) 
                {
                    hasWardOffering = true;
                    break;
                }
            }
        }

        // --- Check for Keep Ultra Rare Achievement ---
        // This achievement triggers if the player kept an ultra-rare (rarity >= 4) item or addon,
        // AND they did NOT use a ward offering to protect it.
        if (!hasWardOffering)
        {
            bool keptUltraRare = false;

            // Check if they kept their ultra-rare base item
            if (itemData != nullptr && 
                startingItem == currentItem && 
                itemData->Rarity >= EItemRarity::UltraRare)
            {
                keptUltraRare = true;
            }
            
            // Check if they kept an ultra-rare first addon
            if (!keptUltraRare && firstAddonData != nullptr && 
                playerData->CurrentLoadout.ItemAddOns.Num() >= 1 &&
                playerData->StartingLoadout.ItemAddOns[0] == playerData->CurrentLoadout.ItemAddOns[0] &&
                firstAddonData->Rarity >= EItemRarity::UltraRare)
            {
                keptUltraRare = true;
            }

            // Check if they kept an ultra-rare second addon
            if (!keptUltraRare && secondAddonData != nullptr && 
                playerData->CurrentLoadout.ItemAddOns.Num() >= 2 &&
                playerData->StartingLoadout.ItemAddOns[1] == playerData->CurrentLoadout.ItemAddOns[1] &&
                secondAddonData->Rarity >= EItemRarity::UltraRare)
            {
                keptUltraRare = true;
            }

            if (keptUltraRare)
            {
                // 0x48 (72) corresponds to DBDCamperScore_KeepUltraRare based on IDA
                if (dbdGameInstance->GameEventTracker != nullptr)
                {
                    dbdGameInstance->GameEventTracker->FireGameflowEvent(EDBDScoreTypes::DBDCamperScore_KeepUltraRare, 1.0f, this);
                }
            }
        }
    }
}

// starts at line 647
void ADBDPlayerState::Multicast_SetInParadise_Implementation(bool killed)
{
    // Set the local inParadise flag to true
    this->_inParadise = true;

    // Standard check to verify the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // The disassembly checks a weak object pointer at offset 0x1B0 inside UDBDGameInstance.
        TWeakObjectPtr<ALevelParadise> levelParadisePtr = dbdGameInstance->Paradise;
        
        if (levelParadisePtr.IsValid())
        {
            levelParadisePtr->UpdateParadise(this);
            
            // Update the user game stats cache after setting paradise state
            this->FullUpdateUserGameStatsCache();
        }
    }
}

// starts at line 665
void ADBDPlayerState::UpdateOngoingScores()
{
    // Check if the array has any elements before iterating
    if (this->_ongoingScoreEvents.Num() > 0)
    {
        // The disassembly iterates through the _ongoingScoreEvents array.
        // It updates a float value at offset 4 of FOngoingScoreData by adding 0.25f.        
        for (FOngoingScoreData& scoreData : this->_ongoingScoreEvents)
        {
            // Increase the elapsed time by the timer tick rate (0.25 seconds)
            scoreData.TimeSinceScore += 0.25f;

            // If the elapsed time has reached or exceeded the target time, apply the score
            if (scoreData.TimeSinceScore >= scoreData.ApplyScoreTimeout)
            {
                // Ensure the GameInstance is valid and is a UDBDGameInstance
                UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
                if (IsValid(dbdGameInstance) == true)
                {
                    // Apply the score event authoritatively
                    // The third parameter 'true' indicates it should redirect to the Game Event System.
                    this->Authority_ApplyScoreEvent(scoreData.ScoreType, scoreData.AccumulatedPercent, true);
                }
            }
        }
    }

    // Remove all ongoing score events that have completed their duration.
    // The disassembly shows a call to TArray::RemoveAll with a lambda.
    this->_ongoingScoreEvents.RemoveAll([](const FOngoingScoreData& scoreData) {
        return scoreData.TimeSinceScore >= scoreData.ApplyScoreTimeout;
    });
}

// starts at line 684
void ADBDPlayerState::Multicast_ScoreEvent_Implementation(FScoreEvent scoreEvent)
{
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == false)
    {
        return;
    }

    // Retrieve the score value definition from design tunables
    FScoreValue* scoreValue = dbdGameInstance->DesignTunables->GetScoreValue(scoreEvent.ScoreType);

    if (scoreValue != nullptr)
    {
        // 1. Log the score event internally for the PlayerState
        // The disassembly shows finding/emplacing an entry in the _scoreEvents map
        FScoreEventArray& eventArray = this->_scoreEvents.FindOrAdd(scoreValue->ID);
        eventArray.Add(scoreEvent);

        // 2. Update cached game stats and thresholds
        this->UpdateGameStatsWithEvent(this->_cachedUserGameStats, scoreEvent);
        this->CalcGameStatsThresholds(this->_cachedUserGameStats);

        float progress = 0.0f;
        float experienceAwarded = scoreEvent.ExperienceAwarded;

        // 3. Handle Analytics and Progress
        AController* controller = Cast<AController>(this->GetOwner());
        
        if (IsValid(controller) == true && controller->IsActorBeingDestroyed() == false && controller->IsLocalController() == true)
        {
            // Check if the controlled pawn is valid and not pending kill
            APawn* pawn = controller->GetPawn();
            if (pawn != nullptr && IsValid(pawn))
            {
                // Update or Create Analytics data for this score type
                FScoreAnalytics& analytics = this->_playerScoreAnalyticsData.FindOrAdd(scoreEvent.ScoreType);
                
                // If it was a new entry, the disassembly shows a complex block initializing 
                // ClientId, MatchId (SessionId), and PlatformName.
                if (analytics.ScoreCount == 0)
                {
                    analytics.ScoreCategory = scoreEvent.ScoreType;
                    
                    ADBDGameState* gameState = dbdGameInstance->GetGameState();
                    if (IsValid(gameState) == true && gameState->IsActorBeingDestroyed() == false)
                    {
                        analytics.MatchId = gameState->_sessionId.ToString();
                    }
                    else
                    {
                        analytics.MatchId = TEXT("None");
                    }

                    analytics.ClientId = IOnlinePresencePlugin::Get().GetPlatformAccountId();
                    analytics.Platform = UDBDAnalytics::GetPlatformName();
                }

                // Increment counts and experience for analytics
                analytics.ScoreCount++;
                analytics.ScoreExperience += experienceAwarded;
            }
        }

        // 4. Calculate UI Progress and handle thresholds
        float skullThreshold = dbdGameInstance->DesignTunables->GetTunableValue(TunableValues::SCORE_TO_SKULLREWARD_THRESHOLD, false);
        
        if (skullThreshold > 0.0f && experienceAwarded > 0.0f)
        {
            // Create a temporary copy to check against current category experience
            FUserGameStats tempStats = this->_cachedUserGameStats;
            
            int32 currentCategoryExp = 0;
            if (tempStats._experiences.Contains(scoreValue->Category))
            {
                currentCategoryExp = tempStats._experiences[scoreValue->Category];
            }

            progress = static_cast<float>(currentCategoryExp) / skullThreshold;
            
            if (progress > 1.0f)
            {
                progress = 1.0f;
                
                // Check against category max
                float categoryMax = dbdGameInstance->DesignTunables->GetTunableValue(TunableValues::SCORE_CATEGORY_MAX, false);
                if ((static_cast<float>(currentCategoryExp) / categoryMax) >= 1.0f)
                {
                    // If at max, signal no further experience (compiler used -1.0f as signal)
                    experienceAwarded = -1.0f;
                }
            }
        }

        // 5. Broadcast the event to the UI and other listeners
        // The disassembly converts display text to string and uses literal string conversion
        FString displayString = UKismetSystemLibrary::MakeLiteralString(scoreValue->DisplayName.ToString());
        
        this->_scoreEventDelegate.Broadcast(
            scoreValue->Category,
            displayString,
            static_cast<int32>(experienceAwarded),
            progress
        );
    }
}

// starts at line 748
void ADBDPlayerState::Authority_ForceEndOngoingScoreEvent(EDBDScoreTypes scoreType)
{
    // Standard check to verify the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Iterate through the ongoing score events
        // The disassembly uses a while loop and manually shifts array elements using memcpy (RemoveSwap behavior).
        // It applies the score event before removing it.
        for (int32 i = 0; i < this->_ongoingScoreEvents.Num(); )
        {
            if (this->_ongoingScoreEvents[i].ScoreType == scoreType)
            {
                // Apply the score event authoritatively
                // The percentToAward is located at offset 12 (0x0C) in the struct
                this->Authority_ApplyScoreEvent(scoreType, this->_ongoingScoreEvents[i].AccumulatedPercent, true);

                // Remove the element from the array
                // The disassembly implements an O(1) RemoveSwap (copying the last element to the current index)
                // and then shrinks the array.
                this->_ongoingScoreEvents.RemoveAtSwap(i, 1, false);
                this->_ongoingScoreEvents.Shrink();
                
                // Do not increment 'i' because the current index now holds a new element (or we reached the end)
            }
            else
            {
                // Increment only if we didn't remove an element
                i++;
            }
        }
    }
}

// starts at line 777
void ADBDPlayerState::Authority_ForceCancelOngoingScoreEvent(EDBDScoreTypes scoreType)
{
    // The second provided pseudo-code block is the compiler-generated expansion of the 
    // standard Unreal Engine TArray::RemoveAll template function.
    // We replace the expanded algorithm with the standard UE template call using a lambda predicate.
    this->_ongoingScoreEvents.RemoveAll([scoreType](const FOngoingScoreData& scoreData)
    {
        // Check if the current ongoing score event matches the type we want to cancel
        return scoreData.ScoreType == scoreType;
    });
}

// starts at line 791
void ADBDPlayerState::AuthorityFireGameplayEvent(EDBDScoreTypes playerGameplayEventType, float amount, AActor* effector, AActor* target)
{
    if (this->Role == ROLE_Authority)
    {
        ADBDPlayerState::Authority_ApplyScoreEvent(this, playerGameplayEventType, amount, false);
        ADBDPlayerState::Multicast_FireGameplayEvent(this, playerGameplayEventType, amount, effector, target);
    }
}

// starts at line 800
void ADBDPlayerState::Multicast_FireGameplayEvent_Implementation(EDBDScoreTypes playerGameplayEventType, float amount, AActor* effector, AActor* target)
{
    // Ensure the event delegate is broadcasted to all bound listeners
    if (this->OnPlayerGameplayEvent.IsBound())
    {
        this->OnPlayerGameplayEvent.Broadcast(playerGameplayEventType, amount, effector, target);
    }

    // Add the event to the recent gameplay events tracker
    this->_recentGameplayEvents.AddGameplayEvent(playerGameplayEventType);
}

// starts at line 814
void ADBDPlayerState::FireScoreEvent(EDBDScoreTypes scoreType, float percentToAward, bool redirectToGameEventSystem)
{
    // Ensure the server has authority and a valid percent to award is provided
    if (this->Role == ROLE_Authority && percentToAward > 0.0f)
    {
        // Cast to UDBDGameInstance
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (IsValid(dbdGameInstance) == true)
        {
            // Retrieve the score value definition from design tunables
            FScoreValue* scoreValue = dbdGameInstance->DesignTunables->GetScoreValue(scoreType);
            
            if (scoreValue != nullptr)
            {
                // If there is no ongoing wait time (e.g. an instant action), apply the score immediately
                if (scoreValue->OngoingWaitTime <= 0.0f)
                {
                    this->Authority_ApplyScoreEvent(scoreType, percentToAward, redirectToGameEventSystem);
                }
                else
                {
                    // Handle continuous/ongoing scores (e.g. repairing a generator, healing)
                    bool foundExistingScore = false;
                    
                    // Check if an ongoing score event of this type already exists in the array
                    for (FOngoingScoreData& ongoingScore : this->_ongoingScoreEvents)
                    {
                        if (ongoingScore.ScoreType == scoreType)
                        {
                            // Reset the time elapsed and accumulate the percent to award
                            ongoingScore.TimeSinceScore = 0.0f;
                            ongoingScore.AccumulatedPercent += percentToAward;
                            foundExistingScore = true;
                            break;
                        }
                    }

                    // If not found, add a new entry to the array
                    if (!foundExistingScore)
                    {
                        FOngoingScoreData newOngoingScore;
                        newOngoingScore.ScoreType = scoreType;
                        newOngoingScore.AccumulatedPercent = percentToAward;
                        newOngoingScore.ApplyScoreTimeout = scoreValue->OngoingWaitTime;
                        newOngoingScore.TimeSinceScore = 0.0f;
                        
                        this->_ongoingScoreEvents.Add(newOngoingScore);
                    }
                }
            }
        }
    }
}

// starts at line 852
void ADBDPlayerState::Authority_ApplyScoreEvent(EDBDScoreTypes scoreType, float percentToAward, bool redirectToGameEventSystem)
{
    if (this->Role == ROLE_Authority && percentToAward > 0.0f)
    {
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (IsValid(dbdGameInstance) == true)
        {
            if (redirectToGameEventSystem == true)
            {
                AActor* effectorPawn = nullptr;
                
                AController* controller = Cast<AController>(this->GetOwner());
                if (IsValid(controller) == true && controller->IsActorBeingDestroyed() == false)
                {
                    effectorPawn = controller->GetPawn();
                }
                
                if (dbdGameInstance->GameEventTracker != nullptr)
                {
                    dbdGameInstance->GameEventTracker->FireGameEvent(scoreType, percentToAward, effectorPawn, nullptr);
                }
            }
            else
            {
                if (dbdGameInstance->DesignTunables != nullptr)
                {
                    FScoreValue* scoreValue = dbdGameInstance->DesignTunables->GetScoreValue(scoreType);
                    if (scoreValue != nullptr)
                    {
                        float perkScoreModifier = this->GetPerkScoreModifier(scoreValue);
                        float baseExperience = static_cast<float>(scoreValue->ExperienceValue) * percentToAward;
                        float experienceToAward = perkScoreModifier * baseExperience;
                        
                        int32 eventCount = 0;
                        FScoreEventArray* eventArray = this->_scoreEvents.Find(scoreValue->ID);
                        if (eventArray != nullptr)
                        {
                            eventCount = eventArray->Array.Num();
                        }
                        
                        if (scoreValue->ExperienceDepreciationCurve != nullptr)
                        {
                            float depreciationMultiplier = scoreValue->ExperienceDepreciationCurve->GetFloatValue(static_cast<float>(eventCount + 1));
                            experienceToAward = experienceToAward * depreciationMultiplier;
                        }
                        
                        if (scoreValue->MaxExperienceValue > 0)
                        {
                            float totalExperienceForCategory = 0.0f;
                            if (eventArray != nullptr)
                            {
                                for (int32 i = 0; i < eventArray->ScoreEvents.Num(); i++)
                                {
                                    totalExperienceForCategory = totalExperienceForCategory + eventArray->Array[i].ExperienceAwarded;
                                }
                            }
                            
                            float maxExperience = static_cast<float>(scoreValue->MaxExperienceValue);
                            if (totalExperienceForCategory >= maxExperience)
                            {
                                experienceToAward = 0.0f;
                            }
                            else if ((totalExperienceForCategory + experienceToAward) >= maxExperience)
                            {
                                experienceToAward = maxExperience - totalExperienceForCategory;
                            }
                        }
                        
                        float offeringsModifier = dbdGameInstance->GetOfferingsScoreModification(scoreValue, this->PlayerId, this->GameRole);
                        float perkBonusModifier = this->GetPerkBonusScoreModifier(scoreValue);
                        
                        float totalBonusModifier = offeringsModifier + perkBonusModifier + 1.0f;
                        float finalScore = totalBonusModifier * experienceToAward;
                        
                        static IConsoleVariable* CVarDisplayScoreText = IConsoleManager::Get().FindConsoleVariable(TEXT("DBD.ShowDebugScore"));
                        if (CVarDisplayScoreText != nullptr && CVarDisplayScoreText->GetFloat() != 0.0f)
                        {
                            FString scoreTypeString = Enum::ToString(scoreType);
                            FString displayString = scoreValue->DisplayName.ToString();
                            FString debugMessage = FString::Printf(TEXT("Score Event: %s Display String: %s Experience Value: %f"), *scoreTypeString, *displayString, finalScore);
                            
                            if (GEngine != nullptr)
                            {
                                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, debugMessage);
                            }
                        }
                        
                        this->Score = this->Score + finalScore;
                        
                        FScoreEvent scoreEvent(scoreType, experienceToAward, totalBonusModifier);
                        this->Multicast_ScoreEvent(scoreEvent);
                    }
                }
            }
        }
    }
}

// starts at line 940
void ADBDPlayerState::FireActivePerkEvent(FName perkID, int32 levelToDisplay, int32 iconFilePathIndex, float percentage, int32 chargeCount)
{
    // Broadcast the active perk event to all bound listeners
    this->_activePerkEventDelegate.Broadcast(perkID, levelToDisplay, iconFilePathIndex, percentage, chargeCount);
}

// starts at line 945
void ADBDPlayerState::FireActiveStatusEffectEvent(FName statusEffectID, float percentage, int iconFilePathIndex, int levelToDisplay)
{
    this->_activeStatusEffectEventDelegate.Broadcast(statusEffectID, percentage, iconFilePathIndex, levelToDisplay);
}

// starts at line 950
void ADBDPlayerState::FireActiveStatusViewEvent(FName statusViewID, FName uniqueSourceID, FStatusViewSource statusViewSource)
{
    this->_activeStatusViewEventDelegate.Broadcast(statusViewID, uniqueSourceID, statusViewSource);
}

// starts at line 960
void ADBDPlayerState::SetSkillCheckInteractionData_Implementation(FSkillCheckInteractionData skillCheckInteraction)
{
    this->Multicast_SetSkillCheckInteractionData(this, skillCheckInteraction);
}

// starts at line 970
void ADBDPlayerState::UpdateSkillCheckInteractionData_Implementation(float progress)
{
    this->Multicast_UpdateSkillCheckInteractionData(progress);
}

// starts at line 975
const FUserGameStats& ADBDPlayerState::GetUserGameStats(bool forceUpdate) const
{
    if (forceUpdate == false)
    {
        return this->_cachedUserGameStats;
    }
    
    this->CalcGameStatsThresholds(this->_cachedUserGameStats);
    return this->_cachedUserGameStats;
}

// starts at line 984
void ADBDPlayerState::CalcGameStatsThresholds(FUserGameStats& stats) const
{
    // Verify we have a DBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    
    if (IsValid(dbdGameInstance) === true)
    {
        // Only perform calculation if the role is Survivor (Escaped check implies they were a camper)
        if (this->GameRole == EPlayerRole::VE_Camper)
        {
            float totalExperience = 0.0f;
            UDBDDesignTunables* tunables = dbdGameInstance->DesignTunables;
            
            if (tunables != nullptr)
            {
                float threshold = tunables->GetTunableValue(TunableValues::SCORE_TO_SKULLREWARD_THRESHOLD, false);
                
                // Iterate through all experience categories using the provided stats TMap
                for (const TPair<EDBDScoreCategory, int32>& experiencePair : stats._experiences)
                {
                    int32 categoryExp = experiencePair.Value;
                    
                    // Clamp the experience value between 0 and the threshold
                    if (categoryExp < 0)
                    {
                        categoryExp = 0;
                    }
                    else if (categoryExp > static_cast<int32>(threshold))
                    {
                        categoryExp = static_cast<int32>(threshold);
                    }
                    
                    totalExperience += static_cast<float>(categoryExp);
                }

                stats.Skulls = -1;

                // The disassembly iterates over an array starting at offset 0x50 from dbdGameInstance+0xF0.
                // This implies a lookup inside the DesignTunables class for skull thresholds.
                // The array elements are floats. The count is at offset 0x58.
                for (int32 i = 0; i < tunables->PipThresholds.Num(); ++i)
                {
                    // If total experience exceeds the threshold at index i, grant a skull
                    if (totalExperience >= tunables->SkullThresholds[i])
                    {
                        stats.Skulls++;
                    }
                }

                // Apply penalties or floor the skull count based on state
                if (this->CanLoseSkulls() && stats.Disconnected)
                {
                    // Apply disconnect penalty tunable value.
                    // The floating point math in assembly is a common pattern for FMath::RoundToInt.
                    float penalty = tunables->GetTunableValue(TunableValues::DISCONNECT_SKULL_PENALTY, false);
                    stats.Skulls = FMath::RoundToInt(penalty);
                }
                else if (!this->CanLoseSkulls() && stats.Skulls < 0)
                {
                    // Ensure we don't return negative skulls if the player cannot lose them
                    stats.Skulls = 0;
                }
            }
        }
    }
}

// starts at line 1020
void ADBDPlayerState::UpdateGameStatsWithEvent(FUserGameStats& gameStats, const FScoreEvent& event)
{
    // Make sure that experience to award is valid
    if (FMath::IsNaN(event.ExperienceAwarded) == false && event.ExperienceAwarded > 0.0f)
    {
        // Verify GameInstance type
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (IsValid(dbdGameInstance) == true)
        {
            // Get the maximum allowed experience per category
            float categoryMax = dbdGameInstance->DesignTunables->GetTunableValue(TunableValues::SCORE_CATEGORY_MAX, false);
            
            // Get the score value details for this specific event type
            FScoreValue* scoreValue = dbdGameInstance->DesignTunables->GetScoreValue(event.ScoreType);
            
            if (scoreValue != nullptr)
            {
                EDBDScoreCategory category = scoreValue->Category;
                
                // Get the current experience in this category
                int32 currentExperience = gameStats->GetExperience(category);
                
                // Calculate the maximum possible experience using the tunable value
                // The floating point logic is FMath::RoundToInt(categoryMax)
                int32 maxAllowedExp = FMath::RoundToInt(categoryMax);
                
                // Calculate new experience by adding the awarded amount (clamped to the remaining allowance)
                // The logic computes min(categoryMax - currentExperience, experienceAwarded)
                float expToAdd = FMath::Min(categoryMax - static_cast<float>(currentExperience), event.ExperienceAwarded);
                int32 newExperience = currentExperience + FMath::RoundToInt(expToAdd);
                
                // Clamp the result between 0 and maxAllowedExp
                newExperience = FMath::Clamp(newExperience, 0, maxAllowedExp);
                
                // Set the updated experience back into the stats struct
                gameStats->SetExperience(category, newExperience);

                // --- Calculate Bonus Experience ---
                
                // Retrieve the active bonus events from the game instance
                // The disassembly uses offset 0x218 on UDBDGameInstance
                const TArray<FBonusPointsEventData>& activeBonusEvents = dbdGameInstance->BonusPointsEvents;
                
                float baseExperience = static_cast<float>(newExperience);
                
                // The base event might have its own modifier (e.g. from perks/offerings).
                // Modifier logic starts at (BonusPointModifier - 1.0) * BaseExperience.
                float totalBonusPoints = (event.BonusPointModifier - 1.0f) * baseExperience;

                // Add bonus points from globally active events
                for (const FBonusPointsEventData& bonusEvent : activeBonusEvents)
                {
                    totalBonusPoints += (bonusEvent.Multiplier - 1.0f) * baseExperience;
                }

                // Save the calculated bonus points into the stats struct for the category
                // The TMap Emplace/Find logic corresponds to:
                int32 roundedBonusPoints = FMath::RoundToInt(totalBonusPoints);
                gameStats.BonusExperience.FindOrAdd(category) = roundedBonusPoints;
            }
        }
    }
}

// starts at line 1052
void ADBDPlayerState::FullUpdateUserGameStatsCache()
{
    // Clear and re-initialize the cached user game stats maps
    this->_cachedUserGameStats.BonusExperience.Empty();
    this->_cachedUserGameStats._experiences.Empty();
    this->_cachedUserGameStats.Disconnected = false;
    this->_cachedUserGameStats.FearTokens = 0;

    // Cast to UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // 1. Fetch persistent data to check if the player disconnected
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

        UDBDPersistentData* persistentData = dbdGameInstance->_persistentData;
        if (persistentData != nullptr)
        {
            FCachedPlayerScoreData* scoreCache = persistentData->GetPlayerScoreCache(netIdRepl);
            if (scoreCache != nullptr)
            {
                // The disassembly checks: (PlayerGameState - 6) <= 1
                this->_cachedUserGameStats.Disconnected = (scoreCache->PlayerGameState == EGameState::VE_Disconnected || 
                                                           scoreCache->PlayerGameState == EGameState::VE_ManuallyLeftMatch);
            }
        }

        // 2. Calculate Fear Tokens
        UDBDDesignTunables* tunables = dbdGameInstance->DesignTunables;
        if (tunables != nullptr)
        {
            float tokensPerSeconds = tunables->GetTunableValue(TunableValues::FEAR_TOKENS_PER_X_SECONDS, false);
            
            if (this->_cachedUserGameStats.Disconnected == false && tokensPerSeconds != 0.0f)
            {
                // Calculate base tokens from total activity time
                float totalActiveTime = this->ActivityTracker->_inactiveTime + this->ActivityTracker->_activeTime;
                int32 baseTokens = FMath::RoundToInt(totalActiveTime / tokensPerSeconds);
                
                int32 tunableBaseTokens = FMath::RoundToInt(tunables->GetTunableValue(TunableValues::FEAR_TOKENS_BASE, false));
                baseTokens += tunableBaseTokens;

                // Calculate tokens lost due to stillness (inactive time)
                float lostPerStillness = tunables->GetTunableValue(TunableValues::FEAR_TOKEN_LOST_PER_X_STILLNESS, false);
                int32 tokensLost = 0;
                if (lostPerStillness != 0.0f)
                {
                    tokensLost = FMath::RoundToInt(this->ActivityTracker->_inactiveTime / lostPerStillness);
                    if (tokensLost < 0)
                    {
                        tokensLost = 0;
                    }
                }

                int32 currentTokens = baseTokens - tokensLost;

                // Clamp to max tokens allowed per match
                int32 maxTokens = FMath::RoundToInt(tunables->GetTunableValue(TunableValues::FEAR_TOKENS_MAX_PER_MATCH, false));
                
                if (currentTokens < 0)
                {
                    currentTokens = 0;
                }
                else if (currentTokens > maxTokens)
                {
                    currentTokens = maxTokens;
                }

                this->_cachedUserGameStats.FearTokens = currentTokens;
            }
        }
    }

    // 3. Iterate through all score events and update the game stats cache
    // The disassembly shows iterating a TMap (this->_scoreEvents) which holds arrays of FScoreEvent
    for (auto& scoreEventPair : this->_scoreEvents)
    {
        FScoreEventArray& eventArray = scoreEventPair.Value;
        
        // Loop through each event in the array
        // The disassembly implies a property inside the value representing the array
        for (const FScoreEvent& scoreEvent : eventArray.ScoreEvents)
        {
            this->UpdateGameStatsWithEvent(this->_cachedUserGameStats, scoreEvent);
        }
    }

    // 4. Update Badges/Skulls if the player is Dead
    // The disassembly checks GameRole == VE_Slasher (which corresponds to 1 in this context)
    if (this->GameRole == EPlayerRole::VE_Slasher)
    {
        int32 totalBadgeScore = 0;
        
        if (IsValid(this->BadgeEvaluator) == true)
        {
            UDBDBadgeRewardData badgeRewardData;
            this->BadgeEvaluator->Local_GetRewardedBadges(badgeRewardData);

            if (badgeRewardData.PrimaryBadge != nullptr)
            {
                totalBadgeScore = badgeRewardData.PrimaryBadge->PipsToAward; 
            }            
        }
        
        this->_cachedUserGameStats.Skulls = totalBadgeScore;
    }

    // 5. Finalize the thresholds
    this->CalcGameStatsThresholds(this->_cachedUserGameStats);
}

// starts at line 1125
bool ADBDPlayerState::CanLoseSkulls() const
{
    // Standard check to verify the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // If the net mode is ListenServer, we assume we can lose skulls (returns true)
        if (this->GetNetMode() != NM_ListenServer)
        {
            // The disassembly checks a boolean flag at offset 0x240 inside UDBDGameInstance.
            return dbdGameInstance->HasServerLeftMatch == false;
        }
    }

    // Default to true if game instance is invalid
    return true;
}

// starts at line 1136
void ADBDPlayerState::LogScoreEvents() const
{
    // Check if there are any score events to log
    if (this->_scoreEvents.Num() > 0)
    {
        // Iterate through all tracked score events
        // The disassembly shows iterating a TMap (this->_scoreEvents)
        for (const auto& scoreEventPair : this->_scoreEvents)
        {
            const FScoreEventArray& eventArray = scoreEventPair.Value;
            
            // Loop through each event in the array
            for (const FScoreEvent& scoreEvent : eventArray.ScoreEvents)
            {
                // Verify GameInstance type
                UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
                if (IsValid(dbdGameInstance) == true)
                {
                    // Check if the player has persistent data. We create a temporary UniqueNetId to do this.
                    FUniqueNetIdRepl netIdRepl;
                    netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

                    FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
                    
                    // Only log if the player has valid persistent data
                    if (playerData != nullptr)
                    {
                        // Check verbosity level against 5 (Log)
                        UE_LOG(LogGameFlow, Log, TEXT("Logging Player Score Event: %i Experience Value: %f"), static_cast<int32>(scoreEvent.ScoreType), scoreEvent.ExperienceAwarded);
                    }
                }
            }
        }
    }
}

// starts at line 1159
void ADBDPlayerState::Reset()
{
    Super::Reset();
}

// starts at line 1164
void ADBDPlayerState::FlushScoreEvents()
{
    // Empty the score events map
    this->_scoreEvents.Empty();

    // The disassembly shows retrieving the GameMode from the World
    UWorld* world = this->GetWorld();
    if (world != nullptr)
    {
        // Verify the GameMode is a UDBDGameMode
        ADBDGameMode* dbdGameMode = Cast<ADBDGameMode>(world->GetAuthGameMode());
        if (IsValid(dbdGameMode) == true && !dbdGameMode->IsActorBeingDestroyed())
        {
            // Clear killer goal points on the GameMode
            dbdGameMode->ClearKillerGoalPoints();
        }
    }

    // Rebuild the user game stats cache
    this->FullUpdateUserGameStatsCache();
}

// starts at line 1178
void ADBDPlayerState::Multicast_SetAsLeftMatch_Implementation()
{
    // Apply the local state changes for leaving the match
    this->Local_SetAsLeftMatch();

    // Broadcast the disconnection event to any bound listeners
    if (this->_onPlayerDisconnectedDelegate.IsBound())
    {
        this->_onPlayerDisconnectedDelegate.Broadcast();
    }
}

// starts at line 1187
void ADBDPlayerState::Local_SetAsLeftMatch()
{
    // Update local gamestate to reflect the player manually left the match
    this->SetPlayerGameState(EGameState::VE_ManuallyLeftMatch);

    // Standard check to verify the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Check if the unique net id is valid
        // The disassembly checks a pointer at 0x3E0, which maps to UniqueId.UniqueNetId.Object
        if (this->UniqueId.GetUniqueNetId().IsValid())
        {
            // Get the hex encoded string of the player's unique net ID
            FString netIdString = this->UniqueId.GetUniqueNetId()->GetHexEncodedString();

            // Fetch the cached player score using the string ID
            FCachedPlayerScoreData* scoreCache = dbdGameInstance->_persistentData->GetPlayerScoreCache(netIdString);
            
            if (scoreCache != nullptr)
            {
                // Force the cached state to manually left match
                scoreCache->PlayerGameState = EGameState::VE_ManuallyLeftMatch;
                
                // Reset their cached score
                scoreCache->PlayerScore = 0;
            }
        }
    }

    // Flush any ongoing/pending score events since the player is leaving
    this->FlushScoreEvents();
}

// starts at line 1215
FDelegateHandle ADBDPlayerState::RegisterPlayerDataListener(const FPlayerDataUpdatedDelegate& delegate)
{
    // The disassembly shows a call to TBaseMulticastDelegate::Add to add the delegate to the _playerDataUpdatedDelegate.
    // The return value is the FDelegateHandle produced by Add.
    return this->_playerDataUpdatedDelegate.Add(delegate);
}

// starts at line 1220
void ADBDPlayerState::UnregisterPlayerDataListener(FDelegateHandle handle)
{
    // Remove the previously registered delegate using its handle
    // The disassembly shows a call to RemoveDelegateInstance (or Remove in Unreal's public API) 
    // on the _playerDataUpdatedDelegate.
    this->_playerDataUpdatedDelegate.Remove(handle);
}

// starts at line 1225
FDelegateHandle ADBDPlayerState::RegisterScoreEventListener(const FScoreEventDelegate& delegate)
{
    // The disassembly explicitly clears the multicast delegate before adding the new one.
    // This implies it effectively functions as a single-cast or actively managed listener slot.
    this->_scoreEventDelegate.Clear();
    
    // Add the new delegate and return its handle.
    return this->_scoreEventDelegate.Add(delegate);
}

// starts at line 1231
void ADBDPlayerState::UnregisterScoreEventListener(FDelegateHandle handle)
{
    // Remove the previously registered score event listener using its handle
    // The disassembly shows a call to RemoveDelegateInstance (which is exposed as Remove() in the public API)
    // on the _scoreEventDelegate at offset 0x480.
    this->_scoreEventDelegate.Remove(handle);
}

// starts at line 1236
FDelegateHandle ADBDPlayerState::RegisterSkillCheckListener(const FSkillCheckUpdatedEventDelegate& delegate)
{
    this->_skillCheckUpdatedEventDelegate.Clear();
    
    return this->_skillCheckUpdatedEventDelegate.Add(delegate);
}

// starts at line 1242
void ADBDPlayerState::UnregisterSkillCheckListener(FDelegateHandle handle)
{
    this->_skillCheckUpdatedEventDelegate.Remove(handle);
}

// starts at line 1247
FDelegateHandle ADBDPlayerState::RegisterActivePerkListener(const FActivePerkEventDelegate& delegate)
{
    this->_activePerkEventDelegate.Clear();
    
    return this->_activePerkEventDelegate.Add(delegate);
}

// starts at line 1253
void ADBDPlayerState::UnregisterActivePerkListener(FDelegateHandle handle)
{
    this->_activePerkEventDelegate.Remove(handle);
}

// starts at line 1258
FDelegateHandle ADBDPlayerState::RegisterActiveStatusEffectListener(const FActiveStatusEffectEventDelegate& delegate)
{
    this->_activeStatusEffectEventDelegate.Clear();
    
    return this->_activeStatusEffectEventDelegate.Add(delegate);
}

// starts at line 1264
void ADBDPlayerState::UnregisterActiveStatusEffectListener(FDelegateHandle handle)
{
    this->_activeStatusEffectEventDelegate.Remove(handle);
}

// starts at line 1274
void ADBDPlayerState::UnregisterActiveStatusViewListener(FDelegateHandle handle)
{
    this->_activeStatusViewEventDelegate.Remove(handle);
}

// starts at line 1279
void ADBDPlayerState::OnRep_DisplayData()
{
    // Broadcast that the display data has been replicated/updated.
    // The disassembly shows template casting artifacts typical of Unreal's delegate macros, 
    // but the core logic is simply broadcasting the delegate with the player state itself as the parameter.
    if (this->_playerDataUpdatedDelegate.IsBound())
    {
        this->_playerDataUpdatedDelegate.Broadcast(this);
    }
}

// starts at line 1285
void ADBDPlayerState::PlayerDataUpdated()
{
    this->RitualHandler->Reset();
    this->OnRep_DisplayData();
}

// starts at line 1291
void ADBDPlayerState::Client_RemoveItemFromInventory_Implementation(FName toRemove, bool updateLoadout)
{
    // Ignore invalid or empty item IDs
    // The disassembly checks against global constants for "EMPTY" and "LOCKED"
    if (toRemove != LoadoutSlotDefaultValue::EMPTY && 
        toRemove != Assuming LoadoutSlotDefaultValue::LOCKED && 
        toRemove != NAME_None)
    {
        // Verify GameInstance type
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (IsValid(dbdGameInstance) == true)
        {
            // Fetch the character's saved profile data based on this PlayerState
            FCharacterSavedProfileData* profileData = dbdGameInstance->GetCharacterSavedProfileDataForPlayerState(this);
            
            if (profileData != nullptr)
            {
                // Attempt to remove the item from the inventory.
                // The function returns an int32, where 1 likely indicates success (item was found and removed).
                if (profileData->RemoveItemFromInventory(toRemove) == 1)
                {
                    // If requested, also remove the item from the active loadout
                    if (updateLoadout)
                    {
                        profileData->CharacterLoadoutData.RemoveItem(toRemove);
                    }

                    // Check if the removed item was an offering to fire analytics/achievements
                    FOfferingProperties* offeringData = dbdGameInstance->DesignTunables->GetOfferingData(toRemove, false);
                    
                    // If it's an offering and its rarity is UltraRare (>= 4)
                    if (offeringData != nullptr && offeringData->Rarity >= EItemRarity::UltraRare) 
                    {
                        if (dbdGameInstance->GameEventTracker != nullptr)
                        {
                            // 0x83 (131) corresponds to DBDPlayerScore_BurnOfferingUltraRare
                            dbdGameInstance->GameEventTracker->FireGameflowEvent(EDBDScoreTypes::DBDPlayerScore_BurnOfferingUltraRare, 1.0f, this);
                        }
                    }
                }
            }
        }
    }
}

// starts at line 1318
bool ADBDPlayerState::IsEscaped() const
{
  return this->PlayerData._playerGameState == EGameState::VE_Escaped || 
         this->PlayerData._playerGameState == EGameState::VE_EscapedInjured;
}

// starts at line 1323
void ADBDPlayerState::Multicast_SetEquipment_Implementation(int32 slotIndex, FName itemId, bool callOnRep)
{
    // Verify GameInstance type
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Set up UniqueNetId replica to lookup persistent data
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

        // Fetch the player's persistent data
        FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
        if (playerData != nullptr)
        {
            // Slot 1 (1 - 1 == 0 in disassembly) -> Item/Power
            if (slotIndex == 1)
            {
                if (this->GameRole == EPlayerRole::VE_Camper) // Survivor Role
                {
                    this->CamperData.EquipedItemId = itemId;
                    playerData->CurrentLoadout.Item = itemId;
                }
                else // Slasher Role
                {
                    this->SlasherData.EquipedPowerId = itemId;
                    playerData->CurrentLoadout.Power = itemId;
                }
            }
            // Slot 8 (8 - 1 == 7 in disassembly) -> Favor/Offering
            else if (slotIndex == 8)
            {
                this->PlayerData.EquipedFavorId = itemId;
                
                if (this->GameRole == EPlayerRole::VE_Camper) // Survivor Role
                {
                    playerData->CurrentLoadout.CamperFavor = itemId;
                }
                else // Slasher Role
                {
                    playerData->CurrentLoadout.SlasherFavor = itemId;
                }
            }

            // Trigger the replication callback manually if requested
            if (callOnRep)
            {
                this->OnRep_DisplayData();
            }
        }
    }
}

// starts at line 1365
void ADBDPlayerState::Server_SetSelectedCharacterId_Implementation(EPlayerRole forRole, int32 id, bool updateDisplayData)
{
    // Ensure the server is executing this (Role == ROLE_Authority, which is 3)
    if (this->Role == ROLE_Authority)
    {
        // Check verbosity level for GameFlow log category (3 is Log)
        UE_LOG(LogGameFlow, Log, TEXT("Authority_SetSelectedCharacterId: %i Player: %s"), id, *this->PlayerName);

        // If on the server, multicast the character selection change to all clients
        this->Multicast_SetSelectedCharacterId(forRole, id, updateDisplayData);
    }
    else
    {
        // Log a warning or info if called on a client (though the RPC mechanism should prevent this execution path normally)
        UE_LOG(LogGameFlow, Log, TEXT("Calling Authority_SetSelectedCharacterId from client"));
    }
}

// starts at line 1378
bool ADBDPlayerState::Server_SetSelectedCharacterId_Validate(EPlayerRole forRole, int32 id, bool updateDisplayData)
{
    return this->GameRole == EPlayerRole::VE_Slasher || (forRole != EPlayerRole::VE_Slasher && id < 0x10000000);
}

// starts at line 1387
void ADBDPlayerState::Multicast_SetSelectedCharacterId_Implementation(EPlayerRole forRole, int32 id, bool updateDisplayData)
{
    this->SetSelectedCharacterIDImpl(forRole, id, updateDisplayData);
}

// starts at line 1392
void ADBDPlayerState::SetSelectedCharacterIDImpl(EPlayerRole forRole, int32 id, bool updateDisplayData)
{
    // Ensure the provided character ID is valid
    if (id >= 0)
    {
        // 1. Update the local PlayerState variables based on the role
        if (forRole == EPlayerRole::VE_Slasher)
        {
            this->SelectedSlasherIndex = id;
        }
        else if (forRole == EPlayerRole::VE_Camper)
        {
            this->SelectedCamperIndex = id;
        }

        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (IsValid(dbdGameInstance) == true)
        {
            // Prepare the unique net ID for persistent data lookup
            FUniqueNetIdRepl netIdRepl;
            netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

            FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
            if (playerData != nullptr)
            {
                // 2. Update the persistent saved data with the new selection
                if (forRole == EPlayerRole::VE_Slasher)
                {
                    playerData->SavedData.SharedData.SelectedSlasherIndex = id;
                }
                else if (forRole == EPlayerRole::VE_Camper)
                {
                    playerData->SavedData.SharedData.SelectedCamperIndex = id;
                }

                // 3. Update the character if the owner is valid and local
                AController* controller = Cast<AController>(this->GetOwner());
                if (IsValid(controller) == true && controller->IsActorBeingDestroyed() == false)
                {
                    // The virtual function check in the pseudo-code likely verifies 
                    // if this is a local controller or if it's safe to process the update.
                    if (controller->IsLocalController())
                    {
                        // Apply the update locally
                        playerData->Local_UpdateToSelectedCharacter(this, true);
                    }
                }
            }
        }
    }

    // 4. Force a replication update for the display data if requested
    if (updateDisplayData)
    {
        this->OnRep_DisplayData();
    }
}

// starts at line 1435
void ADBDPlayerState::Server_SetSelectedCharacterCustomization_Implementation(EPlayerRole forRole, int32 id, const TArray<FName>& customizationItemIds, bool updateDisplayData)
{
    this->Multicast_SetSelectedCharacterCustomization_Implementation(forRole, id, customizationItemIds);
}

// starts at line 1446
void ADBDPlayerState::Multicast_SetSelectedCharacterCustomization_Implementation(EPlayerRole forRole, int32 id, const TArray<FName>& customizationItemIds, bool updateDisplayData)
{
    // Only apply the customization if it matches the currently selected character index for the given role
    if (forRole == EPlayerRole::VE_Slasher) 
    {
        if (this->SelectedSlasherIndex != id)
        {
            return;
        }
    }
    else if (forRole == EPlayerRole::VE_Camper) 
    {
        if (this->SelectedCamperIndex != id)
        {
            return;
        }
    }
    else
    {
        return; // Unhandled role
    }

    // Standard check to verify the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Prepare the unique net ID replica for persistent data lookup
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

        // Fetch the persistent data using the known LatentActionManager pointer offset
        FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
        if (playerData != nullptr)
        {
            // Update the customized items arrays
            playerData->SavedData.SharedData.SelectedCharacterCustomization = customizationItemIds;
            this->PlayerData.CustomizationMesh = customizationItemIds;
        }
    }
}

// starts at line 1464
int32 ADBDPlayerState::GetCurrentCharacterID() const
{
    if (this->GameRole == EPlayerRole::VE_Slasher)
    {
        // Return the selected slasher index (offset 0x754)
        return this->SelectedSlasherIndex;
    }
    else if (this->GameRole == EPlayerRole::VE_Camper)
    {
        // Return the selected camper index (offset 0x74C) if it's valid
        int32 camperIndex = this->SelectedCamperIndex;
        if (camperIndex >= 0)
        {
            return camperIndex;
        }
    }
    
    // Return an invalid ID (-1) for unhandled roles or invalid indices
    return -1;
}

// starts at line 1477
void ADBDPlayerState::AddDataForAnalytics(TArray<FAnalyticsEventAttribute>& OutAttr)
{
    // Log the base GameRole (Killer vs Survivor vs Spectator)
    UBHVRAnalytics::AddAttribute(OutAttr, TEXT("GameRole"), EnumToString(this->GameRole));

    // Serialize and log the general PlayerData struct
    UBHVRAnalytics::AddAttributeUStruct(OutAttr, TEXT("PlayerData"), FPlayerStateData::StaticStruct(), &this->PlayerData);

    // Log the "PrestigeAdjustedCharacterLevel"
    int32 adjustedLevel = this->PlayerData.CharacterLevel + (50 * this->PlayerData.PrestigeLevel);
    UBHVRAnalytics::AddAttribute(OutAttr, TEXT("PlayerData\\PrestigeAdjustedCharacterLevel_i"), adjustedLevel);

    // Serialize and log the UserGameStats struct we copied earlier
    FUserGameStats UserGameStats = this->_cachedUserGameStats;
    UBHVRAnalytics::AddAttributeUStruct(OutAttr, TEXT("UserGameStats"), FUserGameStats::StaticStruct(), &UserGameStats);

    // Log primitive values directly mapped from the PlayerState
    UBHVRAnalytics::AddAttribute(OutAttr, TEXT("Score"), this->Score);
    UBHVRAnalytics::AddAttribute(OutAttr, TEXT("ExactPing"), this->ExactPing);
    UBHVRAnalytics::AddAttribute(OutAttr, TEXT("PlayerName"), this->PlayerName);
    UBHVRAnalytics::AddAttribute(OutAttr, TEXT("OldName"), this->OldName);
    UBHVRAnalytics::AddAttribute(OutAttr, TEXT("SavedNetworkAddress"), this->SavedNetworkAddress);

    // Handle SessionName logic (checking for "None" or "_EMPTY_")
    if (this->SessionName == "_EMPTY_")
    {
        UBHVRAnalytics::AddAttribute(OutAttr, TEXT("SessionName"), TEXT("None"));
    }
    else
    {
        UBHVRAnalytics::AddAttribute(OutAttr, TEXT("SessionName"), this->SessionName.ToString());
    }

    // Default the character name to "<Observer>" in case they aren't playing
    FName characterName(TEXT("<Observer>"));
    
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        int32 selectedIndex = -1;
        
        if (this->GameRole == EPlayerRole::VE_Camper) 
        {
            selectedIndex = this->SelectedCamperIndex;
        }
        else if (this->GameRole == EPlayerRole::VE_Slasher) 
        {
            selectedIndex = this->SelectedSlasherIndex;
        }

        // If a valid index was found for an active role, fetch the character name from the tunables
        if (selectedIndex != -1 && dbdGameInstance->DesignTunables != nullptr)
        {
            characterName = dbdGameInstance->DesignTunables->GetName(selectedIndex);
        }
    }

    // Log the resolved character name
    if (characterName == "_EMPTY_")
    {
        UBHVRAnalytics::AddAttribute(OutAttr, TEXT("CharacterName_sz"), TEXT("None"));
    }
    else
    {
        UBHVRAnalytics::AddAttribute(OutAttr, TEXT("CharacterName_sz"), characterName.ToString());
    }

    // Finally, serialize the role-specific state data struct
    if (this->GameRole == EPlayerRole::VE_Camper)
    {
        UBHVRAnalytics::AddAttributeUStruct(OutAttr, TEXT("CamperData"), FCamperStateData::StaticStruct(), &this->CamperData);
    }
    else if (this->GameRole == EPlayerRole::VE_Slasher)
    {
        UBHVRAnalytics::AddAttributeUStruct(OutAttr, TEXT("SlasherData"), FSlasherStateData::StaticStruct(), &this->SlasherData);
    }
}

// starts at line 1520
float ADBDPlayerState::GetPerkScoreModifier(FScoreValue* ScoreVal) const
{
    float scoreModifier = 1.0f;

    AController* controller = Cast<AController>(this->GetOwner());
    if (controller != nullptr && !controller->IsActorBeingDestroyed())
    {
        // Verify the Pawn is a DBDPlayer
        ADBDPlayer* dbdPlayer = Cast<ADBDPlayer>(controller->GetPawn());
        if (IsValid(dbdPlayer) == true && dbdPlayer->IsActorBeingDestroyed() == false)
        {
            // Ensure the player is valid (virtual call check)
            if (dbdPlayer->IsValidImpl())
            {
                // Get the PerkManager from the player
                UPerkManager* perkManager = dbdPlayer->GetPerkManager();
                if (IsValid(perkManager) == true)
                {
                    EGameplayModifierType modifierTypeToQuery = 0;

                    // Map the ScoreCategory to the corresponding GameplayModifierType
                    switch (ScoreVal->Category)
                    {
                        case DBD_CamperScoreCat_Objectives:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseObjectivePoints; // 37
                            break;
                        case DBD_CamperScoreCat_Survival:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseSurvivalPoints; // 38
                            break;
                        case DBD_CamperScoreCat_Altruism:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseAltruismPoints; // 39
                            break;
                        case DBD_CamperScoreCat_Boldness:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseBoldnessPoints; // 40
                            break;
                        case DBD_SlasherScoreCat_Brutality:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseBrutalityPoints; // 33
                            break;
                        case DBD_SlasherScoreCat_Deviousness:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseDeviousnessPoints; // 34
                            break;
                        case DBD_SlasherScoreCat_Hunter:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseHunterPoints; // 35
                            break;
                        case DBD_SlasherScoreCat_Sacrifice:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseSacrificePoints; // 36
                            break;
                        case DBD_CamperScoreCat_Untracked:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseUntrackedPoints; // 42
                            break;
                        case DBD_CamperScoreCat_Streak:
                            modifierTypeToQuery = EGameplayModifierType::VE_IncreaseStreakPoints; // 41
                            break;
                        default:
                            break;
                    }

                    // If a specific category modifier exists, add it to the base 1.0f multiplier
                    if (modifierTypeToQuery != 0)
                    {
                        // Filter argument '4' maps to VE_All (as seen in decompiled code)
                        scoreModifier += perkManager->GetPerkModifier(modifierTypeToQuery, EGameplayModifierSource::VE_All);
                    }

                    // Always add the global "All Points" modifier (0x2B / 43)
                    scoreModifier += perkManager->GetPerkModifier(EGameplayModifierType::VE_IncreaseAllPoints), EGameplayModifierSource::VE_All);
                }
            }
        }
    }

    return scoreModifier;
}

// starts at line 1569
float ADBDPlayerState::GetPerkBonusScoreModifier(FScoreValue* ScoreVal) const
{
    float scoreModifier = 0.0f;

    // Verify owner is a valid Controller
    AController* controller = Cast<AController>(this->GetOwner());
    if (controller == true && controller->IsActorBeingDestroyed() == false)
    {
        // Verify the Pawn is a DBDPlayer
        ADBDPlayer* dbdPlayer = Cast<ADBDPlayer>(controller->GetCharacter());
        if (IsValid(dbdPlayer) == true && dbdPlayer->IsActorBeingDestroyed() == false)
        {
            // Ensure the player is valid (virtual call check)
            if (dbdPlayer->IsValidImpl())
            {
                // Get the PerkManager from the player
                UPerkManager* perkManager = dbdPlayer->GetPerkManager();
                if (IsValid(perkManager) == true)
                {
                    // Retrieve a specific global bonus point multiplier
                    scoreModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyBonusAllBloodpoints, EGameplayModifierSource::VE_All);
                }
            }
        }
    }

    return scoreModifier;
}

// starts at line 1613
void ADBDPlayerState::CleanLoadout()
{
    // Clear out equipped perks locally
    this->PlayerData.EquipedPerkIds.Empty();
    this->PlayerData.EquipedPerkLevels.Empty();

    // Verify GameInstance type
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // The disassembly creates a UniqueNetId string to lookup cached data.
        // It clears two specific arrays inside the FCachedPlayerScoreData map.
        FUniqueNetIdRepl netIdRepl;
        netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());
        
        FString netIdString = netIdRepl.GetUniqueNetId()->GetHexEncodedString();
        
        // Clear the cached score data arrays for perks
        FCachedPlayerScoreData* scoreCache = dbdGameInstance->_persistentData->GetPlayerScoreCache(netIdString);
        if (scoreCache != nullptr)
        {
            scoreCache->EquipedPerks.Empty();
            scoreCache->EquipedPerkLevels.Empty();
        }

        // Fetch the player's current character ID
        int32 currentCharacterId = -1;
        if (this->GameRole == EPlayerRole::VE_Slasher)
        {
            currentCharacterId = this->SelectedSlasherIndex;
        }
        else if (this->GameRole == EPlayerRole::VE_Camper)
        {
            if (this->SelectedCamperIndex >= 0)
            {
                currentCharacterId = this->SelectedCamperIndex;
            }
        }

        // Look up the default item (power) for the current character
        FName defaultPowerId = dbdGameInstance->GetCharacterDefaultItem(currentCharacterId);

        // Fetch persistent data to clear loadout
        FPlayerPersistentData* playerData = dbdPersistentData->GetPlayerPersistentData(netIdRepl);
        if (playerData != nullptr)
        {
            // Reset the power to the default power
            playerData->CurrentLoadout.Power = defaultPowerId;

            // Clear Power Add-ons (replace anything not locked/empty with empty)
            for (int32 i = 0; i < playerData->CurrentLoadout.PowerAddOns.Num(); ++i)
            {
                FName& addOn = playerData->CurrentLoadout.PowerAddOns[i];
                if (addOn != NAME_None && addOn != LoadoutSlotDefaultValue::EMPTY && addOn != LoadoutSlotDefaultValue::LOCKED)
                {
                    addOn = NAME_None;
                }
            }

            // Clear Slasher Perks
            for (int32 i = 0; i < playerData->CurrentLoadout.SlasherPerks.Num(); ++i)
            {
                FName& perk = playerData->CurrentLoadout.SlasherPerks[i];
                if (perk != NAME_None && perk != LoadoutSlotDefaultValue::EMPTY && perk != LoadoutSlotDefaultValue::LOCKED)
                {
                    perk = NAME_None;
                }
            }
        }
    }
}

// starts at line 1656
void ADBDPlayerState::Multicast_SetSkillCheckInteractionData_Implementation(FSkillCheckInteractionData skillCheckInteraction)
{
    // Update the local SkillCheckInteractionData state
    this->SkillCheckInteractionData = skillCheckInteraction;

    // Broadcast the update event, passing the IsActive state of the skill check
    // The disassembly shows this passing the first byte of the struct (offset 0x800),
    // which aligns with the IsActive boolean property of FSkillCheckInteractionData.
    if (this->_skillCheckUpdatedEventDelegate.IsBound())
    {
        this->_skillCheckUpdatedEventDelegate.Broadcast(this->SkillCheckInteractionData.IsActive);
    }
}

// starts at line 1668
void ADBDPlayerState::Multicast_UpdateSkillCheckInteractionData_Implementation(float progress)
{
    this->SkillCheckInteractionData.Progress = progress;
    
    if (this->_skillCheckUpdatedEventDelegate.IsBound())
    {
        this->_skillCheckUpdatedEventDelegate.Broadcast(true);
    }
}

// starts at line 1675
void ADBDPlayerState::SetCustomizationMesh(const FName& itemId)
{
    // Standard check to verify the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Fetch the customization item data from tunables. 
        FCustomizationItemData* itemData = dbdGameInstance->DesignTunables->GetCustomizationItemData(itemId, true);
        if (itemData != nullptr)
        {
            // Check if the item's mesh asset is loaded or resolvable.
            // The decompiled code shows a complex series of checks resolving a TAssetPtr/FSoftObjectPath.
            // We simplify this to checking if the soft object path is valid.
            if (itemData->ItemMesh.IsValid() || itemData->ItemMesh.ToSoftObjectPath().IsValid())
            {
                bool bReplacedExisting = false;

                // Search through the currently equipped customization meshes
                for (int32 i = 0; i < this->PlayerData.CustomizationMesh.Num(); ++i)
                {
                    FName currentEquippedItemId = this->PlayerData.CustomizationMesh[i];
                    
                    // Look up the data for the currently equipped item
                    FCustomizationItemData* equippedItemData = dbdGameInstance->DesignTunables->GetCustomizationItemData(currentEquippedItemId, true);
                    if (equippedItemData != nullptr)
                    {
                        // If the new item shares the same category as an already equipped item, replace it.
                        if (equippedItemData->PartCategory == itemData->PartCategory)
                        {
                            this->PlayerData.CustomizationMesh[i] = itemId;
                            bReplacedExisting = true;
                            break;
                        }
                    }
                }

                // If no item in the same category was found, add the new item to the array
                if (!bReplacedExisting)
                {
                    this->PlayerData.CustomizationMesh.Add(itemId);
                }
            }
        }
    }
}

// starts at line 1700
void ADBDPlayerState::LogCurrentLoadout()
{
    // Standard check to verify the GameInstance is a UDBDGameInstance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (IsValid(dbdGameInstance) == true)
    {
        // Only log if the current gametype affects progression
        if (dbdGameInstance->GetCurrentGametypeAffectsProgression() == true)
        {
            // Verify verbosity level (3 = Log)
            UE_LOG(LogGameFlow, Log, TEXT("LogCurrentLoadout: Loadout for %s"), *this->PlayerName);

            // Prepare UniqueNetId replica to lookup persistent data
            FUniqueNetIdRepl netIdRepl;
            netIdRepl.SetUniqueNetId(this->UniqueId.GetUniqueNetId());

            FPlayerPersistentData* playerData = dbdGameInstance->_persistentData->GetPlayerPersistentData(netIdRepl);
            if (playerData != nullptr)
            {
                // Log loadout based on role
                if (playerData->GameRole == EPlayerRole::VE_Camper) // Survivor
                {
                    // Avoid dereferencing if array out of bounds, but the assembly implies fixed size allocations 
                    // for loadouts (Perks are at [0], [8], [16], [24] representing 4 slots).
                    UE_LOG(LogGameFlow, Log, TEXT("LogCurrentLoadout: Item: %s Addon1: %s Addon2: %s Perk1: %s %i Perk2: %s %i Perk3: %s %i Perk4: %s %i Offering: %s"),
                            *playerData->CurrentLoadout.Item.ToString(),
                            *playerData->CurrentLoadout.ItemAddOns[0].ToString(),
                            *playerData->CurrentLoadout.ItemAddOns[1].ToString(),
                            *playerData->CurrentLoadout.CamperPerks[0].ToString(), playerData->CurrentLoadout.CamperPerkLevels[0],
                            *playerData->CurrentLoadout.CamperPerks[1].ToString(), playerData->CurrentLoadout.CamperPerkLevels[1],
                            *playerData->CurrentLoadout.CamperPerks[2].ToString(), playerData->CurrentLoadout.CamperPerkLevels[2],
                            *playerData->CurrentLoadout.CamperPerks[3].ToString(), playerData->CurrentLoadout.CamperPerkLevels[3],
                            *playerData->CurrentLoadout.CamperFavor.ToString());
                }
                else if (playerData->GameRole == EPlayerRole::VE_Slasher) // Killer
                {
                    UE_LOG(LogGameFlow, Log, TEXT("LogCurrentLoadout: Power: %s Addon1: %s Addon2: %s Perk1: %s %i Perk2: %s %i Perk3: %s %i Perk4: %s %i Offering: %s"),
                            *playerData->CurrentLoadout.Power.ToString(),
                            *playerData->CurrentLoadout.PowerAddOns[0].ToString(),
                            *playerData->CurrentLoadout.PowerAddOns[1].ToString(),
                            *playerData->CurrentLoadout.SlasherPerks[0].ToString(), playerData->CurrentLoadout.SlasherPerkLevels[0],
                            *playerData->CurrentLoadout.SlasherPerks[1].ToString(), playerData->CurrentLoadout.SlasherPerkLevels[1],
                            *playerData->CurrentLoadout.SlasherPerks[2].ToString(), playerData->CurrentLoadout.SlasherPerkLevels[2],
                            *playerData->CurrentLoadout.SlasherPerks[3].ToString(), playerData->CurrentLoadout.SlasherPerkLevels[3],
                            *playerData->CurrentLoadout.SlasherFavor.ToString());
                }
            }
            else
            {
                UE_LOG(LogGameFlow, Log, TEXT("LogCurrentLoadout: Player persistent data is null."));
            }
        }
    }
}

// starts at line 1751
bool ADBDPlayerState::HasHappened(EDBDScoreTypes gameplayEventType, float maxSecondsAgo) 
{
    return this->_recentGameplayEvents->HasHappened(gameplayEventType, maxSecondsAgo);
}
