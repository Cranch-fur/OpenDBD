// starts at line 19
void ADBDPlayerState_Lobby::RegisterPlayerWithSession(bool bWasFromInvite)
{
    // Ensure we are operating as a client or server over a network connection
    if (this->InternalGetNetMode() != ENetMode::NM_Standalone)
    {
        // Obtain the internal unique net ID object
        if (this->UniqueId.UniqueNetId.IsValid() == true)
        {
            // Fetch the Class Default Object (CDO) for the Lobby Player State class
            // This is used to access default configurations or properties
            UClass* LobbyPlayerStateClass = ADBDPlayerState_Lobby::StaticClass();
            ADBDPlayerState_Lobby* ClassDefaultObject = Cast<ADBDPlayerState_Lobby>(LobbyPlayerStateClass->GetDefaultObject());

            if (ClassDefaultObject != nullptr)
            {
                // Create a map to hold platform IDs for matchmaking presence
                TMap<IOnlinePresencePlugin::EMatchmaking, TSharedRef<const FUniqueNetId>> PlatformIds;

                // Emplace the player's core unique net ID into the map with a specific matchmaking key (32)
                PlatformIds.Emplace(IOnlinePresencePlugin::EMatchmaking::Platform, this->UniqueId.UniqueNetId.ToSharedRef());

                // Format a combined string using the Mirrors ID and the raw Unique Net ID string representation
                FString MirrorsCombinedId = FString::Printf(TEXT("%s|%s"), *this->MirrorsId, *this->UniqueId.UniqueNetId->ToString());

                // Create a new FUniqueNetIdString instance from the combined string
                TSharedRef<const FUniqueNetId> MirrorsNetId = MakeShared<FUniqueNetIdString>(MirrorsCombinedId);

                // Update the internal UniqueId's TSharedFromThis reference to point to the newly created Mirrors net ID
                this->UniqueId.UniqueNetId = MirrorsNetId;

                // Emplace the newly created Mirrors net ID into the map with another matchmaking key (34)
                PlatformIds.Emplace(IOnlinePresencePlugin::EMatchmaking::Mirrors, MirrorsNetId);

                // Load the OnlinePresence module dynamically
                // We use IOnlinePresencePlugin instead of FVoiceModule based on the string name
                IOnlinePresencePlugin* OnlinePresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>("OnlinePresence");

                // Call a specific function on the OnlinePresence module using the CDO's configuration (Offset 0x3F0)
                OnlinePresenceModule->RegisterPlayerOnServices(ClassDefaultObject->SessionName, PlatformIds, bWasFromInvite);
            }
        }
    }
}

// starts at line 42
void ADBDPlayerState_Lobby::SetSelectedCharacterIDImpl(EPlayerRole forRole, int32 id, bool updateDisplayData)
{
    // Call the parent class implementation first
    Super::SetSelectedCharacterIDImpl(forRole, id, updateDisplayData);

    // Retrieve the game instance and cast to UDBDGameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid
    if (IsValid(GameInstance) == true)
    {
        // Retrieve the local player state
        ADBDPlayerState* LocalPlayerState = GameInstance->GetLocalPlayerState();

        // Verify that the local player state is valid, not being destroyed,
        // and is not the current player state
        if (IsValid(LocalPlayerState) == true && LocalPlayerState->IsActorBeingDestroyed() == false && LocalPlayerState != this)
        {
            // Validate the selected character for the lobby state
            this->ValidateSelectedCharacter();
        }
    }
}

// starts at line 57
void ADBDPlayerState_Lobby::ValidateSelectedCharacter()
{
    // Retrieve the game instance and cast it to UDBDGameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid
    if (IsValid(GameInstance) == true)
    {
        int32_t TargetCharacterIndex = -1;

        // Determine the target character index based on the player's role
        // 1 corresponds to the Slasher (Killer) role in EPlayerRole
        if (this->GameRole == EPlayerRole::VE_Slasher)
        {
            TargetCharacterIndex = this->SelectedSlasherIndex;
        }
        else if (this->GameRole == EPlayerRole::VE_Camper)
        {
            TargetCharacterIndex = this->SelectedCamperIndex;
        }

        // Retrieve the character description based on the target index
        FCharacterDescription* CharacterDescription = GameInstance->DesignTunables->GetCharacterDescription(TargetCharacterIndex);

        if (CharacterDescription != nullptr)
        {
            FRequiredDlcId RequiredDlc;

            // Fetch the required DLC ID for the selected character
            CharacterDescription->GetRequiredDLCId(RequiredDlc);

            // Access the DataStore subsystem from the OnlinePresence plugin
            // The method returns a shared reference which we capture
            TSharedRef<IDataStorePresenceSubsystem> DataStoreSubsystem = IOnlinePresencePlugin::DataStore();

            // Check if the user has the required DLC using their UniqueId
            EHasDlc bHasDlc = DataStoreSubsystem->UserHasDlc(this->UniqueId.UniqueNetId, RequiredDlc);

            // The disassembled check is: if (RequiredDlc != 0 && bHasDlc == 1)
            if (RequiredDlc.data != 0 && bHasDlc == EHasDlc::UserDoesNotHaveLicense)
            {
                // Log a warning if the verbosity level is Warning (2) or higher
                UE_LOG(LogGameFlow, Warning, TEXT("Player \"%s\" not authorized for character id: %i"), *this->PlayerName, TargetCharacterIndex);

                int32_t DefaultFallbackIndex = 0x10000000;

                // 2 corresponds to the Camper (Survivor) role in EPlayerRole
                if (this->GameRole == EPlayerRole::VE_Camper)
                {
                    DefaultFallbackIndex = 0;
                }

                // Force the server to set the selected character ID to the fallback
                this->Server_SetSelectedCharacterId(this->GameRole, DefaultFallbackIndex, true);
            }
        }
    }
}

// starts at line 75
void ADBDPlayerState_Lobby::InitSpawnedPawn()
{
    // Call the parent class implementation first
    Super::InitSpawnedPawn();

    // Check if the current pawn weak pointer is valid
    if (this->_pawn.IsValid() == true)
    {
        // Update a specific boolean flag on the spawned lobby pawn.
        // Offset 0x811 corresponds to a property like _isLobbyPawn or a similar boolean state flag.
        this->_pawn->IsOnline = true;
    }
}

// starts at line 84
void ADBDPlayerState_Lobby::ReplacePawn()
{
    // Retrieve the game instance and try to cast it to UDBDGameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Fetch the local player state, if the game instance is valid
    ADBDPlayerState* LocalPlayerState = nullptr;
    if (IsValid(GameInstance) == true)
    {
        LocalPlayerState = GameInstance->GetLocalPlayerState();
    }

    // Retrieve the world and cast the current level script actor to ALobbyLevel
    ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(this->GetWorld()->GetLevelScriptActor(nullptr));

    // The logic checks if we can simply replace the pawn using the base class method.
    // We fall back to the base class if:
    // 1. The pawn weak pointer is already valid
    // 2. The LobbyLevel is invalid or is being destroyed
    // 3. The LocalPlayerState is invalid or is being destroyed
    // 4. The LocalPlayerState does NOT match 'this'
    // 5. The local player start is already available for this role
    if (this->_pawn.IsValid() == true ||
        IsValid(LobbyLevel) == false || LobbyLevel->IsActorBeingDestroyed() == true ||
        IsValid(LocalPlayerState) == false || LocalPlayerState->IsActorBeingDestroyed() == true ||
        LocalPlayerState != this ||
        LobbyLevel->IsLocalPlayerStartAvailable(this->GameRole) == true)
    {
        // Call the parent class implementation
        Super::ReplacePawn();
    }
    else
    {
        // If we reach here, it implies we are the local player, the player start isn't available,
        // and we need to forcibly clear the pawn currently occupying our designated local player start.

        // Get the pawn currently occupying the local player start
        ADBDMenuPlayer* PawnOnLocalStart = this->GetPawnOnLocalPlayerStart();

        // Check if there is a valid pawn that is not already queued for destruction
        if (IsValid(PawnOnLocalStart) == true && PawnOnLocalStart->IsActorBeingDestroyed() == false)
        {
            // If there's an existing active pawn destruction handle, remove it
            if (this->_pawnDestroyHandle.IsValid() == true)
            {
                // Remove the previously bound delegate from the pawn's OnDestroyed event
                // Offset 0x7A0 corresponds to OnDestroyed (or OnDestroyedNonDynamic)
                PawnOnLocalStart->OnDestroyedNonDynamic.Remove(this->_pawnDestroyHandle);
            }

            // Bind the CheckSpawnOnLocalPlayerStartFreed function to the pawn's OnDestroyed event
            this->_pawnDestroyHandle = PawnOnLocalStart->OnDestroyedNonDynamic.AddUObject(this, &ADBDPlayerState_Lobby::CheckSpawnOnLocalPlayerStartFreed);

            // Initiate the destruction sequence for the blocking pawn
            PawnOnLocalStart->BeginDestroySequence();
        }
        else
        {
            // If no valid blocking pawn was found, assume the spot is freed and proceed to check spawn logic
            this->CheckSpawnOnLocalPlayerStartFreed();
        }
    }
}

// starts at line 110
void ADBDPlayerState_Lobby::CheckSpawnOnLocalPlayerStartFreed()
{
    // Retrieve the level script actor and attempt to cast to ALobbyLevel
    ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(this->GetWorld()->GetLevelScriptActor(nullptr));

    // Proceed if LobbyLevel is valid and not pending destruction
    if (IsValid(LobbyLevel) == true && LobbyLevel->IsActorBeingDestroyed() == false)
    {
        // Access the game state from the world
        // Offset 0x58 (88) from UWorld typically points to the AGameStateBase/AGameState instance
        AGameState* GameState = this->GetWorld()->GetGameState();

        if (GameState != nullptr)
        {
            // Iterate over all player states currently tracked by the game state
            for (APlayerState* PlayerState : GameState->PlayerArray)
            {
                // Try to cast each player state to ADBDPlayerState_Lobby
                ADBDPlayerState_Lobby* LobbyPlayerState = Cast<ADBDPlayerState_Lobby>(PlayerState);
                
                // If it's a valid lobby player state and not pending destruction
                if (IsValid(LobbyPlayerState) == true && LobbyPlayerState->IsActorBeingDestroyed() == false)
                {
                    // Check if this player state is currently occupying the local player start for our role
                    APlayerStart* StatePlayerStart = LobbyPlayerState->_playerSpawn.Get();
                    APlayerStart* LocalPlayerStartForRole = LobbyLevel->GetLocalPlayerStart(this->GameRole);

                    if (StatePlayerStart != nullptr && StatePlayerStart == LocalPlayerStartForRole)
                    {
                        // If they are on our desired local start, forcibly free their spawn point
                        LobbyPlayerState->FreeSpawnPointOnPawnDestroyComplete();
                    }
                }
            }
        }
    }

    int32_t TargetCharacterIndex = -1;

    // Determine the target character index based on the player's role
    // 1 corresponds to the Slasher (Killer) role in EPlayerRole
    if (this->GameRole == EPlayerRole::VE_Slasher)
    {
        TargetCharacterIndex = this->SelectedSlasherIndex;
    }
    // 2 corresponds to the Camper (Survivor) role in EPlayerRole
    else if (this->GameRole == EPlayerRole::VE_Camper)
    {
        TargetCharacterIndex = this->SelectedCamperIndex;
    }

    // Spawn a new pawn if a valid character index is selected
    if (TargetCharacterIndex != -1)
    {
        this->SpawnNewPawn();
    }
}

// starts at line 132
ADBDMenuPlayer* ADBDPlayerState_Lobby::GetPawnOnLocalPlayerStart() const
{
    // Retrieve the level script actor and cast to ALobbyLevel
    ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(this->GetWorld()->GetLevelScriptActor(nullptr));

    // Verify LobbyLevel is valid and not being destroyed
    if (IsValid(LobbyLevel) == true && LobbyLevel->IsActorBeingDestroyed() == false)
    {
        // Get the designated local player start for the current player's role
        APlayerStart* LocalPlayerStart = LobbyLevel->GetLocalPlayerStart(this->GameRole);

        // Verify the LocalPlayerStart is valid and not being destroyed
        if (IsValid(LocalPlayerStart) == true && LocalPlayerStart->IsActorBeingDestroyed() == false)
        {
            // Retrieve the game state
            AGameStateBase* GameState = this->GetWorld()->GetGameState();

            if (GameState != nullptr)
            {
                // Iterate through all player states in the game state's PlayerArray
                for (APlayerState* PlayerState : GameState->PlayerArray)
                {
                    // Attempt to cast the player state to ADBDPlayerState_Lobby
                    ADBDPlayerState_Lobby* LobbyPlayerState = Cast<ADBDPlayerState_Lobby>(PlayerState);
                    
                    // Verify the lobby player state is valid and not being destroyed
                    if (IsValid(LobbyPlayerState) == true && LobbyPlayerState->IsActorBeingDestroyed() == false)
                    {
                        // Check if this lobby player state claims to be spawned at our designated LocalPlayerStart
                        if (LobbyPlayerState->_playerSpawn.Get() == LocalPlayerStart)
                        {
                            // If a match is found, return the pawn associated with that player state
                            return LobbyPlayerState->_pawn.Get();
                        }
                    }
                }
            }
        }
    }

    // Return nullptr if no pawn occupies the local player start or if validation fails
    return nullptr;
}
