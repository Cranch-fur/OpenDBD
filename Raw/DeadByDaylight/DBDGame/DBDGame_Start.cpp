void ADBDGame_Start::ADBDGame_Start(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // Calls AGameMode::AGameMode (0x14023B959)
{
    // The ASM manually initializes the VTable at 0x14023B965, which corresponds to the C++ constructor body entry.

    // Initialize primitive types. 
    // The ASM zeroes out registers and moves them into offsets 0x4A0-0x500.
    // While the Delegate (OnDBDInitComplete) and FDelegateHandles have their own constructors,
    // the code below explicitly sets the simple byte flags as seen in _D and _I.

    // Offset: 0x4D0
    this->_loadProgress = 0; // false

    // Offset: 0x4D2
    this->_deferredError = 0; // false

    // Note: The DelegateHandles (_timeoutHandle, _generateAuthTicketHandle, etc.) 
    // are zero-initialized by the ASM (xor eax, eax -> mov [rbx+Offset]).
    // In standard C++, their default constructors handle this invalidation.

    /* Configuration of GameMode standard classes.
       The ASM calls GetPrivateStaticClass for specific custom classes and assigns them
       to the inherited AGameMode members PlayerControllerClass (0x3F0) and PlayerStateClass (0x408).
    */

    // Address: 0x14023B9C0
    // Sets this->PlayerControllerClass to ADBDPlayerControllerBase::StaticClass()
    this->PlayerControllerClass = ADBDPlayerControllerBase::StaticClass();

    // Address: 0x14023B9D3
    // Sets this->PlayerStateClass to ADBDPlayerState::StaticClass()
    this->PlayerStateClass = ADBDPlayerState::StaticClass();
}




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

    if (bCheck20 == false || bCheck31 == true || bCheck38 == true)
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
    if (Auth != nullptr && Auth->IsPendingKill())
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
    if (GameInstance != nullptr)
    {
        // Get the specific class for DBD Game Instance for type checking
        UClass* DBDGameInstanceClass = UDBDGameInstance::StaticClass();

        // Perform an optimized IsA check using ClassTree indices (UE4 internal logic)
        // Based on _D: sub edx, [rax+88h] / cmp edx, [rax+8Ch]
        if (GameInstance->IsA(DBDGameInstanceClass) == true)
        {
            // Check if the GameInstance object is still valid and not being destroyed
            // Based on _D: test al, 1 (checking FUObjectItem flags)
            if (GUObjectArray.IsValidIndex(GameInstance->InternalIndex) == true)
            {
                // Accessing EasyAntiCheat component (assumed at offset 0x500 of GameInstance based on _P/_I)
                /* UNDEFINED ELEMENT */
                UDBDEasyAntiCheat* EACComponent = GameInstance->_eac;

                if (EACComponent != nullptr)
                {
                    // Verify EACComponent object validity in GUObjectArray
                    if (GUObjectArray.IsValidIndex(EACComponent->InternalIndex) == true)
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
    if (success == true)
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
    if (World->GetGameInstance() != nullptr)
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

        if (GenericGameInstance != nullptr)
        {
            // The ASM performs a raw ClassTreeIndex check here to verify inheritance.
            // In standard C++, this is equivalent to checking if the instance is of type UDBDGameInstance.
            UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GenericGameInstance);

            // The ASM also checks GUObjectArray flags (0x20000000) to ensure the object is not Unreachable/PendingKill.
            // IsValid() covers these checks.
            if (DBDGameInstance != nullptr && IsValid(DBDGameInstance) == true)
            {
                // Retrieve persistent data for the local player
                FPlayerPersistentData* PersistentData = DBDGameInstance->GetLocalPlayerPersistentData();

                if (PersistentData != nullptr)
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
    if (this->OnDBDInitComplete.IsBound() == true)
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

    if (bControllerFlag == true)
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

    if (PersistentDataManager != nullptr)
    {
        // If PlayerState is valid, copy data to the Persistent Data Manager.
        if (DBDPlayerState != nullptr)
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
    if (DBDPlayerState != nullptr)
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

    if (StorageFacade != nullptr)
    {
        // Bind the OnPlayerProfileLoadComplete function to the delegate in StorageFacade.
        // Assembly calls ??$AddUObject@... (Delegate AddUObject).
        // The result Handle is stored in this->_loadProfileHandle (Offset 0x4F0).
        this->_loadProfileHandle = StorageFacade->OnProfileLoadComplete.AddUObject(this, &ADBDGame_Start::OnPlayerProfileLoadComplete);

        // Check if a profile load is already currently in progress.
        // Checks boolean at Offset 0xC0 of StorageFacade.
        bool bIsLoadingProfile = StorageFacade->_isLoadingFile;

        // Log a warning if a load is ongoing and Verbosity is high enough.
        if (bIsLoadingProfile == true)
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