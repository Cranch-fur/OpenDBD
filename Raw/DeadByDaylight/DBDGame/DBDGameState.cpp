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