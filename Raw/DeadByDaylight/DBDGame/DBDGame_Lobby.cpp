void ADBDGame_Lobby::AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC)
{
	UWorld* world = this->GetWorld();
	AGameState* game_state = world->GetGameState();

	if (IsValid(game_state))
	{
		game_state->RemovePlayerState(PlayerState);
	}

	PlayerState->Destroy(false, true);
}




void ADBDGame_Lobby::AuthenticateNewPlayer(ADBDPlayerControllerBase* NewPlayer)
{
	if (IsValid(NewPlayer))
	{
		bool local_controller = NewPlayer->IsLocalController();

		if (local_controller == false)
			this->SendServerAuthTicketToClient(NewPlayer);
	}
}




void ADBDGame_Lobby::BeginDestroy()
{
	TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = IOnlinePresencePlugin::Get().Matchmaking();
	MatchmakingSubsystem->ClearOnDestroySessionCompleteDelegate_Handle(this->_onSessionDestroyedHandle);
	this->_onSessionDestroyedHandle.Reset();
	Super::BeginDestroy();
}




void ADBDGame_Lobby::CheckPlayerConfiguration(bool playerConfigurationReady)
{
	// If transitioning from Ready to Unready, reset timer
	if (this->_playerConfigurationReady && playerConfigurationReady == false)
	{
		this->_timerValue = 60.9000015f;
	}
	// If transitioning from Unready to Ready, trigger event
	else if (this->_playerConfigurationReady == false && playerConfigurationReady)
	{
		this->OnPlayerReadyUp(nullptr);
	}

	this->_playerConfigurationReady = playerConfigurationReady;
}




void ADBDGame_Lobby::CheckReadyToTravel()
{
    // Get the world context from this actor/object
    UWorld* World = this->GetWorld();

    bool bAllPlayersReady = true;

    // The pseudo-code uses UWorld::GetPlayerControllerIterator (vtable[0x21] usually points to GetWorld or similar context)
    for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        // Equivalent to FWeakObjectPtr::Get and internal Index checks
        APlayerController* PC = Iterator->Get();

        // Check if PlayerController is valid and not pending kill (rax_3 and InternalIndex checks)
        if (IsValid(PC) == false)
        {
            bAllPlayersReady = false;
            break;
        }

        // Pseudo-code offset 0x3a0 is likely the PlayerState pointer in AController
        // We cast it to ADBDPlayerState as indicated by the GetPrivateStaticClass call
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PC->PlayerState);

        // Validating PlayerState and its "Pending Kill" status (GUObjectArray checks)
        if (IsValid(DBDPlayerState) == false)
        {
            bAllPlayersReady = false;
            break;
        }

        // Pseudo-code offset 0x820 in ADBDPlayerState. 
        // Based on the logic, this is likely a 'bool bIsReady' property.
        // If any player is NOT ready, we break the loop and don't travel.
        // Note: I am assuming 0x820 maps to a boolean check 'IsReady'
        bool bIsReady = DBDPlayerState->IsReadyToTravel;

        if (!bIsReady)
        {
            bAllPlayersReady = false;
            break;
        }
    }

    // If the loop finished without breaking (all players ready), trigger StartTravel
    if (bAllPlayersReady)
    {
        this->StartTravel();
    }
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::CloseLobby()
{
    // Get the Game Instance from the current actor
    UGameInstance* GameInstance = this->GetGameInstance();

    // Ensure the GameInstance is not null and is valid
    if (IsValid(GameInstance) == false)
    {
        return;
    }

    /*
     * The assembly code performs a manual check of the class hierarchy.
     * This is equivalent to checking if the GameInstance is a UDBDGameInstance.
     */
    UClass* RequiredClass = UDBDGameInstance::StaticClass();
    if (GameInstance->IsA(RequiredClass) == false)
    {
        return;
    }

    // Cast to the actual class to work with its memory layout
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    if (IsValid(DBDGameInstance) == false)
    {
        return;
    }

    /*
     * Navigating through pointers using offsets identified in the disassembler:
     * 0x3b0 -> Manager/Subsystem pointer
     * 0x90  -> Intermediate structure/class
     * 0x5f0 -> UDBDServerInstance pointer
     */

     // Accessing the manager at offset 0x3b0
    UGameFlowContextSystem* InternalManager = DBDGameInstance->_contextSystem;
    if (IsValid(InternalManager) == false)
    {
        return;
    }

    // Accessing the intermediate object at offset 0x90
    UOnlineSystemHandler* IntermediateLayer = InternalManager->m_OnlineSystemHandler;
    if (IsValid(IntermediateLayer) == false)
    {
        return;
    }

    // Accessing the Server Instance at offset 0x5f0
    UDBDServerInstance* ServerInstance = IntermediateLayer->_serverInstance;

    /*
     * The assembly checks if the object is valid (not pending kill)
     * before performing the tailcall.
     */
    if (IsValid(ServerInstance) == false)
    {
        return;
    }

    // Creating an empty FString as a placeholder for data_143584f78
    FString EmptySettings = FString();

    /*
     * Call the original function to update host settings.
     * In the assembly, this was a tailcall to UDBDServerInstance::UpdateHostSettings.
     */
    ServerInstance->UpdateHostSettings(EmptySettings);
}




void ADBDGame_Lobby::DBD_ForceStartMatch()
{
    float _timerValue = this->_timerValue;
    this->_forceStarted = true;
    
    if (_timerValue == -1.f || (_timerValue <= 5.9000001f) == false)
        this->_timerValue = 5.9000001f;
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::GameStarted()
{
    // Get the Game Instance from the current actor
    UGameInstance* GameInstance = this->GetGameInstance();

    // Ensure the GameInstance is not null and is valid
    if (IsValid(GameInstance) == false)
    {
        return;
    }

    /*
     * The assembly code performs a manual check of the class hierarchy.
     * This is equivalent to checking if the GameInstance is a UDBDGameInstance.
     */
    UClass* RequiredClass = UDBDGameInstance::StaticClass();
    if (GameInstance->IsA(RequiredClass) == false)
    {
        return;
    }

    // Cast to the actual class to work with its memory layout
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    if (IsValid(DBDGameInstance) == false)
    {
        return;
    }

    /*
     * Navigating through pointers using offsets identified in the disassembler:
     * 0x3b0 -> Manager/Subsystem pointer
     * 0x90  -> Intermediate structure/class
     * 0x5f0 -> UDBDServerInstance pointer
     */

     // Accessing the manager at offset 0x3b0
    UGameFlowContextSystem* InternalManager = DBDGameInstance->_contextSystem;
    if (IsValid(InternalManager) == false)
    {
        return;
    }

    // Accessing the intermediate object at offset 0x90
    UOnlineSystemHandler* IntermediateLayer = InternalManager->m_OnlineSystemHandler;
    if (IsValid(IntermediateLayer) == false)
    {
        return;
    }

    // Accessing the Server Instance at offset 0x5f0
    UDBDServerInstance* ServerInstance = IntermediateLayer->_serverInstance;

    /*
     * The assembly checks if the object is valid (not pending kill)
     * before performing the tailcall.
     */
    if (IsValid(ServerInstance) == false)
    {
        return;
    }

    // Creating an empty FString as a placeholder for data_143584f98
    FString EmptySettings = FString();

    /*
     * Call the original function to update host settings.
     * In the assembly, this was a tailcall to UDBDServerInstance::UpdateHostSettings.
     */
    ServerInstance->UpdateHostSettings(EmptySettings);
}



/* UNFINISHED FUNCTION */
FString ADBDGame_Lobby::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    // Retrieve the PlayerState and cast it to the custom DeadByDaylight PlayerState
    ADBDPlayerState* PlayerState = Cast<ADBDPlayerState>(NewPlayerController->PlayerState);

    // Retrieve the GameInstance and cast it to the custom DeadByDaylight GameInstance
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Validation checks for PlayerState and GameInstance
    if (PlayerState == nullptr)
    {
        return;
        // Handle invalid PlayerState (logic implied by disassembly flow, though usually handled by engine)
    }

    if (DBDGameInstance == nullptr)
    {
        return;
        // Handle invalid GameInstance
    }

    // Parsing "Mirror" option (Level/Prestige related)
    // Offset 0x74c in disassembly
    int32 MirrorLevel = 0;
    FString MirrorOptionValue = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Mirror" */);
    if (MirrorOptionValue.IsEmpty() == false)
    {
        MirrorLevel = FCString::Atoi(*MirrorOptionValue);
        if (MirrorLevel < 0)
        {
            MirrorLevel = 0;
        }
    }
    // Set the Mirror Level on the PlayerState (Variable at 0x74c)
    /* UNDEFINED ELEMENT Member_0x74c */
    PlayerState->SelectedCamperIndex = MirrorLevel;

    // Parsing "Badge" option
    // Offset 0x754 in disassembly
    int32 BadgeId = 0x10000000;
    FString BadgeOptionValue = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Badge" */);
    if (BadgeOptionValue.IsEmpty() == false)
    {
        BadgeId = FCString::Atoi(*BadgeOptionValue);
    }
    // Set the Badge ID on the PlayerState (Variable at 0x754)
    PlayerState->SelectedSlasherIndex = BadgeId;

    // Determine the Game Role (Camper or Slasher)
    // Using 0 for Camper and 1 for Slasher based on UE4 conventions in this context
    int32 RoleIndex = FCString::Atoi(*UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Role" */));
    // EPlayerRole is likely an enum. 0 = VE_Camper, 1 = VE_Slasher
    EPlayerRole GameRole = (EPlayerRole)RoleIndex;

    // Set the Role on the PlayerState
    PlayerState->SetGameRole(GameRole);

    // Parse Equipment based on Role
    if (GameRole == VE_Camper)
    {
        // Parsing Camper "Equip" (Item)
        FString EquipOption = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Equip" */);
        FName ItemId = NAME_None;

        if (EquipOption.IsEmpty() == false)
        {
            ItemId = FName(*EquipOption);
        }
        else
        {
            // Offset 0x760 (Camper Item) uses a default or empty value if parsing fails
            /* UNDEFINED ELEMENT Member_0x760_DefaultLogic */
        }

        // Store Camper Item (Offset 0x760)
        PlayerState->CamperData.EquipedItemId = ItemId;
    }
    else if (GameRole == /* UNDEFINED ELEMENT VE_Slasher */)
    {
        // Parsing Slasher "Equip" (Power)
        FString EquipOption = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Equip" */);
        FName PowerId = NAME_None;

        if (EquipOption.IsEmpty() == false)
        {
            PowerId = FName(*EquipOption);
        }
        else
        {
            /* UNDEFINED ELEMENT Member_0x7a0_DefaultLogic */
        }

        // Store Slasher Power (Offset 0x7a0)
        PlayerState->SlasherData.EquipedPowerId = PowerId;
    }

    // Parse Perks (Shared logic logic, stored in different arrays based on role)
    FString PerksString = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Perks" */);

    TArray<FString> ParsedPerks;
    // Parse semi-colon separated string
    PerksString.ParseIntoArray(ParsedPerks, TEXT(";"), true);

    // Target Array pointer setup
    TArray<FName>* TargetPerksArray = nullptr;

    if (GameRole == VE_Camper)
    {
        // Offset 0x768 (Camper Perks Array)
        TargetPerksArray = &PlayerState->CamperData.EquipedItemAddonIds;
    }
    else if (GameRole == VE_Slasher)
    {
        // Offset 0x7a8 (Slasher Perks Array)
        TargetPerksArray = &PlayerState->SlasherData.EquipedPowerAddonIds;
    }

    // Clear existing perks
    if (TargetPerksArray != nullptr)
    {
        TargetPerksArray->Empty();

        // Iterate and add parsed perks
        for (int32 i = 0; i < ParsedPerks.Num(); i++)
        {
            FString PerkStr = ParsedPerks[i];
            if (PerkStr.Len() > 0)
            {
                // Logic to handle "Empty" slots or valid Perk IDs
                // The disassembly checks if the string length/content meets specific criteria (<= 1 char)
                if (/* UNDEFINED ELEMENT CheckIfStringIsEmptyRepresentation */)
                {
                    // Add a None/Empty name
                    TargetPerksArray->Add(NAME_None);
                }
                else
                {
                    TargetPerksArray->Add(FName(*PerkStr));
                }
            }
        }
    }

    // Parsing specific customization options (likely Addons or Skins)
    FString CustomizationOption1 = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Option_1" */);
    if (CustomizationOption1.IsEmpty() == false)
    {
        // Offset 0x7c0
        PlayerState->PlayerData.EquipedFavorId = FName(*CustomizationOption1);
    }

    // Parsing Integer Options (Offsets 0x798 and 0x758)
    PlayerState->SlasherData.SlasherSkulls = UGameplayStatics::GetIntOption(Options, /* UNDEFINED ELEMENT "IntOption1" */, 0);
    PlayerState->CamperData.CamperSkulls = UGameplayStatics::GetIntOption(Options, /* UNDEFINED ELEMENT "IntOption2" */, 0);

    // Parsing Addons List
    // Offset 0x7e8 (Addons Array)
    PlayerState->PlayerData.CustomizationMesh.Empty();

    FString AddonsString = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Addons" */);
    TArray<FString> ParsedAddons;
    AddonsString.ParseIntoArray(ParsedAddons, TEXT(";"), true);

    for (int32 i = 0; i < ParsedAddons.Num(); i++)
    {
        FString AddonStr = ParsedAddons[i];
        if (AddonStr.Len() > 1) // Check logic from disassembly
        {
            PlayerState->PlayerData.CustomizationMesh.Add(FName(*AddonStr));
        }
        else
        {
            PlayerState->PlayerData.CustomizationMesh.Add(NAME_None);
        }
    }

    // Parsing Favors/Offerings
    // Offset 0x7d8 (Favors Array)
    PlayerState->PlayerData.EquipedPerkLevels.Empty();

    FString FavorsString = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "Favors" */);
    TArray<FString> ParsedFavors;
    FavorsString.ParseIntoArray(ParsedFavors, TEXT(";"), true);

    for (int32 i = 0; i < ParsedFavors.Num(); i++)
    {
        FString FavorStr = ParsedFavors[i];
        // Similar parsing logic converting String to FName or int32 depending on exact type
        // Disassembly suggests conversion to int32 via _wtoi or similar logic
        int32 FavorId = FCString::Atoi(*FavorStr);
        PlayerState->PlayerData.EquipedPerkLevels.Add(FavorId);
    }

    // Specific logic interacting with GameInstance inventory or state
    // Disassembly checks a byte flag at offset 0x30 inside a struct at GameInstance + 0x3b8
    if (DBDGameInstance != nullptr)
    {
        UDBDPersistentData* UnknownStruct = DBDGameInstance->_persistentData;
        if (UnknownStruct != nullptr && UnknownStruct->_gamePersistentData.SessionInfos.GameType == PartyMode)
        {
            // Consuming offering or verifying inventory logic
            // Loops through the array at 0x7d8 and modifies it based on GameInstance data (0xf0)

            /* UNDEFINED ELEMENT InventoryLogicBlock */
        }
    }

    // Parsing one final option group (Offset 0x728)
    FString LastOptionStr = UGameplayStatics::ParseOption(Options, /* UNDEFINED ELEMENT "LastOption" */);
    if (LastOptionStr.IsEmpty() == false)
    {
        PlayerState->MirrorsId = LastOptionStr;
    }

    // Cleanup logic for temporary arrays is handled automatically by UE4 TArray destructors/stack unwinding.

    // Lobby Timer Logic
    // _timerValue is at offset 0x460 in ADBDGame_Lobby
    const float TimerResetValue = 60.9f;
    const float TimerUnset = -1.0f;

    // Check if timer is not set and if player count is within lobby limits (usually < 5 for DBD)
    if (this->_timerValue != TimerUnset)
    {
        // Disassembly: ucomiss xmm0, dword [rdi+0x460] (0xbf800000 = -1.0f)
    }

    // Access GameState to check player array size
    AGameState* CurrentGameState = this->GameState;
    if (CurrentGameState != nullptr)
    {
        if (this->_timerValue == TimerUnset && CurrentGameState->PlayerArray.Num() < 5)
        {
            this->_timerValue = TimerResetValue;
        }
    }

    // Call the base class implementation (Unreal Engine AGameMode)
    // Disassembly calls AGameMode::InitNewPlayer at the end
    Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}



/* UNFINISHED FUNCTION */
bool ADBDGame_Lobby::IsInPartyMode() const
{
    // 140265e48: Initialize the result to false (0).
    bool bResult = false;

    // TFunction<void __cdecl(UDBDPersistentData const *)>
    TFunction<void(const UDBDPersistentData*)> CheckPartyModeCallback =
        [&bResult](const class UDBDPersistentData* PersistentData)
        {
            // 140247a93: Read the byte at offset 0x30 (SessionInfos.GameType).
            uint8_t GameType = PersistentData->SessionInfos.GameType;

            // 140247a9a - 140247a9f: Check if GameType is 1 (PartyMode) or 2 (SurvivorGroup).
            // Logic: (GameType - 1) <= 1

            // 140247aa3: Write the result to the captured variable.
            bResult = (GameType == PartyMode);
        };

    // 140265e98: Call the utility function using the separated TFunction variable.
    WorldExt::CallIfPersistentDataIsValid(this, CheckPartyModeCallback);

    // 140265e9d: Return the result.
    return bResult;
}



/* UNFINISHED FUNCTION */
bool ADBDGame_Lobby::IsInSurvivorGroupsMode() const
{
    // 140265e48: Initialize the result to false (0).
    bool bResult = false;

    // Выделяем аргумент (лямбду) в отдельную переменную TFunction
    TFunction<void __cdecl(UDBDPersistentData const*)> CheckPartyModeCallback =
        [&bResult](const class UDBDPersistentData* PersistentData)
        {
            // 140247a93: Read the byte at offset 0x30 (SessionInfos.GameType).
            uint8_t GameType = PersistentData->SessionInfos.GameType;

            // 140247a9a - 140247a9f: Check if GameType is 1 (PartyMode) or 2 (SurvivorGroup).
            // Logic: (GameType - 1) <= 1

            // 140247aa3: Write the result to the captured variable.
            bResult = (GameType == SurvivorGroup);
        };

    // 140265e98: Call the utility function using the separated TFunction variable.
    WorldExt::CallIfPersistentDataIsValid(this, CheckPartyModeCallback);

    // 140265e9d: Return the result.
    return bResult;
}



/* UNFINISHED FUNCTION */
APlayerController* ADBDGame_Lobby::Login(UPlayer* NewPlayer, ENetRole RemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    // Retrieve and cast the GameInstance
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Validation logic before login
    if (DBDGameInstance != nullptr)
    {
        // Check if the Lobby is in Party Mode
        if (this->IsInPartyMode())
        {
            // Check the internal timer value (Offset 0x460)
            // The timer must not be between 0.0f and 5.9f (inclusive) to allow login
            // Disassembly logic: !(_timerValue < 0f) && !(_timerValue > 5.9f)
            if (this->_timerValue >= 0.0f && this->_timerValue <= 5.9f)
            {
                // If the timer is within the restricted range, reject the login and set the error message
                // The string is loaded from a data segment (likely a localized FText)
                ErrorMessage = /* UNDEFINED ELEMENT "LobbyTimerErrorText" FText::ToString() */;
                return nullptr;
            }
        }
    }

    // Call the base GameMode Login function
    APlayerController* NewPlayerController = Super::Login(NewPlayer, RemoteRole, Portal, Options, UniqueId, ErrorMessage);

    // If the login was successful and the controller is valid
    if (NewPlayerController != nullptr)
    {
        // Check if the object is valid and not pending kill (Bitwise check on offset 0x140 in disassembly)
        if (IsValid(NewPlayerController))
        {
            // Manually set the HUD class for the new player controller
            // Offset 0x3a8 corresponds to the HUDClass member in AGameMode
            NewPlayerController->ClientSetHUD(this->HUDClass);
        }
    }

    // Trigger OnPlayerReadyUp with nullptr. 
    // This is called regardless of login success or failure, likely to update the lobby UI state.
    this->OnPlayerReadyUp(nullptr);

    return NewPlayerController;
}




void ADBDGame_Lobby::Logout(AController* Exiting)
{
    // Call the base GameMode Logout implementation
    Super::Logout(Exiting);

    // Retrieve the GameState (Offset 0x418)
    AGameState* CurrentGameState = this->GameState;

    // Check if the number of players in the array is less than 5.
    // The disassembly compares the array count (converted to float) with 5.0f (0x40a00000).
    // If it is greater than or equal to 5, it skips the reset logic (jumps to calling OnPlayerReadyUp).
    if (CurrentGameState != nullptr && CurrentGameState->PlayerArray.Num() < 5)
    {
        // Try to cast the GameState to the custom ADBDGameState
        ADBDGameState* DBDGameState = Cast<ADBDGameState>(CurrentGameState);

        // Check if the cast was successful and the object is valid (disassembly checks internal flags)
        if (DBDGameState != nullptr && IsValid(DBDGameState))
        {
            // Reset specific members in ADBDGameState
            // Offset 0x612 (Byte)
            DBDGameState->PlayersReadyToStart = 0;

            // Offset 0x614 (Int32) - Set to -1 (0xffffffff)
            DBDGameState->SecondsLeftInLobby = -1;
        }

        // Reset the lobby timer value (Offset 0x460) to -1.0f (0xbf800000)
        this->_timerValue = -1.0f;
    }

    // Call OnPlayerReadyUp, likely to update UI or ready status after a player leaves
    this->OnPlayerReadyUp(Exiting);
}




void ADBDGame_Lobby::OnPlayerReadyUp(AController* PC)
{
    // Offset 0x460: _timerValue
    // Check if the timer is currently stopped (-1.0f) or currently in a long countdown (> 5.9f).
    // 0xbf800000 = -1.0f, 0x40bccccd = ~5.9f
    bool bIsTimerStoppedOrLong = (this->_timerValue == -1.0f || this->_timerValue > 5.9000001f);

    // Section: Game Mode Logic checks to determine if we should proceed to the Readiness Loop
    if (this->IsInPartyMode())
    {
        // Party Mode Logic (0x14026c696)
        // Ensure we have enough players to verify readiness (at least 5 for a full match usually)
        AGameState* CurrentGameState = this->GameState;
        if (CurrentGameState != nullptr && CurrentGameState->PlayerArray.Num() < 5)
        {
            // If we don't have enough players and the timer isn't already short, stop/reset logic.
            if (bIsTimerStoppedOrLong)
            {
                goto StopTimerLabel;
            }
            return;
        }
    }
    else if (this->IsInSurvivorGroupsMode())
    {
        // Survivor Groups Mode Logic (0x14026c5e0 -> 0x14026c680)
        // If the timer is already running a short countdown (<= 5.9f and > 0.0f), do nothing.
        if (!bIsTimerStoppedOrLong && this->_timerValue > 0.0f)
        {
            return;
        }
        // Otherwise proceed to readiness loop
    }
    else
    {
        // Standard Public Lobby Logic (0x14026c5f0)
        AGameState* CurrentGameState = this->GameState;

        // Validate GameState is of the correct custom type
        if (CurrentGameState != nullptr && CurrentGameState->IsA(ADBDGameState::StaticClass()))
        {
            int32 SurvivorCount = 0;
            int32 KillerCount = 0;
            int32 SpectatorCount = 0; // Void* in pseudo, but usually int for counts

            // 0x14026c634: Helper to get role counts
            ADBDGameState::GetPlayerRoleCounts(CurrentGameState, &SurvivorCount, &KillerCount, &SpectatorCount);

            int32 TotalPlayers = SurvivorCount + KillerCount;

            // Logic checking valid composition to start a match
            // 1. Total players must be >= 2 (0x14026c659)
            // 2. If timer is currently stopped/long, strictly enforce composition:
            //    - Total <= 5
            //    - At least 1 Survivor
            //    - Exactly 1 Killer
            bool bCompositionValid = false;

            if (TotalPlayers >= 2)
            {
                if (bIsTimerStoppedOrLong)
                {
                    if (TotalPlayers <= 5 && SurvivorCount > 0 && KillerCount == 1)
                    {
                        bCompositionValid = true;
                    }
                }
                else
                {
                    // If timer is already short, we might be lenient or just proceed
                    bCompositionValid = true;
                }
            }

            if (!bCompositionValid)
            {
                if (bIsTimerStoppedOrLong)
                {
                    goto StopTimerLabel;
                }
                return;
            }
        }
        else
        {
            // Invalid GameState
            return;
        }
    }

    // Section: Check Player Readiness (0x14026c6bd)
    AGameState* CheckGameState = this->GameState;
    if (CheckGameState == nullptr)
    {
        return; // Should have been caught above, but safety check
    }

    // Iterate through all players in the GameState
    for (int32 i = 0; i < CheckGameState->PlayerArray.Num(); i++)
    {
        APlayerState* PS = CheckGameState->PlayerArray[i];

        // Cast to custom PlayerState to access specific flags
        ADBDPlayerState* DBDPS = Cast<ADBDPlayerState>(PS);

        if (DBDPS != nullptr)
        {
            // Offset 0x748: Boolean flag for "IsReady"
            // Offset 0x750: Enum/Byte for Role. 3 is likely Spectator.
            bool bIsReady = DBDPS->IsPlayerReady != 0;
            EPlayerRole GameRole = DBDPS->GameRole;

            // If player is NOT ready AND NOT a spectator
            if (!bIsReady && GameRole != VE_Observer)
            {
                // If no specific controller caused this update, any unready player halts the timer.
                if (PC == nullptr)
                {
                    goto StopTimerLabel;
                }

                // If a controller IS passed (the one who just toggled state),
                // we ignore them in this check. This implies we assume they are transitioning to Ready,
                // or we only care if *others* are unready.
                // Disassembly: cmp rbx, qword [rbp+0x3a0]; jne ... (Jump if Not Equal to Reset)
                if (DBDPS != PC->PlayerState)
                {
                    goto StopTimerLabel;
                }
            }
        }
    }

    // If we passed the loop, everyone is ready (or the only unready person is the PC acting now).
    // Set timer to Short Countdown (5.9s)
    // 0x14026c81e
    this->_timerValue = 5.9000001f;
    return;

StopTimerLabel:
    // 0x14026c83a
    // Only reset the timer if we are in specific custom modes.
    // Public lobbies likely handle cancellations differently (e.g. matchmaking backfill).
    if (this->IsInPartyMode() || this->IsInSurvivorGroupsMode())
    {
        this->_timerValue = -1.0f;
    }
}




void ADBDGame_Lobby::OnSessionDestroyed(FName SessionName, bool bWasSuccessful)
{
    // Retrieve the GameInstance and cast it to UDBDGameInstance
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Basic Validation: Ensure GameInstance exists and is valid
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Check if the GameInstance is valid (not pending kill, reachable)
    if (!IsValid(DBDGameInstance))
    {
        return;
    }

    // Access specific internal members of the GameInstance
    // Offset 0x3b0 in UDBDGameInstance seems to point to a manager or handler object
    UGameFlowContextSystem* ManagerObject = DBDGameInstance->_contextSystem;

    if (ManagerObject != nullptr)
    {
        // Check if the ManagerObject is valid
        if (IsValid(ManagerObject))
        {
            // Access a member at offset 0x90 within the ManagerObject
            UOnlineSystemHandler* SubObject = ManagerObject->m_OnlineSystemHandler;

            if (SubObject != nullptr)
            {
                // Access a member at offset 0x5f0 within the SubObject
                UDBDServerInstance* TargetObject = SubObject->_serverInstance;

                if (TargetObject != nullptr)
                {
                    // Check validity of the TargetObject
                    if (IsValid(TargetObject))
                    {
                        // Check a specific flag or state in the GameInstance
                        // Offset 0x3b8 points to a struct/object, check byte at offset 0x30
                        UDBDPersistentData* StateStruct = DBDGameInstance->_persistentData;

                        // Condition check: cmp byte [rax+0x30], dil (dil is bWasSuccessful/0 depending on register usage context)
                        // In disassembly, rdi is zeroed out (xor edi, edi), so it compares against 0 (false).
                        // If byte at 0x30 is NOT 0 (i.e., is true), we proceed.
                        if (StateStruct != nullptr && StateStruct->_gamePersistentData.SessionInfos.GameType == Online)
                        {
                            // Error Handling Logic
                            // Log the error "Registering Steam Auth or EAC Error"
                            UE_LOG(GameFlow, Log, TEXT("Registering Steam Auth or EAC Error (Error Type: %i; Error Code: %i)"), 10, 0);

                            // Set Disconnected State on GameInstance
                            DBDGameInstance->SetIsDisconnected(true);

                            // Set specific error codes on GameInstance
                            // Offset 0x229 (Byte) = 10 (0xa)
                            DBDGameInstance->DisconnectError = MirrorsUnscheduledSessionDestruction;

                            // Offset 0x22c (Int32) = 0
                            DBDGameInstance->LastAuthOrEACErrorCode = 0;

                            // Clear or set an array at offset 0x230
                            DBDGameInstance->AuthOrEACErrorAdditionalInfo = FString();
                        }
                    }
                }
            }
        }
    }
}




