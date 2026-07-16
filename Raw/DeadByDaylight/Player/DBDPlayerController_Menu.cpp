// starts at line 14
ADBDPlayerController_Menu::ADBDPlayerController_Menu(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) // Call to super constructor
{
    // Disable automatic management of the active camera target
    this->bAutoManageActiveCameraTarget = false;

    // The disassembly performs a ClassTreeIndex comparison, which is the inline expansion 
    // of Unreal Engine's type casting or IsA() check.
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (IsValid(DBDGameInstance) == true)
    {
        // The IDA pseudo-code has an obvious hallucination for offset 0x380
        TSharedPtr<FDBDAnalogCursor> AnalogCursor = DBDGameInstance->AnalogCursor;

        if (AnalogCursor.IsValid())
        {
            // Offset 0x48 represents the IsUsingGamePad boolean property
            if (AnalogCursor->IsUsingGamePad == true)
            {
                // Center the cursor on the screen
                AnalogCursor->CenterCursor();
            }
        }
    }
}

// starts at line 25
void ADBDPlayerController_Menu::PostInitializeComponents()
{
    // Call the parent class implementation
    Super::PostInitializeComponents();

    this->bShowMouseCursor = true;
}

// starts at line 32
void ADBDPlayerController_Menu::LocalSendCurrentLoadout()
{
    // Check if the current player controller is local
    if (this->IsLocalController() == true)
    {
        // Retrieve the player state and cast it to the menu-specific player state
        ADBDPlayerState_Menu* PlayerStateMenu = Cast<ADBDPlayerState_Menu>(this->PlayerState);
        
        if (PlayerStateMenu != nullptr)
        {
            // Retrieve the game instance and cast it to the DBD-specific game instance
            UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
            
            if (GameInstance != nullptr)
            {
                // Copy the unique network ID from the player state
                FUniqueNetIdRepl UniqueNetId = PlayerStateMenu->UniqueId;
                
                // Retrieve the player's persistent data using the game instance's persistent data manager
                FPlayerPersistentData* PlayerPersistentData = UDBDPersistentData::GetPlayerPersistentData(GameInstance->_persistentData, UniqueNetId);
                
                // Retrieve the local character's saved profile data
                FCharacterSavedProfileData* LocalCharacterSavedProfileData = GameInstance->GetLocalCharacterSavedProfileData();
                
                if (PlayerPersistentData != nullptr)
                {
                    // Validate the persistent data (ignoreKillerPower parameter = true)
                    PlayerPersistentData->ValidateData(true);
                }
                
                // Ensure both data structures are valid before proceeding
                if (PlayerPersistentData != nullptr && LocalCharacterSavedProfileData != nullptr)
                {
                    // Extract the loadout data from the saved profile (identified at offset 0x58)
                    FPlayerLoadoutData CharacterLoadoutData = LocalCharacterSavedProfileData->CharacterLoadoutData;
                    
                    // Send the loadout data to the server via RPC
                    this->Server_SetPlayerLoadout(CharacterLoadoutData);
                    
                    // Update the local persistent data with the current loadout (offset 0x88)
                    PlayerPersistentData->CurrentLoadout = CharacterLoadoutData;
                    
                    // Update the local persistent data with the starting loadout (offset 0x08)
                    PlayerPersistentData->StartingLoadout = CharacterLoadoutData;
                }
            }
        }
    }
}

// starts at line 58
void ADBDPlayerController_Menu::Local_SendCharacterData()
{
    // Retrieve the game instance and cast it to the custom DBD game instance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    
    // Retrieve the player state and cast it to the custom DBD player state
    ADBDPlayerState* PlayerState = Cast<ADBDPlayerState>(this->PlayerState);
    
    // Ensure both the game instance and player state are valid.
    // The disassembly inline checks for RF_Unreachable (Flags & 0x20000000) and bActorIsBeingDestroyed.
    // This is the expanded form of Unreal Engine's standard IsValid() check.
    if (IsValid(GameInstance) == true && IsValid(PlayerState) == true && PlayerState->IsActorBeingDestroyed() == false)
    {
        // The disassembly manually copies the FUniqueNetIdRepl and increments the SharedReferenceCount.
        FUniqueNetIdRepl UniqueNetId = PlayerState->UniqueId;
        
        // Retrieve the persistent data for the player using the unique network ID
        FPlayerPersistentData* PlayerPersistentData = UDBDPersistentData::GetPlayerPersistentData(GameInstance->_persistentData, UniqueNetId);
        
        if (PlayerPersistentData != nullptr)
        {
            // Retrieve the current character profile data
            FCharacterSavedProfileData* CurrentCharacterData = PlayerPersistentData->GetCurrentCharacterData();
            
            if (CurrentCharacterData != nullptr)
            {
                // Check if the local player is currently playing as a Slasher (Killer)
                // Offset 0x750 corresponds to the GameRole property
                bool bIsSlasher = (PlayerState->GameRole == EPlayerRole::VE_Slasher);
                
                // Sync the PlayerState loadout arrays with the profile data based on the current role.
                // The disassembly uses the TArray copy assignment operator.
                if (bIsSlasher == true)
                {
                    PlayerState->PlayerData.EquipedPerkIds = CurrentCharacterData->CharacterLoadoutData.SlasherPerks;
                    PlayerState->PlayerData.EquipedPerkLevels = CurrentCharacterData->CharacterLoadoutData.SlasherPerkLevels;
                }
                else
                {
                    PlayerState->PlayerData.EquipedPerkIds = CurrentCharacterData->CharacterLoadoutData.CamperPerks;
                    PlayerState->PlayerData.EquipedPerkLevels = CurrentCharacterData->CharacterLoadoutData.CamperPerkLevels;
                }
                
                // Initialize the synchronization structure that will be sent to the server
                FPlayerDataSync PlayerDataSync;
                
                // Copy the current customization array to the sync structure
                PlayerDataSync.CustomizationMesh = CurrentCharacterData->CurrentCustomization;
                
                // Populate the internal PlayerData struct with values from the PlayerState
                PlayerDataSync.PlayerData.CharacterLevel = PlayerState->PlayerData.CharacterLevel;
                PlayerDataSync.PlayerData.PrestigeLevel = PlayerState->PlayerData.PrestigeLevel;
                PlayerDataSync.PlayerData.EquipedFavorId = PlayerState->PlayerData.EquipedFavorId;
                PlayerDataSync.PlayerData.EquipedPerkIds = PlayerState->PlayerData.EquipedPerkIds;
                PlayerDataSync.PlayerData.EquipedPerkLevels = PlayerState->PlayerData.EquipedPerkLevels;
                PlayerDataSync.PlayerData.CustomizationMesh = PlayerState->PlayerData.CustomizationMesh;
                PlayerDataSync.PlayerData.IsLeavingMatch = PlayerState->PlayerData.IsLeavingMatch;
                PlayerDataSync.PlayerData._playerGameState = PlayerState->PlayerData._playerGameState;
                
                // Set the currently equipped item
                PlayerDataSync.EquipedItemId = PlayerState->CamperData.EquipedItemId;
                
                // Set the equipped addons based on the player's role
                if (bIsSlasher == true)
                {
                    PlayerDataSync.EquipedItemAddonIds = CurrentCharacterData->CharacterLoadoutData.PowerAddOns;
                }
                else
                {
                    PlayerDataSync.EquipedItemAddonIds = CurrentCharacterData->CharacterLoadoutData.ItemAddOns;
                }
                
                // Determine the correct character index to send to the server based on the GameRole
                int32 CharacterIndex = -1;
                
                if (PlayerState->GameRole == EPlayerRole::VE_Slasher)
                {
                    CharacterIndex = PlayerState->SelectedSlasherIndex;
                }
                else if (PlayerState->GameRole == EPlayerRole::VE_Camper)
                {
                    CharacterIndex = PlayerState->SelectedCamperIndex;
                }
                
                // Send the finalized character data to the server
                this->Server_SetCharacterData(CharacterIndex, PlayerDataSync);
            }
        }
    }
}

// starts at line 84
void ADBDPlayerController_Menu::Server_SetPlayerLoadout_Implementation(FPlayerLoadoutData desiredLoadout)
{
    // Retrieve the player state and cast it to the menu-specific player state
    ADBDPlayerState_Menu* PlayerStateMenu = Cast<ADBDPlayerState_Menu>(this->PlayerState);
    
    // Check if the cast was successful and the player state is valid
    if (PlayerStateMenu != nullptr)
    {
        // Retrieve the game instance and cast it to the custom DBD game instance
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        
        // Check if the cast was successful and the game instance is valid
        if (GameInstance != nullptr)
        {
            // Copy the unique network ID from the player state.
            // This replicates the manual struct copying and reference counting in the disassembly.
            FUniqueNetIdRepl UniqueNetId = PlayerStateMenu->UniqueId;
            
            // Retrieve the persistent data for this player using their unique network ID.
            FPlayerPersistentData* PlayerPersistentData = UDBDPersistentData::GetPlayerPersistentData(GameInstance->_persistentData, UniqueNetId);
            
            // If the persistent data was successfully found
            if (PlayerPersistentData != nullptr)
            {
                // Update the current loadout with the one provided by the client (offset 0x88)
                PlayerPersistentData->CurrentLoadout = desiredLoadout;
                
                // Also update the starting loadout with the new loadout (offset 0x08)
                PlayerPersistentData->StartingLoadout = desiredLoadout;
            }
        }
    }
}

// starts at line 104
bool ADBDPlayerController_Menu::Server_SetPlayerLoadout_Validate(FPlayerLoadoutData desiredLoadout)
{
    return true;
}

// starts at line 109
void ADBDPlayerController_Menu::TogglePlayerReadyState()
{
    // Cast the player state to the lobby-specific subclass
    ADBDPlayerState_Lobby* LobbyPlayerState = Cast<ADBDPlayerState_Lobby>(this->PlayerState);

    // Verify that the actor is valid (not marked as unreachable or pending kill/destroy)
    if (IsValid(LobbyPlayerState) == true && LobbyPlayerState->IsActorBeingDestroyed() == false)
    {
        // Invert the current "Ready" state (boolean property at offset 0x748).
        // The disassembly checks if the byte is zero; if so, it sets the argument for SetPlayerReady to true.

        // Call the function to update the ready state
        this->SetPlayerReady(LobbyPlayerState->IsPlayerReady == false);
    }
}

// starts at line 112
bool ADBDPlayerController_Menu::Server_SetCustomizationMesh_Validate(FName)
{
    return true;
}

// starts at line 118
void ADBDPlayerController_Menu::SetPlayerReady(bool isReady)
{
    // Attempt to cast the player state to the lobby-specific subclass
    ADBDPlayerState_Lobby* LobbyPlayerState = Cast<ADBDPlayerState_Lobby>(this->PlayerState);

    // The disassembly performs multiple checks to verify object validity
    if (IsValid(LobbyPlayerState) == true && LobbyPlayerState->IsActorBeingDestroyed() == false)
    {
        // Update the ready state locally on the player state.
        // The disassembly indicates this boolean property is located at offset 0x748.
        LobbyPlayerState->IsPlayerReady = isReady;

        // Call a virtual function on the LobbyPlayerState to notify it of the state change.
        // The disassembly calls the function at VTable offset 0x758.
        LobbyPlayerState->OnRep_DisplayData();

        // Re-read the value to ensure it was set correctly, then send an RPC to the server.
        this->Server_SetPlayerReady(LobbyPlayerState->IsPlayerReady);
    }
}

// starts at line 129
void ADBDPlayerController_Menu::Server_SetPlayerReady_Implementation(bool isReady)
{
    // Retrieve the game mode from the world.
    // The disassembly calls a virtual function at offset 0x108 (GetWorld), then gets AuthorityGameMode (offset 0xF0).
    AGameMode* GameMode = this->GetWorld()->GetAuthGameMode();

    // Cast the game mode to the lobby-specific subclass
    ADBDGame_Lobby* LobbyGameMode = Cast<ADBDGame_Lobby>(GameMode);

    // The disassembly verifies the game mode is valid (not unreachable or pending kill).
    if (IsValid(LobbyGameMode) == true && LobbyGameMode->IsActorBeingDestroyed() == false)
    {
        // Retrieve the player state
        APlayerState* PlayerState = this->PlayerState;
        
        if (PlayerState != nullptr)
        {
            // Cast the player state to the menu-specific subclass
            ADBDPlayerState_Menu* MenuPlayerState = Cast<ADBDPlayerState_Menu>(PlayerState);
            
            if (MenuPlayerState != nullptr)
            {
                // Update the "Ready" state on the server.
                // The property is located at offset 0x748.
                MenuPlayerState->IsPlayerReady = isReady;
                
                // Notify the player state that the ready state has changed.
                // This calls the virtual function at offset 0x758.
                MenuPlayerState->OnRep_DisplayData();
            }
        }
        
        // Notify the lobby game mode that a player has readied up.
        // The disassembly passes 0 (nullptr) for the AController* parameter.
        LobbyGameMode->OnPlayerReadyUp(nullptr);
    }
}

// starts at line 149
void ADBDPlayerController_Menu::Server_SetReadyToTravel_Implementation()
{
    // Check if there is a valid authority game mode running in the world
    AGameMode* AuthorityGameMode = this->GetWorld()->GetAuthGameMode();

    if (AuthorityGameMode != nullptr)
    {
        // Cast the player state to the menu-specific subclass
        ADBDPlayerState_Menu* MenuPlayerState = Cast<ADBDPlayerState_Menu>(this->PlayerState);

        if (MenuPlayerState != nullptr)
        {
            // Update the travel readiness state to true.
            // The disassembly assigns '1' to a byte at offset 0x820.
            MenuPlayerState->IsReadyToTravel = true;
        }

        // Cast the authority game mode to the lobby-specific subclass
        ADBDGame_Lobby* LobbyGameMode = Cast<ADBDGame_Lobby>(AuthorityGameMode);

        if (LobbyGameMode != nullptr)
        {
            // Trigger the travel process in the game mode
            LobbyGameMode->StartTravel();
        }
    }
}

// starts at line 172
void ADBDPlayerController_Menu::Server_SetCharacterData_Implementation(int characterIndex, const FPlayerDataSync& playerDataSync)
{
    // Cast the player state to the menu-specific subclass
    ADBDPlayerState_Menu* MenuPlayerState = Cast<ADBDPlayerState_Menu>(this->PlayerState);

    // The disassembly performs multiple checks to verify object validity
    if (IsValid(MenuPlayerState) == true && MenuPlayerState->IsActorBeingDestroyed() == false)
    {
        // Determine the role based on the character index.
        // If characterIndex is -1 (INDEX_NONE), role is VE_Observer.
        // If characterIndex < 0x10000000, role is Camper (1). Otherwise, Slasher (2).
        EPlayerRole PlayerRole = EPlayerRole::VE_Observer;
        if (characterIndex != INDEX_NONE)
        {
            if (characterIndex < 0x10000000)
            {
                PlayerRole = EPlayerRole::VE_Camper;
            }
            else
            {
                PlayerRole = EPlayerRole::VE_Slasher;
            }
        }

        // Call server functions on the PlayerState to set the basic character info
        MenuPlayerState->Server_SetSelectedCharacterId(PlayerRole, characterIndex, true);
        MenuPlayerState->Server_SetSelectedCharacterCustomization(PlayerRole, characterIndex, playerDataSync.CustomizationMesh, true);

        // Call server functions on the PlayerController to set levels and perks
        this->Server_SetCharacterLevel(playerDataSync.PlayerData.CharacterLevel, playerDataSync.PlayerData.PrestigeLevel, false);
        this->Server_SetEquipedPerks(playerDataSync.PlayerData.EquipedPerkIds, playerDataSync.PlayerData.EquipedPerkLevels, false);

        // Equip items and add-ons using the slot index enum.
        
        MenuPlayerState->Server_SetEquipment(8, playerDataSync.PlayerData.EquipedFavorId, false);
        MenuPlayerState->Server_SetEquipment(1, playerDataSync.EquipedItemId, true);

        // Check and set Add-ons based on the ArrayNum
        if (playerDataSync.EquipedItemAddonIds.Num() > 0)
        {
            MenuPlayerState->Server_SetEquipment(2, playerDataSync.EquipedItemAddonIds[0], false);
        }

        if (playerDataSync.EquipedItemAddonIds.Num() > 1)
        {
            MenuPlayerState->Server_SetEquipment(3, playerDataSync.EquipedItemAddonIds[1], false);
        }

        // Check for custom game conditions before updating the GameState
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (GameInstance != nullptr)
        {
            // Access the game mode state. In the disassembly, this checks a byte at offset 0x30 inside a manager 
            // stored at GameInstance offset 0x3B8.
            bool bIsCustomMatch = GameInstance->_persistentData->_gamePersistentData.SessionInfos.GameType == EGameType::PartyMode;
            
            if (bIsCustomMatch == true) 
            {
                ADBDGameState* GameState = this->GetWorld()->GetGameState<ADBDGameState>();
    
                if (IsValid(GameState) == true && GameState->IsActorBeingDestroyed() == false)
                {
                    // Extract the game preset data
                    FGamePresetData& GamePresetData = GameInstance->_persistentData->_gamePersistentData.GamePresetData;
                    
                    // Multicast the character update to all clients based on the custom game preset
                    GameState->Multicast_UpdateCharacterFromGamePreset(MenuPlayerState, GamePresetData);
                }
            }
        }

        // Call a virtual function on the MenuPlayerState, likely to update resource size or notify of a change.
        MenuPlayerState->OnRep_DisplayData();
    }
}

// starts at line 215
void ADBDPlayerController_Menu::Server_SetCharacterLevel_Implementation(int32 characterLevel, int32 prestigeLevel, bool callOnRep)
{
    // Cast the generic PlayerState to the menu-specific subclass
    ADBDPlayerState_Menu* MenuPlayerState = Cast<ADBDPlayerState_Menu>(this->PlayerState);

    // The disassembly performs inline validation checks for the object
    if (IsValid(MenuPlayerState) == true && MenuPlayerState->IsActorBeingDestroyed() == false)
    {
        // Update the character level and prestige level properties on the MenuPlayerState.
        // The disassembly writes to offsets 0x7B8 and 0x7BC respectively.
        MenuPlayerState->PlayerData.CharacterLevel = characterLevel;
        MenuPlayerState->PlayerData.PrestigeLevel = prestigeLevel;

        // If requested, notify the player state that the data has changed.
        if (callOnRep == true)
        {
            // The disassembly calls a virtual function at offset 0x758 (index 235).
            MenuPlayerState->OnRep_DisplayData();
        }
    }
}

// starts at line 235
void ADBDPlayerController_Menu::Server_SetCustomizationMesh_Implementation(FName itemId)
{
    // Cast the generic PlayerState to the menu-specific subclass
    ADBDPlayerState_Menu* MenuPlayerState = Cast<ADBDPlayerState_Menu>(this->PlayerState);

    // Validates the PlayerState by expanding IsValid()
    if (IsValid(MenuPlayerState) == true && MenuPlayerState->IsActorBeingDestroyed() == false)
    {
        // Update the customization mesh on the player state with the provided FName.
        MenuPlayerState->SetCustomizationMesh(itemId);

        // Notify the player state that a change occurred (calls VTable function at offset 0x758).
        MenuPlayerState->OnRep_DisplayData();

        // Retrieve the game instance and cast it to the custom DBD game instance
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        // Validate the game instance using the same expanded IsValid() checks
        if (IsValid(GameInstance) == true)
        {
            // Copy the unique network ID, replicating the manual reference counting seen in the assembly
            FUniqueNetIdRepl UniqueNetId = MenuPlayerState->UniqueId;

            // Retrieve the persistent data for this player
            FPlayerPersistentData* PlayerPersistentData = UDBDPersistentData::GetPlayerPersistentData(GameInstance->_persistentData, UniqueNetId);

            if (PlayerPersistentData != nullptr)
            {
                // Update the persistent data with the current CustomizationMesh from the player state.
                // The disassembly shows a TArray assignment from PlayerState offset 0x7E8 to PersistentData offset 0x190.
                PlayerPersistentData->SavedData.SharedData.SelectedCharacterCustomization = MenuPlayerState->PlayerData.CustomizationMesh;
            }
        }
    }
}

// starts at line 256
void ADBDPlayerController_Menu::Server_SetEquipedPerks_Implementation(const TArray<FName>& perkIds, const TArray<int32>& perkLevels, bool callOnRep)
{
    // Cast the player state to the menu-specific subclass
    ADBDPlayerState_Menu* MenuPlayerState = Cast<ADBDPlayerState_Menu>(this->PlayerState);

    // Inline validation checks
    if (IsValid(MenuPlayerState) == true && MenuPlayerState->IsActorBeingDestroyed() == false)
    {
        // Update the player state's equipped perks arrays.

        // Offset 0x7C8 corresponds to EquipedPerkIds
        MenuPlayerState->PlayerData.EquipedPerkIds.Empty();
        MenuPlayerState->PlayerData.EquipedPerkIds.Append(perkIds);

        // Offset 0x7D8 corresponds to EquipedPerkLevels
        MenuPlayerState->PlayerData.EquipedPerkLevels.Empty();
        MenuPlayerState->PlayerData.EquipedPerkLevels.Append(perkLevels);

        // Call the OnRep notification if requested
        if (callOnRep == true)
        {
            MenuPlayerState->OnRep_DisplayData();
        }

        // Retrieve the game instance and cast it to UDBDGameInstance
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        // Validate the game instance using IsValid() logic
        if (IsValid(GameInstance) == true)
        {
            // Copy the unique network ID
            FUniqueNetIdRepl UniqueNetId = MenuPlayerState->UniqueId;

            // Retrieve the persistent data for this player
            FPlayerPersistentData* PlayerPersistentData = UDBDPersistentData::GetPlayerPersistentData(GameInstance->_persistentData, UniqueNetId);

            if (PlayerPersistentData != nullptr)
            {
                // Clear the current perks in the persistent loadout
                PlayerPersistentData->CurrentLoadout.ClearPerks();

                // Determine the player's role (offset 0x750 in ADBDPlayerState_Menu)
                if (MenuPlayerState->GameRole == EPlayerRole::VE_Slasher)
                {
                    // The disassembly implements a manual loop to copy the elements 
                    // if the array bounds allow it.
                    if (PlayerPersistentData->CurrentLoadout.SlasherPerks.Num() >= perkIds.Num() &&
                        PlayerPersistentData->CurrentLoadout.SlasherPerkLevels.Num() >= perkLevels.Num() &&
                        perkIds.Num() > 0)
                    {
                        for (int32 i = 0; i < perkIds.Num(); ++i)
                        {
                            PlayerPersistentData->CurrentLoadout.SlasherPerks[i] = perkIds[i];
                            PlayerPersistentData->CurrentLoadout.SlasherPerkLevels[i] = perkLevels[i];
                        }
                    }
                }
                else
                {
                    if (PlayerPersistentData->CurrentLoadout.CamperPerks.Num() >= perkIds.Num() &&
                        PlayerPersistentData->CurrentLoadout.CamperPerkLevels.Num() >= perkLevels.Num() &&
                        perkIds.Num() > 0)
                    {
                        for (int32 i = 0; i < perkIds.Num(); ++i)
                        {
                            PlayerPersistentData->CurrentLoadout.CamperPerks[i] = perkIds[i];
                            PlayerPersistentData->CurrentLoadout.CamperPerkLevels[i] = perkLevels[i];
                        }
                    }
                }
            }
        }
    }
}

// starts at line 306
bool ADBDPlayerController_Menu::Server_SetEquipedPerks_Validate(const TArray<FName>& perkIds, const TArray<int32>& perkLevels, bool callOnRep)
{
    // The server validates that the number of perk IDs matches the number of perk levels provided.
    // It also ensures that the total number of perks does not exceed the maximum allowed (4).
    if (perkLevels.Num() == perkIds.Num())
    {
        if (perkIds.Num() <= 4)
        {
            return true;
        }
    }
    
    return false;
}
