void ADBDGame_Start::AuthTicketReceived(AuthenticatedPresenceBase* Auth, bool Authority)
{
    // Check if we have authority (ASM: test Authority, Authority)
    if (Authority == false)
    {
        return;
    }

    // Load the "OnlinePresence" module
    // ASM: call FModuleManager::LoadModuleChecked
    // Note: IDA Pseudo identified this as FAnalyticsET, but the string "OnlinePresence" implies a specific module.
    // We use IModuleInterface generic here to allow the offset-based access below.
    IModuleInterface& OnlinePresenceModule = FModuleManager::LoadModuleChecked<IModuleInterface>(FName("OnlinePresence"));

    // -------------------------------------------------------------------------
    // /* UNDEFINED ELEMENT */
    // Accessing specific Delegate at Offset 0xF0 of the Module Interface
    // -------------------------------------------------------------------------
    // ASM: lea this, [rax+0F0h]; call RemoveDelegateInstance
    // It removes the _generateAuthTicketHandle from a delegate residing at offset 0xF0 in the module.
    unsigned char* ModuleAddr = reinterpret_cast<unsigned char*>(&OnlinePresenceModule);
    TBaseMulticastDelegate<void, bool, int, int>* ModuleDelegate =
        reinterpret_cast<TBaseMulticastDelegate<void, bool, int, int>*>(ModuleAddr + 0xF0);

    ModuleDelegate->Remove(this->_generateAuthTicketHandle);

    // -------------------------------------------------------------------------
    // /* UNDEFINED VTABLE */
    // Validation Checks on the Module Interface
    // -------------------------------------------------------------------------
    // The ASM dereferences the module to get the interface pointer, then calls virtual functions at specific offsets.
    // 1. [VTable+0xA0] (Index 20)
    // 2. [VTable+0xF8] (Index 31)
    // 3. [VTable+0x130] (Index 38)

    // Get the VTable pointer
    void* InterfacePtr = &OnlinePresenceModule;
    void** VTable = *reinterpret_cast<void***>(InterfacePtr);

    FOnlinePresenceCheckFunc CheckFunc20 = reinterpret_cast<FOnlinePresenceCheckFunc>(VTable[20]); // 0xA0 / 8
    FOnlinePresenceCheckFunc CheckFunc31 = reinterpret_cast<FOnlinePresenceCheckFunc>(VTable[31]); // 0xF8 / 8
    FOnlinePresenceCheckFunc CheckFunc38 = reinterpret_cast<FOnlinePresenceCheckFunc>(VTable[38]); // 0x130 / 8

    // Logic: if (!Check20 || Check31 || Check38) -> Fail
    bool bCheck20 = CheckFunc20(InterfacePtr);
    bool bCheck31 = CheckFunc31(InterfacePtr);
    bool bCheck38 = CheckFunc38(InterfacePtr);

    if (bCheck20 == false || bCheck31 || bCheck38)
    {
        // Initialization Failed
        // Enum value 7 inferred from ASM (mov dl, 7)
        this->GameInitComplete((ELoadCompleteState)7);
        return;
    }

    // -------------------------------------------------------------------------
    // Success Path
    // -------------------------------------------------------------------------

    // Set Load Progress to 5 (ASM: mov dl, 5)
    this->SetLoadProgress(StartEAC);

    // Get Game Instance (ASM: call AActor::GetGameInstance)
    UGameInstance* GameInstance = this->GetGameInstance();

    // Check if GameInstance is valid (ASM: test rax, rax)
    if (GameInstance == nullptr)
    {
        // Fail State (ASM: mov dl, 2)
        this->GameInitComplete(FailedEAC);
        return;
    }

    // Check if GameInstance is of type UDBDGameInstance
    // ASM performs an explicit ClassTreeIndex check
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    if (DBDGameInstance == nullptr)
    {
        this->GameInitComplete(FailedEAC);
        return;
    }

    // Validity check (PendingKill / Validity of flags)
    // ASM: checks GUObjectArray flags for 0x20000000 (RF_PendingKill / RF_MirroredGarbage)
    if (DBDGameInstance->IsPendingKill())
    {
        this->GameInitComplete(FailedEAC);
        return;
    }

    // -------------------------------------------------------------------------
    // /* UNDEFINED ELEMENT */
    // Accessing UDBDEasyAntiCheat at Offset 0x500 of UDBDGameInstance
    // -------------------------------------------------------------------------
    // ASM: mov r8, [rdi+500h] where rdi is GameInstance
    UDBDEasyAntiCheat* EasyAntiCheat = DBDGameInstance->_eac;

    if (EasyAntiCheat == nullptr)
    {
        this->GameInitComplete(FailedEAC);
        return;
    }

    // Check IsPendingKill on the EAC object
    // ASM checks GUObjectArray flags again for r8
    if (EasyAntiCheat->IsPendingKill())
    {
        this->GameInitComplete(FailedEAC);
        return;
    }

    // Check flags on the passed Auth parameter
    // ASM: mov eax, [Auth+8]; shr eax, 1Dh; test al, 1
    if (Auth && Auth->IsPendingKill())
    {
        this->GameInitComplete(FailedEAC);
        return;
    }

    // -------------------------------------------------------------------------
    // Bind EAC Init Delegate and Create Client
    // -------------------------------------------------------------------------

    // /* UNDEFINED ELEMENT */
    // Access Delegate at Offset 0x30 of EasyAntiCheat
    // ASM: lea this, [r8+30h]; ... AddUObject ...
    TBaseMulticastDelegate<void, bool>* EACDelegate = EasyAntiCheat->OnInitComplete;

    // Bind ADBDGame_Start::EACInitComplete
    // ASM: mov [rbx+4E8h], this (Stores the FDelegateHandle returned by AddUObject)
    this->_initEACHandle = EACDelegate->AddUObject(this, &ADBDGame_Start::EACInitComplete);

    // Call CreateClient
    // ASM: call ?CreateClient@UDBDEasyAntiCheat@@QEAAXXZ
    EasyAntiCheat->CreateClient();
}




EDeferredError ADBDGame_Start::ConsumeDeferredError()
{
    // Store the current deferred error in a local variable to return it later
    // Based on _D: movzx eax, byte ptr [this+4D2h]
    EDeferredError _deferredErrorValue = this->_deferredError;

    // Reset the internal deferred error state to None (0)
    // Based on _D: mov byte ptr [this+4D2h], 0
    this->_deferredError = EDeferredError::None;

    // Return the error that was stored prior to the reset
    // Based on _D: retn
    return _deferredErrorValue;
}




void ADBDGame_Start::EACInitComplete(bool success)
{
    // Retrieve the current Game Instance
    UGameInstance* GameInstance = this->GetGameInstance();

    // Verify GameInstance validity
    if (GameInstance)
    {
        // Get the specific class for DBD Game Instance for type checking
        UClass* DBDGameInstanceClass = UDBDGameInstance::StaticClass();

        // Perform an optimized IsA check using ClassTree indices (UE4 internal logic)
        // Based on _D: sub edx, [rax+88h] / cmp edx, [rax+8Ch]
        if (GameInstance->IsA(DBDGameInstanceClass))
        {
            // Check if the GameInstance object is still valid and not being destroyed
            // Based on _D: test al, 1 (checking FUObjectItem flags)
            if (GUObjectArray.IsValidIndex(GameInstance->InternalIndex))
            {
                // Accessing EasyAntiCheat component (assumed at offset 0x500 of GameInstance based on _P/_I)
                /* UNDEFINED ELEMENT */
                UDBDEasyAntiCheat* EACComponent = GameInstance->_eac;

                if (EACComponent)
                {
                    // Verify EACComponent object validity in GUObjectArray
                    if (GUObjectArray.IsValidIndex(EACComponent->InternalIndex))
                    {
                        // Remove the initialization delegate now that it has fired
                        // Based on _D: call TBaseMulticastDelegate::RemoveDelegateInstance
                        /* UNDEFINED ELEMENT */
                        EACComponent->OnInitComplete.RemoveDelegateInstance(this->_initEACHandle);
                    }
                }
            }
        }
    }

    // Branching logic based on initialization success
    if (success)
    {
        // EAC was successful, proceed to profile loading
        this->InitPlayerProfile();
    }
    else
    {
        // EAC failed, trigger game initialization completion with an error state
        // State 2 corresponds to ELoadCompleteState::EACError
        this->GameInitComplete(ELoadCompleteState::EACError);
    }
}




void ADBDGame_Start::GameInitComplete(ELoadCompleteState completionState)
{
    // Retrieve the World pointer
    UWorld* World = this->GetWorld();

    // Determine the correct TimerManager.
    // The disassembly checks the GameInstance first, then falls back to the World's TimerManager.
    FTimerManager* TimerManager = nullptr;
    if (World->GetGameInstance())
    {
        TimerManager = &World->GetGameInstance()->GetTimerManager();
    }
    else
    {
        TimerManager = &World->GetTimerManager();
    }

    // Clear the timeout timer used during initialization
    // FTimerManager::InternalClearTimer is the internal implementation of ClearTimer
    TimerManager->ClearTimer(this->_timeoutHandle);

    // Explicitly invalidate the handle (as seen in ASM: mov [rdi+4D8h], rbp)
    this->_timeoutHandle.Invalidate();

    // Handle Success State
    if (completionState == ELoadCompleteState::Success)
    {
        // Retrieve the Game Instance as a generic Actor -> GameInstance cast
        UGameInstance* GenericGameInstance = this->GetGameInstance();

        if (GenericGameInstance)
        {
            // The ASM performs a raw ClassTreeIndex check here to verify inheritance.
            // In standard C++, this is equivalent to checking if the instance is of type UDBDGameInstance.
            UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GenericGameInstance);

            // The ASM also checks GUObjectArray flags (0x20000000) to ensure the object is not Unreachable/PendingKill.
            // IsValid() covers these checks.
            if (DBDGameInstance && IsValid(DBDGameInstance))
            {
                // Retrieve persistent data for the local player
                FPlayerPersistentData* PersistentData = DBDGameInstance->GetLocalPlayerPersistentData();

                if (PersistentData)
                {
                    // Validate the data. Second argument in ASM is 0 (false).
                    PersistentData->ValidateData(false);
                }
            }
        }
    }

    // Prepare strings for Analytics and Logging
    // The ASM calls a specific static helper Enum::ToString for these types.
    /* UNDEFINED ELEMENT */
    FString ProgressString = Enum::ToString(this->_loadProgress);

    /* UNDEFINED ELEMENT */
    FString StateString = Enum::ToString(this->_loadCompletionState);

    // Format: "State : Progress"
    FString ResponseString = FString::Printf(TEXT("%s : %s"), *StateString, *ProgressString);

    // Send analytics data
    // The ASM explicitly passes: Success (bool), the formatted string, and the current UTC ticks.
    bool bIsSuccess = (completionState == ELoadCompleteState::Success);
    this->SendMirrorsAnalytics(bIsSuccess, ResponseString, FDateTime::UtcNow());

    // Update the internal load progress to 'Complete' (Enum value 9 in ASM corresponds to Complete)
    this->SetLoadProgress(ELoadProgress::Complete);

    // Update the local completion state member
    this->_loadCompletionState = completionState;

    // Logging Logic
    // ASM Checks: if (GameFlow.Verbosity >= 5)
    // 5 corresponds to ELogVerbosity::Log or ELogVerbosity::Verbose depending on the engine version mapping.
    // Assuming 'GameFlow' is a custom log category declared via DEFINE_LOG_CATEGORY_STATIC or similar.
    if (UE_LOG_ACTIVE(GameFlow, Log))
    {
        /* UNDEFINED ELEMENT */
        FString NewStateString = Enum::ToString(completionState);

        UE_LOG(GameFlow, Log, TEXT("[GameInit] New Init Complete State: %s"), *NewStateString);
    }

    // Broadcast the completion delegate
    if (this->OnDBDInitComplete.IsBound())
    {
        this->OnDBDInitComplete.Broadcast(completionState, this->_loadProgress);
    }
}




void ADBDGame_Start::InitDBD()
{
    // Initialize the load progress to the starting state
    this->SetLoadProgress(ELoadProgress::Init);

    // Retrieve the GameInstance using the Actor's helper function
    UGameInstance* GameInstance = this->GetGameInstance();

    // Check if GameInstance exists
    if (GameInstance == nullptr)
    {
        // If GameInstance is missing, signal an initialization error (Error Code 7)
        this->GameInitComplete(ELoadProgress::InitError);
    }
    else
    {
        // Attempt to cast the generic GameInstance to the specific UDBDGameInstance
        // The disassembly compares ClassTreeIndex to verify inheritance (standard Cast implementation)
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

        // Check if the cast was successful (part of the ClassTreeIndex check in ASM)
        if (DBDGameInstance == nullptr)
        {
            this->GameInitComplete(ELoadProgress::InitError);
        }
        else
        {
            // Verify if the object is pending kill or unreachable (InternalIndex check in ASM)
            // The bitwise operation (*(uint32_t*)((char*)rdx_1 + 8) >> 0x1d) checks flags like EInternalObjectFlags::PendingKill
            if (IsValid(DBDGameInstance) == false)
            {
                this->GameInitComplete(ELoadProgress::InitError);
            }
            else
            {
                // ---------------------------------------------------------------------------------
                // Timer Setup
                // The disassembly (lines 140262413 - 1402625e0) performs raw memory allocation
                // to construct an FTimerUnifiedDelegate. In C++, this translates to SetTimer.
                // ---------------------------------------------------------------------------------

                // Access the TimerManager via the World (vtable[0x21] on 'this' usually resolves to GetWorld logic)
                FTimerManager& TimerManager = this->GetWorld()->GetTimerManager();

                // Set a timer to call OnInitTimeout after 30.0 seconds
                TimerManager.SetTimer(
                    this->_timeoutHandle,
                    this,
                    &ADBDGame_Start::OnInitTimeout,
                    30.0f,
                    false // Loop = false
                );

                // Update progress state
                this->_loadProgress = ELoadProgress::CheckingPlatformService;

                // ---------------------------------------------------------------------------------
                // Logging Block (CheckingPlatformService)
                // ---------------------------------------------------------------------------------
                // Check Verbosity level on global/static GameFlow struct
                if (/* UNDEFINED ELEMENT */ GameFlow.Verbosity >= 5)
                {
                    // Convert Enum to String for logging
                    FString EnumAsString = /* UNDEFINED ELEMENT */ Enum::ToString(ELoadProgress::CheckingPlatformService);

                    // Log the progress change. 
                    // FMsg::Logf_Internal__VA corresponds to UE_LOG logic.
                    UE_LOG(LogDBDGame, Log, TEXT("[GameInit] New Init Progress: %s"), *EnumAsString);
                }

                // ---------------------------------------------------------------------------------
                // Module Loading and Service Check ("OnlinePresence")
                // ---------------------------------------------------------------------------------
                // Load the "OnlinePresence" module. The ASM marks it as FAdvertising, but the string is "OnlinePresence".
                // We use a generic IModuleInterface or specific interface pointer.
                void* OnlinePresenceModule = FModuleManager::LoadModuleChecked<void>(TEXT("OnlinePresence"));

                // Access the module's VTable to call a function at offset 0x88.
                // This function likely checks if the service is available/initialized.
                // rax_28 = (*(uint64_t*)((char*)vtable + 0x88))(rax_27, vtable);
                bool bIsServiceAvailable = (bool) /* UNDEFINED VTABLE */ ((bool(*)(void*))(*(void***)OnlinePresenceModule)[17])(OnlinePresenceModule); // 0x88 = 136 bytes = index 17 (if 64-bit pointers)

                if (bIsServiceAvailable == false)
                {
                    // If service check fails, report error
                    this->GameInitComplete(ELoadProgress::InitError);
                }
                else
                {
                    // Update progress state
                    this->_loadProgress = ELoadProgress::ConnectingToMirrors;

                    // ---------------------------------------------------------------------------------
                    // Logging Block (ConnectingToMirrors)
                    // ---------------------------------------------------------------------------------
                    if (/* UNDEFINED ELEMENT */ GameFlow.Verbosity >= 5)
                    {
                        FString EnumAsString = /* UNDEFINED ELEMENT */ Enum::ToString(ELoadProgress::ConnectingToMirrors);
                        UE_LOG(LogDBDGame, Log, TEXT("[GameInit] New Init Progress: %s"), *EnumAsString);
                    }

                    // ---------------------------------------------------------------------------------
                    // Auth Ticket Logic
                    // ---------------------------------------------------------------------------------
                    // Reload/Access the module again to bind a delegate.
                    // The ASM accesses offset 0x1E (30) in the module structure. This is likely an Event/Delegate property.
                    // It binds ADBDGame_Start::AuthTicketReceived to it.

                    // Note: 0x1E (30) is an odd offset for a pointer in 64-bit architecture (usually aligned to 8). 
                    // However, strictly following the instruction to not replace undefined elements:

                    this->_generateAuthTicketHandle = /* UNDEFINED ELEMENT */
                        ((FOnAuthTicketGenerated*)((char*)OnlinePresenceModule + 0x1E))->AddUObject(this, &ADBDGame_Start::AuthTicketReceived).ID;

                    // Call a virtual function at offset 0x58 (88) on the module to trigger the auth flow.
                    // (*(uint64_t*)((char*)vtable_1 + 0x58))(rax_35, vtable_1);
                    /* UNDEFINED VTABLE */
                    ((void(*)(void*))(*(void***)OnlinePresenceModule)[11])(OnlinePresenceModule); // 0x58 = 88 bytes = index 11
                }
            }
        }
    }
}




void ADBDGame_Start::InitPlayerProfile()
{
    // Set the initial load progress. 
    // The value 0x6 corresponds to the disassembly instruction 'mov dl, 0x6'.
    this->SetLoadProgress(6);

    // Retrieve the GameInstance and cast it to UDBDGameInstance.
    // The assembly performs manual class tree checks, which corresponds to the standard Cast<T> in UE4.
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Validation check for GameInstance.
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Retrieve the local PlayerController.
    ADBDPlayerControllerBase* LocalPlayerController = DBDGameInstance->GetLocalDBDPlayerController();

    // Validation check for PlayerController.
    if (LocalPlayerController == nullptr)
    {
        return;
    }

    // Check specific bit flag on the PlayerController (Offset 0x140, bit 0x4).
    // If the bit is set, the function returns.
    // This is a custom member variable check not standard to UE4.
    bool bControllerFlag = (*(uint8*)((uint8*)LocalPlayerController + 0x140) & 0x4) != 0;

    if (bControllerFlag)
    {
        return;
    }

    // Retrieve the PlayerState from the Controller and cast to ADBDPlayerState.
    // Assembly offset 0x3A0 on the controller usually points to PlayerState.
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(LocalPlayerController->PlayerState);

    // Retrieve the Persistent Data Manager from the GameInstance (Offset 0x3B8).
    // We treat this as a void pointer or generic UObject because the specific class definition is not provided,
    // but the assembly performs pointer arithmetic on this address.
    uint8* PersistentDataManager = *(uint8**)((uint8*)DBDGameInstance + 0x3B8);

    if (PersistentDataManager)
    {
        // If PlayerState is valid, copy data to the Persistent Data Manager.
        if (DBDPlayerState)
        {
            // Call UDBDPersistentData::CopyData.
            // Argument 2: Pointer to destination/source at offset 0x168 of PersistentDataManager.
            // Argument 3: Boolean 'true' or value 1 (from 'mov r8b, 0x1').
            UDBDPersistentData::CopyData(DBDPlayerState, (void*)(PersistentDataManager + 0x168), true);
        }

        // Clear the PlayerLoadoutData at offset 0x1F0 of PersistentDataManager.
        FPlayerLoadoutData::Clear((void*)(PersistentDataManager + 0x1F0));
    }

    // Handle UniqueNetId and Online Presence.
    if (DBDPlayerState)
    {
        // Reference to the UniqueNetId from the PlayerState.
        // The assembly constructs a FUniqueNetIdRepl on the stack here.
        FUniqueNetIdRepl& UniqueNetId = DBDPlayerState->UniqueId;

        // Add the Player Data ID to the Persistent Data.
        UDBDPersistentData::AddKeepPlayerDataID((UObject*)PersistentDataManager, &UniqueNetId);

        // Access the DataStore via the OnlinePresencePlugin.
        // The result is an interface or object that processes the DataStore.
        /* UNDEFINED ELEMENT */
        auto DataStoreInterface = IOnlinePresencePlugin::DataStore(&UniqueNetId);

        if (DataStoreInterface.IsValid())
        {
            // Call a virtual function at offset 0xE0 on the DataStore interface.
            // This is likely an Initialization or Update call.
            /* UNDEFINED VTABLE */
            // (*(void(__fastcall **)(void*))(*(uint64_t*)DataStoreInterface.Get() + 0xE0))(DataStoreInterface.Get());
        }

        // The assembly handles destruction of the FUniqueNetIdRepl copy (var_38) here (decrementing ref counts),
        // which is handled automatically by C++ destructors at the end of scope.
    }

    // Update Steam Inventory via GameInstance.
    DBDGameInstance->DBD_UpdateSteamInventory();

    // Retrieve the PlayerDataStorageFacade from GameInstance (Offset 0x3C0).
    // We treat this as a generic pointer to access the delegate and members.
    UPlayerDataStorageFacade* StorageFacade = DBDGameInstance->_playerDataFacade;

    if (StorageFacade)
    {
        // Bind the OnPlayerProfileLoadComplete function to the delegate in StorageFacade.
        // Assembly calls ??$AddUObject@... (Delegate AddUObject).
        // The result Handle is stored in this->_loadProfileHandle (Offset 0x4F0).
        this->_loadProfileHandle = StorageFacade->OnProfileLoadComplete.AddUObject(this, &ADBDGame_Start::OnPlayerProfileLoadComplete);

        // Check if a profile load is already currently in progress.
        // Checks boolean at Offset 0xC0 of StorageFacade.
        bool bIsLoadingProfile = StorageFacade->_isLoadingFile;

        // Log a warning if a load is ongoing and Verbosity is high enough.
        if (bIsLoadingProfile)
        {
            // Check Log Verbosity (GameFlow category, Level 3/Warning).
            if (UE_LOG_ACTIVE(GameFlow, Warning))
            {
                UE_LOG(GameFlow, Warning, TEXT("We should never have an ongoing load queued when we start the game init flow, but it can happen in XB1 edge cases."));
            }
        }

        // Trigger the Full Profile Load.
        bool bLoadStarted = StorageFacade->LoadFullProfile();

        // If the load failed to start, log a warning.
        if (bLoadStarted == false)
        {
            if (UE_LOG_ACTIVE(GameFlow, Warning))
            {
                UE_LOG(GameFlow, Warning, TEXT("Issues with the Local_CloudLoadProfile. Can happen in XB1 edge cases"));
            }
        }
    }
}




void ADBDGame_Start::LoadPlayerCurrenciesFromMirrors()
{
    // Set the current load progress state to 'LoadingMirrorCurrencies' (Value 0x8 from disassembly)
    this->SetLoadProgress(LoadingMirrorCurrencies);

    // Retrieve the current GameInstance
    UGameInstance* GameInstance = this->GetGameInstance();

    // Check if GameInstance exists
    if (GameInstance == nullptr)
    {
        return;
    }

    // Cast generic GameInstance to the project-specific UDBDGameInstance
    // The disassembly performs a manual class hierarchy check (IsA/Cast logic)
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // Check if the cast was successful
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Ensure the GameInstance object is valid (not pending kill/garbage collected)
    // Disassembly checks GUObjectArray flags here
    if (IsValid(DBDGameInstance) == false)
    {
        return;
    }

    // Retrieve the local DBD Player Controller from the Game Instance
    ADBDPlayerControllerBase* LocalDBDPlayerController = DBDGameInstance->GetLocalDBDPlayerController();

    // Check if the Player Controller exists
    if (LocalDBDPlayerController == nullptr)
    {
        return;
    }

    // Ensure the Player Controller is valid (not pending kill) and check an internal state flag
    // Disassembly checks GUObjectArray flags (IsValid) and a specific bit at offset 0x140
    if (IsValid(LocalDBDPlayerController) == false)
    {
        return;
    }

    /* UNDEFINED ELEMENT: Additional check for bit 4 at offset 0x140 in ADBDPlayerControllerBase (e.g., if (LocalDBDPlayerController->SomeStateFlag & 0x4)) */
    // Since the flag definition is missing, we proceed assuming IsValid is the primary safety check.

    // Register a callback for when the currencies have finished loading
    // The handle is stored in _loadCurrenciesHandle (offset 0x500)
    // We use AddUObject to bind the member function OnPlayerCurrenciesLoadComplete to the delegate
    this->_loadCurrenciesHandle = LocalDBDPlayerController->OnMirrorCurrenciesLoadComplete.AddUObject(this, &ADBDGame_Start::OnPlayerCurrenciesLoadComplete);

    // Initialize the currency mirroring process on the local player controller
    LocalDBDPlayerController->Local_InitMirrorCurrencies();
}




void ADBDGame_Start::OnInitTimeout()
{
    // Initialize the response string indicating the timeout reason
    // The disassembly explicitly resizes an array and copies "Init Timeout" into it, 
    // which corresponds to the constructor or assignment of an FString in UE4.
    FString ResponseString = TEXT("Init Timeout");

    // Get the current Coordinated Universal Time (UTC)
    FDateTime CurrentTime = FDateTime::UtcNow();

    // Send analytics data regarding the connection attempt
    // 1st argument: 0 (false) - Likely indicates a failure or 'Success' flag set to false
    // 2nd argument: The response string defined above
    // 3rd argument: The timestamp in ticks
    this->SendMirrorsAnalytics(false, ResponseString, CurrentTime.GetTicks());

    // Finalize the game initialization process with a failure state
    // The value 0x6 corresponds to 'FailedBlockingNoDBDServer' based on the context
    this->GameInitComplete(FailedBlockingNoDBDServer);
}




void ADBDGame_Start::OnPlayerCurrenciesLoadComplete(bool success)
{
    // Retrieve the Game Instance
    UGameInstance* GameInstance = this->GetGameInstance();

    // Check if GameInstance is valid before proceeding
    if (GameInstance)
    {
        // Cast to the project-specific GameInstance
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

        // Ensure the cast was successful and the object is valid (not pending kill)
        if (DBDGameInstance)
        {
            if (IsValid(DBDGameInstance))
            {
                // Get the local Player Controller
                ADBDPlayerControllerBase* LocalDBDPlayerController = DBDGameInstance->GetLocalDBDPlayerController();

                // Check if the Player Controller exists
                if (LocalDBDPlayerController)
                {
                    // Ensure the Player Controller is valid
                    if (IsValid(LocalDBDPlayerController))
                    {
                        // Check an internal state flag on the Player Controller (Offset 0x140, bit 4)
                        // If the flag is set (jne jump in disassembly), we skip the unbinding process.
                        if (LocalDBDPlayerController->bActorIsBeingDestroyed == false)
                        {
                            // Unbind the delegate to prevent multiple calls
                            LocalDBDPlayerController->OnMirrorCurrenciesLoadComplete.Remove(this->_loadCurrenciesHandle);

                            // Invalidate the handle
                            this->_loadCurrenciesHandle.Reset();
                        }
                    }
                }
            }
        }
    }

    // Call GameInitComplete with status 0x1 (Likely 'Success' or 'Complete')
    // This function is called regardless of whether the cleanup above succeeded or failed
    this->GameInitComplete(Success);
}




void ADBDGame_Start::OnPlayerProfileLoadComplete(uint8_t success, const FString& errorString)
{
    // Retrieve the Game Instance to access global managers
    UGameInstance* GameInstance = this->GetGameInstance();

    if (GameInstance)
    {
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

        // Check if the current GameInstance is a type of UDBDGameInstance using the class tree
        if (IsValid(DBDGameInstance))
        {
                // Access the specific DBD Game Instance (offset 0x3C0 contains a sub-manager)
                /* UNDEFINED ELEMENT */
                // The disassembly shows access to GameInstance + 0x3C0, then + 0x30 for the delegate collection
                // This corresponds to removing the profile load handle.

                // Remove the delegate instance to prevent repeated calls
                // Equivalent to: DBDGameInstance->GetProfileManager()->OnProfileLoadComplete.Remove(this->_loadProfileHandle);
                /* UNDEFINED ELEMENT: TBaseMulticastDelegate<...>::RemoveDelegateInstance */

                this->_loadProfileHandle.Reset();
        }
    }

    // Logic for Analytics and Attribute tracking
    // The following section reconstructs the BHVR Analytics event "SaveFileLoad"
    TArray<FAnalyticsEventAttribute> AnalyticsAttributes; /* UNDEFINED ELEMENT: Reconstructed from Original_6 */

    // Add "ProfileLoadSuccess_i" attribute
    FString SuccessAttrKey = FString(TEXT("ProfileLoadSuccess_i"));
    bool bSuccessValue = (success != 0);

    // AddAttributes is a template helper for BHVR Analytics
    /* UNDEFINED ELEMENT */
    // this->AddAttributes(&AnalyticsAttributes, SuccessAttrKey, bSuccessValue);

    if (success == false)
    {
        // If loading failed, add "ProfileLoadError_szni" attribute with the error string
        FString ErrorAttrKey = FString(TEXT("ProfileLoadError_szni"));

        /* UNDEFINED ELEMENT */
        // this->AddAttributes(&AnalyticsAttributes, ErrorAttrKey, errorString);
    }

    // Record the analytics event "SaveFileLoad"
    FString EventName = FString(TEXT("SaveFileLoad"));

    /* UNDEFINED ELEMENT */
    UBHVRAnalytics::RecordEvent(EventName, AnalyticsAttributes);

    // Determine the next step of the game initialization flow
    if (success != false)
    {
        // If profile loaded successfully, proceed to load currencies
        this->LoadPlayerCurrenciesFromMirrors();
    }
    else
    {
        // If profile failed to load, terminate initialization with a recoverable error state (0x4)
        // 0x4 corresponds to EProviderStep::FailedRecoverableProfileLoad
        this->GameInitComplete(FailedRecoverableProfileLoad);
    }

    // Manual memory cleanup observed in disassembly for the temporary analytics structures
    // In standard UE4 C++, TArray and FString handles this via destructors.
}




void ADBDGame_Start::SendMirrorsAnalytics(uint8_t success, FString responseString, FDateTime callTime)
{
    // Create an array of attributes (referred to as Original_7 in logs)
    TArray<FAnalyticsEventAttribute> attributes;

    // Parameter Success_i (address 140272281)
    attributes.Add(FAnalyticsEventAttribute(TEXT("Success_i"), (int32_t)success));

    // If failure, add Response_sz (address 1402722ac)
    if (!success)
    {
        attributes.Add(FAnalyticsEventAttribute(TEXT("Response_sz"), responseString));
    }

    // Calculate time difference (address 140272352)
    // % 0x3e8 in the log is millisToRespond % 1000
    int32_t millisToRespond = (int32_t)((FDateTime::UtcNow().GetTicks() - callTime.GetTicks()) / 10000);
    attributes.Add(FAnalyticsEventAttribute(TEXT("MillisecondsToRespond_i"), millisToRespond % 1000));

    // Send event (address 1402723da)
    UBHVRAnalytics::RecordEvent(TEXT("MirrorsLoginRequest"), attributes);

    // In the log, responseString is forcibly cleared at the end (140272442)
    // In standard code, the FString destructor handles this at the end of the function.
}




void ADBDGame_Start::SetLoadProgress(ELoadProgress newProgress)
{
    // Set the new progress value
    this->_loadProgress = newProgress;

    // Check the verbosity level of the GameFlow log category
    // 5 corresponds to the "Verbose" level in Unreal Engine
    if (GameFlow.Verbosity < Verbose)
        return;

    // Convert Enum value to string (e.g., ELoadProgress::Connecting -> "Connecting")
    FString progressString = Enum::ToString(newProgress);

    // Output to log via internal Unreal Engine function
    // Equivalent to standard UE_LOG(LogGameFlow, Verbose, TEXT("[GameInit] New Init Progress: %s"), *progressString)
    UE_LOG(LogGameFlow, Verbose, TEXT("[GameInit] New Init Progress: %s"), *progressString);
}




void ADBDGame_Start::StartPlay(class ADBDGame_Start* this)
{
    // Call the parent class implementation
    Super::StartPlay();

    // Retrieve the current GameInstance
    UGameInstance* rax = this->GetGameInstance();

    // Check if the GameInstance is valid
    if (IsValid(rax))
    {
        // Cast to the specific UDBDGameInstance class
        UDBDGameInstance* rbx = Cast<UDBDGameInstance>(rax);

        // Check if the cast was successful (rbx is not null)
        if (IsValid(rbx))
        {
            // Initialize procedural generation data
            rbx->InitProceduralGenerationData();
        }
    }

    // Check the status of the Online Subsystem
    DBDOnlineUtils::CheckOnlineSubsystem();
}