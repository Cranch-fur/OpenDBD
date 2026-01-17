void UDBDEasyAntiCheat::Authority_HandleMessageFromClient(class ADBDPlayerControllerBase* player, const TArray<unsigned char, FDefaultAllocator>* message, const uint32 messageLength)
{
    // Check if the EasyAntiCheat server instance is valid
    if (this->_server)
    {
        // Check log verbosity for "VeryVerbose" (Level 5 in disassembly)
        // Disassembly: 140246c1c cmp byte [rel LogDBDEasyAntiCheat], 0x5
        if (UE_LOG_ACTIVE(LogDBDEasyAntiCheat, VeryVerbose))
        {
            // Log the incoming message length
            // Disassembly calls FMsg::Logf_Internal__VA directly
            UE_LOG(LogDBDEasyAntiCheat, VeryVerbose, TEXT("Authority_HandleMessageFromClient: length %d"), messageLength);
        }

        // Retrieve the Client ID associated with the player controller
        // Disassembly: 140246c63 call UDBDEasyAntiCheat::GetClientIDFromPlayer
        uint64 ClientID = this->GetClientIDFromPlayer(player);

        // Validate that we received a valid Client ID (non-zero)
        // Disassembly: 140246c6b test rax, rax / je ...
        if (ClientID != 0)
        {
            // Check log verbosity again before the second log
            if (UE_LOG_ACTIVE(LogDBDEasyAntiCheat, VeryVerbose))
            {
                // Log that we are pushing the message to the server
                UE_LOG(LogDBDEasyAntiCheat, VeryVerbose, TEXT("Authority_HandleMessageFromClient: pushing to server for client id %d"), ClientID);
            }

            // Allocate a local buffer on the stack to hold the message data
            // Disassembly: 140246be9 sub rsp, 0x150 (allocates stack frame)
            // IDA analysis identifies this as a 256-byte buffer (szMessage)
            uint8 LocalMessageBuffer[256];

            // Only proceed with copying if messageLength is greater than 0
            if (messageLength != 0)
            {
                // Retrieve the raw data pointer from the TArray
                // Disassembly accesses message->AllocatorInstance.Data
                const uint8* SourceData = message->GetData();

                // Loop to copy data from TArray to the local stack buffer
                // Disassembly: 140246cd0 - 140246ce4 (Loop)
                for (uint32 Index = 0; Index < messageLength; Index = Index + 1)
                {
                    // WARNING: Disassembly shows no bounds check against the 256-byte stack buffer size here.
                    // A messageLength > 256 would cause a stack overflow. 
                    // Logic preserved from disassembly for fidelity.
                    LocalMessageBuffer[Index] = SourceData[Index];
                }
            }

            // Push the message to the EasyAntiCheat server instance via Virtual Function
            // Disassembly: 140246cfb call qword [rax+0x40]
            // Offset 0x40 (64 decimal) corresponds to the 8th function in the VTable (assuming 8-byte pointers).
            // Context identifies this as IGameServer::PushNetworkMessage.
            this->_server->PushNetworkMessage(ClientID, LocalMessageBuffer, messageLength);
        }
    }
}




void UDBDEasyAntiCheat::Authority_RegisterClient(class ADBDPlayerControllerBase* player, FString szPlayerGUID, FString szOwnerGUID, FString szPlayerName)
{
    // Check if the EasyAntiCheat server instance is valid
    if (this->_server)
    {
        // Convert FStrings (TCHAR) to ANSI (UTF-8) for the external library.
        // Disassembly performs manual stack allocation and conversion (140246d7e - 140246fc5).
        // This logic corresponds to the FTCHARToUTF8 helper class in UE4.

        FTCHARToUTF8 ConvertedPlayerName(*szPlayerName);
        FTCHARToUTF8 ConvertedOwnerGUID(*szOwnerGUID);
        FTCHARToUTF8 ConvertedPlayerGUID(*szPlayerGUID);

        // Call the registration function on the server interface.
        // Disassembly: 140246fff call qword [rax+0x10]
        // Offset 0x10 (16) corresponds to the 3rd virtual function (Index 2).
        // Arguments passed: Server, Player, PlayerGUID (char*), "0.0.0.0", OwnerGUID (char*), PlayerName (char*), 0.

        /* UNDEFINED VTABLE */
        // Calling function at VTable offset 0x10. Context implies RegisterClient.
        this->_server->RegisterClient(player, ConvertedPlayerGUID.Get(), "0.0.0.0", ConvertedOwnerGUID.Get(), ConvertedPlayerName.Get());

        // Check log verbosity for "VeryVerbose" (Level 5)
        // Disassembly: 140247042 cmp byte [rel LogDBDEasyAntiCheat], 0x5
        if (UE_LOG_ACTIVE(LogDBDEasyAntiCheat, VeryVerbose))
        {
            // Log the registration with the player pointer/ID
            // Disassembly uses %d with the pointer 'player' (r14)
            UE_LOG(LogDBDEasyAntiCheat, VeryVerbose, TEXT("Authority_RegisterClient: ID: %d"), player);
        }

        // Create a weak pointer to the player
        // Disassembly: 140247087 - 140247098
        FWeakObjectPtr WeakPlayerPtr;
        WeakPlayerPtr = player;

        // Add the player to the local client tracking array.
        // Disassembly: Accesses TArray at offset 0xE0 (1402470a9 - 1402470f0)
        // The logic performs a manual array ResizeGrow and assignment.
        // We assume 'ConnectedClients' is the TArray<FEACClientTracker> at offset 0xE0.

        FEACClientTracker NewClientEntry;
        NewClientEntry.Player = player;
        NewClientEntry.WeakPlayer = WeakPlayerPtr;

        this->ConnectedClients.Add(NewClientEntry);
    }

    // Note: FString destructors for szPlayerGUID, szOwnerGUID, and szPlayerName
    // are called automatically here by the C++ compiler (as seen in disassembly cleanup).
}




void UDBDEasyAntiCheat::Authority_UnregisterClient(class ADBDPlayerControllerBase* player)
{
    // Basic pointer validation
    // Disassembly: 140247380 test rdx, rdx
    if (player == nullptr)
    {
        return;
    }

    // Validate Object Flags (Check against GUObjectArray and internal flags like PendingKill)
    // Disassembly: 140247393 - 1402473f9 checks bits in GUObjectArray and player->0x140
    if (IsValid(player) == false)
    {
        return;
    }

    // Check if the EAC server instance is valid
    // Disassembly: 1402473ff cmp [rsi+0xb8], rbx
    if (this->_server == nullptr)
    {
        return;
    }

    // Check if there are any clients currently registered
    // Disassembly: 14024740c cmp [rsi+0xe8], ebx
    if (this->_clientInfos.Num() <= 0)
    {
        return;
    }

    // Iterate through the connected clients array to find the player
    // Disassembly: Loop starting at 140247430
    for (int32 Index = 0; Index < this->_clientInfos.Num(); Index = Index + 1)
    {
        // Retrieve the WeakObjectPtr at offset 0x8 of the struct and resolve it
        // Disassembly: 140247441 call FWeakObjectPtr::Get
        if (this->_clientInfos[Index].WeakPlayer.Get() == player)
        {
            // Check log verbosity (VeryVerbose)
            // Disassembly: 14024745a cmp byte [rel LogDBDEasyAntiCheat], 0x5
            if (UE_LOG_ACTIVE(LogDBDEasyAntiCheat, VeryVerbose))
            {
                // Log the unregistration event with the raw pointer address
                // Disassembly: 1402474a4 call FMsg::Logf_Internal__VA
                UE_LOG(LogDBDEasyAntiCheat, VeryVerbose, TEXT("Authority_UnregisterClient: ID: %p"), this->_clientInfos[Index].Player);
            }

            // Call UnregisterClient on the server interface
            // Disassembly: 1402474c0 call qword [rax+0x20]
            // Offset 0x20 (32) corresponds to the 4th virtual function.
            /* UNDEFINED VTABLE */
            this->_server->UnregisterClient(this->_clientInfos[Index].Player);

            // Remove the client from the tracking array
            // Disassembly: 1402474c3 - 1402474f4
            // The disassembly explicitly calculates the move size, calls memmove, and then ResizeShrink.
            // This is the implementation of TArray::RemoveAt.
            this->_clientInfos.RemoveAt(Index);

            // Exit the function after processing the removal
            return;
        }
    }
}




void UDBDEasyAntiCheat::CreateClient()
{
    // Load the EasyAntiCheat library plugin
    // 14024bd77: LoadLibraryA
    this->_hEACClientLibrary = LoadLibraryA(EASYANTICHEAT_CLIENT_PLUGIN);

    // Check if the library was loaded successfully
    if (this->_hEACClientLibrary == nullptr)
    {
        // 14024c13b: Log error if DLL not found
        // Corresponds to failure path at 14024c113
        UE_LOG(LogDBDEasyAntiCheat, Error, TEXT("failed to create game client, could not find dll"));
        return;
    }

    // Get the address of the "CreateGameClient" function from the library
    // 14024bd97: GetProcAddress
    void* CreateGameClientPtr = GetProcAddress((HMODULE)this->_hEACClientLibrary, "CreateGameClient");

    // Check if the function address was found
    if (CreateGameClientPtr == nullptr)
    {
        // 14024c0d4: Log error if function not found
        // Corresponds to failure path at 14024bda0
        UE_LOG(LogDBDEasyAntiCheat, Error, TEXT("failed to create game client, could not find function"));
        return;
    }

    // Call the function to create the client interface
    // 14024bdad: call rax
    // We cast the pointer to the appropriate function signature.
    typedef easyanticheat::client::hydra::IGameClient* (*CreateGameClientFunc)(const char*);
    CreateGameClientFunc CreateGameClient = (CreateGameClientFunc)CreateGameClientPtr;

    // Passing "GameClientInterfaceV009" as version string
    this->_client = CreateGameClient("GameClientInterfaceV009");

    // Check if the client was successfully created
    if (this->_client)
    {
        // 14024bdc4: Log success (Verbosity >= Log/Verbose)
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("client created"));

        // Initialize the client via its Virtual Table
        // 14024be45: call qword [rax] (Offset 0x0)
        // Arguments: 
        // 1. this->_client (RCX)
        // 2. InternalVTableHelperCtorCaller<class AAIController> (RDX) - Unknown specific helper
        // 3. UDBDEasyAntiCheat::Impl::OnInitFinished (R8) - Callback
        // 4. this (R9) - Context

        /* UNDEFINED VTABLE */
        this->_client->Initialize( /* UNDEFINED ELEMENT */ InternalVTableHelperCtorCaller<AAIController>, UDBDEasyAntiCheat::Impl::OnInitFinished, this);

        // Retrieve the Game Instance
        UDBDGameInstance* gameInstance = this->_gameInstance;

        // 14024be6c: Check if GameInstance is valid
        // The disassembly explicitly checks the GUObjectArray to ensure the object is not PendingKill or Unreachable.
        // In Unreal Engine C++, IsValid() performs these checks.
        if (gameInstance && IsValid(gameInstance))
        {
            // 14024becf: Access TimerManager from GameInstance
            /* UNREAL AUTO GENERATED FUNCTION */
            FTimerManager& TimerManager = gameInstance->GetTimerManager();

            // 14024bfdb: Set the authorization timer
            // The disassembly constructs the delegate manually, but functional C++ uses the SetTimer helper.
            // Binds UDBDEasyAntiCheat::TickClientAuthorization to this object.
            // Rate: 0.1f, Loop: true
            TimerManager.SetTimer(this->_clientAuthorizationTimerHandle, this, &UDBDEasyAntiCheat::TickClientAuthorization, 0.1f, true);
        }
    }
    else
    {
        // 14024bdf8: Handle client creation failure (Returned nullptr)

        // Log error
        UE_LOG(LogDBDEasyAntiCheat, Error, TEXT("failed to create game client, null pointer returned"));

        // Broadcast initialization completion with failure status (false/Unloaded)
        // 14024bdfe: Broadcast
        this->OnInitComplete.Broadcast(false);
    }
}




void UDBDEasyAntiCheat::CreateServer()
{
    // Load the EasyAntiCheat server library
    // 14024c867: LoadLibraryA
    this->_hEACServerLibrary = LoadLibraryA(EASYANTICHEAT_SERVER_PLUGIN);

    // Check if the library was loaded successfully
    if (this->_hEACServerLibrary == nullptr)
    {
        // 14024ccac: Log error if DLL not found
        // Corresponds to failure path at 14024cc7b
        UE_LOG(LogDBDEasyAntiCheat, Error, TEXT("Failed to create game server, could not find dll"));
        return;
    }

    // Get the address of the "CreateGameServer" function from the library
    // 14024c887: GetProcAddress
    void* CreateGameServerPtr = GetProcAddress((HMODULE)this->_hEACServerLibrary, "CreateGameServer");

    // Check if the function address was found
    if (CreateGameServerPtr == nullptr)
    {
        // 14024cc6d: Log error if function not found
        // Corresponds to failure path at 14024c890
        UE_LOG(LogDBDEasyAntiCheat, Error, TEXT("Failed to create game server, could not find function"));
        return;
    }

    // Cast the pointer to the appropriate function signature
    typedef easyanticheat::server::hydra::IGameServer* (*CreateGameServerFunc)(const char*);
    CreateGameServerFunc CreateGameServer = (CreateGameServerFunc)CreateGameServerPtr;

    // Call the function to create the server interface with version string "GameServerInterfaceV009"
    // 14024c89d: call rax
    this->_server = CreateGameServer("GameServerInterfaceV009");

    // Check if the server interface was successfully created
    if (this->_server)
    {
        // Set the log callback for the EAC server
        // 14024c90c: call qword [rax+0x48] (VTable Offset +0x48)
        /* UNDEFINED VTABLE */
        this->_server->SetLogCallback(UDBDEasyAntiCheat::Impl::OnEACLogMessage, 3);

        // Initialize the EAC server
        // 14024c923: call qword [rax] (VTable Offset +0x0)
        // Arguments: 60 (0x3c), "DBD EAC Server"
        /* UNDEFINED VTABLE */
        this->_server->Initialize(60, "DBD EAC Server");

        // Associate the server with the host game client
        // 14024c936: call qword [rax+0x58] (VTable Offset +0x58)
        /* UNDEFINED VTABLE */
        this->_server->SetServerHostGameClient(this->_client);

        // Log success
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("server created. Passed client %x"), this->_client);

        // Retrieve the GameInstance
        UDBDGameInstance* GameInstance = this->_gameInstance;

        // Check if GameInstance is valid
        // The disassembly explicitly checks the GUObjectArray to ensure the object is not PendingKill or Unreachable.
        if (GameInstance && IsValid(GameInstance))
        {
            // Log timer setup
            UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("setting timer"));

            // Get the TimerManager from the GameInstance
            // 14024ca41: Access TimerManager
            /* UNREAL AUTO GENERATED FUNCTION */
            FTimerManager& TimerManager = GameInstance->GetTimerManager();

            // Set the server authorization timer
            // Binds UDBDEasyAntiCheat::TickServerAuthorization to this instance
            // Interval: 0.1f, Looping: true
            TimerManager.SetTimer(this->_serverAuthorizationTimerHandle, this, &UDBDEasyAntiCheat::TickServerAuthorization, 0.1f, true);
        }
    }
    else
    {
        // 14024c8e8: Handle server creation failure (Returned nullptr)
        UE_LOG(LogDBDEasyAntiCheat, Error, TEXT("Failed to create game server, null pointer returned"));
    }
}




void UDBDEasyAntiCheat::DestroyClient()
{
    // 140251a96: Check if the EasyAntiCheat library is loaded
    if (this->_hEACClientLibrary == nullptr)
    {
        return;
    }

    // 140251aa7: Check if the client interface exists
    if (this->_client)
    {
        // Retrieve the GameInstance
        UDBDGameInstance* GameInstance = this->_gameInstance;

        // 140251abc: Check if the GameInstance is valid (not null and not pending kill via GUObjectArray)
        if (GameInstance && IsValid(GameInstance))
        {
            // 140251aea: Access the TimerManager from the GameInstance
            /* UNREAL AUTO GENERATED FUNCTION */
            FTimerManager& TimerManager = GameInstance->GetTimerManager();

            // 140251afd: Clear the client authorization timer
            // The disassembly calls InternalClearTimer and zeroes the handle; ClearTimer helper does both.
            TimerManager.ClearTimer(this->_clientAuthorizationTimerHandle);
        }

        // 140251b1c: Call the Destroy function on the client interface
        // Disassembly: call qword [rax+0x8] (VTable Offset +0x8)
        /* UNDEFINED VTABLE */
        this->_client->Destroy();

        // 140251b1f: Nullify the client pointer
        this->_client = nullptr;

        // 140251b31: Log the destruction if verbosity allows
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("client destroyed"));
    }

    // 140251b6e: Free the loaded library DLL
    FreeLibrary((HMODULE)this->_hEACClientLibrary);

    // 140251b74: Nullify the library handle
    this->_hEACClientLibrary = nullptr;
}




void UDBDEasyAntiCheat::DestroyServer()
{
    // 140251d46: Check if the EasyAntiCheat server library is loaded
    if (this->_hEACServerLibrary == nullptr)
    {
        return;
    }

    // 140251d57: Check if the server interface exists
    if (this->_server)
    {
        // Retrieve the GameInstance
        UDBDGameInstance* GameInstance = this->_gameInstance;

        // 140251d6c: Check if the GameInstance is valid (not null and not pending kill via GUObjectArray)
        if (GameInstance && IsValid(GameInstance))
        {
            // 140251da1: Access the TimerManager from the GameInstance
            /* UNREAL AUTO GENERATED FUNCTION */
            FTimerManager& TimerManager = GameInstance->GetTimerManager();

            // 140251dad: Clear the server authorization timer
            // The disassembly calls InternalClearTimer and zeroes the handle; ClearTimer helper does both.
            TimerManager.ClearTimer(this->_serverAuthorizationTimerHandle);
        }

        // 140251dcc: Call the Destroy function on the server interface
        // Disassembly: call qword [rax+0x8] (VTable Offset +0x8)
        /* UNDEFINED VTABLE */
        this->_server->Destroy();

        // 140251dcf: Nullify the server pointer
        this->_server = nullptr;

        // 140251dda: Log the destruction if verbosity allows
        // 0x5 corresponds to ELogVerbosity::Log or Verbose depending on specific engine configuration
        if (LogDBDEasyAntiCheat.Verbosity >= 5)
        {
            UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("server destroyed"));
        }
    }

    // 140251e1e: Free the loaded server library DLL
    FreeLibrary((HMODULE)this->_hEACServerLibrary);

    // 140251e24: Nullify the library handle
    this->_hEACServerLibrary = nullptr;
}




void UDBDEasyAntiCheat::DestroySession()
{
    // 140251e46: Check logging verbosity (0x5 corresponds to Log/Verbose)
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // 140251e81: Log execution
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("DestroySession"));
    }

    // 140251e86: Check if the server interface is initialized
    if (this->_server)
    {
        // 140251e97: Check if there are any registered clients in the array
        if (this->_clientInfos.Num() > 0)
        {
            // Iterate through the client info array
            for (int32 i = 0; i < this->_clientInfos.Num(); i++)
            {
                // 140251ec1: Access the client data from the array
                // The disassembly indicates a stride of 0x10 (16 bytes) per element.
                // The argument passed to UnregisterClient is the first 64-bit value of the element.
                auto& ClientData = this->_clientInfos[i];

                // 140251ec5: Call UnregisterClient on the server interface
                // Disassembly: call qword [rax+0x20] (VTable Offset +0x20)
                /* UNDEFINED VTABLE */
                this->_server->UnregisterClient(ClientData);
            }
        }

        // 140251eeb: Reset ArrayNum to 0
        // 140251ef6: Resize internal memory to 0 (effectively freeing memory)
        // In Unreal Engine, TArray::Empty() performs these operations.
        this->_clientInfos.Empty();
    }

    // 140251f03: Chain call to DestroyServer
    this->DestroyServer();
}




uint64_t UDBDEasyAntiCheat::GetClientIDFromPlayer(ADBDPlayerControllerBase* player)
{
    // Iterate through the stored client information array
    // 140255eb7: Check loop condition (i < ArrayNum)
    for (int32 i = 0; i < this->_clientInfos.Num(); i++)
    {
        // Access the array element. 
        // Based on the disassembly stride (shl rax, 0x4), the element size is 16 bytes.
        // Offset 0x00: uint64_t ClientID
        // Offset 0x08: FWeakObjectPtr Player

        // 140255ed5: Check if the stored weak pointer resolves to the provided player
        // The disassembly explicitly calls FWeakObjectPtr::Get() on offset 0x8 of the element
        if (this->_clientInfos[i].Player.Get() == player)
        {
            // 140255efb: Match found

            // Check logging verbosity
            if (LogDBDEasyAntiCheat.Verbosity >= 5)
            {
                // Log the ClientID. %p suggests it's treated as a pointer/handle for logging.
                UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("GetClientIDFromPlayer: %p"), this->_clientInfos[i].ClientId);
            }

            // Return the ClientID found at offset 0x00
            return this->_clientInfos[i].ClientId;
        }
    }

    // 140255ee9: Player not found in the list, return 0
    return 0;
}




ADBDPlayerControllerBase* UDBDEasyAntiCheat::GetPlayerFromClientID(uint64 clientObject)
{
    // Check logging verbosity (0x5 corresponds to ELogVerbosity::Verbose)
    // 14025caaa: cmp byte [rel LogDBDEasyAntiCheat], 0x5
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // 14025caed: Log function call with the client ID (treated as a pointer %p for display)
        UE_LOG(LogDBDEasyAntiCheat, Verbose, TEXT("GetPlayerFromClientID: %p"), clientObject);
    }

    // Iterate through the registered client info array
    // 14025caf2: Loop setup
    for (int32 i = 0; i < this->_clientInfos.Num(); ++i)
    {
        // Access array element (Stride 16 bytes: 0x00 = ID, 0x08 = WeakPtr)
        // 14025cb10: Compare stored ClientID with the requested clientObject
        if (this->_clientInfos[i].ClientId == clientObject)
        {
            // 14025cb48: Match found. Resolve the WeakObjectPtr and return the player controller.
            // The disassembly executes a tail call to FWeakObjectPtr::Get.
            return (ADBDPlayerControllerBase*)this->_clientInfos[i].Player.Get();
        }
    }

    // 14025cb2f: No match found
    return nullptr;
}




FString UDBDEasyAntiCheat::Impl::GetLogLevelString(easyanticheat::server::LogLevel Level)
{
    // 14025a8c9: Logic corresponds to a switch statement on the Level enum.
    // The disassembly performs sequential subtraction to check the value.

    // Check for Level 1 (Error)
    // 14025a8c9: sub edx, 0x1; je 0x14025a993
    if ((int32)Level == 1)
    {
        return TEXT("Error");
    }

    // Check for Level 2 (Warning)
    // 14025a8d2: sub edx, 0x1; je 0x14025a961
    if ((int32)Level == 2)
    {
        return TEXT("Warning");
    }

    // Check for Level 3 (Info)
    // 14025a8db: sub edx, 0x1; je 0x14025a92f
    if ((int32)Level == 3)
    {
        return TEXT("Info");
    }

    // Check for Level 4 (Debug)
    // 14025a8e5: cmp edx, 0x1; je 0x14025a917
    if ((int32)Level == 4)
    {
        return TEXT("Debug");
    }

    // Default case (Invalid)
    // 14025a8fc: Loads "<invalid>" string
    return TEXT("<invalid>");
}




bool UDBDEasyAntiCheat::Impl::OnClientIntegrityViolationFunc(easyanticheat::client::ClientIntegrityViolationType violationType, const char* violationCause, const char* message, void* callbackParameter)
{
    // 140268cb0: Register shuffling to prepare for member function call.
    // RCX (this) comes from R9 (callbackParameter)
    // RDX (Arg1) comes from ECX (violationType)
    // R8  (Arg2) comes from RDX (violationCause)
    // R9  (Arg3) comes from R8  (message)

    UDBDEasyAntiCheat* Self = static_cast<UDBDEasyAntiCheat*>(callbackParameter);

    // 140268cbe: Tail call optimization (JMP) to the member function
    return Self->OnClientIntegrityViolation(violationType, violationCause, message);
}




void UDBDEasyAntiCheat::Impl::OnEACLogMessage(easyanticheat::server::LogLevel Level, const char* Message)
{
    // 140268fed: Check logging verbosity (0x5 corresponds to Log/Verbose)
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // Convert the incoming C-style char* Message to an Unreal FString (TCHAR*)
        // The disassembly shows TStringConversion usage, likely via the UTF8_TO_TCHAR macro or similar helper.
        // 140269002-140269029: String conversion logic
        // If Message is null, the result is null.
        const TCHAR* LogMessage = Message ? UTF8_TO_TCHAR(Message) : nullptr;

        // Convert the numeric Log Level to a human-readable string
        // 140269049: Call GetLogLevelString
        FString LogLevelStr = UDBDEasyAntiCheat::Impl::GetLogLevelString(Level);

        // 140269098: Log the formatted message
        // Format: "OnEACLogMessage, [Level]: [Message]"
        // The disassembly checks if the string data is valid; if not, it likely falls back to empty string.
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("OnEACLogMessage, %s: %s"), *LogLevelStr, LogMessage);
    }
}




void UDBDEasyAntiCheat::Impl::OnInitFinished(easyanticheat::client::EACAsyncResult Result, const char* szMessage, void* CallbackParameter)
{
    // 14026b06c: Check logging verbosity (0x5 corresponds to Log/Verbose)
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // Convert C-string to TCHAR for logging
        const TCHAR* LogMessage = szMessage ? UTF8_TO_TCHAR(szMessage) : nullptr;
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("OnInitFinished %d: %s"), (int32)Result, LogMessage);
    }

    // 14026b120 - 14026b165: Convert szMessage to FString for Analytics
    FString Message = szMessage ? FString(UTF8_TO_TCHAR(szMessage)) : FString();

    // 14026b189: Create EventName string "Init"
    FString EventName = TEXT("Init");

    // 14026b1b3: Log the analytics event
    // Disassembly passes arguments in RCX, RDX, R8. No 'this' pointer observed, implies static function.
    // Signature likely: static void LogAnalyticsEvent(const FString& EventName, int32 Result, const FString& Message);
    UDBDEasyAntiCheat::LogAnalyticsEvent(EventName, (int32)Result, Message);

    // 14026b1e2: Broadcast the result via the delegate
    // CallbackParameter is the UDBDEasyAntiCheat instance pointer.
    UDBDEasyAntiCheat* Self = static_cast<UDBDEasyAntiCheat*>(CallbackParameter);

    if (Self)
    {
        // Determine success boolean. Disassembly checks if Result (EBP) is 0.
        // If Result == 0 (InitSuccess), Broadcast true (1). Otherwise false.
        bool bSuccess = (Result == easyanticheat::client::EACAsyncResult::InitSuccess);

        // Offset 0x30 corresponds to OnInitComplete based on context from CreateClient.
        Self->OnInitComplete.Broadcast(bSuccess);
    }
}




bool UDBDEasyAntiCheat::Impl::OnValidateServerFunc(easyanticheat::client::ServerHostValidationResult Result, const char* szMessage, void* CallbackParameter)
{
    // 14026d1f0: Register shuffling to prepare for member function call.
    // The CallbackParameter (void*) passed to the C-style API is actually the 'this' pointer for the UDBDEasyAntiCheat instance.
    // RCX (this) is loaded from R8 (CallbackParameter).
    // RDX (Arg1) is loaded from ECX (Result).
    // R8  (Arg2) is loaded from RDX (szMessage).

    UDBDEasyAntiCheat* Self = static_cast<UDBDEasyAntiCheat*>(CallbackParameter);

    // 14026d1fb: Tail call optimization (JMP) to the member function
    return Self->OnValidateServer(Result, szMessage);
}




void UDBDEasyAntiCheat::LogAnalyticsEvent(const FString& EventName, int32 Result, const FString& Message)
{
    // Initialize an array to store analytics attributes
    // 1402676b5: TArray initialization
    TArray<FAnalyticsEventAttribute> Attributes;

    // Add the validation result attribute
    // Key: "validation_result_i"
    // Value: Result (Integer)
    // 1402676f5: Call AddAttributes template for integer value
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::AddAttributes(Attributes, FString(TEXT("validation_result_i")), Result);

    // Add the message attribute
    // Key: "message_sz"
    // Value: Message (String)
    // 140267749: Call AddAttributes template for FString value
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::AddAttributes(Attributes, FString(TEXT("message_sz")), Message);

    // Format the full event name
    // Format: "Anti-Cheat [EventName]"
    // 140267779: FString::Printf
    FString FullEventName = FString::Printf(TEXT("Anti-Cheat %s"), *EventName);

    // Record the event with the analytics system
    // 140267786: Call RecordEvent
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::RecordEvent(FullEventName, Attributes);

    // 140267799 - 1402677d4: Cleanup logic
    // In C++, the destructors for 'Attributes' (and its contained FStrings) and 'FullEventName'
    // are called automatically when they go out of scope, matching the free loop in disassembly.
}




bool UDBDEasyAntiCheat::OnClientIntegrityViolation(easyanticheat::client::ClientIntegrityViolationType violationType, const char* violationCause, const char* message)
{
    // 1402687db: Check logging verbosity (0x5 corresponds to Log/Verbose)
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // Convert C-strings to TCHAR for logging
        const TCHAR* ViolationCauseStr = violationCause ? UTF8_TO_TCHAR(violationCause) : nullptr;
        const TCHAR* MessageStr = message ? UTF8_TO_TCHAR(message) : nullptr;

        // 1402688e8: Log the violation details
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("[%s] Type: %i, ViolationCause: %s, Message: %s"),
            TEXT("UDBDEasyAntiCheat::OnClientIntegrityViolation"),
            (int32)violationType,
            ViolationCauseStr,
            MessageStr);
    }

    // Prepare Analytics Event
    // 140268927: Initialize array/string structures for analytics

    // Add "violation_cause_sz" attribute
    // 14026897c: Call AddAttributes helper
    /* UNDEFINED ELEMENT */
    TArray<FAnalyticsEventAttribute> Attributes;
    UBHVRAnalytics::AddAttributes(Attributes, FString(TEXT("violation_cause_sz")), FString(violationCause ? UTF8_TO_TCHAR(violationCause) : TEXT("")));

    // Add "message_sz" attribute
    // 1402689d7: Call AddAttributes helper
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::AddAttributes(Attributes, FString(TEXT("message_sz")), FString(message ? UTF8_TO_TCHAR(message) : TEXT("")));

    // Construct Event Name: "Anti-Cheat ClientIntegrityViolation"
    // 1402689fe: FString::Printf
    FString EventName = FString::Printf(TEXT("Anti-Cheat %s"), TEXT("ClientIntegrityViolation"));

    // Record the event
    // 140268a0d: UBHVRAnalytics::RecordEvent
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::RecordEvent(EventName, Attributes);

    // Retrieve Game Instance
    UDBDGameInstance* GameInstance = this->_gameInstance;

    // 140268a6f: Check if GameInstance is valid (Pointer check + Object Array check for PendingKill)
    if (GameInstance && IsValid(GameInstance))
    {
        // 140268aa6: Check for specific violation type 12 (CorruptedNetworkMessageFlow)
        if (violationType == easyanticheat::client::ClientIntegrityViolationType::CorruptedNetworkMessageFlow) // 0xC
        {
            // Convert message to TCHAR Array for storage
            FString MsgFString = message ? UTF8_TO_TCHAR(message) : TEXT("");

            // 140268b02: Check GameFlow Log verbosity
            if (GameFlow.Verbosity >= 5)
            {
                // 140268b52: Log Error
                UE_LOG(GameFlow, Log, TEXT("Registering Steam Auth or EAC Error (Error Type: %i; Error Code: %i)"), 3, 12);
            }

            // Update GameInstance State
            // 140268b5c: Set Disconnected
            GameInstance->SetIsDisconnected(true);

            // 140268b68: Set Disconnect Error Type (3 = EACServerValidationFailure or similar internal code)
            GameInstance->DisconnectError = (EDisconnectError)3;

            // 140268b74: Set specific Error Code (12)
            GameInstance->LastAuthOrEACErrorCode = 12;

            // 140268b7e: Store additional info (the message)
            // The disassembly creates a TArray<uint16> (TCHAR) from the string and assigns it.
            GameInstance->AuthOrEACErrorAdditionalInfo = MsgFString.GetCharArray();
        }
        // 140268b99: Check for other non-zero violation types
        else if (violationType != easyanticheat::client::ClientIntegrityViolationType::None)
        {
            // Convert message to TCHAR Array for storage
            FString MsgFString = message ? UTF8_TO_TCHAR(message) : TEXT("");

            // 140268bf4: Check GameFlow Log verbosity
            if (GameFlow.Verbosity >= 5)
            {
                // 140268c41: Log Error
                UE_LOG(GameFlow, Log, TEXT("Registering Steam Auth or EAC Error (Error Type: %i; Error Code: %i)"), 6, (int32)violationType);
            }

            // Update GameInstance State
            // 140268c4b: Set Disconnected
            GameInstance->SetIsDisconnected(true);

            // 140268c57: Set Disconnect Error Type (6 = EACClientIntegrityViolation)
            GameInstance->DisconnectError = (EDisconnectError)6;

            // 140268c63: Set specific Error Code
            GameInstance->LastAuthOrEACErrorCode = (int32)violationType;

            // 140268c6a: Store additional info
            GameInstance->AuthOrEACErrorAdditionalInfo = MsgFString.GetCharArray();

            // 140268c84: Trigger disconnection handling immediately for general integrity violations
            // Note: This call is NOT present in the CorruptedNetworkMessageFlow branch in the disassembly.
            GameInstance->HandleDisconnectError();
        }
    }

    // 140268cad: Return true (1)
    return true;
}




bool UDBDEasyAntiCheat::OnValidateServer(easyanticheat::client::ServerHostValidationResult Result, const char* Message)
{
    // 14026cf27: Check logging verbosity (0x5 corresponds to Log/Verbose)
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // Convert C-string to TCHAR for logging
        const TCHAR* MsgPtr = Message ? UTF8_TO_TCHAR(Message) : nullptr;

        // 14026cfbe: Log the validation result
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("OnValidateServer: result = %d: %s"), (int32)Result, MsgPtr);
    }

    // Prepare Analytics Event
    // 14026cfee - 14026d024: Convert Message to FString
    FString MsgString = Message ? FString(UTF8_TO_TCHAR(Message)) : FString();

    // 14026d048: Create Event Name "ValidateServer"
    FString EventName = TEXT("ValidateServer");

    // 14026d073: Log the analytics event
    UDBDEasyAntiCheat::LogAnalyticsEvent(EventName, (int32)Result, MsgString);

    // Retrieve Game Instance
    UDBDGameInstance* GameInstance = this->_gameInstance;

    // 14026d09b: Check if GameInstance is valid
    if (GameInstance && IsValid(GameInstance))
    {
        // 14026d0df: Check if the validation failed. 
        // Checks if Result is non-zero (assuming 0 is Success).
        // The disassembly includes a check `Result != ServerHostAccountStatusOk`, implying specific status filtering.
        if (Result != easyanticheat::client::ServerHostValidationResult::ServerHostValidationResultSuccess)
        {
            // 14026d13a: Check GameFlow Log verbosity
            if (GameFlow.Verbosity >= 5)
            {
                // 14026d186: Log Error
                // Error Type 3 corresponds to EACServerValidationFailure
                UE_LOG(GameFlow, Log, TEXT("Registering Steam Auth or EAC Error (Error Type: %i; Error Code: %i)"), 3, (int32)Result);
            }

            // Update GameInstance State
            // 14026d190: Set Disconnected
            GameInstance->SetIsDisconnected(true);

            // 14026d19c: Set Disconnect Error Type (3 = EACServerValidationFailure)
            GameInstance->DisconnectError = EDisconnectErrors::EACServerValidationFailure;

            // 14026d1a8: Set specific Error Code
            GameInstance->LastAuthOrEACErrorCode = (int32)Result;

            // 14026d1af: Store additional info (the message)
            GameInstance->AuthOrEACErrorAdditionalInfo = MsgString.GetCharArray();
        }
    }

    // 14026d1ec: Return true (1)
    return true;
}




void UDBDEasyAntiCheat::Slave_Connected()
{
    // 140276540: Check if the client interface pointer is valid
    if (this->_client)
    {
        // 140276548: Call ConnectionReset on the client interface
        // Disassembly: call qword [rax+0x10] (VTable Offset +0x10)
        /* UNDEFINED VTABLE */
        this->_client->ConnectionReset();

        // 140276560: Call ValidateServerHost on the client interface
        // Disassembly: call qword [rax+0x30] (VTable Offset +0x30)
        // Arguments: 15000 (0x3a98), 0 (nullptr), 0 (nullptr)
        /* UNDEFINED VTABLE */
        this->_client->ValidateServerHost(15000, nullptr, nullptr);
    }

    // 140276563: Set the connected to server flag to true
    this->_connectedToServer = true;

    // 14027656a: Check logging verbosity (0x5 corresponds to Log/Verbose)
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // 1402765a2: Log "slave connected"
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("slave connected"));
    }
}




void UDBDEasyAntiCheat::Slave_Disconnected()
{
    // 1402765c0: Check if the client interface pointer is valid
    if (this->_client)
    {
        // 1402765c8: Call ConnectionReset on the client interface
        // Disassembly: call qword [rax+0x10] (VTable Offset +0x10)
        /* UNDEFINED VTABLE */
        this->_client->ConnectionReset();
    }

    // 1402765cb: Set the connected to server flag to false
    this->_connectedToServer = false;

    // 1402765d2: Check logging verbosity (0x5 corresponds to Log/Verbose)
    if (LogDBDEasyAntiCheat.Verbosity >= 5)
    {
        // 14027660a: Log "slave disconnected"
        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("slave disconnected"));
    }
}




void UDBDEasyAntiCheat::Slave_HandleMessageFromServer(const TArray<uint8>& message, uint32 messageLength)
{
    // 140276644: Check if the client interface pointer is valid
    if (this->_client)
    {
        // 14027665b: Check logging verbosity (0x5 corresponds to Log/Verbose)
        if (LogDBDEasyAntiCheat.Verbosity >= 5)
        {
            // 140276697: Log the message length
            UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("Slave_HandleMessageFromServer: length %d"), messageLength);
        }

        // 1402766a5: Local stack buffer allocation
        // Based on stack frame size (0x150) and cookie position (0x140), buffer at 0x40 is likely 256 bytes.
        uint8 szMessage[256];

        // 14027669e: Check if messageLength is greater than 0
        if (messageLength > 0)
        {
            // 1402766a2: Access TArray data pointer
            const uint8* Data = message.GetData();

            // 1402766b0 - 1402766c4: Loop to copy data from TArray to local stack buffer
            // Note: Disassembly does not show explicit bounds checking against 256 bytes, 
            // relying on the caller to ensure messageLength fits.
            for (uint32 i = 0; i < messageLength; i++)
            {
                szMessage[i] = Data[i];
            }
        }

        // 1402766d5: Call PushNetworkMessage on the client interface
        // Disassembly: call qword [rax+0x20] (VTable Offset +0x20)
        /* UNDEFINED VTABLE */
        this->_client->PushNetworkMessage(szMessage, messageLength);
    }
}




void UDBDEasyAntiCheat::TickClientAuthorization()
{
    // Check if the client interface pointer is valid
    // 140279763: test rcx, rcx
    if (this->_client == nullptr)
    {
        return;
    }

    // Check if connected to the server
    // 14027976c: cmp byte [r14+0xa8], 0x0
    if (this->_connectedToServer)
    {
        void* MessageBuffer = nullptr;
        uint32 MessageLength = 0;

        // Loop to process all pending network messages from EAC
        // 140279785: call qword [rax+0x18] (VTable Offset +0x18) -> PopNetworkMessage
        // The loop continues as long as PopNetworkMessage returns true
        /* UNDEFINED VTABLE */
        while (this->_client->PopNetworkMessage(&MessageBuffer, &MessageLength))
        {
            UDBDGameInstance* GameInstance = this->_gameInstance;

            // Check if GameInstance is valid
            if (GameInstance && IsValid(GameInstance))
            {
                // Get the local player controller
                // 140279802: call UDBDGameInstance::GetLocalDBDPlayerController
                ADBDPlayerControllerBase* LocalPlayer = UDBDGameInstance::GetLocalDBDPlayerController(GameInstance);

                // Check if PlayerController is valid
                // 140279845: test byte [rbp+0x140], 0x4 -> Checks internal flags (likely IsPendingKill)
                if (LocalPlayer && IsValid(LocalPlayer))
                {
                    // Check logging verbosity (0x5 corresponds to Log/Verbose)
                    if (LogDBDEasyAntiCheat.Verbosity >= 5)
                    {
                        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("TickClientAuthorization: sending message to server. length %d"), MessageLength);
                    }

                    // Prepare the message array to send to the server
                    TArray<uint8> MessageArray;

                    if (MessageLength > 0 && MessageBuffer)
                    {
                        // Copy the data from the EAC buffer to the TArray
                        // The disassembly shows a manual copy loop/resize logic.
                        MessageArray.Append((uint8*)MessageBuffer, MessageLength);
                    }

                    // Send the message to the server via RPC
                    // 140279937: call ADBDPlayerControllerBase::Server_EACMessageFromClient
                    LocalPlayer->Server_EACMessageFromClient(MessageArray, MessageLength);
                }
            }
        }

        // Poll status with both Integrity Violation and Server Validation callbacks
        // 1402799a9: call qword [rax+0x28] (VTable Offset +0x28) -> PollStatus
        /* UNDEFINED VTABLE */
        this->_client->PollStatus(UDBDEasyAntiCheat::Impl::OnClientIntegrityViolationFunc, UDBDEasyAntiCheat::Impl::OnValidateServerFunc, this);
    }
    else
    {
        // Not connected: Poll status only checking for Integrity Violations (Server Validation is nullptr)
        // 1402799c0: call qword [rax+0x28] (VTable Offset +0x28) -> PollStatus
        /* UNDEFINED VTABLE */
        this->_client->PollStatus(UDBDEasyAntiCheat::Impl::OnClientIntegrityViolationFunc, nullptr, this);
    }
}




void UDBDEasyAntiCheat::TickServerAuthorization()
{
    // Check if the server interface is valid
    // 1402799eb: test rcx, rcx
    if (this->_server == nullptr)
    {
        return;
    }

    // --- Part 1: Process Client Updates (Authentication Status) ---

    easyanticheat::server::hydra::ClientUpdate* ClientUpdate = nullptr;

    // Loop through all pending client updates
    // 140279a17: call GetNextClientUpdate (VTable + 0x28)
    // The function returns true if an update was retrieved, false otherwise.
    /* UNDEFINED VTABLE */
    while (this->_server->GetNextClientUpdate(&ClientUpdate))
    {
        // Check the client status
        // 140279a21: mov ecx, dword [rax+0x8] (ClientUpdate->Status)
        // 2 = ClientAuthenticatedLocal
        // 5 = ClientAuthenticatedRemote
        // If the status is NOT authenticated (i.e., disconnected, banned, invalid), proceed to remove/kick.
        if (ClientUpdate->Status != easyanticheat::server::ClientStatus::ClientAuthenticatedLocal 
         && ClientUpdate->Status != easyanticheat::server::ClientStatus::ClientAuthenticatedRemote)
        {
            // Iterate through the local _clientInfos array to find the matching client
            // 140279a39: Loop setup
            for (int32 i = 0; i < this->_clientInfos.Num(); ++i)
            {
                // Check if ClientID matches
                // 140279a6a: cmp qword [rbx+rdx], rcx
                if (this->_clientInfos[i].ClientId == ClientUpdate->ClientObject)
                {
                    // 140279a74 - 140279c72: Verify validity of GameInstance, World, and Player WeakPtr.
                    // The disassembly performs extensive checks (GUObjectArray lookups) to ensure 
                    // we are accessing valid memory before logging or modifying state.

                    // Log the removal action
                    // 140279c78: Check verbosity
                    if (LogDBDEasyAntiCheat.Verbosity >= 5)
                    {
                        UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("TickServerAuthorization: Kicking client id: %p"), (void*)this->_clientInfos[i].ClientId);
                    }

                    // Remove the client info from the array
                    // 140279d3a - 140279d69: Logic corresponds to TArray::RemoveAt(i)
                    // (memmove to shift elements, decrement Num, ResizeShrink)
                    this->_clientInfos.RemoveAt(i);

                    // Break the inner loop since the client was found and handled
                    break;
                }
            }
        }
    }

    // --- Part 2: Process Network Messages (Server -> Client) ---

    void* MessageBuffer = nullptr;
    uint32 MessageLength = 0;
    uint64 ClientHandle = 0; // Represents the client identifier (pointer/ID)

    // Loop to pop all pending network messages intended for clients
    // 140279da8: call PopNetworkMessage (VTable + 0x30)
    // Returns the Client Handle (non-zero) if a message exists, 0 otherwise.
    /* UNDEFINED VTABLE */
    while ((ClientHandle = this->_server->PopNetworkMessage(&MessageBuffer, &MessageLength)) != 0)
    {
        // Resolve the PlayerController associated with this Client Handle
        // 140279dc9: call GetPlayerFromClientID
        ADBDPlayerControllerBase* Player = UDBDEasyAntiCheat::GetPlayerFromClientID(ClientHandle);

        // Check if the PlayerController is valid
        // 140279e0c: test byte [r14+0x140], 0x4 (Likely checking IsPendingKill internal flag)
        if (Player && IsValid(Player))
        {
            // Log message details
            if (LogDBDEasyAntiCheat.Verbosity >= 5)
            {
                UE_LOG(LogDBDEasyAntiCheat, Log, TEXT("TickServerAuthorization: sending message to client %p, length %d"), (void*)ClientHandle, MessageLength);
            }

            // Prepare the message array
            TArray<uint8> MessageArray;

            if (MessageLength > 0 && MessageBuffer)
            {
                // 140279ec8: Loop copying bytes from buffer to TArray
                MessageArray.Append((uint8*)MessageBuffer, MessageLength);
            }

            // Send the message to the specific client via RPC
            // 140279ed5: call ADBDPlayerControllerBase::Client_EACMessageFromServer
            Player->Client_EACMessageFromServer(MessageArray, MessageLength);
        }
    }
}