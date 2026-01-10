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