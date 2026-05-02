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




bool ADBDGameState::IsEscapeDoorOpen() const
{
    // Retrieve the current Dead By Daylight Game Mode. 
    // The disassembly shows 'xor ecx, ecx' prior to the call, passing nullptr as the WorldContextObject.
    ADBDGameMode* GameMode = UDBDUtilities::GetDBDGameMode(nullptr); /* UNDEFINED ELEMENT */

    // Ensure the pointer is valid before attempting to access its properties
    if (GameMode != nullptr)
    {
        // The disassembly manually accesses GUObjectArray.ObjObjects.Objects and bitshifts by 0x1D (29).
        // In UE4, the 29th bit flag corresponds to EInternalObjectFlags::PendingKill.
        if (GameMode->IsPendingKill() == false)
        {
            // The disassembly also checks the 3rd bit (0x4) at offset 0x140.
            // Based on the structure, this corresponds to the bActorIsBeingDestroyed property.
            if (GameMode->bActorIsBeingDestroyed == false)
            {
                // Both validity checks passed, return the state of the escape door
                return GameMode->EscapeOpened;
            }
        }
    }

    // Return 0 (false) if the GameMode is null, pending kill, or currently being destroyed
    return 0;
}




bool ADBDGameState::IsHatchOpen() const
{
    return this->_isHatchOpen;
}




bool ADBDGameState::IsHatchVisible() const
{
    return (this->_activatedGeneratorCount > this->_survivorLeft);
}




bool ADBDGameState::IsLevelReady()
{
    // Retrieve the UWorld object.
    // The disassembly accesses the VTable at offset 0x108.
    // According to the VTable dump, 0x108 corresponds to UObject::GetWorld().
    UWorld* World = this->GetWorld();

    // The decompiled code uses GetPrivateStaticClass and manually checks ClassTreeIndex and ClassTreeNumChildren.
    // This is Unreal Engine's standard inline behavior for Cast<UDBDGameInstance>().
    UDBDGameInstance* DBDGameInstance = nullptr;

    if (World != nullptr)
    {
        // Offset 0x120 in UWorld conventionally points to OwningGameInstance in UE4.
        UGameInstance* GameInstance = World->GetGameInstance();
        DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    }

    // Check if the game instance cast was successful
    if (DBDGameInstance != nullptr)
    {
        // We can now access the Builder directly via the struct member
        if (DBDGameInstance->Builder.IsValid() == true)
        {
            // Retrieve the actual AProceduralLevelBuilder object
            AProceduralLevelBuilder* ProceduralLevelBuilder = DBDGameInstance->Builder.Get();

            if (ProceduralLevelBuilder != nullptr)
            {
                // Call HasLevelBeenSpawned to determine if the level generation is complete.
                /* UNDEFINED ELEMENT */
                bool bIsLevelSpawned = ProceduralLevelBuilder->HasLevelBeenSpawned(&(this->_builtLevelData.Dependencies), this->_builtLevelData.TileCount);

                // If the level has not been spawned, return false (0)
                if (bIsLevelSpawned == false)
                {
                    return false;
                }
            }
        }
    }

    // If the procedural level builder is not valid, or if the level has been fully spawned, return true (1)
    return true;
}




bool ADBDGameState::IsLevelSetupDone() const
{
    // Retrieve the World from the current GameState via VTable (Offset 0x108 maps to GetWorld)
    UWorld* World = this->GetWorld();

    // Access the owning Game Instance (Offset +0x120 in UWorld corresponds to OwningGameInstance)
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(World->GetGameInstance());

    // Validate that the GameInstance exists
    if (GameInstance != nullptr)
    {
        // Check if the instance correctly belongs to the UDBDGameInstance class tree
        // The disassembly compares ClassTreeIndex and ClassTreeNumChildren, which is an optimized IsA check
        if (GameInstance->IsA(UDBDGameInstance::StaticClass()) == true)
        {
            // Internal validity check: verifies if the object is pending kill via GUObjectArray flags (bit 29)
            if (GameInstance->IsPendingKill() == false)
            {
                // Fetch the local player state from the Game Instance
                /* UNDEFINED ELEMENT */
                ADBDPlayerState* LocalPlayerState = GameInstance->GetLocalPlayerState();

                // Validate that the local player state exists
                if (LocalPlayerState != nullptr)
                {
                    // Internal validity check: verifies if the player state object is pending kill via GUObjectArray
                    if (LocalPlayerState->IsPendingKill() == false)
                    {
                        // Check if the actor is currently being destroyed (bit 2 in __bitfield140 at offset 0x140)
                        if (LocalPlayerState->bActorIsBeingDestroyed == false)
                        {
                            // Verify the player's role in the game (Offset 0x750, 3 represents VE_Observer)
                            if (LocalPlayerState->GameRole == VE_Observer)
                            {
                                // If the player is strictly an observer, return the ready-to-play state
                                return this->_levelReadyToPlay;
                            }
                        }
                    }
                }
            }
        }
    }

    // If any structural checks fail or the player is not an observer, fall back to returning the loaded state
    return this->_gameLevelLoaded;
}




bool ADBDGameState::IsObsessionTargetAlive()
{
    // Return the cached state immediately if the obsession target pointer is null
    if (this->_obsessionTarget == nullptr)
    {
        return this->_cachedObsessionEscaped;
    }

    // GUObjectArray bitfield checks: Verify that the object is not pending kill (garbage collection flag check)
    // The disassembly unpacks IsValid/IsPendingKill operations natively used by Unreal Engine 4
    if (this->_obsessionTarget->IsPendingKill() == false)
    {
        // Check if the actor is in the process of being destroyed via AActor::__bitfield140
        if (this->_obsessionTarget->bActorIsBeingDestroyed == false)
        {
            // The disassembly performs a redundant check for the PendingKill flag here, 
            // likely due to UE4's inlined macros during the compilation process.
            if (this->_obsessionTarget->IsPendingKill() == false)
            {
                /* UNDEFINED VTABLE */
                // Calls an unknown virtual function on ACamperPlayer at VTable offset 0x1070 (Index 526).
                // Expected to return a boolean evaluating some state of the camper.
                if (this->_obsessionTarget->IsValidImpl() == true)
                {
                    /* UNDEFINED ELEMENT */
                    // Check if the obsession target has already escaped
                    bool isEscaped = this->_obsessionTarget->IsEscaped();

                    // Cache the escaped state back into the GameState
                    this->_cachedObsessionEscaped = isEscaped;

                    /* UNDEFINED ELEMENT */
                    // Check if the obsession target is dead
                    uint8_t isDead = this->_obsessionTarget->IsDead();

                    // If the target is not dead, they are considered alive (returning 1)
                    if (isDead == false)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }

    // Return the cached state if the target is no longer valid, is being destroyed, 
    // or if the virtual function state check failed.
    return this->_cachedObsessionEscaped;
}




bool ADBDGameState::IsObsessionTargetAliveInLevel()
{
    // Verify that the obsession target pointer is valid
    if (this->_obsessionTarget != nullptr)
    {
        // The GUObjectArray bitwise checks in the disassembly correspond to Unreal Engine's IsPendingKill().
        // We check if the object is pending kill and if the actor is currently being destroyed (bActorIsBeingDestroyed).
        if (this->_obsessionTarget->IsPendingKill() == false && this->_obsessionTarget->bActorIsBeingDestroyed == false)
        {
            // Due to compiler optimizations and inline expansions, the PendingKill state is evaluated again.
            if (this->_obsessionTarget->IsPendingKill() == false)
            {
                // Call the undefined virtual function and verify the result
                if (this->_obsessionTarget->IsValidImpl() != false)
                {
                    // Check if the obsession target is dead
                    /* UNDEFINED ELEMENT */
                    if (this->_obsessionTarget->IsDead() == false)
                    {
                        // Check if the obsession target has already escaped or is in paradise
                        /* UNDEFINED ELEMENT */
                        if (this->_obsessionTarget->IsInParadise() == false)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    // Return 0 if the target is invalid, dead, being destroyed, or in paradise
    return false;
}




bool ADBDGameState::IsOfferingReceived() const
{
    return this->_levelOfferings.OfferingReady;
}




bool ADBDGameState::IsOnePlayerLeft()
{
    return (this->_survivorLeft == 1);
}




void ADBDGameState::KillerServerFound(const FServerJoiningData* joinParams)
{
    // Update the transition step for the survivor group
    this->_currentSurvivorGroupTransitionStep = ESurvivorGroupTransitionSteps::JoiningKiller;

    // Retrieve UWorld via VTable offset 0x108 (UObject::GetWorld)
    /* UNDEFINED VTABLE */
    UWorld* world = this->GetWorld();
    if (world == nullptr)
    {
        return;
    }

    // In UE 4.13, offset 0x120 from UWorld points to the OwningGameInstance
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(world->GetGameInstance());
    if (gameInstance == nullptr)
    {
        return;
    }

    // The disassembled index check against GUObjectArray and bitwise shift (>> 0x1D & 1)
    // is Unreal Engine's internal validation for EInternalObjectFlags::PendingKill.
    if (gameInstance->IsPendingKill() == true)
    {
        return;
    }

    UGameFlowContextSystem* contextSystem = gameInstance->_contextSystem;
    if (contextSystem == nullptr)
    {
        return;
    }

    if (contextSystem->IsPendingKill() == true)
    {
        return;
    }

    UOnlineSystemHandler* onlineSystemHandler = contextSystem->m_OnlineSystemHandler;
    if (onlineSystemHandler == nullptr)
    {
        return;
    }

    if (onlineSystemHandler->IsPendingKill() == true)
    {
        return;
    }

    ADBDPlayerState* playerState = UDBDGameInstance::GetPrimaryPlayerState(gameInstance);
    if (playerState == nullptr)
    {
        return;
    }

    if (playerState->IsPendingKill() == true)
    {
        return;
    }

    // Checking __bitfield140 bit 2 (0x04) maps to bActorIsBeingDestroyed
    if (playerState->bActorIsBeingDestroyed == true)
    {
        return;
    }

    // Register networking delegates
    /* UNDEFINED ELEMENT */
    this->AddOnJoinSessionCompleteDelegate();

    /* UNDEFINED ELEMENT */
    this->AddOnLobbySessionFoundDelegate();

    /* UNDEFINED ELEMENT */
    this->AddOnJoinSessionLogDelegate();

    /* UNDEFINED ELEMENT */
    this->AddOnJoinSessionErrorDelegate();

    // Load the OnlinePresence plugin module
    IOnlinePresencePlugin* onlinePresencePlugin = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(FName("OnlinePresence"));
    if (onlinePresencePlugin == nullptr)
    {
        return;
    }

    // Checking if module is available via VTable offset 0xA0
    /* UNDEFINED VTABLE */
    bool isPresenceAvailable = onlinePresencePlugin->IsAvailable();
    if (isPresenceAvailable == false)
    {
        return;
    }

    // Construct a new session search result wrapped in a Shared Pointer.
    // The 0x18 allocation in disassembly corresponds to the Reference Controller block for TSharedPtr.
    TSharedPtr<FOnlineSessionSearchResult> searchResult = MakeShareable(new FOnlineSessionSearchResult());
    if (searchResult.IsValid() == false)
    {
        return;
    }

    // Fill session data
    /* UNDEFINED ELEMENT */
    FServerJoiningData::FillSessionFromJoinParams(joinParams, searchResult.Get());

    // Request Matchmaking Subsystem
    /* UNDEFINED ELEMENT */
    TSharedRef<IMatchmakingPresenceSubsystem> matchmakingSubsystem = onlinePresencePlugin->Matchmaking();

    // Attempt to join session using VTable offset 0x1A8
    /* UNDEFINED VTABLE */
    bool bMatchmakingStarted = matchmakingSubsystem->JoinSession(playerState->UniqueId.UniqueNetId.Object, NAME_GameSession, searchResult);

    if (bMatchmakingStarted == false)
    {
        // Setup analytics attributes for failure
        // TArray resize and FString allocations seen in disassembly are abstracted here
        TArray<FAttribute> failureAttributes;
        FString eventName = FString(TEXT("SWF_JoinServer_Failed_szni"));

        /* UNDEFINED ELEMENT */
        UBHVRAnalytics::AddAttributes(failureAttributes, eventName, TEXT("Failed_AlreadyInSearch"));

        FString analyticsCategory = FString(TEXT("SurviveWithFriend"));

        /* UNDEFINED ELEMENT */
        UBHVRAnalytics::RecordEvent(analyticsCategory, failureAttributes);

        // Note: The manual FMemory::Free calls seen in the disassembly (at 0x140267108, 0x140267129, etc.)
        // are compiler-generated cleanups (destructors) for the FString and TArray scoped variables.
        // In high-level C++, these are handled automatically when the variables go out of scope.
    }

    // TSharedPtr reference counts are decremented automatically upon exiting the function scope.
}




void ADBDGameState::Multicast_JoinKillerServerFailed_Implementation()
{
    // Accessing UWorld via the VTable (Offset 0x108 maps to GetWorld)
    UWorld* World = this->GetWorld();
    if (World == nullptr)
    {
        return;
    }

    // Retrieving the OwningGameInstance from UWorld (Offset 0x120)
    UGameInstance* GameInstance = World->GetGameInstance();
    if (GameInstance == nullptr)
    {
        return;
    }

    // Verifying that the GameInstance inherits from UDBDGameInstance
    if (GameInstance->IsA(UDBDGameInstance::StaticClass()) == false)
    {
        return;
    }

    // Checking if the GameInstance object is valid in the GUObjectArray (not pending kill)
    if (IsValid(GameInstance) == false)
    {
        return;
    }

    // Initializing Analytics Event Attributes array
    TArray<FAnalyticsEventAttribute> AnalyticsAttributes; /* UNDEFINED ELEMENT */

    // Finding the target join parameter setting using a lambda predicate
    FDBDJoinParamSetting* JoinParamSetting = this->_serverJoiningData.Settings.FindByPredicate([](const FDBDJoinParamSetting& Setting)
        {
            // Predicate logic is encapsulated inside the lambda in disassembly
            return true;
        });

    // Copying the string value if the setting was successfully found
    FString ParamValue = TEXT("");
    if (JoinParamSetting != nullptr)
    {
        ParamValue = JoinParamSetting->Value;
    }

    // Adding the extracted attribute to the analytics array
    AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("SWF_KillerServerJoinedFailed_Id_szni"), ParamValue)); /* UNDEFINED ELEMENT */

    // Recording the analytics event for Survive With Friends
    UBHVRAnalytics::RecordEvent(TEXT("SurviveWithFriend"), AnalyticsAttributes); /* UNDEFINED ELEMENT */

    // Displaying a debug message on the screen
    if (GEngine != nullptr)
    {
        FString DebugMessage = FString::Printf(TEXT("Multicast_JoinKillerServerFailed_Implementation"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, DebugMessage, true, FVector2D::UnitVector);
    }

    // Fetching UOnlineSystemHandler from UDBDGameInstance (Offset 0x3B0)
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    if (DBDGameInstance != nullptr)
    {
        UOnlineSystemHandler* OnlineHandler = DBDGameInstance->OnlineSystemHandler; /* UNDEFINED ELEMENT */
        if (OnlineHandler != nullptr)
        {
            // Ensuring the OnlineHandler object is valid before interaction
            if (IsValid(OnlineHandler) == true)
            {
                // Destroying the current active session
                FName SessionName = FName(TEXT("GameSession"));
                UOnlineSystemHandler::DestroyExistingSession(OnlineHandler, SessionName, 0); /* UNDEFINED ELEMENT */
            }
        }

        // Fetching UDBDPersistentData from UDBDGameInstance (Offset 0x3B8)
        UDBDPersistentData* PersistentData = DBDGameInstance->_persistentData; /* UNDEFINED ELEMENT */
        if (PersistentData != nullptr)
        {
            // Transitioning the game type state back to SurvivorGroup
            PersistentData->SetGameType(EGameType::SurvivorGroup); /* UNDEFINED ELEMENT */
        }
    }
}




void ADBDGameState::Multicast_KillerSearchStarted_Implementation()
{
    this->_killerServerSearchStarted = true;
}




void ADBDGameState::Multicast_KillerServerSearchFailed_Implementation()
{
    this->_killerServerSearchFailed = true;
}




void ADBDGameState::Multicast_SetBuiltLevelData_Implementation(const FName* themeName, const FName* themeWeather, const FString* mapName, int32_t tileCount, const TArray<FDependency, FDefaultAllocator>* levelDependencies)
{
    // Assign the theme name to the built level data structure
    this->_builtLevelData.ThemeName = *themeName;

    // Assign the theme weather to the built level data structure
    this->_builtLevelData.ThemeWeather = *themeWeather;

    // Assign the map name to the built level data structure
    // TArray<uint16_t>::operator= is invoked under the hood for FString::Data
    this->_builtLevelData.MapName = *mapName;

    // Set the total amount of tiles for the level
    this->_builtLevelData.TileCount = tileCount;

    // Assign the array of level dependencies to the built level data structure
    // The compiler uses a tail call optimization (jmp) here, but it is a standard assignment
    this->_builtLevelData.Dependencies = *levelDependencies;
}




void ADBDGameState::Multicast_SetGameEnded_Implementation(bool hasServerLeftGame)
{
    // Update the server state indicating if the server has left the game
    this->SetServerLeftGame(hasServerLeftGame); /* UNDEFINED ELEMENT */

    // Retrieve the Game Instance associated with this GameState
    UGameInstance* GameInstance = this->GetGameInstance();

    // Ensure the Game Instance is valid
    if (GameInstance == nullptr)
    {
        return;
    }

    // Verify if the Game Instance is of type UDBDGameInstance.
    // The decompiled code shows an inline UObject::IsA() check using ClassTreeIndex math.
    UClass* DBDGameInstanceClass = UDBDGameInstance::StaticClass();
    if (GameInstance->IsA(DBDGameInstanceClass) == false)
    {
        return;
    }

    // Inline bitwise check against GUObjectArray to see if the object is pending destruction.
    // (EInternalObjectFlags::PendingKill evaluates to 1 << 29, which matches the >> 0x1d & 1 shift in ASM)
    if (GameInstance->IsPendingKill() == true)
    {
        return;
    }

    // Safely cast to UDBDGameInstance since the IsA check successfully passed
    UDBDGameInstance* DBDGameInstance = static_cast<UDBDGameInstance*>(GameInstance);

    // Retrieve the local player controller
    ADBDPlayerControllerBase* LocalPlayerController = DBDGameInstance->GetLocalDBDPlayerController(); /* UNDEFINED ELEMENT */

    // Ensure the Player Controller is valid
    if (LocalPlayerController == nullptr)
    {
        return;
    }

    // Check if the Player Controller is marked as PendingKill or is in the process of being destroyed.
    // The bitwise check (__bitfield140 & 4) exactly corresponds to bActorIsBeingDestroyed in the AActor structure.
    if (LocalPlayerController->IsPendingKill() == true || LocalPlayerController->bActorIsBeingDestroyed == true)
    {
        return;
    }

    // Perform a final safety check to ensure the Player Controller is still valid before firing the client event
    if (LocalPlayerController->IsPendingKill() == false)
    {
        // Notify the client that the game has ended
        LocalPlayerController->Client_GameEnded(); /* UNDEFINED ELEMENT */
    }
}




void ADBDGameState::Multicast_SetGameLevelLoaded_Implementation()
{
    if (this->_gameLevelLoaded)
        return;
    
    this->_gameLevelLoaded = true;

    this->SetupGameLoaded();
}




void ADBDGameState::Multicast_SetKillerGoalPoints_Implementation(FUniqueNetIdRepl playerId, int32_t killerGoals)
{
    if (GetLocalRole() == ROLE_Authority) return;

    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GetGameInstance());
    if (!DBDGameInstance) return;

    UDBDPersistentData* PersistentData = DBDGameInstance->_persistentData;
    if (PersistentData)
    {
        FCachedPlayerScoreData* ScoreCache = PersistentData->GetPlayerScoreCache(playerId);

        if (ScoreCache)
        {
            ScoreCache->KillerGoalPoints = killerGoals;
        }
    }
}




void ADBDGameState::Multicast_SetServerLeftGame_Implementation(bool hasServerLeft)
{
    this->SetServerLeftGame(hasServerLeft);
}




void ADBDGameState::Multicast_UpdateCharacterFromGamePreset_Implementation(ADBDPlayerState_Menu* playerState_Menu, FGamePresetData* gamePresetData)
{
    // Retrieve the base GameInstance associated with this Actor
    UGameInstance* GameInstance = this->GetGameInstance();

    // Try to safely cast the GameInstance to UDBDGameInstance.
    // The disassembly explicitly shows Unreal Engine's internal ClassTreeIndex casting logic,
    // which developers call using the Cast<> template.
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // Extract the UDBDPersistentData pointer from UDBDGameInstance at the specific memory offset (0x3B8).
    // Pointer arithmetic is used here to bypass the missing UDBDGameInstance class structure.
    // Note: The original disassembly does not perform a "== nullptr" check on DBDGameInstance before dereferencing.
    UDBDPersistentData*  = DBDGameInstance->_persistentData; /* UNDEFINED ELEMENT */

    // Apply the game preset data to the retrieved persistent data instance.
    // It is represented here matching the Pseudo C call structure.
    PersistentData->ApplyGamePresetData(gamePresetData); /* UNDEFINED ELEMENT */

    // Update the characters using the provided game preset data within the current GameState
    this->UpdateCharactersFromGamePreset(gamePresetData);

    // Note: The parameter 'playerState_Menu' is completely unused in this function's scope.
    // The disassembly also shows a tailcall to the FGamePresetData destructor (FGamePresetData::~FGamePresetData), 
    // which implies standard C++ scope destruction being handled natively at the end of the execution block.
}




void ADBDGameState::OnBeforeLevelStarts()
{
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GetGameInstance());

    if (IsValid(DBDGameInstance))
    {
        ADBDPlayerController* DBDPlayerController = Cast<ADBDPlayerController>(DBDGameInstance->GetFirstLocalPlayerController());

        if (IsValid(DBDPlayerController))
        {
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(DBDPlayerController->PlayerState);

            if (IsValid(DBDPlayerState))
            {
            }
        }
    }

    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        SlateApp.ClearAllUserFocus(EFocusCause::SetDirectly);
        SlateApp.SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);
    }
}




void ADBDGameState::OnFindFriendSessionComplete(int32_t LocalUserNum, uint8_t bWasSuccessful, const FOnlineSessionSearchResult* SearchResult)
{
    // Clear the delegate to prevent redundant calls and memory leaks.
    this->ClearOnFindFriendSessionCompleteDelegate();

    // Create an array to hold analytics event attributes.
    TArray<FAnalyticsEventAttribute> AnalyticsAttributes;

    // Verify that the operation was successful and that the OwningUserId (typically a TSharedPtr) is valid.
    if (bWasSuccessful != false && SearchResult->Session.OwningUserId.Get() != nullptr)
    {
        FOnlineSessionInfo* SessionInfo = SearchResult->Session.SessionInfo.Get();

        // Check if the SessionInfo pointer is valid, then validate the session itself using its virtual table.
        // We use offset 0x20 (index 4) which is highly likely to be the IsValid() method on FOnlineSessionInfo in UE4.13.
        if (SessionInfo != nullptr && SessionInfo->IsValid() != false)
        {
            const TCHAR* OwningUserNameStr;

            // Retrieve the owning user name or fallback to an empty string if the array is empty.
            if (SearchResult->Session.OwningUserName.Len() == 0)
            {
                OwningUserNameStr = TEXT("");
            }
            else
            {
                OwningUserNameStr = *SearchResult->Session.OwningUserName;
            }

            // Format a descriptive log string with lobby session details.
            FString JoinLobbyLog = FString::Printf(
                TEXT("JoiningLobbySession %s PCon : %d JInProg : %d JVPres : %d Advert : %d"),
                OwningUserNameStr,
                SearchResult->Session.NumOpenPublicConnections,
                SearchResult->Session.SessionSettings.bAllowJoinInProgress,
                SearchResult->Session.SessionSettings.bAllowJoinViaPresence,
                SearchResult->Session.SessionSettings.bShouldAdvertise
            );

            FString EventFailCheck = TEXT("SWF_JoinServerFail_Check_szni");

            // Populate the analytics attributes array with the generated strings.
            /* UNDEFINED ELEMENT */ AddAttributes(AnalyticsAttributes, EventFailCheck, JoinLobbyLog);

            // Log the formatted string using standard Unreal Engine macro (which expands to FMsg::Logf_Internal).
            UE_LOG(GameFlow, Log, TEXT("SWF_JoinServerFail_Check_szni %s"), *JoinLobbyLog);
        }
    }

    FString EventName = TEXT("SurviveWithFriend");

    // Send the recorded attributes to the analytics system.
    /* UNDEFINED ELEMENT */ UBHVRAnalytics::RecordEvent(EventName, AnalyticsAttributes);

    // Retrieve the UWorld object. Offset 0x108 maps exactly to GetWorld() inside the provided VTable.
    UWorld* World = this->GetWorld();

    if (World != nullptr)
    {
        // Extract the OwningGameInstance from the valid World object.
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(World->GetGameInstance());

        // Validate that the instance exists, matches our custom class hierarchy, and passes the internal GUObjectArray memory checks.
        if (GameInstance != nullptr && GameInstance->IsA(UDBDGameInstance::StaticClass()) != false && IsValid(GameInstance) != false)
        {
            // Retrieve the local player's state.
            ADBDPlayerState* LocalPlayerState = GameInstance->GetLocalPlayerState();

            if (LocalPlayerState != nullptr)
            {
                // Notify the server that the killer has joined. 0 usually stands for 'false' or a default enum state.
                LocalPlayerState->Server_KillerServerJoined(false);
            }
        }
    }
}




void ADBDGameState::OnJoinSessionComplete(class FName sessionName, enum EOnJoinSessionCompleteResult::Type result)
{
    // Retrieve the Game Instance via the World
    /* UNDEFINED VTABLE */
    // Resolving vtable + 0x108 (GetWorld) -> offset 0x120 (GameInstance)
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetWorld()->GetGameInstance());

    // Ensure the GameInstance is valid and matches the target class
    if (GameInstance != nullptr)
    {
        // Clear locally assigned delegates to prevent dangling callbacks
        /* UNDEFINED ELEMENT */
        this->ClearOnLobbySessionFoundDelegate();

        /* UNDEFINED ELEMENT */
        this->ClearOnJoinSessionCompleteDelegate();

        /* UNDEFINED ELEMENT */
        this->ClearOnJoinSessionErrorDelegate();

        /* UNDEFINED ELEMENT */
        this->ClearOnJoinSessionLogDelegate();

        // Check if the Context System is initialized
        UGameFlowContextSystem* ContextSystem = GameInstance->_contextSystem;
        if (ContextSystem != nullptr)
        {
            // Access the Online System Handler via the Context System
            UOnlineSystemHandler* OnlineSystemHandler = ContextSystem->m_OnlineSystemHandler;
            if (OnlineSystemHandler != nullptr)
            {
                // Remove the join session complete delegate instance explicitly 
                /* UNDEFINED ELEMENT */
                OnlineSystemHandler->OnJoinSessionsCompleteDelegate.Remove(this->_onJoinSessionsCompleteDelegateHandle);
            }
        }

        // Check if the join session attempt was successful (0 usually means Success in UE's EOnJoinSessionCompleteResult)
        if (result == EOnJoinSessionCompleteResult::Success)
        {
            // Load the Online Presence advertising module
            /* UNDEFINED ELEMENT */
            IOnlinePresencePlugin* OnlinePresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(FName("OnlinePresence"));
            if (OnlinePresenceModule->IsConnected())
            {
                // Fetch the Matchmaking Presence Subsystem
                /* UNDEFINED ELEMENT */
                TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = OnlinePresenceModule->GetMatchmaking();

                // Get the session using the subsystem
                /* UNDEFINED VTABLE */
                FOnlineSession SearchResult = MatchmakingSubsystem->GetNamedSession(/* data_143621a20 */);

                // Assign join parameters and complete the session transition
                UOnlineSystemHandler* GameInstanceOnlineHandler = GameInstance->GetOnlineSystemHandler();
                if (GameInstanceOnlineHandler != nullptr)
                {
                    /* UNDEFINED ELEMENT */
                    GameInstanceOnlineHandler->SetJoinParamsFromSession(SearchResult);

                    /* UNDEFINED ELEMENT */
                    GameInstanceOnlineHandler->OnJoinSessionComplete(sessionName, EOnJoinSessionCompleteResult::Success);
                }
            }
        }
        else
        {
            // The join session attempt failed
            /* UNDEFINED ELEMENT */
            IOnlinePresencePlugin* OnlinePresenceModule = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(FName("OnlinePresence"));
            if (OnlinePresenceModule->IsConnected())
            {
                /* UNDEFINED ELEMENT */
                TSharedRef<IMatchmakingPresenceSubsystem> MatchmakingSubsystem = OnlinePresenceModule->GetMatchmaking();

                // Prepare analytics payload attributes for Survive With Friends (SWF) failure
                FString BaseErrorStr = FString(TEXT("SWF_KillerServerJoinedFailed_i"));
                FString ErrorIdStr = FString(TEXT("SWF_KillerServerJoinedFailed_Id_szni"));
                FString SessionExistsStr = FString(TEXT("Failed_SessionAlreadyExists"));
                FString EventNameStr = FString(TEXT("SurviveWithFriend"));

                // Record the analytics event with the local settings payload
                /* UNDEFINED ELEMENT */
                UBHVRAnalytics::RecordEvent(EventNameStr, BaseErrorStr, this->_serverJoiningData.Settings);

                // Add delegate to attempt finding a friend's session instead
                /* UNDEFINED ELEMENT */
                this->AddOnFindFriendSessionCompleteDelegate();

                // Check if a session already exists for this specific ID
                /* UNDEFINED VTABLE */
                bool bSessionExists = MatchmakingSubsystem->HasSession(0, this->_serverJoiningData.SessionId);

                // If it does not exist, trigger the friend session complete flow with an empty search result
                if (bSessionExists == false)
                {
                    FOnlineSession EmptySearchResult;

                    /* UNDEFINED ELEMENT */
                    this->OnFindFriendSessionComplete(0, false, EmptySearchResult);
                }
            }
        }
    }
}




void ADBDGameState::OnJoinSessionError(FString* joinError)
{
    // Ensure the joinError pointer is valid before proceeding
    if (joinError == nullptr)
    {
        return;
    }

    // Initialize an array to hold analytics event attributes
    TArray<FAnalyticsEventAttribute> AnalyticsAttributes;

    // Create a string for the attribute name
    FString AttributeName = FString(TEXT("SWF_JoinServer_Failed_szni"));

    // Add the join error as an attribute
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::AddAttributes(AnalyticsAttributes, AttributeName, *joinError);

    // Create a string for the event name
    FString EventName = FString(TEXT("SurviveWithFriend"));

    // Record the analytics event with the collected attributes
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::RecordEvent(EventName, AnalyticsAttributes);

    // Extract the raw string data for logging
    // The disassembly explicitly checks the ArrayNum to avoid null access on string data
    const TCHAR* LogData = TEXT("");
    if (joinError->Len() == 0)
    {
        LogData = TEXT("");
    }
    else
    {
        LogData = **joinError;
    }

    // Log the error message
    // The assembly checks if verbosity is >= 5, which corresponds to ELogVerbosity::Log in UE4.
    // FMsg::Logf_Internal__VA is the underlying macro expansion of UE_LOG.
    UE_LOG(LogGameFlow, Log, TEXT("SWF_JoinServer_Failed %s"), LogData);

    // Note: The manual memory free operations (FMemory::Free) seen in the disassembly 
    // are automatically handled here by the destructors of TArray and FString 
    // when they fall out of scope at the end of the function.
}




void ADBDGameState::OnLobbySessionFound(uint8_t bWasSuccessful)
{
    // Initialize the attribute array to hold analytics data.
    // Represented as raw pointers 'attr' and 'i_2' in the disassembly.
    /* UNDEFINED ELEMENT */
    TArray<FAnalyticsEventAttribute> AnalyticsAttributes;

    // Convert the string to a wide character format for Unreal Engine's FString.
    // In disassembly, this uses FGenericPlatformString::Convert to allocate memory dynamically.
    FString JoinServerAttribute = TEXT("SWF_JoinServer_LobbySessionFound_ini");

    // Add the initialized attribute to the array.
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::AddAttributes(AnalyticsAttributes, JoinServerAttribute, bWasSuccessful);

    // Prepare the main event name for the analytics record.
    FString EventName = TEXT("SurviveWithFriend");

    // Fire the analytics event, passing the array of attributes.
    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::RecordEvent(EventName, AnalyticsAttributes);

    // Check if the current log verbosity allows logging (0x5 translates to ELogVerbosity::Log).
    // Accessing the global 'GameFlow' log category structure.
    if (GameFlow.Verbosity >= ELogVerbosity::Log)
    {
        // Internal engine call to FMsg::Logf_Internal__VA, which is the macro expansion for UE_LOG.
        // It logs the file path, line number (0x200 / 512), log category, verbosity, and the string format.
        UE_LOG(LogGameFlow, Log, TEXT("LobbySessionFound %d"), SuccessfulInt);
    }
}