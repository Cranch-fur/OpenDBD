// starts at line 18
UDBDOnlineSessionClient::UDBDOnlineSessionClient(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize the boolean flags. 
    this->_allowAcceptedInvites = true;
    this->_hasPlayTogetherNotification = false;

    // Initialize the play together user index
    this->_playTogetherUserIndex = 0;
}

// starts at line 22
void UDBDOnlineSessionClient::RegisterOnlineDelegates()
{
    // Call the parent class implementation first
    Super::RegisterOnlineDelegates();
    
    // Bind OnEndBeforeJoinSessionCompleteDelegate to UDBDOnlineSessionClient::OnEndBeforeJoinSessionComplete
    this->OnEndBeforeJoinSessionCompleteDelegate.BindUObject(this, &UDBDOnlineSessionClient::OnEndBeforeJoinSessionComplete);

    // Bind OnDestroyBeforeJoinSessionCompleteDelegate to UDBDOnlineSessionClient::OnDestroyBeforeJoinSessionComplete
    this->OnDestroyBeforeJoinSessionCompleteDelegate.BindUObject(this, &UDBDOnlineSessionClient::OnDestroyBeforeJoinSessionComplete);

    // Bind _onPlayerDisconnectedDelegate to UDBDOnlineSessionClient::OnSetPlayerAsDisconnectedComplete
    this->_onPlayerDisconnectedDelegate.BindUObject(this, &UDBDOnlineSessionClient::OnSetPlayerAsDisconnectedComplete);

    // Bind DBDOnJoinSessionCompleteDelegate to UDBDOnlineSessionClient::DBD_OnJoinSessionComplete
    this->DBDOnJoinSessionCompleteDelegate.BindUObject(this, &UDBDOnlineSessionClient::DBD_OnJoinSessionComplete);
}

// starts at line 31
void UDBDOnlineSessionClient::JoinSession(FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
    // Copy the session search result into the class member cache
    this->CachedSessionResult.Session = SearchResult.Session;
    this->CachedSessionResult.PingInMs = SearchResult.PingInMs;

    int32 gameType = -1;
    SearchResult.Session.SessionSettings.Get<int32>("GAMETYPE", gameType);

    // Determine the actual session name to use based on the game type
    FName sessionToUse = GameSessionName;
    if (gameType == 2)
    {
        sessionToUse = PartySessionName;
    }

    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->OuterPrivate);
    if (DBDGameInstance != nullptr)
    {
        ADBDPlayerController* LocalPlayerController = DBDGameInstance->GetLocalPlayerController();
        ADBDPlayerState* LocalPlayerState = DBDGameInstance->GetLocalPlayerState();

        // Check if the player is in a state where they need to gracefully leave the current game first
        if (LocalPlayerController != nullptr && IsValid(LocalPlayerState) && LocalPlayerState->IsActorBeingDestroyed() == false)
        {
            if (LocalPlayerState->_inParadise == false && 
                LocalPlayerState->PlayerData._playerGameState != EGameState::VE_Disconnected && 
                LocalPlayerState->PlayerData._playerGameState != EGameState::VE_ManuallyLeftMatch)
            {
                // Manage the player disconnected delegate before leaving
                if (this->OnPlayerDisconnectedHandle.IsValid() == true)
                {
                    LocalPlayerState->_onPlayerDisconnectedDelegate.Remove(this->OnPlayerDisconnectedHandle);
                    this->OnPlayerDisconnectedHandle.Reset();
                }

                this->OnPlayerDisconnectedHandle = LocalPlayerState->_onPlayerDisconnectedDelegate.Add(this->_onPlayerDisconnectedDelegate);
                
                LocalPlayerController->Server_LeaveGame();
                return; // Exit and wait for the leave process to complete
            }
        }

        // If not in a game or already handled, shut down the net driver
        UWorld* World = DBDGameInstance->GetWorld();
        if (GEngine != nullptr)
        {
            GEngine->ShutdownWorldNetDriver(World);
        }
    }

    // Access the Matchmaking Presence Subsystem
    TSharedRef<IMatchmakingPresenceSubsystem> Matchmaking = IOnlinePresencePlugin::Matchmaking();

    // Check session states to see if we need to End an existing session before joining a new one
    if (Matchmaking->GetSessionState(PartySessionName) == EOnlineSessionState::Ended || Matchmaking->GetSessionState(PartySessionName) == EOnlineSessionState::NoSession)
    {
        if (Matchmaking->GetSessionState(GameSessionName) == EOnlineSessionState::Ended || Matchmaking->GetSessionState(GameSessionName) == EOnlineSessionState::NoSession)
        {
            // Standard Join Logic
            ULocalPlayer* LocalPlayer = DBDGameInstance->GetLocalPlayerByIndex(0);
            UOnlineSystemHandler* OnlineSystemHandler = DBDGameInstance->GetOnlineSystemHandler();
            
            // Clear current search params/SessionId
            OnlineSystemHandler->_searchParams.SessionId.Empty();

            // Swap out the default Join delegate for the DBD specific one
            if (this->OnJoinSessionCompleteDelegateHandle.IsValid() == true)
            {
                Matchmaking->ClearOnJoinSessionCompleteDelegate_Handle(this->OnJoinSessionCompleteDelegateHandle);
                this->OnJoinSessionCompleteDelegateHandle.Reset();
            }

            this->DBDOnJoinSessionCompleteDelegateHandle = Matchmaking->AddOnJoinSessionCompleteDelegate_Handle(this->DBDOnJoinSessionCompleteDelegate);

            // Attempt to join via GameInstance
            if (DBDGameInstance->JoinSession(LocalPlayer, SearchResult) == false)
            {
                // If immediate failure, execute delegate with failure result (0x7 = Pending | Failure | 0x4)
                this->DBDOnJoinSessionCompleteDelegate.ExecuteIfBound(sessionToUse, EOnJoinSessionCompleteResult::UnknownError);
            }
        }
        else
        {
            // Need to end GameSession first
            if (this->OnEndBeforeJoinSessionCompleteDelegateHandle.IsValid() == true)
            {
                Matchmaking->ClearOnEndSessionCompleteDelegate_Handle(this->OnEndBeforeJoinSessionCompleteDelegateHandle);
                this->OnEndBeforeJoinSessionCompleteDelegateHandle.Reset();
            }

            this->OnEndBeforeJoinSessionCompleteDelegateHandle = Matchmaking->AddOnEndSessionCompleteDelegate_Handle(this->OnEndBeforeJoinSessionCompleteDelegate);
            Matchmaking->EndSession(GameSessionName);
        }
    }
    else
    {
        // Need to end PartySession first
        if (this->OnEndBeforeJoinSessionCompleteDelegateHandle.IsValid() == true)
        {
            Matchmaking->ClearOnEndSessionCompleteDelegate_Handle(this->OnEndBeforeJoinSessionCompleteDelegateHandle);
            this->OnEndBeforeJoinSessionCompleteDelegateHandle.Reset();
        }

        this->OnEndBeforeJoinSessionCompleteDelegateHandle = Matchmaking->AddOnEndSessionCompleteDelegate_Handle(this->OnEndBeforeJoinSessionCompleteDelegate);
        Matchmaking->EndSession(PartySessionName);
    }
}

// starts at line 120
void UDBDOnlineSessionClient::DBD_OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    // Log the result of the join session attempt
    UE_LOG(LogOnline, Log, TEXT("OnJoinSessionComplete %s bSuccess: %d"), *SessionName.ToString(), Result);

    // Get the Matchmaking Presence Subsystem and clear the join delegate
    TSharedRef<IMatchmakingPresenceSubsystem> Matchmaking = IOnlinePresencePlugin::Matchmaking();
    Matchmaking->ClearOnJoinSessionCompleteDelegate_Handle(this->DBDOnJoinSessionCompleteDelegateHandle);
    this->DBDOnJoinSessionCompleteDelegateHandle.Reset();

    // If the join was successful, initiate travel to the session
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (DBDGameInstance != nullptr)
        {
            UOnlineSystemHandler* OnlineSystemHandler = DBDGameInstance->GetOnlineSystemHandler();
            if (IsValid(OnlineSystemHandler))
            {
                OnlineSystemHandler->TravelToSession(SessionName);
            }
        }
    }

    // Reset the from invite flag
    this->bIsFromInvite = false;
}

// starts at line 140
void UDBDOnlineSessionClient::OnSetPlayerAsDisconnectedComplete()
{
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->OuterPrivate);
    
    // IsValid checks both for nullptr and !IsPendingKill() (flag 0x20000000)
    if (IsValid(DBDGameInstance))
    {
        ADBDPlayerState* LocalPlayerState = DBDGameInstance->GetLocalPlayerState();
        
        // Additionally check if the actor is not being destroyed (bitfield offset 0x140 & 4)
        if (IsValid(LocalPlayerState) && LocalPlayerState->IsActorBeingDestroyed() == false)
        {
            // Remove the disconnected delegate handler if it's currently bound
            if (this->OnPlayerDisconnectedHandle.IsValid() == true)
            {
                LocalPlayerState->_onPlayerDisconnectedDelegate.Remove(this->OnPlayerDisconnectedHandle);
                this->OnPlayerDisconnectedHandle.Reset();
            }
        }
        
        // Execute the join session using cached data
        this->JoinSession(GameSessionName, this->CachedSessionResult);
    }
}

// starts at line 156
void UDBDOnlineSessionClient::OnEndBeforeJoinSessionComplete(FName SessionName, bool bWasSuccessful)
{
    // Access the Matchmaking Presence Subsystem
    TSharedRef<IMatchmakingPresenceSubsystem> Matchmaking = IOnlinePresencePlugin::Matchmaking();

    // Clear the OnEnd delegate handler as it has just been executed
    Matchmaking->ClearOnEndSessionCompleteDelegate_Handle(this->OnEndBeforeJoinSessionCompleteDelegateHandle);
    
    // Clear the existing OnDestroy delegate handler if it's currently bound
    if (this->OnDestroyBeforeJoinSessionCompleteDelegateHandle.IsValid() == true)
    {
        Matchmaking->ClearOnDestroySessionCompleteDelegate_Handle(this->OnDestroyBeforeJoinSessionCompleteDelegateHandle);
        this->OnDestroyBeforeJoinSessionCompleteDelegateHandle.Reset();
    }

    // Bind the OnDestroy delegate to wait for the session destruction to finish
    this->OnDestroyBeforeJoinSessionCompleteDelegateHandle = Matchmaking->AddOnDestroySessionCompleteDelegate_Handle(this->OnDestroyBeforeJoinSessionCompleteDelegate);

    // Get the outer game instance to process the session destruction
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->OuterPrivate);
    
    // Ensure the game instance is valid and not pending destruction
    if (IsValid(DBDGameInstance))
    {
        // End the current authentication session
        DBDGameInstance->EndAuthentication();

        // Get the online system handler and instruct it to destroy the existing session
        UOnlineSystemHandler* OnlineSystemHandler = DBDGameInstance->GetOnlineSystemHandler();
        
        // Destroy the existing session. Note: the `setBusy` parameter is passed as false (0)
        OnlineSystemHandler->DestroyExistingSession(SessionName, false);
    }
}

// starts at line 177
void UDBDOnlineSessionClient::OnDestroyBeforeJoinSessionComplete(FName SessionName, bool bWasSuccessful)
{
    // Load the "OnlinePresence" module and check if it's available
    IOnlinePresencePlugin& PresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>("OnlinePresence");
    
    // Virtual function index 0x88 (decimal 136) corresponds to IsInitialized() in the module interface
    if (PresenceModule.IsInitialized() == true) 
    {
        // Access the Matchmaking Presence Subsystem
        TSharedRef<IMatchmakingPresenceSubsystem> Matchmaking = IOnlinePresencePlugin::Matchmaking();
        
        // Clear the OnDestroy delegate handler as it has just been executed
        Matchmaking->ClearOnDestroySessionCompleteDelegate_Handle(this->OnDestroyBeforeJoinSessionCompleteDelegateHandle);
    }
    
    // Continue with the join process using the cached result
    this->JoinSession(SessionName, this->CachedSessionResult);
}

// starts at line 188
void UDBDOnlineSessionClient::OnSessionUserInviteAccepted(bool bWasSuccess, int32 ControllerId, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& InviteResult)
{
    // Check if the client is currently allowed to accept invites
    if (this->_allowAcceptedInvites)
    {
        int32 hostVersion = -1;
        int32 gameTypeVal = -1;

        // Try to retrieve the VERSION_MATCH_KEY from the invite's session settings
        if (InviteResult.Session.SessionSettings.Get<int32>(VERSION_MATCH_KEY, hostVersion))
        {
            // Get the local build version hash
            int32 localVersion = GetDBDVersionNumberAsHash();
            bool bVersionsMatch = (hostVersion == localVersion);

            // Try to retrieve the GAMETYPE from the invite's session settings
            if (InviteResult.Session.SessionSettings.Get<int32>("GAMETYPE", gameTypeVal))
            {
                // Ensure a valid game type (fallback to 3 if negative/invalid)
                uint8 finalGameType = 3;
                if (gameTypeVal >= 0)
                {
                    finalGameType = (uint8)gameTypeVal;
                }

                // If versions match and it is a valid game type, proceed with the join
                if (bVersionsMatch && finalGameType != 0)
                {
                    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->OuterPrivate);
                    if (IsValid(DBDGameInstance))
                    {
                        UOnlineSystemHandler* OnlineSystemHandler = DBDGameInstance->GetOnlineSystemHandler();
                        if (IsValid(OnlineSystemHandler))
                        {
                            OnlineSystemHandler->SetJoinParamsFromSession(InviteResult);
                        }
                    }

                    // Call the base class implementation (pass UserId by value implicitly since it's a TSharedPtr)
                    Super::OnSessionUserInviteAccepted(bWasSuccess, ControllerId, UserId, InviteResult);
                    
                    // Broadcast the internal multicast delegate for systems listening to invite joins
                    this->OnJoinFromInvite.Broadcast(bWasSuccess, ControllerId, UserId, InviteResult);
                }
                else 
                {
                    // Versions mismatched or invalid game type, log the failure
                    UE_LOG(LogGameFlow, Log, TEXT("Trying to join a friend with a different version of the game."));
                }
            }
        }
    }
}

// starts at line 294
void UDBDOnlineSessionClient::OnPlayTogetherEventReceived(int32 UserIndex, TArray<TSharedPtr<const FUniqueNetId>> UserIdList)
{
    // Set the flag indicating a Play Together notification has been received
    this->_hasPlayTogetherNotification = true;

    // Store the received user index and invite list internally
    this->_playTogetherUserIndex = UserIndex;
    this->_playTogetherInviteList = UserIdList;

    // Create a local copy of the UserIdList to broadcast
    // The disassembly shows a manual copy loop with ref-count increments, 
    // which is the standard behavior of the TArray copy constructor when it contains TSharedPtrs.
    TArray<TSharedPtr<const FUniqueNetId>> BroadcastList = UserIdList;

    // Broadcast the Play Together host request to any listeners
    this->OnPlayTogetherHostRequest.Broadcast(UserIndex, BroadcastList);
}

// starts at line 317
void UDBDOnlineSessionClient::SetAllowInvites(bool invitesAllowed)
{
    // Log the current status of invites being allowed or disallowed
    UE_LOG(LogGameFlow, Log, TEXT("SetAllowInvites: %s"), invitesAllowed ? TEXT("true") : TEXT("false"));

    // Set the internal flag controlling whether invites can be accepted
    this->_allowAcceptedInvites = invitesAllowed;
}

// starts at line 323
bool UDBDOnlineSessionClient::CheckAndConsumePlayTogetherNotification()
{
    // Store the current state of the play together notification flag
    bool bHadNotification = this->_hasPlayTogetherNotification;

    // Consume the notification by resetting the flag to false
    this->_hasPlayTogetherNotification = false;

    // Return whether a notification was present before consumption
    return bHadNotification;
}
