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




void ADBDGameState::AddOnLobbySessionFoundDelegate()
{
    if (this->_onLobbySessionFoundDelegateHandle.IsValid() == false)
    {
        const FName PresenceModuleName = FName("OnlinePresence");

        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            FLobbySessionFoundDelegate NewDelegate;
            NewDelegate.BindUObject(this, &ADBDGameState::OnLobbySessionFound);

            MatchmakingSubsystem->AddOnLobbySessionFoundDelegate_Handle(this->_onLobbySessionFoundDelegateHandle, NewDelegate);
        }
    }
}




void ADBDGameState::AddTrap(AInteractable* toAdd)
{
    // Add the provided interactable element (trap) to the array of traps
    this->_traps.Add(toAdd);
}




void ADBDGameState::Authority_BroadcastMatchChanges(const FGamePresetData& gamePresetData)
{
    // Assign the new game preset data to the internal member variable
    // Offset 0x758 in ADBDGameState structure
    this->_gamePresetData = gamePresetData;

    // Update all characters based on the newly provided preset data
    this->UpdateCharactersFromGamePreset(gamePresetData);

    // Call the replication notify function to handle logic 
    // that should occur when the preset data changes
    this->OnRep_GamePresetData();
}




void ADBDGameState::Authority_EnableObsession()
{
    // Proceed only if the server has authority (Role == ROLE_Authority, which equals 3)
    if (this->Role == ROLE_Authority)
    {
        bool bRequiresGameInstanceCall = false;

        // Check if obsession target is null
        if (this->_obsessionTarget == nullptr)
        {
            bRequiresGameInstanceCall = true;
        }
        else
        {
            // Verify object validity, mimicking the GUObjectArray and bitfield checks from the disassembly.
            // In Unreal Engine 4, these low-level flags correspond to PendingKill and destruction states.
            if (this->_obsessionTarget->IsPendingKill() == true)
            {
                bRequiresGameInstanceCall = true;
            }
            else if (this->_obsessionTarget->bActorIsBeingDestroyed == true)
            {
                bRequiresGameInstanceCall = true;
            }
            else
            {
                /* UNDEFINED VTABLE */
                // Calling an unknown virtual function at vtable offset 0x1070
                bool bIsTargetValid = this->_obsessionTarget->IsValidImpl();

                if (bIsTargetValid == false)
                {
                    bRequiresGameInstanceCall = true;
                }
            }
        }

        // If the target is invalid, null, or rejected by the virtual function, execute the fallback
        if (bRequiresGameInstanceCall == true)
        {
            /* UNDEFINED ELEMENT */
            // Reconstructing the TFunction (lambda) allocation from the disassembly
            TFunction<void(class UDBDGameInstance*)> GameInstanceCallback = [this](class UDBDGameInstance* GameInstance)
                {
                    // The lambda body is not contained within this function's disassembly
                };

            /* UNDEFINED ELEMENT */
            UDBDUtilities::CallIfGameInstanceValid(this, GameInstanceCallback);
        }
    }
}




void ADBDGameState::Authority_EscapeThroughHatch()
{
    // Retrieve the UWorld instance from the VTable.
    // The VTable at offset 0x108 maps directly to GetWorld().
    UWorld* World = this->GetWorld();

    if (World == nullptr)
    {
        return;
    }

    // Retrieve the OwningGameInstance from UWorld (offset 0x120 in UE 4.13).
    UGameInstance* BaseGameInstance = World->GetGameInstance();

    if (BaseGameInstance == nullptr)
    {
        return;
    }

    // Safely cast the base GameInstance to UDBDGameInstance.
    // The disassembly uses GUObjectArray checks, which is Unreal Engine's standard internal validation for Cast<T>.
    /* UNREAL AUTO GENERATED FUNCTION */
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(BaseGameInstance);

    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Check if the server possesses authority.
    // Offset 0x110 in AActor corresponds to the Role property, and 3 is ROLE_Authority.
    if (this->Role == ROLE_Authority)
    {
        // Increment the counter for campers who escaped through the hatch.
        this->_camperEscapedThroughHatch += 1;

        // Check if exactly 4 campers have escaped.
        if (this->_camperEscapedThroughHatch == 4)
        {
            // Iterate through the array of connected player states.
            for (APlayerState* PlayerState : this->PlayerArray)
            {
                if (PlayerState == nullptr)
                {
                    continue;
                }

                // Safely cast the generic APlayerState to ADBDPlayerState.
                /* UNREAL AUTO GENERATED FUNCTION */
                ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);

                if (DBDPlayerState == nullptr)
                {
                    continue;
                }

                // Check if the actor is currently pending destruction.
                // Offset 0x140 bit 2 (0x4) evaluates bActorIsBeingDestroyed in the AActor bitfield.
                if (DBDPlayerState->bActorIsBeingDestroyed == false)
                {
                    // Retrieve an undefined status byte from the player state.
                    /* UNDEFINED ELEMENT */
                    EPlayerRole PlayerStatus = DBDPlayerState->GameRole;

                    // Proceed if the player status is equal to 2.
                    if (PlayerStatus == VE_Camper)
                    {
                        // Retrieve the Owner of the PlayerState, which is typically the PlayerController.
                        // Offset 0x90 in AActor is the Owner property.
                        AActor* OwnerActor = DBDPlayerState->GetOwner();

                        if (OwnerActor == nullptr)
                        {
                            continue;
                        }

                        // Safely cast the owner to ADBDPlayerController.
                        /* UNREAL AUTO GENERATED FUNCTION */
                        ADBDPlayerController* PlayerController = Cast<ADBDPlayerController>(OwnerActor);

                        if (PlayerController == nullptr)
                        {
                            continue;
                        }

                        // Retrieve the UGameEventTracker from the GameInstance at offset 0x108.
                        /* UNDEFINED ELEMENT */
                        UGameEventTracker* GameEventTracker = DBDGameInstance->GameEventTracker;

                        if (GameEventTracker == nullptr)
                        {
                            continue;
                        }

                        // Retrieve an unknown target actor from the PlayerController at offset 0x388.
                        /* UNDEFINED ELEMENT */
                        APawn* TargetActor = PlayerController->Pawn;

                        // Trigger the hatch escape game event.
                        // Argument 1 (dl): 5 (DBDCamperScore_AllEscapeThroughHatch)
                        // Argument 2 (xmm2): 1.0f
                        // Argument 3 (r9): TargetActor
                        // Argument 4 (stack): nullptr
                        float EventValue = 1.0f;
                        GameEventTracker->FireGameEvent(EDBDScoreTypes::DBDCamperScore_AllEscapeThroughHatch, EventValue, TargetActor, nullptr);
                    }
                }
            }
        }
    }
}




void ADBDGameState::Authority_EvaluateObsessionTarget(ADBDPlayer* potentialTarget)
{
    // Check if the server has authority.
    // Role 3 typically corresponds to ROLE_Authority.
    if (this->Role == ROLE_Authority)
    {
        // Ensure the potential target is a valid pointer.
        if (potentialTarget == nullptr)
        {
            return;
        }

        // Safely cast the generic ADBDPlayer to ACamperPlayer.
        // The disassembly uses ClassTreeIndex comparisons which is the internal UE representation of Cast<T>.
        /* UNREAL AUTO GENERATED FUNCTION */
        ACamperPlayer* CamperTarget = Cast<ACamperPlayer>(potentialTarget);

        if (CamperTarget == nullptr)
        {
            return;
        }

        // Check if the potential target is currently pending destruction.
        // Offset 0x140 bit 2 evaluates bActorIsBeingDestroyed in the AActor bitfield.
        if (CamperTarget->bActorIsBeingDestroyed == true)
        {
            return;
        }

        // Call an undefined virtual function on the potential target at offset 0x1070.
        // This likely evaluates a condition like CanBecomeObsession().
        /* UNDEFINED VTABLE */
        bool CamperTarget_IsValidImpl = CamperTarget->IsValidImpl();

        if (CamperTarget_IsValidImpl == false)
        {
            return;
        }

        // Increment the total obsession weight pool.
        this->_totalObsessionWeight += 1;

        // If there is currently no obsession target, assign the new valid target immediately.
        if (this->_obsessionTarget == nullptr)
        {
            this->_obsessionTarget = CamperTarget;
            return;
        }

        // Check if the currently assigned obsession target is pending destruction.
        if (this->_obsessionTarget->bActorIsBeingDestroyed == true)
        {
            this->_obsessionTarget = CamperTarget;
            return;
        }

        // Evaluate the current obsession target using the same undefined virtual function.
        /* UNDEFINED VTABLE */
        bool _obsessionTarget_IsValidImpl = this->_obsessionTarget->IsValidImpl();

        if (_obsessionTarget_IsValidImpl == false)
        {
            this->_obsessionTarget = CamperTarget;
            return;
        }

        // Retrieve the priority weight for the current obsession target.
        /* UNDEFINED ELEMENT */
        float CurrentTargetWeight = this->_obsessionTarget->GetObsessionTargetWeight();

        // If the weight is zero or negative, overwrite the target.
        if (CurrentTargetWeight <= 0.0f)
        {
            this->_obsessionTarget = CamperTarget;
            return;
        }

        // Reservoir sampling distribution logic to give the new target a chance to become the obsession.
        if (this->_totalObsessionWeight > 0)
        {
            // Generate a random float based on standard C rand() normalized by RAND_MAX approximation.
            /* UNDEFINED ELEMENT */
            int32_t RandValue = rand();
            float NormalizedRand = (float)RandValue * 0.0000305185094f;

            // Calculate threshold using integer division as exactly specified in disassembly (idiv).
            int32_t ThresholdInt = 1 / this->_totalObsessionWeight;
            float ThresholdFloat = (float)ThresholdInt;

            // Apply the threshold probability.
            if (NormalizedRand <= ThresholdFloat)
            {
                this->_obsessionTarget = CamperTarget;
            }
        }
    }
}




void ADBDGameState::Authority_SetActivatedGeneratorCount(int32_t value)
{
    // Check if the current instance has network authority.
    // In Unreal Engine, ROLE_Authority corresponds to value 3.
    // Using explicit boolean comparison to avoid the '!' operator.
    if ((this->Role == ROLE_Authority) == false)
    {
        return;
    }

    // Cache the current generator count before updating.
    int32_t cachedGeneratorCount = this->_activatedGeneratorCount;

    // Apply the new value to the class member.
    this->_activatedGeneratorCount = value;

    // If the value did not change, exit the function to prevent unnecessary broadcasts.
    if (cachedGeneratorCount == value)
    {
        return;
    }

    // Broadcast the standard C++ multicast delegate.
    this->OnActivatedGeneratorCountChanged.Broadcast(value);

    // Broadcast the dynamic multicast delegate.
    // Note: The disassembly shows ProcessMulticastDelegate, which is the internal 
    // Unreal Engine generated implementation for broadcasting dynamic delegates.
    // In source code, this is simply written as .Broadcast().
    this->OnActivatedGeneratorCountChangedDynamic.Broadcast(this->_activatedGeneratorCount);
}




void ADBDGameState::Authority_SetAllPlayerLoaded()
{
    // Check if the current instance has network authority.
    // In Unreal Engine, ROLE_Authority corresponds to the integer value 3.
    // Using explicit boolean comparison to strictly avoid the '!' operator.
    if ((this->Role == ROLE_Authority) == false)
    {
        return;
    }

    // Broadcast the dynamic multicast delegate to notify listeners that all players have loaded.
    // The disassembly shows a tailcall (jmp) to ProcessMulticastDelegate with a nullptr argument.
    // ProcessMulticastDelegate is the Unreal Header Tool (UHT) generated backend for dynamic delegates,
    // and passing nullptr simply indicates that this specific event takes no parameters.
    this->AuthorityOnAllPlayerLoaded.Broadcast();
}




void ADBDGameState::Authority_SetHatchOpen(uint8_t opened)
{
    // Verify if the current instance has server authority (ROLE_Authority value is 3)
    // We use explicit comparison with false to avoid using the "!" operator
    if ((this->Role == ROLE_Authority) == false)
    {
        return;
    }

    // Update the internal hatch state with the provided parameter
    this->_isHatchOpen = opened;

    // Trigger the multicast delegate to notify all subscribed listeners about the state change
    // C++ will handle the implicit cast from uint8_t to bool for the broadcast parameter
    this->OnHatchOpened.Broadcast(opened);
}




void ADBDGameState::Authority_SetLevelReadyToPlay()
{
    // Verify if the current game state instance has server authority
    // Explicitly checking against false to avoid the NOT operator
    if ((this->Role == ROLE_Authority) == false)
    {
        return;
    }

    // Set the internal flag indicating the level is ready
    this->_levelReadyToPlay = true;

    // Trigger the multicast delegate. 
    // In disassembly, this translates to ProcessMulticastDelegate, but in high-level C++ it is invoked via Broadcast.
    this->OnLevelReadyToPlay.Broadcast();

    // Proceed to the pre-level start logic
    /* UNDEFINED ELEMENT */
    this->OnBeforeLevelStarts();
}




void ADBDGameState::Authority_SetServerJoiningData(const FServerJoiningData* joiningData)
{
    // Copy the session ID directly from the provided struct
    this->_serverJoiningData.SessionId = joiningData->SessionId;

    // Copy the session ID string (Unreal Engine's TArray assignment operator is implicitly invoked)
    this->_serverJoiningData.SessionIdStr = joiningData->SessionIdStr;

    // Copy the current joining step (byte-sized enum)
    this->_serverJoiningData.Step = joiningData->Step;

    // Copy the server join settings (Unreal Engine's TArray assignment operator is implicitly invoked)
    this->_serverJoiningData.Settings = joiningData->Settings;

    // Store the step in a local variable for subsequent evaluations
    EServerJoiningDataType step = this->_serverJoiningData.Step;

    // Check if the killer server has been successfully found (Enum value 0x1)
    if (step == EServerJoiningDataType::KillerServerFound)
    {
        // Tail call optimization executed in assembly via 'jmp'
        this->KillerServerFound(&this->_serverJoiningData); /* UNDEFINED ELEMENT */
        return;
    }

    // Check if the client state is set to travel to the killer (Enum value 0x2)
    if (step == EServerJoiningDataType::TravelToKiller)
    {
        this->TravelToKillerServer(); /* UNDEFINED ELEMENT */
    }
}




void ADBDGameState::Authority_SetSurvivorLeft(int32_t survivorRemaining)
{
    // Verify if the executing instance has network authority (3 represents ENetRole::ROLE_Authority)
    if (this->Role == ROLE_Authority)
    {
        // Assign the new survivor count to the internal state variable
        this->_survivorLeft = survivorRemaining;

        // Package the parameter required for the delegate broadcast
        int32_t Parameters = survivorRemaining;

        // Trigger the delegate to notify all bound listeners that the survivor count has changed
        // This is the underlying engine implementation of OnSurvivorsLeftChanged.Broadcast(survivorRemaining);
        /* UNREAL AUTO GENERATED FUNCTION */
        this->OnSurvivorsLeftChanged.ProcessMulticastDelegate<UObject>(&Parameters);
    }
}




void ADBDGameState::Authority_SignalEscapeDoorActivated()
{
    // Check if the current game state has network authority (ROLE_Authority is typically 3)
    // We explicitly use '== false' to avoid the '!' operator per the requirements.
    if ((this->Role == ROLE_Authority) == false)
    {
        return;
    }

    // Mark the escape door as activated (uint8_t used as a boolean flag)
    this->_escapeDoorActivated = true;

    // Trigger the dynamic multicast delegate for the escape door activation
    /* UNREAL AUTO GENERATED FUNCTION */
    /* UNDEFINED ELEMENT */
    this->OnEscapeDoorActivated.ProcessMulticastDelegate<UObject>(nullptr);

    // Broadcast the standard multicast delegate to update the HUD
    this->OnEscapeDoorActivatedHud.Broadcast();

    // Retrieve the Game Instance associated with this actor
    UGameInstance* gameInstance = this->GetGameInstance();

    UDBDGameInstance* DBDGameInst = Cast<UDBDGameInstance>(gameInstance);

    // Ensure the retrieved game instance pointer is valid before proceeding
    if (DBDGameInst == nullptr)
    {
        return;
    }

    UGameEventTracker::FireGameEvent(
        DBDGameInst->GameEventTracker, // Accessing an internal property (likely the Tracker instance) via direct offset 0x108
        EDBDScoreTypes::DBDCamperScore_PowerExitGates,                                     // DBDCamperScore_PowerExitGates identifier (24 in decimal)
        1.0f,                                     // Event value or weight
        nullptr,
        nullptr
    );
}




void ADBDGameState::ClearOnFindFriendSessionCompleteDelegate()
{
    //is delegate handle valid check
    if (this->_onFindFriendSessionCompleteDelegateHandle.IsValid() == false)
    {
        //preparing module name
        const FName PresenceModuleName = FName("OnlinePresence");

        //loading online presence module
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        //checking if online presence module connected
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            //getting matchmaking module subsystem
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            //clearing delegate handle
            MatchmakingSubsystem->ClearOnFindFriendSessionCompleteDelegate_Handle(&this->_onFindFriendSessionCompleteDelegateHandle);
        }
    }
}




void ADBDGameState::ClearOnJoinSessionCompleteDelegate()
{
    //is delegate handle valid check
    if (this->_onJoinSessionsCompleteDelegateHandle.IsValid() == false)
    {
        //preparing module name
        const FName PresenceModuleName = FName("OnlinePresence");

        //loading online presence module
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        //checking if online presence module connected
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            //getting matchmaking module subsystem
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            //clearing delegate handle
            MatchmakingSubsystem->ClearOnJoinSessionCompleteDelegate_Handle(&this->_onJoinSessionsCompleteDelegateHandle);
        }
    }
}




void ADBDGameState::ClearOnJoinSessionLogDelegate()
{
    //is delegate handle valid check
    if (this->_onJoinSessionLogDelegateHandle.IsValid() == false)
    {
        //preparing module name
        const FName PresenceModuleName = FName("OnlinePresence");

        //loading online presence module
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        //checking if online presence module connected
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            //getting matchmaking module subsystem
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            //clearing delegate handle
            MatchmakingSubsystem->ClearOnJoinSessionLogDelegate_Handle(&this->_onJoinSessionLogDelegateHandle);
        }
    }
}




void ADBDGameState::ClearOnLobbySessionFoundDelegate()
{
    //is delegate handle valid check
    if (this->_onLobbySessionFoundDelegateHandle.IsValid() == false)
    {
        //preparing module name
        const FName PresenceModuleName = FName("OnlinePresence");

        //loading online presence module
        IOnlinePresencePlugin* LoadedModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(PresenceModuleName);

        //checking if online presence module connected
        bool bIsModuleReady = LoadedModule->IsConnected();

        if (bIsModuleReady == true)
        {
            //getting matchmaking module subsystem
            TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = LoadedModule->GetMatchmaking();

            //clearing delegate handle
            MatchmakingSubsystem->ClearOnLobbySessionFoundDelegate_Handle(&this->_onLobbySessionFoundDelegateHandle);
        }
    }
}




void ADBDGameState::DefaultTimer()
{
    // Call the parent class implementation to ensure the core game state timer logic is processed
    AGameState::DefaultTimer();

    // The assembly reads a 32-bit integer from offset 0x3a8 (which corresponds to ElapsedTime in AGameState)
    // and converts it to a single-precision float using the 'cvtdq2ps' instruction.
    float currentElapsedTime = this->ElapsedTime;

    // Retrieve the global Wwise AkAudioDevice instance.
    // This is part of the Wwise Audio plugin, which acts as an external integration in Unreal Engine.
    /* UNDEFINED ELEMENT */
    FAkAudioDevice* audioDevice = FAkAudioDevice::Get();

    // Set the Real-Time Parameter Control (RTPC) value to update the game's audio system based on the elapsed match time.
    // The assembly directly executes the call without a null check, pushing 0 and nullptr as the 4th and 5th arguments.
    /* UNDEFINED ELEMENT */
    audioDevice->SetRTPCValue(TEXT("AudioRTPC_Gameplay_Time"), currentElapsedTime, 0, nullptr);
}




float ADBDGameState::GetHeartbeatEmitterTerrorRadius(class AActor* emitter) const
{
    // Retrieve the GameInstance and cast it to UDBDGameInstance.
    // The disassembly handles this via a ClassTreeIndex check (standard UE4 Cast operation).
    UGameInstance* BaseGameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(BaseGameInstance);

    // Verify if the Slasher (Killer) reference is populated.
    if (this->Slasher == nullptr)
    {
        return 0.0f;
    }

    // Perform standard Unreal Engine object validity checks.
    // In disassembly, this correlates to the GUObjectArray internal index and 'unreachable' bitwise checks.
    if (IsValid(this->Slasher) == false)
    {
        return 0.0f;
    }

    // Verify that the Slasher is not currently pending destruction.
    if (this->Slasher->bActorIsBeingDestroyed == true)
    {
        return 0.0f;
    }

    // Execute an unknown virtual function on the Slasher at offset 0x1070 to check state validity.
    /* UNDEFINED VTABLE */
    bool bIsSlasherStateValid = this->Slasher->IsValidImpl();

    if (bIsSlasherStateValid == false)
    {
        return 0.0f;
    }

    // Validate that the cast to UDBDGameInstance was successful.
    if (DBDGameInstance == nullptr)
    {
        return 0.0f;
    }

    // Verify that the Game Instance itself is valid and not pending kill.
    if (IsValid(DBDGameInstance) == false)
    {
        return 0.0f;
    }

    // Retrieve the Design Tunables object from the Game Instance.
    // The offset 0xF0 points to this data member inside the UDBDGameInstance class.
    /* UNDEFINED ELEMENT */
    UDBDDesignTunables* DesignTunables = DBDGameInstance->DesignTunables;

    if (DesignTunables == nullptr)
    {
        return 0.0f;
    }

    if (IsValid(DesignTunables) == false)
    {
        return 0.0f;
    }

    // Retrieve the Perk Manager via an interface virtual function call at offset 0x10.
    /* UNDEFINED VTABLE */
    UPerkManager* PerkManager = this->Slasher->GetPerkManager_VTableFunc_0x10();

    // Fetch the perk modifier specifically for the Terror Radius.
    // VE_ModifyEmittersTerrorRadiusAdditive = 0x1E (30), VE_All = 0x4 (4).
    /* UNDEFINED ELEMENT */
    float PerkModifier = UPerkManager::GetPerkModifier(PerkManager, EGameplayModifierType::VE_ModifyEmittersTerrorRadiusAdditive, VE_All);

    // Retrieve the tunable base value for the Terror Radius using a constant data reference.
    /* UNDEFINED ELEMENT */
    float TunableValue = UDBDDesignTunables::GetTunableValue(DesignTunables, data_143621c88, 0);

    // The final Terror Radius is the sum of the base tunable value and any active perk modifiers.
    return PerkModifier + TunableValue;
}




void ADBDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Execute the parent class replication setup
    AGameState::GetLifetimeReplicatedProps(OutLifetimeProps);

    // The disassembled code represents the preprocessor expansion of the Unreal Engine DOREP_LIFETIME macros.
    // For readable and functional C++ code in Unreal Engine 4.13, we restore the original macros.
    // These macros natively handle the thread-safe static UProperty initialization, the ArrayDim iterators,
    // and condition checks (cond == 0) found in the disassembly.
    //
    // Note: Because this uses static class macros, explicit "this->" pointers are not required or valid here.

    DOREP_LIFETIME(ADBDGameState, _activatedGeneratorCount);
    DOREP_LIFETIME(ADBDGameState, _builtLevelData);
    DOREP_LIFETIME(ADBDGameState, _gameLevelLoaded);
    DOREP_LIFETIME(ADBDGameState, _escapeDoorActivated);
    DOREP_LIFETIME(ADBDGameState, _isHatchOpen);
    DOREP_LIFETIME(ADBDGameState, _levelOfferings);
    DOREP_LIFETIME(ADBDGameState, _levelReadyToPlay);
    DOREP_LIFETIME(ADBDGameState, _playerDistributionReady);
    DOREP_LIFETIME(ADBDGameState, _sessionId);
    DOREP_LIFETIME(ADBDGameState, CamperCount);
    DOREP_LIFETIME(ADBDGameState, CamperDeadCount);
    DOREP_LIFETIME(ADBDGameState, CamperEscaped);
    DOREP_LIFETIME(ADBDGameState, CamperInMeatLockerCount);
    DOREP_LIFETIME(ADBDGameState, GeneratorNeeded);
    DOREP_LIFETIME(ADBDGameState, IsGameEnded);
    DOREP_LIFETIME(ADBDGameState, _survivorLeft);
    DOREP_LIFETIME(ADBDGameState, PlayersReadyToStart);
    DOREP_LIFETIME(ADBDGameState, SecondsLeftInLobby);
    DOREP_LIFETIME(ADBDGameState, WaitingForEscape);
    DOREP_LIFETIME(ADBDGameState, _obsessionTarget);
    DOREP_LIFETIME(ADBDGameState, _serverJoiningData);
    DOREP_LIFETIME(ADBDGameState, _gamePresetData);
    DOREP_LIFETIME(ADBDGameState, _usingWeakenedMechanic);
}




APawn* ADBDGameState::GetLocalPlayerBasePawn() const
{
    // Access GetWorld() from the VTable (Offset 0x108 maps to GetWorld in UObject)
    UWorld* World = this->GetWorld();

    // The decompiled code assumes World is valid and directly accesses offset 0x120.
    // In high-level C++, it is safer to perform an explicit check for nullptr first.
    if (World == nullptr)
    {
        return nullptr;
    }

    // Retrieve OwningGameInstance from UWorld (Offset 0x120)
    UGameInstance* GameInstance = World->OwningGameInstance;

    if (GameInstance == nullptr)
    {
        return nullptr;
    }

    // Retrieve the UClass for the DeadByDaylight game instance
    UClass* DBDGameInstanceClass = UDBDGameInstance::StaticClass(); /* UNREAL AUTO GENERATED FUNCTION */

    // Perform an IsA check. The disassembly checks the ClassTreeIndex and ClassTreeNumChildren directly.
    // In Unreal Engine C++, this is seamlessly handled by the IsA() method.
    if (GameInstance->IsA(DBDGameInstanceClass) == false)
    {
        return nullptr;
    }

    // The disassembly manually checks the GUObjectArray to see if the GameInstance is pending kill (>> 0x1D & 1).
    // The equivalent standard Unreal Engine method for this check is IsValid().
    if (IsValid(GameInstance) == false)
    {
        return nullptr;
    }

    // Get the first local player controller.
    // The disassembly calls this passing GameInstance as the context and nullptr as the argument.
    APlayerController* PlayerController = GameInstance->GetFirstLocalPlayerController(nullptr);

    if (PlayerController == nullptr)
    {
        return nullptr;
    }

    // The disassembly checks both the GUObjectArray (PendingKill flag) and the bActorIsBeingDestroyed flag (__bitfield140 & 4).
    // Calling IsValid() on an Actor properly covers both of these checks in standard UE C++.
    if (IsValid(PlayerController) == false)
    {
        return nullptr;
    }

    // Retrieve the pawn from the player controller (Offset 0x388)
    APawn* Pawn = PlayerController->GetPawn();

    if (Pawn == nullptr)
    {
        return nullptr;
    }

    // Finally, check if the Pawn is valid, pending kill, or being destroyed.
    if (IsValid(Pawn) == false)
    {
        return nullptr;
    }

    return Pawn;
}




ADBDPlayer* ADBDGameState::GetLocalPlayerPawn() const
{
    // Retrieve the UWorld object
    class UWorld* World = this->GetWorld();

    if (World == nullptr)
    {
        return nullptr;
    }

    // Access OwningGameInstance from UWorld
    class UGameInstance* GameInstance = World->OwningGameInstance;

    if (GameInstance == nullptr)
    {
        return nullptr;
    }

    // Cast GameInstance to UDBDGameInstance (generates the GetPrivateStaticClass and ClassTree checks in disassembly)
    class UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    if (DBDGameInstance == nullptr)
    {
        return nullptr;
    }

    // Retrieve the first local player controller
    class APlayerController* PlayerController = GameInstance->GetFirstLocalPlayerController(nullptr);

    // IsValid macro handles the GUObjectArray index bounds and bIsPendingKill checks seen in the disassembly
    if (IsValid(PlayerController) == false)
    {
        return nullptr;
    }

    // Retrieve the Pawn attached to the PlayerController
    class APawn* Pawn = PlayerController->Pawn;

    if (IsValid(Pawn) == false)
    {
        return nullptr;
    }

    // Cast Pawn to ADBDPlayer (generates the ADBDPlayer::GetPrivateStaticClass check in disassembly)
    class ADBDPlayer* DBDPlayer = Cast<ADBDPlayer>(Pawn);

    if (DBDPlayer == nullptr)
    {
        return nullptr;
    }

    // Execute the virtual function located at VTable offset 0x1070 that returns a boolean
    /* UNDEFINED VTABLE */
    bool bConditionMet = DBDPlayer->IsValidImpl();

    if (bConditionMet == false)
    {
        return nullptr;
    }

    return DBDPlayer;
}




ADBDPlayerState* ADBDGameState::GetLocalPlayerState() const
{
    // Get the World object from the current game state. The GetWorld() function is a standard
    // virtual function in UObject, located at VTable index 0x108.
    // In Unreal Engine, UWorld is the top-level object representing the simulation space.
    UWorld* World = this->GetWorld();

    // Check if the World object is valid. Without a world, we cannot get the game instance.
    if (World == nullptr)
    {
        return nullptr;
    }

    // Get the UGameInstance from the UWorld object.
    // UGameInstance is a high-level object that persists for the entire game session.
    UGameInstance* GameInstance = World->GetGameInstance();

    // Verify that the retrieved GameInstance is not null.
    if (GameInstance == nullptr)
    {
        return nullptr;
    }

    // Attempt to cast the generic UGameInstance to our specific UDBDGameInstance.
    // This is to ensure we are running within the expected game context.
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    if (DBDGameInstance == nullptr)
    {
        return nullptr;
    }

    // This function call is non-standard for Unreal Engine and its purpose is not clear from the context.
    // It might be a custom engine modification or a specific game logic check.
    /* UNDEFINED ELEMENT */
    // if (FNullHttpRequest::GetContentLength(DBDGameInstance) <= 0)
    // {
    //     return nullptr;
    // }

    // Retrieve the first local player controller from the game instance.
    // A local player controller is one that is controlled by the user on the current machine.
    APlayerController* LocalPlayerController = DBDGameInstance->GetFirstLocalPlayerController();

    // Check if the player controller was successfully retrieved.
    if (LocalPlayerController == nullptr)
    {
        return nullptr;
    }

    // A check on a bitfield at offset 0x140 of the APlayerController object.
    // This memory location in AActor corresponds to a union of several boolean flags.
    // The check for bit 4 likely corresponds to checking if the actor is marked for destruction (bActorIsBeingDestroyed).
    if (LocalPlayerController->IsPendingKillOrUnreachable() == true)
    {
        return nullptr;
    }

    // Get the APlayerState from the APlayerController. The PlayerState holds state information
    // for a specific player, which is replicated across the network.
    APlayerState* PlayerState = LocalPlayerController->PlayerState;

    // Check if the PlayerState object is valid.
    if (PlayerState == nullptr)
    {
        return nullptr;
    }

    // A check on a bitfield at offset 0x140 of the APlayerState object.
    // Similar to the controller check, this is likely verifying the object is not pending destruction.
    if (PlayerState->IsPendingKillOrUnreachable() == true)
    {
        return nullptr;
    }

    // Attempt to cast the generic APlayerState to our game-specific ADBDPlayerState.
    // If the cast is successful, we have found the correct local player state.
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);
    if (DBDPlayerState == nullptr)
    {
        return nullptr;
    }

    // Return the successfully found and validated local player state.
    return DBDPlayerState;
}




int32 ADBDGameState::GetNumberOfActiveSurvivors() const
{
    // Initialize a counter for the number of active survivors.
    int32 NumberOfActiveSurvivors = 0;

    // The GetObjectsOfClass function is internally represented by this high-level Unreal Engine iterator.
    // It finds all spawned instances of the specified class (ACamperPlayer) in the game world.
    for (TObjectIterator<ACamperPlayer> It; It; ++It)
    {
        ACamperPlayer* CamperPlayer = *It;

        // Check if the CamperPlayer object pointer is valid and the object itself is not being destroyed.
        if (CamperPlayer != nullptr)
        {
            // Check if the survivor is neither dead nor in a special state called "paradise".
            if (CamperPlayer->IsDead() == false && CamperPlayer->IsInParadise() == false)
            {
                // If the survivor is active, increment the counter.
                NumberOfActiveSurvivors++;
            }
        }
    }

    // Return the final count of active survivors.
    return NumberOfActiveSurvivors;
}




int32_t ADBDGameState::GetNumberOfOtherActiveSurvivors(class ACamperPlayer* exception) const
{
    // Array to hold the results of the search
    TArray<UObject*> Results;

    // Counter for the active survivors
    int32_t activeSurvivorsCount = 0;

    // Fetch all objects of class ACamperPlayer in the world
    /* UNREAL AUTO GENERATED FUNCTION */
    GetObjectsOfClass(ACamperPlayer::StaticClass(), Results, true, RF_ClassDefaultObject, EInternalObjectFlags::None);

    // Iterate through all found objects
    int32_t arrayNum = Results.Num();
    for (int32_t i = 0; i < arrayNum; i = i + 1)
    {
        class ACamperPlayer* camperPlayer = (ACamperPlayer*)Results[i];

        // Ensure the pointer is valid before accessing
        if (camperPlayer == nullptr)
        {
            continue;
        }

        // The disassembly manually checks the FUObjectItem flags at offset 0x08, shifting by 0x1D (29).
        // In Unreal Engine 4.13, bit 29 represents the RF_Unreachable flag.
        // High-level equivalent uses the built-in UE4 safety checks.
        bool isUnreachable = camperPlayer->IsUnreachable();
        if (isUnreachable == false)
        {
            // Check if the camper is currently dead
            /* UNDEFINED ELEMENT */
            bool isDead = camperPlayer->IsDead();
            if (isDead == false)
            {
                // Check if the camper is in paradise (sacrificed / disconnected state)
                /* UNDEFINED ELEMENT */
                bool isInParadise = camperPlayer->IsInParadise();
                if (isInParadise == false)
                {
                    // If the camper is not the exception passed in the arguments, increment the count
                    if (camperPlayer != exception)
                    {
                        activeSurvivorsCount++;
                    }
                }
            }
        }
    }

    // Results array memory (FMemory::Free in disassembly) is automatically freed by TArray destructor
    return activeSurvivorsCount;
}




int32_t ADBDGameState::GetObserverCount() const
{
    int32_t observerCount = 0;

    // Iterate through the PlayerArray to check each player's state.
    // The low-level code iterates using pointer arithmetic from the array's data start to its end, 
    // which translates to a standard range-based for loop in high-level Unreal Engine C++.
    for (APlayerState* playerState : this->PlayerArray)
    {
        // Ensure the player state pointer is valid before proceeding.
        if (playerState == nullptr)
        {
            continue;
        }

        // Attempt to cast the generic APlayerState to the Dead By Daylight specific ADBDPlayerState.
        // The low-level assembly fetches the private static class and performs a ClassTreeIndex check against the object's ClassPrivate member.
        // This is Unreal Engine's standard internal implementation for Cast<T>().
        ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(playerState);

        // If the cast fails, it means the player state is not an ADBDPlayerState, so we skip it.
        if (dbdPlayerState == nullptr)
        {
            continue;
        }

        // Check if the player is in the Observer state.
        // The low-level code checks a specific byte at offset 0x750 against the value 3.
        // Since the ADBDPlayerState structure is not provided, we access the memory address directly.
        // In a complete high-level project, this would map to a designated property (e.g., dbdPlayerState->PlayerRole == EPlayerRole::Observer).
        EPlayerRole observerStateField = dbdPlayerState->GameRole;

        // Verify if the value matches the observer state (3).
        if (*observerStateField == VE_Observer)
        {
            observerCount++;
        }
    }

    return observerCount;
}




void ADBDGameState::GetPlayerRoleCounts(int32_t* survivorCount, int32_t* killerCount, int32_t* spectatorCount) const
{
    // Initialize the role counters to zero
    *spectatorCount = 0;
    *killerCount = 0;
    *survivorCount = 0;

    // Iterate through the PlayerArray (translating low-level array pointer arithmetic to standard range-based for loop)
    for (APlayerState* PlayerState : this->PlayerArray)
    {
        // Check if the current PlayerState pointer is valid
        if (PlayerState != nullptr)
        {
            // The low-level ClassTreeIndex and ClassTreeNumChildren calculation is an inlined IsA() check
            if (PlayerState->IsA(ADBDPlayerState::StaticClass() /* UNREAL AUTO GENERATED FUNCTION */) == true)
            {
                // The low-level GUObjectArray bitshift check translates to IsPendingKill()
                // The 0x140 offset bitfield check translates to bActorIsBeingDestroyed
                if (PlayerState->IsPendingKill() == false && PlayerState->bActorIsBeingDestroyed == false)
                {
                    // Cast to the specific Dead by Daylight player state class
                    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);

                    if (DBDPlayerState != nullptr)
                    {
                        // Extract the player role enum/identifier from offset 0x750
                        uint8_t PlayerRole = DBDPlayerState->GameRole; /* UNDEFINED ELEMENT */

                        // Increment the respective counter based on the role identifier
                        if (PlayerRole == 1)
                        {
                            *killerCount += 1;
                        }
                        else if (PlayerRole == 2)
                        {
                            *survivorCount += 1;
                        }
                        else if (PlayerRole == 3)
                        {
                            *spectatorCount += 1;
                        }
                    }
                }
            }
        }
    }
}




ADBDPlayerState* ADBDGameState::GetPlayerStateByIDString(class FString* id) const
{
    // Check if there are any players in the array
    if (this->PlayerArray.Num() > 0)
    {
        // Iterate through all connected players
        for (int32 i = 0; i < this->PlayerArray.Num(); ++i)
        {
            APlayerState* playerState = this->PlayerArray[i];

            // Proceed only if the player state pointer is valid
            if (playerState != nullptr)
            {
                // Cast the base player state to the game-specific player state.
                // This replaces the inline GUObjectArray and ClassTreeIndex low-level checks.
                ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(playerState);

                if (dbdPlayerState != nullptr)
                {
                    // Retrieve the underlying FUniqueNetId from the UniqueId wrapper.
                    // The offset 0x3E0 corresponds to the object pointer inside TSharedPtr.
                    const FUniqueNetId* uniqueNetId = dbdPlayerState->UniqueId.GetUniqueNetId().Get();

                    if (uniqueNetId != nullptr)
                    {
                        /* UNDEFINED VTABLE */
                        // Offset 0x18: virtual bool IsValid() const
                        bool bIsValid = uniqueNetId->IsValid();

                        if (bIsValid == true)
                        {
                            /* UNDEFINED VTABLE */
                            // Offset 0x10: virtual int32 GetSize() const
                            int32 idSize = uniqueNetId->GetSize();

                            if (idSize > 0)
                            {
                                /* UNDEFINED VTABLE */
                                // Offset 0x8: virtual const uint8* GetBytes() const
                                const uint8* idBytes = uniqueNetId->GetBytes();

                                if (idBytes != nullptr)
                                {
                                    FString resultString;
                                    int32 newMax = idSize * 2;

                                    // Prepare the FString buffer to hold the hex representation
                                    if (newMax != 0)
                                    {
                                        resultString.GetCharArray().ResizeTo(newMax);
                                    }

                                    if (idSize != 0)
                                    {
                                        for (int32 j = 0; j < idSize; ++j)
                                        {
                                            /* UNDEFINED ELEMENT */
                                            // Custom or non-standard utility function to convert a byte to hex
                                            ByteToHex(idBytes[j], &resultString);
                                        }
                                    }

                                    // Compare the generated hex string with the target ID.
                                    // The FString operator== handles the case-insensitive Stricmp automatically.
                                    if (resultString == *id)
                                    {
                                        return dbdPlayerState;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Return nullptr if no matching player state is found
    return nullptr;
}




bool ADBDGameState::GetSelectedOffering(int32 index, FSelectedOffering* selectedOffering) const
{
    // Validate the index against the current size of the offerings array.
    // The disassembly compares index (edx) with the ArrayNum (offset 0x670).
    if (index >= this->_levelOfferings.Offerings.Num())
    {
        return false;
    }

    // Ensure the output destination is not a null pointer before attempting to copy.
    if (selectedOffering == nullptr)
    {
        return false;
    }

    // Access the internal data allocator and copy the structure.
    // The assembly uses a 128-bit move (movups xmm0) to copy the 16-byte FSelectedOffering structure.
    // In high-level C++, this is equivalent to a standard assignment.
    *selectedOffering = this->_levelOfferings.Offerings[index];

    return true;
}




void ADBDGameState::HandleSurvivorGroupTransition()
{
    // vtable + 0x108 is GetWorld()
    UWorld* World = this->GetWorld();

    // Offset 0xF0 in UWorld is typically the NetDriver or a similar core object in UE 4.13
    // We check if it is valid.
    AGameMode* UnknownWorldObject = World->AuthorityGameMode; /* UNDEFINED ELEMENT */

    // High-level macro for inline GUObjectArray validity and pending kill checks
    if (!IsValid(UnknownWorldObject) || UnknownWorldObject->IsPendingKill())
    {
        return;
    }

    if (this->_currentSurvivorGroupTransitionStep != ESurvivorGroupTransitionSteps::JoiningKiller)
    {
        if (this->_currentSurvivorGroupTransitionStep == ESurvivorGroupTransitionSteps::TravellingToKiller && this->PlayerArray.Num() == 1)
        {
            this->_currentSurvivorGroupTransitionStep = ESurvivorGroupTransitionSteps::Idle;

            // Offset 0x120 in UWorld is typically GameState
            ADBDGameState* CurrentGameState = Cast<ADBDGameState>(World->OwningGameInstance); /* UNDEFINED ELEMENT */

            if (IsValid(CurrentGameState))
            {
                if (CurrentGameState->PlayerArray.Num() > 0)
                {
                    APlayerState* FirstPlayerState = CurrentGameState->PlayerArray[0];
                    if (IsValid(FirstPlayerState))
                    {
                        // 0x90 offset in PlayerState or similar custom component
                        UOnlineSystemHandler* OnlineHandler = Cast<UOnlineSystemHandler>(FirstPlayerState->Owner); /* UNDEFINED ELEMENT */
                        if (IsValid(OnlineHandler))
                        {
                            // data_143621a20 likely represents a session name or similar FName/String
                            FName SessionName = FName("SWF_TravellingSWFHost_szni"); /* UNDEFINED ELEMENT */
                            OnlineHandler->TravelToSession(SessionName);

                            FString AnalyticsEventString = TEXT("SWF_TravellingSWFHost_szni");
                            TArray<FAnalyticsEventAttribute> AnalyticsAttributes;
                            AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("SessionId"), this->_serverJoiningData.SessionIdStr));

                            UBHVRAnalytics::RecordEvent(AnalyticsEventString, AnalyticsAttributes); /* UNDEFINED ELEMENT */
                        }
                    }
                }
            }
        }

        FString AnalyticsEventString = TEXT("SurviveWithFriend");
        TArray<FAnalyticsEventAttribute> AnalyticsAttributes;
        UBHVRAnalytics::RecordEvent(AnalyticsEventString, AnalyticsAttributes); /* UNDEFINED ELEMENT */
    }
    else
    {
        bool bAllPlayersReady = true;

        for (int32 i = 0; i < this->PlayerArray.Num(); ++i)
        {
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerArray[i]);
            if (IsValid(DBDPlayerState))
            {
                // 0x9A2 is likely a custom replicated property representing player readiness or connection state in DBDPlayerState
                EKillerJoiningState PlayerStateStatus = DBDPlayerState->_killerJoiningState; /* UNDEFINED ELEMENT */
                if (PlayerStateStatus == EKillerJoiningState::Disconnected)
                {
                    bAllPlayersReady = false;
                }
            }
        }

        if (bAllPlayersReady == false)
        {
            return;
        }

        bool bShouldTravel = false;

        for (int32 i = 0; i < this->PlayerArray.Num(); ++i)
        {
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerArray[i]);
            if (IsValid(DBDPlayerState))
            {
                EKillerJoiningState PlayerStateStatus = DBDPlayerState->_killerJoiningState; /* UNDEFINED ELEMENT */
                if (PlayerStateStatus == EKillerJoiningState::ConnectionFailed)
                {
                    bShouldTravel = true;
                    break;
                }
            }
        }

        if (bShouldTravel == true)
        {
            this->_currentSurvivorGroupTransitionStep = ESurvivorGroupTransitionSteps::TravellingToKiller;
            this->_travelToKillerServer = true;

            this->SetGameType(EGameType::Online); /* UNDEFINED ELEMENT */

            FGuid SessionIdCopy = this->_serverJoiningData.SessionId;
            FString SessionIdStrCopy = this->_serverJoiningData.SessionIdStr;

            this->_serverJoiningData.SessionIdStr.Empty();
            this->_serverJoiningData.Step = EServerJoiningDataType::TravelToKiller;
            this->_serverJoiningData.Settings.Empty();

            if (this->_serverJoiningData.Step == EServerJoiningDataType::KillerServerFound)
            {
                this->KillerServerFound(&this->_serverJoiningData);
            }
            else if (this->_serverJoiningData.Step == EServerJoiningDataType::TravelToKiller)
            {
                this->TravelToKillerServer();
            }

            FString AnalyticsEventString = TEXT("SWF_TravelSWF_MembersToKiller_szni");
            TArray<FAnalyticsEventAttribute> AnalyticsAttributes;
            AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("SessionId"), this->_serverJoiningData.SessionIdStr));

            UBHVRAnalytics::RecordEvent(AnalyticsEventString, AnalyticsAttributes); /* UNDEFINED ELEMENT */
        }
        else
        {
            for (int32 i = 0; i < this->PlayerArray.Num(); ++i)
            {
                ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerArray[i]);
                if (IsValid(DBDPlayerState))
                {
                    DBDPlayerState->_killerJoiningState = Disconnected; /* UNDEFINED ELEMENT */
                }
            }

            this->_currentSurvivorGroupTransitionStep = ESurvivorGroupTransitionSteps::Idle;
            this->_serverJoiningData.SessionIdStr.Empty();
            this->_serverJoiningData.Step = EServerJoiningDataType::Idle;
            this->_serverJoiningData.Settings.Empty();

            if (this->_serverJoiningData.Step == EServerJoiningDataType::KillerServerFound)
            {
                this->KillerServerFound(&this->_serverJoiningData);
            }
            else if (this->_serverJoiningData.Step == EServerJoiningDataType::TravelToKiller)
            {
                this->TravelToKillerServer();
            }

            this->Multicast_JoinKillerServerFailed();

            ADBDGameState* CurrentGameState = Cast<ADBDGameState>(World->GetGameState()); /* UNDEFINED ELEMENT */
            if (IsValid(CurrentGameState))
            {
                if (CurrentGameState->PlayerArray.Num() > 0)
                {
                    APlayerState* FirstPlayerState = CurrentGameState->PlayerArray[0];
                    if (IsValid(FirstPlayerState))
                    {
                        // 0x578 in UOnlineSystemHandler or custom class check
                        UOnlineSystemHandler* OnlineHandler = Cast<UOnlineSystemHandler>(FirstPlayerState->Owner); /* UNDEFINED ELEMENT */
                        if (IsValid(OnlineHandler))
                        {
                            bool HandlerStatus = OnlineHandler->_quickmatchCancelled; /* UNDEFINED ELEMENT */
                            if (HandlerStatus == false)
                            {
                                this->_killerServerSearchAttempt += 1;

                                UGameInstance* GameInstance = this->GetGameInstance();
                                ULocalPlayer* FirstPlayer = GameInstance->GetFirstGamePlayer();

                                FString SessionIdOut;
                                OnlineHandler->StartQuickmatch(FirstPlayer, this->PlayerArray.Num(), 0, SessionIdOut, 0xFFFFFFFF); /* UNDEFINED ELEMENT */
                            }
                        }
                    }
                }
            }

            FString JoinFailedStr = TEXT("SWF_KillerServerJoinedFailed_Id_szni");
            FString AnalyticsEventString = TEXT("SWF_JoinKillerConnectionFailed_SteamId_szni");

            TArray<FAnalyticsEventAttribute> AnalyticsAttributes;
            // 0x738 is likely an array of JoinedKillerIds or similar property
            TArray<uint32> JoinedIds = this->JoinedKillerIds;

            // Convert to string or appropriate format to record
            FString JoinedIdsString;
            /* UNDEFINED ELEMENT */ // Formatting logic omitted in low-level

            AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("JoinedKillers"), JoinedIdsString));
            UBHVRAnalytics::RecordEvent(AnalyticsEventString, AnalyticsAttributes); /* UNDEFINED ELEMENT */
        }
    }
}




void ADBDGameState::InitGamePresetData()
{
    // Retrieve the UWorld instance using the VTable function at offset 0x108.
    // According to the provided VTable, 0x108 corresponds to UWorld* GetWorld(UObject*).
    UWorld* CurrentWorld = this->GetWorld();

    if (CurrentWorld == nullptr)
    {
        return;
    }

    // Retrieve the GameInstance from the World. 
    // In Unreal Engine, OwningGameInstance is typically located at offset 0x120 in UWorld.
    // We use a Cast to ensure it's specifically a UDBDGameInstance.
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(CurrentWorld->GetGameInstance());

    // Check if the cast was successful and the GameInstance exists
    if (GameInstance == nullptr)
    {
        return;
    }

    // The assembly performs a class tree index check:
    // gameInstance->ClassPrivate->ClassTreeIndex - rax_2->ClassTreeIndex > rax_2->ClassTreeNumChildren
    // This is the low-level implementation of Unreal Engine's IsA() function.
    // We replace the low-level tree check with the standard high-level IsA() call.
    if (GameInstance->IsA(UDBDGameInstance::StaticClass()) == false)
    {
        return;
    }

    // The assembly performs a bitwise check against GUObjectArray.ObjObjects.Objects[InternalIndex]
    // Specifically checking if the object is reachable/valid and not pending kill or garbage collected.
    // This translates to the high-level IsValid() global function in Unreal Engine.
    if (IsValid(GameInstance) == false)
    {
        return;
    }

    // Initialize the GamePresetData structure belonging to this GameState, passing the GameInstance.
    /* UNDEFINED ELEMENT */
    FGamePresetData* presetdata_ptr = &this->_gamePresetData;
    presetdata_ptr->Init(GameInstance);

    // Apply the newly initialized GamePresetData to the PersistentData stored within the GameInstance.
    /* UNDEFINED ELEMENT */
    GameInstance->_persistentData->ApplyGamePresetData(presetdata_ptr);
}




void ADBDGameState::IntroCompleted()
{
    // Check if the intro has already been completed or the game has ended
    if (this->_introCompleted == true || this->IsGameEnded == true)
    {
        return;
    }

    // Mark the intro as completed
    this->_introCompleted = true;

    // Retrieve the current UWorld instance via the VTable method
    UWorld* World = this->GetWorld();

    // Retrieve the GameInstance and perform a safe cast to our custom game instance class
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(World->GetGameInstance());

    // Abort if the GameInstance is null
    if (GameInstance == nullptr)
    {
        return;
    }

    // Iterate through all the Player Controllers in the current world
    for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PlayerController = Iterator->Get();

        // High-level engine check: resolves underlying GUObjectArray, PendingKill, and Unreachable checks
        if (IsValid(PlayerController) == true)
        {
            // The method called at offset 0x628 of APlayerController's VTable is not standard or is deeply custom.
            // Often used for checks like IsLocalController() or HasClientLoadedCurrentWorld().
            if (PlayerController->IsLocalController() == true)
            {
                // Retrieve player data using a property at offset 0x388.
                // In UE 4.13 APlayerController, this is likely an extended PlayerState or custom PlayerData component.
                /* UNDEFINED ELEMENT */
                UDBDPlayerData* PlayerData = UDBDPlayerData::GetPlayerData(PlayerController->Pawn);

                // High-level engine check for the retrieved PlayerData object
                if (IsValid(PlayerData) == true)
                {
                    PlayerData->SetIntroCompleted();
                }
            }
        }
    }

    // Finally, notify the Game Instance that the intro is complete and online multiplayer is starting
    GameInstance->OnEnteringOnlineMultiplayer();
}




bool ADBDGameState::IsEscapeDoorActivated()
{
    return this->_escapeDoorActivated;
}