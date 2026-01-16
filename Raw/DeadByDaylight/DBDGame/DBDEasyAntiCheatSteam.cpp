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