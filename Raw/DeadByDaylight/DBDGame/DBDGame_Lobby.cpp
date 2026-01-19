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




void ADBDGame_Lobby::PostInitializeComponents()
{
    // Retrieve the Game Instance associated with this actor
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the Game Instance exists
    if (DBDGameInstance != nullptr)
    {
        // The disassembly performs a low-level check against GUObjectArray using the InternalIndex.
        // It specifically checks bit 29 (0x1D) of the object flags (RF_PendingKill / RF_Garbage).
        // If the bit is NOT set, the object is considered valid for this operation.
        if (IsValid(DBDGameInstance) == true)
        {
            // Enable network connection checking with the first argument as true (1) and second as false (0)
            DBDGameInstance->SetNetworkConnectionCheckingEnabled(true, false);
        }
    }

    // Retrieve the Matchmaking Presence Subsystem via the OnlinePresencePlugin.
    // The disassembly shows this returns a TSharedRef/TSharedPtr (struct return on stack).
    TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = IOnlinePresencePlugin::Matchmaking();

    // Check if there is an existing session destroyed handle currently bound
    if (this->_onSessionDestroyedHandle.IsValid() == true)
    {
        // Call the virtual function at offset 0x220 to remove the delegate using the existing handle.
        // We reset the handle ID to 0 immediately after.
        MatchmakingSubsystem->ClearOnDestroySessionCompleteDelegate_Handle(this->_onSessionDestroyedHandle);
        this->_onSessionDestroyedHandle.Reset();
    }

    // Prepare the delegate for OnSessionDestroyed.
    // The disassembly (0x14026d813 - 0x14026d8a6) performs complex memory allocation (TInlineAllocator)
    // to construct the delegate payload, binding ADBDGame_Lobby::OnSessionDestroyed to 'this'.
    FOnSessionDestroyedDelegate SessionDestroyedDelegate;
    SessionDestroyedDelegate.BindUObject(this, &ADBDGame_Lobby::OnSessionDestroyed);

    // Call the virtual function at offset 0x218 to add the new delegate.
    // The return value is the new FDelegateHandle.
    this->_onSessionDestroyedHandle = MatchmakingSubsystem->AddOnDestroySessionCompleteDelegate_Handle(SessionDestroyedDelegate);

    // Call the parent class implementation.
    // Note: The pseudo code shows the TSharedRef destructor logic happens after this call (scope end).
    Super::PostInitializeComponents();
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::PostLogin(APlayerController* NewPlayer)
{
    // Call the parent class implementation first
    Super::PostLogin(NewPlayer);

    // Retrieve the GameInstance and cast it to the project-specific class
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid (corresponds to GUObjectArray checks in disassembly)
    if (IsValid(DBDGameInstance) == true)
    {
        // Load the "OnlinePresence" module/plugin
        // /* UNDEFINED ELEMENT */ - Loading specific module interface by name "OnlinePresence"
        IOnlinePresencePlugin* OnlinePresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>("OnlinePresence");

        // Call a virtual function (offset 0xA0) on the module interface. 
        // Likely checking if the presence service is available or enabled.
        if (OnlinePresenceModule->IsConnected() == true)
        {
            // Retrieve and cast the PlayerState
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(NewPlayer->PlayerState);

            // Verify PlayerState validity and check a specific state flag (offset 0x140, bit 4)
            // If the flag is set (result != 0), the function exits early.
            if (IsValid(DBDPlayerState) == true && (DBDPlayerState->bActorIsBeingDestroyed) == false)
            {
                // Initialize local persistent data structure
                FPlayerPersistentData LocalPlayerData;

                // Copy data from the PlayerState into the local structure
                UDBDPersistentData::CopyData(DBDPlayerState, &LocalPlayerData, true);

                // Increment reference count for UniqueNetId (handled by SharedPointer internals in asm)
                // Retrieve persistent data from the GameInstance's manager (offset 0x3B8)
                // /* UNDEFINED ELEMENT */ - 0x3B8 is likely UDBDPersistentDataManager
                bool bHasPersistentData = DBDGameInstance->_persistentData->GetPlayerPersistentData(DBDPlayerState->UniqueId);

                // Cast the controller to the project-specific base
                ADBDPlayerControllerBase* DBDController = Cast<ADBDPlayerControllerBase>(NewPlayer);

                // Authenticate the new player within the lobby logic
                this->AuthenticateNewPlayer(DBDController);

                // Check specific status in PersistentDataManager (0x3B8). Offset 0x30 might be an enum (e.g., AuthorizationStatus).
                // If status is NOT 2, proceed with Anti-Cheat registration.
                if (DBDGameInstance->_persistentData->_gamePersistentData.SessionInfos.GameType != SurvivorGroup)
                {
                    // Retrieve EasyAntiCheat instance from GameInstance offset 0x500
                    UDBDEasyAntiCheat* EasyAntiCheat = DBDGameInstance->_eac;

                    if (IsValid(EasyAntiCheat) == true)
                    {
                        // Check if the player is a local controller via VTable 0x628 (IsLocalController)
                        // Also checks a bitfield at 0x3BC bit 1.
                        if ((DBDController->bIsPlayerController) && DBDController->IsLocalController() == true)
                        {
                            EasyAntiCheat->CreateServer();
                        }

                        // Register the client with EAC authority
                        FString AccountIdStr = DBDPlayerState->UniqueId->ToString();

                        // /* UNDEFINED VTABLE */ - Calling FUniqueNetId::vtable+0x20 to get GUID string representation
                        FString OwnerGuid = DBDPlayerState->UniqueId->ToString();

                        EasyAntiCheat->Authority_RegisterClient(DBDController, DBDPlayerState->UniqueId, OwnerGuid);

                        // Validate the server from the client's perspective
                        DBDController->Client_ValidateServer();
                    }
                }

                // If GetPlayerPersistentData returned false (or specific bit check), skip experience logic
                // The asm checks var_3f8 (return of GetPlayerPersistentData) and the bitfield at 0x3bc.
                if (bHasPersistentData == true && (DBDController->bIsPlayerController))
                {
                    // Check if this is a local controller again via VTable 0x628
                    if (DBDController->IsLocalController() == true)
                    {
                        // Update previous skull counts from the persistent data
                        LocalPlayerData.PreviousSlasherSkulls = LocalPlayerData.SlasherSkulls; // Mapping based on offset 0x48/0x4C logic in asm
                        LocalPlayerData.PreviousCamperSkulls = LocalPlayerData.CamperSkulls;

                        // Calculate Max Experience from Design Tunables
                        int32 MaxExperience = 2147483647; // 0x7FFFFFFF
                        UDBDDesignTunables* DesignTunables = DBDGameInstance->DesignTunables; // Offset 0xF0

                        if (IsValid(DesignTunables) == true)
                        {
                            MaxExperience = DesignTunables->MaxExperience;
                        }

                        // Cap the previous experience if necessary
                        if (LocalPlayerData.PreviousExperience < MaxExperience)
                        {
                            LocalPlayerData.PreviousExperience = MaxExperience;
                        }

                        // Copy data back to the PlayerState or PersistentData
                        UDBDPersistentData::CopyData(&LocalPlayerData, DBDPlayerState, true);
                    }
                }

                // Update the Persistent Data Manager with the modified local data
                _persistentData->SetPlayerPersistentData(DBDPlayerState->UniqueId, &LocalPlayerData);

                // Final profile loading logic based on controller type
                if ((DBDController->bIsPlayerController) && DBDController->IsLocalController() == true)
                {
                    // Load full profile via facade (GameInstance offset 0x3C0)
                    DBDGameInstance->_playerDataFacade->LoadFullProfile();

                    DBDController->Local_LoadProfileStats();
                    DBDController->ResetPreGameCachedValues();
                }
                else
                {
                    // Request profile from client
                    DBDController->Client_RequestPlayerProfile();
                }

                // Destructors for LocalPlayerData and related structs (Rituals, Loadouts) are called implicitly here
            }
        }
    }
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::PostSeamlessTravel()
{
    // Call the parent class implementation
    Super::PostSeamlessTravel();

    // Retrieve the GameState and cast it to the project-specific class
    ADBDGameState* DBDGameState = Cast<ADBDGameState>(this->GameState);

    // Check if the GameState is valid (corresponds to IsValid/GUObjectArray checks in disassembly)
    if (IsValid(DBDGameState) == true)
    {
        // Check a specific flag bit at offset 0x140 (Bit 2, value 4)
        // The disassembly skips the update if this bit is set (jne). 
        // Therefore, we execute if the bit is false.
        if ((DBDGameState->bActorIsBeingDestroyed) == false)
        {
            DBDGameState->Server_UpdateGameRole();
        }
    }

    // Retrieve the GameInstance and cast it to the project-specific class
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid
    if (IsValid(DBDGameInstance) == true)
    {
        // Access a manager at offset 0x3B8 (likely PersistentDataManager based on prior context)
        UObject* Manager_0x3B8 = DBDGameInstance->_persistentData;

        // Check if the manager at 0x3B8 is valid
        if (IsValid(Manager_0x3B8) == true)
        {
            // Access a member at offset 0x3B0
            UObject* Member_0x3B0 = DBDGameInstance->_contextSystem;

            // Check if the member at 0x3B0 is valid
            if (IsValid(Member_0x3B0) == true)
            {
                // Access a deeply nested UDBDServerInstance via pointer indirection
                // 1. Get pointer at offset 0x90 from Member_0x3B0
                // 2. Get pointer at offset 0x5F0 from that intermediate object
                // /* UNDEFINED ELEMENT */ - Explicit pointer arithmetic used to replicate disassembly access
                UOnlineSystemHandler* IntermediateObj = Member_0x3B0->m_OnlineSystemHandler;
                UDBDServerInstance* DBDServerInstance = IntermediateObj->_serverInstance;

                // Check if the ServerInstance is valid
                if (IsValid(DBDServerInstance) == true)
                {
                    // Select the game type string based on a status byte at offset 0x30 in Manager_0x3B8
                    // Disassembly logic: if ((Status - 1) <= 1) -> use String 2, else String 1
                    // This implies: if Status is 1 or 2, use String 2.
                    EGameType Status = Manager_0x3B8->_gamePersistentData.SessionInfos.GameType;
                    FString GameType;

                    if (Status == PartyMode || Status == SurvivorGroup)
                    {
                        GameType = FString(TEXT("/* UNDEFINED STR */ data_143584f88"));
                    }
                    else
                    {
                        GameType = FString(TEXT("/* UNDEFINED STR */ data_143584f78"));
                    }

                    // Update host settings on the server instance
                    DBDServerInstance->UpdateHostSettings(GameType);
                }
            }
        }
    }
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    // Call the parent class implementation
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    // Retrieve the GameInstance and cast it to the project-specific class
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid (corresponds to GUObjectArray checks in disassembly)
    if (IsValid(DBDGameInstance) == true)
    {
        // Check if the lobby is currently in Party Mode
        if (this->IsInPartyMode() == true)
        {
            // Check the lobby timer value
            // The disassembly checks if _timerValue is between 0.0f and 5.9f (inclusive)
            if (this->_timerValue >= 0.0f && this->_timerValue <= 5.9000001f)
            {
                // Convert a localized text (likely "Lobby is starting") to string and assign to ErrorMessage
                // /* UNDEFINED ELEMENT */ - Accessing static FText data_143621820
                ErrorMessage = FText::ToString(/* UNDEFINED ELEMENT */ LocalizedString_LobbyStarting);
            }

            // Retrieve the Local Player from the Game Instance
            ULocalPlayer* LocalPlayer = DBDGameInstance->GetLocalPlayer();

            // Check if the Local Player is valid
            if (IsValid(LocalPlayer) == true)
            {
                // Access a manager at offset 0x3B8 (Likely UDBDOnlineManager or UDBDPersistentDataManager)
                // /* UNDEFINED ELEMENT */ - Accessing member at 0x3B8
                UDBDPersistentData* Manager_0x3B8 = DBDGameInstance->_persistentData;

                if (Manager_0x3B8 != nullptr)
                {
                    // Check a boolean flag at offset 0x101 inside the manager (Likely bIsFriendsOnlyLobby)
                    // /* UNDEFINED ELEMENT */ - Accessing bool at 0x101
                    bool bCheckFriends = Manager_0x3B8->_gamePersistentData.GamePresetData._privateMatch;

                    if (bCheckFriends != false)
                    {
                        // Check if the local player is friends with the incoming player
                        // Passing ControllerId (offset 0xE0 of LocalPlayer) and the UniqueNetId object (offset 0x8 of FUniqueNetIdRepl)
                        if (DBDOnlineUtils::IsLocalPlayerFriendWith(LocalPlayer->ControllerId, UniqueId.UniqueNetId) == false)
                        {
                            // If not friends, set the ErrorMessage (likely "Friends Only Lobby")
                            // /* UNDEFINED ELEMENT */ - Accessing static FText data_143621838
                            ErrorMessage = FText::ToString(/* UNDEFINED ELEMENT */ LocalizedString_FriendsOnly);
                        }
                    }
                }
            }
        }
    }
}




void ADBDGame_Lobby::ReadyPlayersUp() const
{
    // Iterate through the player controller list in the world
    // The disassembly calls UWorld::GetPlayerControllerIterator (vtable offset 0x108 usually for GetWorld() call before)
    // and returns a container iterator structure.
    for (FConstPlayerControllerIterator It = this->GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        // Retrieve the controller weak pointer from the iterator
        TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr = *It;

        // Resolve the weak pointer to a raw pointer
        APlayerController* PlayerController = PlayerControllerWeakPtr.Get();

        // Check if the resolved pointer is valid
        if (IsValid(PlayerController) == true)
        {
            // Cast the controller to the specific menu controller class
            // The disassembly explicitly checks ClassTreeIndex logic, which corresponds to Cast<T>
            ADBDPlayerController_Menu* DBDMenuController = Cast<ADBDPlayerController_Menu>(PlayerController);

            // Verify the cast succeeded
            // The disassembly performs multiple IsValid/GUObjectArray checks on the casted pointer
            // and also checks a specific bit flag at offset 0x140 (likely RF_PendingKill or similar internal flag).
            // We encapsulate this in the standard IsValid() check.
            if (IsValid(DBDMenuController) == true)
            {
                // Call SetPlayerReady with 'true' (1)
                DBDMenuController->SetPlayerReady(true);
            }
        }
    }
}




void ADBDGame_Lobby::ResetStartTimer()
{
    // Retrieve the GameState pointer. 
    // In AGameMode, the GameState member is typically at offset 0x418 (depending on engine version/padding).
    ADBDGameState* DBDGameState = Cast<ADBDGameState>(this->GameState);

    // Reset the internal timer value to -1.0f.
    // The constant 0xbf800000 in disassembly represents -1.0f in IEEE 754 floating point.
    this->_timerValue = -1.0f;

    // Check if the GameState is valid.
    // The disassembly performs the standard IsValid() checks:
    // 1. Null check
    // 2. Class hierarchy check (Cast)
    // 3. Global Object Array (GUObjectArray) index check
    // 4. Object flags check (e.g., RF_PendingKill/RF_Garbage at offset 0x140)
    if (IsValid(DBDGameState) == true)
    {
        // Convert the floating-point timer value to a signed integer (truncation).
        // The instruction 'cvttss2si' performs this conversion.
        int32 TimerValueAsInt = (int32)this->_timerValue;

        // Update the member variable at offset 0x614 in ADBDGameState.
        // This is likely a replicated integer representation of the timer.
        DBDGameState->SecondsLeftInLobby = TimerValueAsInt;
    }
}




void ADBDGame_Lobby::SendServerAuthTicketToClient(ADBDPlayerControllerBase* NewPlayer)
{
    // Retrieve the Game Instance and cast it to the project-specific class
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Validate the Game Instance
    // The disassembly performs manual GUObjectArray checks which corresponds to IsValid()
    if (IsValid(DBDGameInstance) == true)
    {
        // Retrieve the Authentication component from the Game Instance
        // /* UNDEFINED ELEMENT */ - Custom accessor in UDBDGameInstance
        UDBDAuthentication* Authentication = DBDGameInstance->GetAuthentication();

        // Validate the Authentication component
        if (IsValid(Authentication) == true)
        {
            // Create a local authentication ticket.
            // The disassembly allocates space on the stack (var_28) for the return value, 
            // indicating FAuthenticationInfo is a struct returned by value.
            // /* UNDEFINED ELEMENT */ - Custom function in UDBDAuthentication
            FAuthenticationInfo AuthTicket = Authentication->CreateLocalTicket();

            // Send the ticket to the new player controller.
            // The second argument '1' is passed as a boolean (true).
            // /* UNDEFINED ELEMENT */ - Custom function in ADBDPlayerControllerBase
            NewPlayer->SendAuthTicket(AuthTicket, true);

            // The disassembly at the end (0x140272531+) handles the destructor for AuthTicket,
            // specifically decrementing a SharedReferenceCount. 
            // This is handled automatically in C++ when AuthTicket goes out of scope.
        }
    }
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::StartPlay()
{
    // Call the parent class implementation first
    Super::StartPlay();

    // Retrieve the GameInstance and cast it to the project-specific class
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid (corresponds to IsValid/GUObjectArray checks in disassembly)
    if (IsValid(DBDGameInstance) == true)
    {
        // Access a member at offset 0x3B0 in the GameInstance
        // /* UNDEFINED ELEMENT */ - Accessing member at 0x3B0. This seems to be an intermediate manager/object.
        UGameFlowContextSystem* Member_0x3B0 = DBDGameInstance->_contextSystem;

        // Check if the member at 0x3B0 is valid
        if (IsValid(Member_0x3B0) == true)
        {
            // Access a deeply nested UDBDServerInstance via pointer indirection
            // 1. Get pointer at offset 0x90 from Member_0x3B0 (likely another intermediate object)
            // 2. Get pointer at offset 0x5F0 from that intermediate object to get UDBDServerInstance
            // /* UNDEFINED ELEMENT */ - Explicit pointer arithmetic used to replicate disassembly access
            UOnlineSystemHandler* IntermediateObj = Member_0x3B0->m_OnlineSystemHandler;

            // Check if IntermediateObj is valid
            // Note: The disassembly performs GUObjectArray validation on IntermediateObj (0x140277f72 - 0x140277f90)
            // We model this as a standard IsValid check on the object itself if we could treat it as UObject*.
            // Assuming IntermediateObj is a UObject* based on the context of GUObjectArray checks.
            if (IsValid(IntermediateObj) == true)
            {
                // Retrieve the ServerInstance from offset 0x5F0
                UDBDServerInstance* DBDServerInstance = IntermediateObj->_serverInstance;

                // Check if the ServerInstance is valid
                if (IsValid(DBDServerInstance) == true)
                {
                    // Update host settings on the server instance.
                    // The disassembly passes a pointer to a specific string/data (data_143584f88).
                    // Based on previous context (PostSeamlessTravel), this string is likely a GameType string.
                    FString GameType(TEXT("/* UNDEFINED STR */ data_143584f88"));

                    DBDServerInstance->UpdateHostSettings(GameType);
                }
            }
        }
    }
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::StartTravel()
{
    // Call the iterator to get player controllers
    // The disassembly calls vtable offset 0x108 (GetWorld) then GetPlayerControllerIterator
    for (FConstPlayerControllerIterator It = this->GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        // Resolve the weak pointer
        TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr = *It;
        APlayerController* PlayerController = PlayerControllerWeakPtr.Get();

        // Standard object validity checks
        if (IsValid(PlayerController) == true)
        {
            // Check specific flag on the controller (offset 0x140 bit 4, likely RF_PendingKill/Garbage)
            // If the flag is set (jne label), we skip valid logic and mark travel readiness.
            // The disassembly structure is: if (valid) { ... } else { FAIL }
            // If any check fails, it jumps to 0x140278547 which sets _checkReadyToTravel = 1
            if ((PlayerController->bActorIsBeingDestroyed) == false)
            {
                // Cast to ADBDPlayerState
                ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerController->PlayerState);

                // If PlayerState is invalid, or cast failed
                if (IsValid(DBDPlayerState) == true)
                {
                    // Check if the player is ready to travel.
                    // This is checking a boolean flag at offset 0x820 in ADBDPlayerState.
                    // If flag is 0 (false), set _checkReadyToTravel = 1 and return.
                    if (DBDPlayerState->IsReadyToTravel == false)
                    {
                        this->_checkReadyToTravel = true;
                        return;
                    }
                }
                else
                {
                    // PlayerState invalid/null -> Fail check
                    this->_checkReadyToTravel = true;
                    return;
                }
            }
            else
            {
                // Controller invalid/pending kill -> Fail check
                this->_checkReadyToTravel = true;
                return;
            }
        }
        else
        {
            // Controller pointer null -> Fail check
            this->_checkReadyToTravel = true;
            return;
        }
    }

    // If loop completes without returning early, all players are ready.
    this->_checkReadyToTravel = false;

    // Retrieve GameInstance
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (IsValid(DBDGameInstance) == true)
    {
        // Access GameState pointer from lobby (offset 0x418 in disassembly usually refers to GameState)
        AGameState* GameState = this->GameState;

        if (IsValid(GameState) == true)
        {
            // Retrieve PlayerArray data from GameState
            // The disassembly iterates the PlayerArray (TArray<APlayerState*>) manually.
            // It counts the number of players based on their Role (offset 0x750 in PlayerState? likely GameSpecificRole).
            int32 KillerCount = 0;
            int32 SurvivorCount = 0;
            int32 SpectatorCount = 0;

            for (APlayerState* PlayerState : GameState->PlayerArray)
            {
                if (IsValid(PlayerState) == true)
                {
                    // Cast to ADBDPlayerState to access role
                    ADBDPlayerState* DBDState = Cast<ADBDPlayerState>(PlayerState);
                    if (DBDState)
                    {
                        // Check Role (offset 0x750 based on disassembly logic)
                        // The disassembly checks byte at 0x750.
                        // Logic:
                        // val - 1 == 0 -> val = 1 (Killer?) -> Increment RBP (SurvivorCount/KillerCount mixed?)
                        // val - 2 == 0 -> val = 2 (Survivor?) -> Increment R14 (SurvivorCount/KillerCount mixed?)
                        // val == 3 (Spectator?) -> Increment RSI (SpectatorCount)
                        // Note: The mapping of 1/2 to Killer/Survivor is inferred.
                        // Based on logging: "Number of players (survivors + killers) = %d" is (R14 + RBP)
                        EPlayerRole Role = DBDState->GameRole;

                        if (Role == VE_Slasher) // Slasher/Killer
                        {
                            SurvivorCount++; // Variables named based on log sum
                        }
                        else if (Role == VE_Camper) // Camper/Survivor
                        {
                            KillerCount++;
                        }
                        else if (Role == VE_Observer) // Spectator
                        {
                            SpectatorCount++;
                        }
                    }
                }
            }

            // Update persistent data manager in GameInstance (Offset 0x3B8)
            // Offset 0x28 = Total Players (Survivors + Killers)
            // Offset 0x2C = Spectators
            UDBDPersistentData* PersistentManager = DBDGameInstance->_persistentData;
            if (PersistentManager)
            {
                PersistentManager->_gamePersistentData.PlayerCount = SurvivorCount + KillerCount;
                PersistentManager->_gamePersistentData.SpectatorCount = SpectatorCount;
            }

            // Logging if verbosity allows
            // Accessing GameFlow global struct/object
            if (/* UNDEFINED ELEMENT */ GameFlow.Verbosity >= 6)
            {
                UE_LOG(LogDBD, Log, TEXT("Number of players (survivors + killers) = %d"), SurvivorCount + KillerCount);
                UE_LOG(LogDBD, Log, TEXT("Number of spectators = %d"), SpectatorCount);
            }

            // Server Instance Logic
            // Access member at 0x3B0 in GameInstance -> then 0x90 -> then 0x5F0 (UDBDServerInstance)
            // This replicates the pointer chasing in disassembly.
            UGameFlowContextSystem* Obj3B0 = DBDGameInstance->_contextSystem;
            if (Obj3B0)
            {
                UOnlineSystemHandler* Obj90 = Obj3B0->m_OnlineSystemHandler;
                if (Obj90)
                {
                    UDBDServerInstance* ServerInstance = Obj90->_serverInstance;
                    if (IsValid(ServerInstance) == true)
                    {
                        // Update Host Settings with a specific string
                        // The string comes from data_143584f98
                        FString HostSettingsString(TEXT("/* UNDEFINED STR */ data_143584f98"));
                        ServerInstance->UpdateHostSettings(HostSettingsString);
                    }
                }
            }

            // Game Session Unregister
            // Call virtual function at 0x660 on GameSession (UnregisterPlayer? or similar)
            if (this->GameSession)
            {
                // Disassembly calls function at vtable+0x660 with SessionName as argument and 4 zeros.
                // Assuming UnregisterPlayer(FName SessionName, UniqueId, ...)
                this->GameSession->UpdateSessionJoinability(this->GameSession->SessionName, nullptr, false, false, false);
            }
        }
    }

    // Call Parent StartTravel or specific internal travel logic
    // The disassembly calls vtable[0x21] which is likely GetWorld() again, 
    // then creates a string/array (TArray<uint16_t>) and calls a function at offset 0x1F8 on the World object?
    // Or it calls ADBDGame_Lobby::StartTravel internal.
    // The end of the function (0x1402789c7) calls a virtual function on 'r14' (which was 'this' at start, then vtable[0x21] result).
    // Let's assume standard Engine travel.

    // Construct URL/String from data
    // /* UNDEFINED ELEMENT */ - Construction of travel URL from data_1435850c0/b8/60/58

    // this->ProcessServerTravel(URL); // High level equivalent
}



/* UNFINISHED FUNCTION */
void ADBDGame_Lobby::Tick(float DeltaSeconds)
{
    // Call the parent class implementation first
    Super::Tick(DeltaSeconds);

    // Check if the lobby is waiting to travel to the match
    if (this->_checkReadyToTravel != false)
    {
        this->CheckReadyToTravel();
    }

    // Load the OnlinePresence module to check connectivity/status
    // /* UNDEFINED ELEMENT */ - Accessing IOnlinePresencePlugin (or similar) interface
    IOnlinePresencePlugin* OnlinePresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>("OnlinePresence");

    // Call virtual function at 0xA0 (likely IsConnected or IsAvailable)
    // If it returns false (0), we redirect to the splash screen.
    if (OnlinePresenceModule->IsConnected() == false)
    {
        // Retrieve GameInstance
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (IsValid(DBDGameInstance) == true)
        {
            // Navigate back to the splash screen. 
            // 0x29 (41) is passed as an argument, likely a specific EGameState or Reason enum.
            DBDGameInstance->TravelToSplashScreen(0x29);
        }
        return;
    }

    // Retrieve the GameState
    ADBDGameState* DBDGameState = Cast<ADBDGameState>(this->GameState);

    if (IsValid(DBDGameState) == true)
    {
        // Handle Game Preset Data Initialization
        if (this->_mustInitgamePresetData != false)
        {
            this->_mustInitgamePresetData = false;

            UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
            if (IsValid(DBDGameInstance) == true)
            {
                // Access Persistent Data Manager at offset 0x3B8
                UDBDPersistentData* PersistentDataManager = DBDGameInstance->_persistentData;

                if (PersistentDataManager != nullptr)
                {
                    // Check a flag at offset 0x60. If 1, we might need to reset data.
                    // /* UNDEFINED ELEMENT */ - Accessing bool at offset 0x60
                    EGameType bFlag0x60 = PersistentDataManager->_gamePersistentData.LastSessionInfos.GameType;

                    if (bFlag0x60 == PartyMode && DBDGameInstance->ShouldResetPlayerData() == false)
                    {
                        // Broadcast changes if we don't need to reset
                        // /* UNDEFINED ELEMENT */ - Passing pointer to data at 0x90
                        void* DataPtr = PersistentDataManager->_gamePersistentData.GamePresetData;
                        DBDGameState->Authority_BroadcastMatchChanges(DataPtr);
                    }
                    else
                    {
                        // Initialize preset data
                        DBDGameState->InitGamePresetData();
                    }
                }
            }
        }

        // Check player configuration
        // /* UNDEFINED ELEMENT */ - Accessing byte at 0x6EF in GameState
        uint8 ConfigByte = DBDGameState->_playerDistributionReady;
        this->CheckPlayerConfiguration(ConfigByte);

        // --- Timer Logic ---

        // Threshold constant used in comparisons (5.9000001f)
        const float TimerThreshold = 5.9000001f;

        // If the game hasn't been forced start, and the timer is above the threshold (or counting down)
        if (this->_forceStarted == false && this->_timerValue > TimerThreshold)
        {
            // Party Mode Checks
            if (this->IsInPartyMode() == false)
            {
                // If not in party mode, check if we have enough players (e.g. 5 for standard match)
                if (DBDGameState->PlayerArray.Num() < 5)
                {
                    this->_timerValue = -1.0f;
                }
            }
            else
            {
                // Survivor Groups Mode Checks
                if (this->IsInSurvivorGroupsMode() == false)
                {
                    // If not survivor groups, check minimum player count (e.g. 2)
                    if (DBDGameState->PlayerArray.Num() < 2)
                    {
                        this->_timerValue = -1.0f;
                    }
                }
                else
                {
                    // Complex condition for Survivor Groups / Player Config
                    if (this->IsInSurvivorGroupsMode() == true || this->_playerConfigurationReady == true)
                    {
                        if (this->IsInPartyMode() == true && this->_timerValue <= TimerThreshold)
                        {
                            this->_timerValue = -1.0f;
                        }
                    }
                    else
                    {
                        this->_timerValue = -1.0f;
                    }
                }
            }
        }

        // Timer Countdown
        if (this->_timerValue > 0.0f)
        {
            float PreviousTimer = this->_timerValue;
            this->_timerValue -= DeltaSeconds;

            // If timer crossed the threshold (5.9s) downwards, ready up players
            if (PreviousTimer >= TimerThreshold && this->_timerValue < TimerThreshold)
            {
                this->ReadyPlayersUp();
            }

            // Clamp to 0
            if (this->_timerValue <= 0.0f)
            {
                this->_timerValue = 0.0f;
            }
        }

        // --- Replication State Update ---

        // Determine if the timer state is "Active/Running" (represented by 1) or "Stopped" (0)
        // The disassembly compares _timerValue against 0.0f (via xmm6).
        uint8 bTimerActive = (this->_timerValue > 0.0f) ? 1 : 0;

        // Check and update a replicated byte at 0x612 in GameState
        // /* UNDEFINED ELEMENT */ - Accessing Member_0x612 (likely bTimerActive replicated var)
        uint8 ReplicatedStatePtr = DBDGameState->PlayersReadyToStart;

        if (ReplicatedStatePtr != bTimerActive)
        {
            ReplicatedStatePtr = bTimerActive;
        }

        // Update the integer representation of the timer at 0x614 in GameState
        // /* UNDEFINED ELEMENT */ - Accessing Member_0x614
        DBDGameState->SecondsLeftInLobby = this->_timerValue;
    }
}