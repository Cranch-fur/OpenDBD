bool UDBDAuthentication::AuthenticateRemoteTicket(const FString* authServerTicket, uint64_t steamID, uint32_t ticketLength, ADBDPlayerControllerBase* player)
{
    // Retrieve the Matchmaking presence subsystem
    /* UNDEFINED ELEMENT */
    TSharedRef<IMatchmakingPresenceSubsystem> matchmakingPresenceRef = IOnlinePresencePlugin::Get().Matchmaking();
    IMatchmakingPresenceSubsystem* presenceSubsystem = matchmakingPresenceRef->Object;

    // Create the delegate for the remote authentication callback
    // The disassembly shows TBaseUObjectMethodDelegateInstance allocation, which is Unreal's internal delegate binding
    /* UNDEFINED ELEMENT */
    FOnPresenceRemoteAuthDelegate authDelegate = FOnPresenceRemoteAuthDelegate::CreateUObject(this, &UDBDAuthentication::OnPresenceRemoteAuthDelegate);

    TSharedPtr<FPresenceAuthTicket> remoteTicket;

    // Call RegisterRemoteAuth on the subsystem. The disassembly shows a direct VTable offset call (rsi->vtable + 0xe8)
    /* UNDEFINED VTABLE */
    presenceSubsystem->RegisterRemoteAuth(&remoteTicket, authServerTicket, steamID, authDelegate);

    // Verify if the ticket was successfully returned/allocated
    if (remoteTicket.IsValid() == false)
    {
        // Log the failure if the verbosity level allows it (Verbosity >= 3 represents Warning/Log)
        if (LogDBDAuthentication.Verbosity >= 3)
        {
            UE_LOG(LogDBDAuthentication, Warning, TEXT("Failed to acquire Steam auth session ticket for local user"));
        }

        // Kick or disconnect the player due to authentication failure
        this->KickOrDisconnectPlayer(steamID, 0xFFFFFFFF, player);

        return false;
    }

    // Initialize the remote authentication info structure
    FAuthenticationInfo remoteInfo;
    remoteInfo.Player = player; // Assigned as FWeakObjectPtr internally
    remoteInfo.Ticket = remoteTicket;

    // Add the new remote info to the internal tracking array
    this->_remoteInfos.Add(remoteInfo);

    // Setup attributes for analytics
    TArray<FAnalyticsEventAttribute> analyticsAttributes;

    // The disassembly converts standard strings to Unreal wide strings for analytics
    FString authResultString = TEXT("");
    analyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("dbd_authentication_result_i"), authResultString));

    // Convert SteamID to FString
    FString steamIDStr = FString::Printf(TEXT("%llu"), steamID);
    analyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("dbd_authentication_steamid_sz"), steamIDStr));

    // Record the immediate authentication event via BHVR Analytics
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::RecordEvent(TEXT("AuthenticationImmediate"), analyticsAttributes);

    // Verify game instance validity
    if (this->_gameInstance != nullptr)
    {
        // Disassembly manually checks GUObjectArray flags (0x20000000) for unreachability/PendingKill. 
        // In UE4 C++, this is equivalent to IsPendingKill().
        if (this->_gameInstance->IsPendingKill() == false)
        {
            UWorld* world = this->_gameInstance->GetWorld();

            if (world != nullptr)
            {
                if (world->IsPendingKill() == false)
                {
                    // Ensure the current instance is not the server
                    if (world->IsServer() == false)
                    {
                        if (player != nullptr)
                        {
                            if (player->IsPendingKill() == false)
                            {
                                // The player is valid, create a local ticket and send it
                                const FAuthenticationInfo* localTicketInfo = this->CreateLocalTicket();

                                if (localTicketInfo != nullptr)
                                {
                                    player->SendAuthTicket(0, localTicketInfo);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}




void UDBDAuthentication::CancelTickets()
{
    // Retrieve the Matchmaking presence subsystem reference
    // IOnlinePresencePlugin is likely a custom or specific plugin interface, not standard UE4
    /* UNDEFINED ELEMENT */
    TSharedRef<IMatchmakingPresenceSubsystem> matchmakingPresenceRef = IOnlinePresencePlugin::Get().Matchmaking();

    // Extract the raw pointer from the shared reference
    IMatchmakingPresenceSubsystem* presenceSubsystem = matchmakingPresenceRef->Object; // Alternatively: matchmakingPresenceRef.Get()

    // Call CancelAllAuthTickets on the subsystem. 
    // The disassembly shows a direct VTable offset call (rax+0xf8)
    /* UNDEFINED VTABLE */
    presenceSubsystem->CancelAllAuthTickets();

    // NOTE: The extensive reference counting logic shown in the disassembly and pseudocode 
    // (SharedReferenceCount--, WeakReferenceCount--, DestroyObject) is handled automatically 
    // by Unreal Engine's TSharedRef destructor when 'matchmakingPresenceRef' goes out of scope.
}




FAuthenticationInfo* UDBDAuthentication::CreateLocalTicket(FAuthenticationInfo* result)
{
    // Retrieve the Matchmaking presence subsystem
    /* UNDEFINED ELEMENT */
    TSharedRef<IMatchmakingPresenceSubsystem> matchmakingPresenceRef = IOnlinePresencePlugin::Get().Matchmaking();
    IMatchmakingPresenceSubsystem* presenceSubsystem = matchmakingPresenceRef->Object;

    // Create the delegate for the local authentication callback
    // The disassembly shows TBaseUObjectMethodDelegateInstance allocation, mapped to standard UE delegate binding
    /* UNDEFINED ELEMENT */
    FOnPresenceLocalAuthDelegate authDelegate = FOnPresenceLocalAuthDelegate::CreateUObject(this, &UDBDAuthentication::OnPresenceLocalAuthDelegate);

    TSharedPtr<FPresenceAuthTicket> localTicket;

    // Call GenerateAuthTicket on the subsystem. The disassembly shows a direct VTable offset call (rax+0xe0).
    /* UNDEFINED VTABLE */
    presenceSubsystem->GenerateAuthTicket(&localTicket, authDelegate);

    // Verify if the ticket was successfully generated
    if (localTicket.IsValid())
    {
        // Add the new local ticket to the internal tracking array
        FAuthenticationInfo newLocalInfo;
        newLocalInfo.Ticket = localTicket;

        // FWeakObjectPtr default initialization handles the ObjectIndex/SerialNumber (-1 / 0) logic
        newLocalInfo.Player.Reset();

        this->_localInfos.Add(newLocalInfo);

        // Populate the return result
        result->Ticket = localTicket;
        result->Player.Reset();
    }
    else
    {
        // Log the failure if the verbosity level allows it (Verbosity >= 3 represents Warning/Log)
        if (LogDBDAuthentication.Verbosity >= 3)
        {
            UE_LOG(LogDBDAuthentication, Warning, TEXT("Failed to acquire Steam auth session ticket for local user"));
        }

        // Populate the return result with null/empty data upon failure
        result->Ticket = nullptr;
        result->Player.Reset();
    }

    // Note: Local scope TSharedPtr and TSharedRef objects (like matchmakingPresenceRef and localTicket) 
    // will automatically handle reference count decrements here.
    return result;
}




void UDBDAuthentication::Initialize(UDBDGameInstance* gameInstance)
{
    // Store the provided game instance reference into the class member variable
    this->_gameInstance = gameInstance;
}




void UDBDAuthentication::KickOrDisconnectPlayer(uint64_t steamID, uint32_t error, ADBDPlayerControllerBase* player)
{
    // Ensure the game instance is valid and not pending kill
    if (this->_gameInstance != nullptr && this->_gameInstance->IsPendingKill() == false)
    {
        UWorld* world = this->_gameInstance->GetWorld();

        if (world != nullptr && world->IsPendingKill() == false)
        {
            // Check if we are currently running as the server
            if (world->IsServer())
            {
                // Verbose logging (Verbosity >= 6 indicates Verbose/VeryVerbose)
                if (LogDBDAuthentication.Verbosity >= 6)
                {
                    UE_LOG(LogDBDAuthentication, Verbose, TEXT("Kicking player out of lobby due to non-conforming player %llu"), steamID);
                }

                // Disassembly path follows checking AuthorityGameMode, GameSession, and PlayerController validity
                AGameMode* authGameMode = world->AuthorityGameMode;
                if (authGameMode != nullptr && authGameMode->IsPendingKill() == false)
                {
                    AGameSession* gameSession = authGameMode->GameSession;
                    if (gameSession != nullptr && gameSession->IsPendingKill() == false)
                    {
                        if (player != nullptr && player->IsPendingKill() == false)
                        {
                            // Prepare the kick reason text
                            FString kickReasonStr = TEXT("SteamAuthFailureKickedFromServer.");
                            FText kickReasonText = FText::FromString(kickReasonStr);

                            // The disassembly shows a deeply nested pointer dereference ending in a call at offset 0x620 (1568 in decimal)
                            // This maps to AGameSession::KickPlayer(APlayerController*, FText)
                            /* UNDEFINED ELEMENT */
                            gameSession->KickPlayer(player, kickReasonText);
                        }
                    }
                }
            }
            else
            {
                // Client-side disconnection handling
                if (LogDBDAuthentication.Verbosity >= 6)
                {
                    UE_LOG(LogDBDAuthentication, Verbose, TEXT("Disconnecting from server due to non-conforming player %llu"), steamID);
                }

                if (this->_gameInstance != nullptr && this->_gameInstance->IsPendingKill() == false)
                {
                    // Create an empty FString for additional info
                    FString additionalInfo = TEXT("");

                    // Register the disconnect error with the game instance
                    // The '1' passed as the second argument (dl/rdx) corresponds to EDisconnectErrors enum value
                    // The final '1' (byte on the stack) likely corresponds to a boolean flag (e.g., bShowPopup)
                    /* UNDEFINED ELEMENT */
                    this->_gameInstance->RegisterDisconnectError(EDisconnectErrors::SteamAuthFailure, error, additionalInfo, true);
                }
            }
        }
    }
}




void UDBDAuthentication::OnPresenceLocalAuthDelegate(bool IsValid, TSharedPtr<FPresenceAuthTicket> Ticket, uint32_t ErrorCode)
{
    // Check if the authentication is NOT valid
    if (IsValid == false)
    {
        // If there is no specific error code
        if (ErrorCode == 0)
        {
            // Verbosity level 6 corresponds to ELogVerbosity::Verbose / VeryVerbose
            if (LogDBDAuthentication.Verbosity >= 6)
            {
                UE_LOG(LogDBDAuthentication, Verbose, TEXT("Disconnecting local user: %u"), 0);
            }
        }
        else
        {
            // Verbosity level 3 corresponds to ELogVerbosity::Warning
            if (LogDBDAuthentication.Verbosity >= 3)
            {
                UE_LOG(LogDBDAuthentication, Warning, TEXT("Error for local user: %u"), ErrorCode);
            }
        }

        // Iterate through the local infos array to find and remove the matching ticket
        if (this->_localInfos.Num() > 0)
        {
            for (int32 i = 0; i < this->_localInfos.Num(); i++)
            {
                // Compare the raw object pointers of the shared pointers
                if (this->_localInfos[i].Ticket.Get() == Ticket.Get())
                {
                    // The disassembly details manual destructor calls, memmove (memory shift), 
                    // ArrayNum decrement, and ResizeShrink. In Unreal Engine C++, all of this 
                    // is handled automatically by TArray::RemoveAt()
                    this->_localInfos.RemoveAt(i);
                    break;
                }
            }
        }
    }
    else
    {
        // Authentication is valid
        if (LogDBDAuthentication.Verbosity >= 6)
        {
            UE_LOG(LogDBDAuthentication, Verbose, TEXT("Confirming local user"));
        }
    }

    // NOTE: The end of the disassembly/pseudocode contains a large block of logic for decrementing 
    // the SharedReferenceCount and WeakReferenceCount of the Ticket, and potentially calling DestroyObject.
    // In standard C++, this is handled automatically by the TSharedPtr destructor when the 'Ticket' 
    // parameter goes out of scope, so it does not need to be manually written.
}




void UDBDAuthentication::OnPresenceRemoteAuthDelegate(bool IsValid, TSharedPtr<FPresenceAuthTicket> Ticket, uint32_t ErrorCode)
{
    // Check if the authentication is NOT valid
    if (IsValid == false)
    {
        // Check if there is a specific error code
        if (ErrorCode != 0)
        {
            // Ensure the ticket is valid before accessing its members, and check verbosity (3 = Warning)
            if (Ticket.IsValid() == true && LogDBDAuthentication.Verbosity >= 3)
            {
                UE_LOG(LogDBDAuthentication, Warning, TEXT("Error for remote user %llu: %u"), Ticket->UserId, ErrorCode);
            }
        }
        else
        {
            // Verbosity level 6 corresponds to ELogVerbosity::Verbose / VeryVerbose
            if (LogDBDAuthentication.Verbosity >= 6)
            {
                // Note: The disassembly explicitly uses the string "Disconnecting local user" here despite it being a remote auth delegate
                UE_LOG(LogDBDAuthentication, Verbose, TEXT("Disconnecting local user %llu: %u"), Ticket->UserId, 0);
            }
        }

        // Iterate through the remote infos array to find and remove the matching ticket
        if (this->_remoteInfos.Num() > 0)
        {
            for (int32 i = 0; i < this->_remoteInfos.Num(); i++)
            {
                // Compare the raw object pointers of the shared pointers
                if (this->_remoteInfos[i].Ticket.Get() == Ticket.Get())
                {
                    // If an error occurred, retrieve the associated player and kick/disconnect them
                    if (ErrorCode != 0)
                    {
                        // Resolve the weak object pointer to the player controller
                        ADBDPlayerControllerBase* player = this->_remoteInfos[i].Player.Get();

                        this->KickOrDisconnectPlayer(Ticket->UserId, ErrorCode, player);
                    }

                    // The disassembly details manual destructor calls, memmove (memory shift), 
                    // ArrayNum decrement, and ResizeShrink. In Unreal Engine C++, all of this 
                    // is handled automatically by TArray::RemoveAt()
                    this->_remoteInfos.RemoveAt(i);
                    break;
                }
            }
        }
    }
    else
    {
        // Authentication is valid
        if (LogDBDAuthentication.Verbosity >= 6)
        {
            UE_LOG(LogDBDAuthentication, Verbose, TEXT("Confirming remote user %llu"), Ticket->UserId);
        }
    }

    // NOTE: The end of the disassembly/pseudocode contains a large block of logic for decrementing 
    // the SharedReferenceCount and WeakReferenceCount of the Ticket, and potentially calling DestroyObject.
    // In standard C++, this is handled automatically by the TSharedPtr destructor when the 'Ticket' 
    // parameter goes out of scope, so it does not need to be manually written.
}