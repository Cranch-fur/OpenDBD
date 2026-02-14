void ADBDGameState::AddHeartbeatEmitter(AActor* toAdd)
{
    // The disassembly performs a direct addition to the array at offset 0x730.
    // No explicit nullptr check for 'toAdd' is performed in the provided assembly block
    // before the TArray call, implying the TArray method handles the insertion logic.

    this->_heartbeatEmitters.Add(toAdd);
}




void ADBDGameState::AddOnFindFriendSessionCompleteDelegate()
{
    // Check if the delegate handle is already valid (ID is not 0) to prevent duplicate registration.
    // Assembly: 140243dab cmp qword [rcx+0x6d0], 0x0
    if (this->_onFindFriendSessionCompleteDelegateHandle.IsValid() == false)
    {
        // Prepare the module name "OnlinePresence".
        // Assembly: 140243dc2 lea rdx, [rel `string'::OnlinePresence]
        const FName PresenceModuleName = FName("OnlinePresence");

        // Load the module checked. The pseudo-code suggests FAdvertising, but the string literal implies OnlinePresence.
        // We load the module interface first.
        // Assembly: 140243dd6 call FModuleManager::LoadModuleChecked
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        // Perform a virtual function check on the loaded module at VTable offset 0xA0 (160).
        // If this returns 0 (false), the function exits.
        // Assembly: 140243de1 call qword [rdx+0xa0]
        /* UNDEFINED ELEMENT - Virtual check on the module interface (Offset 0xA0) */
        // We assume this checks if the specific service is available or initialized.
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            // Retrieve the Matchmaking Subsystem.
            // The assembly calls a static helper or global accessor: IOnlinePresencePlugin::Matchmaking.
            // This returns a TSharedRef (or TSharedPtr) which handles reference counting.
            // Assembly: 140243e04 call IOnlinePresencePlugin::Matchmaking

            /* UNDEFINED ELEMENT - Custom Plugin Accessor */
            // Note: The return type is inferred as TSharedRef<IMatchmakingPresenceSubsystem> based on usage and destructor logic.
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            // Create and bind the delegate to this instance.
            // The assembly manually allocates memory (TInlineAllocator) and constructs the TBaseUObjectMethodDelegateInstance.
            // In C++, this is equivalent to BindUObject.
            // The bound function is ADBDGameState::OnFindFriendSessionComplete.

            /* UNDEFINED ELEMENT - Specific Delegate Type Definition */
            // We use a generic name 'FFindFriendSessionCompleteDelegate' for the type associated with the subsystem.
            FFindFriendSessionCompleteDelegate NewDelegate;
            NewDelegate.BindUObject(this, &ADBDGameState::OnFindFriendSessionComplete);

            // Register the delegate with the subsystem via a virtual function call.
            // The function at offset 0x238 takes the Module Name and the Delegate as arguments.
            // It returns an FDelegateHandle which is stored in the class member.
            // Assembly: 140243e9f call qword [rax+0x238]

            /* UNDEFINED VTABLE - Method at Offset 0x238 in IMatchmakingPresenceSubsystem */
            MatchmakingSubsystem->AddOnFindFriendSessionCompleteDelegate_Handle(this->_onFindFriendSessionCompleteDelegateHandle, NewDelegate);

            // The destructor for MatchmakingSubsystem (TSharedRef) is called here implicitly as it goes out of scope.
            // Assembly reference: 140243f33 (checking ReferenceController and decrementing)
        }
    }
}




void ADBDGameState::AddOnJoinSessionCompleteDelegate()
{
    // Check if the delegate handle is already valid (ID is not 0) to prevent duplicate registration.
    // Assembly: 140243f9b cmp qword [rcx+0x6c0], 0x0
    if (this->_onJoinSessionsCompleteDelegateHandle.IsValid() == false)
    {
        // Prepare the module name "OnlinePresence".
        // Assembly: 140243fb2 lea rdx, [rel `string'::OnlinePresence]
        const FName PresenceModuleName = FName("OnlinePresence");

        // Load the module checked. The pseudo-code suggests FAdvertising, but the string literal implies OnlinePresence.
        // We load the module interface first.
        // Assembly: 140243fc6 call FModuleManager::LoadModuleChecked
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        // Perform a virtual function check on the loaded module at VTable offset 0xA0 (160).
        // If this returns 0 (false), the function exits.
        // Assembly: 140243fd1 call qword [rdx+0xa0]
        /* UNDEFINED ELEMENT - Virtual check on the module interface (Offset 0xA0) */
        // We assume this checks if the specific service is available or initialized.
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            // Retrieve the Matchmaking Subsystem.
            // The assembly calls a static helper or global accessor: IOnlinePresencePlugin::Matchmaking.
            // This returns a TSharedRef (or TSharedPtr) which handles reference counting.
            // Assembly: 140243ff4 call IOnlinePresencePlugin::Matchmaking

            /* UNDEFINED ELEMENT - Custom Plugin Accessor */
            // Note: The return type is inferred as TSharedRef<IMatchmakingPresenceSubsystem> based on usage and destructor logic.
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            // Create and bind the delegate to this instance.
            // The assembly manually allocates memory (TInlineAllocator) and constructs the TBaseUObjectMethodDelegateInstance.
            // In C++, this is equivalent to BindUObject.
            // The bound function is ADBDGameState::OnJoinSessionComplete.

            /* UNDEFINED ELEMENT - Specific Delegate Type Definition */
            // We use a generic name 'FJoinSessionCompleteDelegate' for the type associated with the subsystem.
            FJoinSessionCompleteDelegate NewDelegate;
            NewDelegate.BindUObject(this, &ADBDGameState::OnJoinSessionComplete);

            // Register the delegate with the subsystem via a virtual function call.
            // The function at offset 0x258 (600) takes the Module Name and the Delegate as arguments.
            // It returns an FDelegateHandle which is stored in the class member.
            // Assembly: 14024408f call qword [rax+0x258]

            /* UNDEFINED VTABLE - Method at Offset 0x258 in IMatchmakingPresenceSubsystem */
            MatchmakingSubsystem->AddOnJoinSessionCompleteDelegate_Handle(this->_onJoinSessionsCompleteDelegateHandle, NewDelegate);

            // The destructor for MatchmakingSubsystem (TSharedRef) is called here implicitly as it goes out of scope.
            // Assembly reference: 140244123 (checking ReferenceController and decrementing)
        }
    }
}




void ADBDGameState::AddOnJoinSessionErrorDelegate()
{
    // Check if the delegate handle is already valid (ID is not 0) to prevent duplicate registration.
    // Assembly: 14024418b cmp qword [rcx+0x6c8], 0x0
    if (this->_onOnJoinSessionErrorDelegateHandle.IsValid() == false)
    {
        // Prepare the module name "OnlinePresence".
        // Assembly: 1402441a2 lea rdx, [rel `string'::OnlinePresence]
        const FName PresenceModuleName = FName("OnlinePresence");

        // Load the module checked.
        // Assembly: 1402441b6 call FModuleManager::LoadModuleChecked
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        // Perform a virtual function check on the loaded module at VTable offset 0xA0 (160).
        // If this returns 0 (false), the function exits.
        // Assembly: 1402441c1 call qword [rdx+0xa0]
        /* UNDEFINED ELEMENT - Virtual check on the module interface (Offset 0xA0) */
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            // Retrieve the Matchmaking Subsystem.
            // The assembly calls a static helper: IOnlinePresencePlugin::Matchmaking.
            // Returns a TSharedRef<IMatchmakingPresenceSubsystem>.
            // Assembly: 1402441e4 call IOnlinePresencePlugin::Matchmaking

            /* UNDEFINED ELEMENT - Custom Plugin Accessor */
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            // Create and bind the delegate to this instance.
            // The assembly manually allocates memory (TInlineAllocator) and constructs the TBaseUObjectMethodDelegateInstance.
            // The bound function is ADBDGameState::OnJoinSessionError.
            // Based on the mangled name in pseudo code (VFString), this delegate likely takes an FString parameter.

            /* UNDEFINED ELEMENT - Specific Delegate Type Definition */
            // We use a generic name 'FJoinSessionErrorDelegate' for the type associated with the subsystem.
            FJoinSessionErrorDelegate NewDelegate;
            NewDelegate.BindUObject(this, &ADBDGameState::OnJoinSessionError);

            // Register the delegate with the subsystem via a virtual function call.
            // The function at offset 0x268 (616) takes the Module Name and the Delegate as arguments.
            // It returns an FDelegateHandle which is stored in the class member.
            // Assembly: 14024427f call qword [rax+0x268]

            /* UNDEFINED VTABLE - Method at Offset 0x268 in IMatchmakingPresenceSubsystem */
            MatchmakingSubsystem->AddOnJoinSessionErrorDelegate_Handle(this->_onOnJoinSessionErrorDelegateHandle, NewDelegate);

            // The destructor for MatchmakingSubsystem (TSharedRef) is called here implicitly as it goes out of scope.
            // Assembly reference: 140244313 (checking ReferenceController and decrementing)
        }
    }
}




void ADBDGameState::AddOnJoinSessionLogDelegate()
{
    // Check if the delegate handle is already valid (ID is not 0) to prevent duplicate registration.
    // Assembly: 14024437b cmp qword [rcx+0x6b0], 0x0
    if (this->_onJoinSessionLogDelegateHandle.IsValid() == false)
    {
        // Prepare the module name "OnlinePresence".
        // Assembly: 140244392 lea rdx, [rel `string'::OnlinePresence]
        const FName PresenceModuleName = FName("OnlinePresence");

        // Load the module checked.
        // Assembly: 1402443a6 call FModuleManager::LoadModuleChecked
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        // Perform a virtual function check on the loaded module at VTable offset 0xA0 (160).
        // If this returns 0 (false), the function exits.
        // Assembly: 1402443b1 call qword [rdx+0xa0]
        /* UNDEFINED ELEMENT - Virtual check on the module interface (Offset 0xA0) */
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            // Retrieve the Matchmaking Subsystem.
            // The assembly calls a static helper: IOnlinePresencePlugin::Matchmaking.
            // Returns a TSharedRef<IMatchmakingPresenceSubsystem>.
            // Assembly: 1402443d4 call IOnlinePresencePlugin::Matchmaking

            /* UNDEFINED ELEMENT - Custom Plugin Accessor */
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            // Create and bind the delegate to this instance.
            // The assembly manually allocates memory and constructs the delegate.
            // The bound function is ADBDGameState::OnLobbySessionLog.
            // Based on pseudo code (VFString), this likely accepts a string (Log message).

            /* UNDEFINED ELEMENT - Specific Delegate Type Definition */
            // We use a generic name 'FJoinSessionLogDelegate' for the type associated with the subsystem.
            FJoinSessionLogDelegate NewDelegate;
            NewDelegate.BindUObject(this, &ADBDGameState::OnLobbySessionLog);

            // Register the delegate with the subsystem via a virtual function call.
            // The function at offset 0x278 (632) takes the Module Name and the Delegate as arguments.
            // It returns an FDelegateHandle which is stored in the class member.
            // Assembly: 14024446f call qword [rax+0x278]

            /* UNDEFINED VTABLE - Method at Offset 0x278 in IMatchmakingPresenceSubsystem */
            MatchmakingSubsystem->AddOnJoinSessionLogDelegate_Handle(this->_onJoinSessionLogDelegateHandle, NewDelegate);

            // The destructor for MatchmakingSubsystem (TSharedRef) is called here implicitly.
            // Assembly reference: 140244503
        }
    }
}