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




