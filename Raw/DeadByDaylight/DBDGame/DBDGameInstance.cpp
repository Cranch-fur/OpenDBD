void UDBDGameInstance::AddActorKnowledgeCollection(UActorKnowledgeCollection* collection)
{
    // Verify that the passed pointer is not null before proceeding.
    if (collection == nullptr)
    {
        return;
    }

    // The disassembly explicitly checks the Global Object Array (GUObjectArray) 
    // to verify the object's internal flags (specifically checking against garbage collection flags).
    // In Unreal Engine 4.13, checking if an object is "Pending Kill" covers this logic.
    if (collection->IsPendingKill() == false)
    {
        // Add the validated collection to the instance's array.
        // The disassembly indicates an offset of 0x3f0, which corresponds to the _actorKnowledgeCollections member.
        this->_actorKnowledgeCollections.Add(collection);
    }
}




void UDBDGameInstance::AddAudioBank(UAkAudioBank* bank)
{
    // Verify that the input pointer is not null
    if (bank == nullptr)
    {
        return;
    }

    // Check the internal object flags to ensure the object is not marked for garbage collection.
    // The assembly performs a lookup in GUObjectArray and checks bit 29 (0x20000000), 
    // which corresponds to EInternalObjectFlags::PendingKill in UE4.
    if (bank->IsPendingKill() == false)
    {
        // Add the audio bank to the member array located at offset 0x408
        this->_audioBanks.Add(bank);
    }
}




void UDBDGameInstance::AddPlayer(ADBDPlayer* player)
{
    // Unlike previous add functions, this function does not perform a nullptr check
    // or a PendingKill check on the input argument. It directly adds the pointer to the array.
    
    // The member '_players' is located at offset 0x5e8.
    this->_players.Add(player);
}




void UDBDGameInstance::ApplyOfferingModification()
{
    // Retrieve the lighting modification value. 
    // 0x18 (24) corresponds to the LightingModification enum/tag.
    // 0xffffffff corresponds to INDEX_NONE (-1).
    float lightingModification = this->GetOfferingsModification(EOfferingEffectType::LightingModification, -1);

    // Retrieve the fog modification value.
    // 0x19 (25) corresponds to the FogModification enum/tag.
    float fogModification = this->GetOfferingsModification(EOfferingEffectType::FogModification, -1);

    // The disassembly adds 1.0f to the fog modification result before use.
    // addss xmm7, dword [rel __real@3f800000]
    float adjustedFogValue = fogModification + 1.0f;

    // Retrieve the Lighting Helper and apply the lighting modification.
    // Note: The disassembly does not perform a nullptr check on the result of GetLightingHelper.
    ULightingHelper* lightingHelper = this->GetLightingHelper();
    lightingHelper->SetOfferingExposureMultiplier(lightingModification);

    // Check if the procedural level builder (WeakPtr at offset 0x178) is valid.
    if (this->Builder.IsValid() == true)
    {
        // Resolve the weak pointer to get the builder instance.
        AProceduralLevelBuilder* levelBuilder = (AProceduralLevelBuilder*)this->Builder.Get();

        // Apply the adjusted fog value.
        levelBuilder->OfferingAdjustFog(adjustedFogValue);
    }
}




bool UDBDGameInstance::AreExtraConditionsMet() const
{
    // The function utilizes a Critical Section (offset 0x5c0) to ensure thread safety 
    // while accessing the shared array.
    // The disassembly explicitly attempts 'TryEnterCriticalSection' first, and falls back 
    // to 'EnterCriticalSection' if the lock cannot be acquired immediately.
    // We treat this as a standard Lock operation on the member variable.
    
    // Note: Casting 'this' to non-const might be required if the CriticalSection member 
    // is not marked as 'mutable' in the header.
    const_cast<UDBDGameInstance*>(this)->LoadAudioCriticalSection.Lock();

    // Read the number of elements in the pending load data array (offset 0x438).
    int32_t pendingBanksCount = this->_asyncAudioBanksPendingLoadData.Num();

    // Release the Critical Section lock.
    const_cast<UDBDGameInstance*>(this)->LoadAudioCriticalSection.Unlock();

    // If there are no pending banks (count is 0), the conditions are met.
    if (pendingBanksCount == 0)
    {
        return true;
    }

    return false;
}




void UDBDGameInstance::AuthorityReceivePlayerData(FUniqueNetIdRepl* id, FPlayerSavedProfileDataShared* playerProfileData, FPlayerLoadoutData* playerLoadout)
{
    // Stack variable for FString (used later for UpdateHostSettings), initialized to empty
    FString gameType;

    // Retrieve the GameState.
    // The ASM calls a virtual function at vtable[0x21] (likely GetWorld()) and adds offset 0xF0 (GameState).
    // We cast the result to ADBDGameState.
    UObject* WorldContext = /* UNDEFINED VTABLE */ nullptr; 
    ADBDGameState* DBDGameState = nullptr;
    if (WorldContext)
    {
        DBDGameState = *(ADBDGameState**)((uint8_t*)WorldContext + 0xF0);
    }

    // Validate the GameState object (Standard UE4 IsValidLowLevel check logic seen in ASM)
    if (DBDGameState && DBDGameState->IsValidLowLevel())
    {
        // ---------------------------------------------------------
        // 1. Prepare Persistent Data
        // ---------------------------------------------------------

        // Create a local instance of PersistentData on the stack (var_3b8) to use as a fallback or intermediate
        FPlayerPersistentData LocalPersistentData;
        
        // Copy the UniqueNetId. The ASM manually increments the shared reference count, 
        // which matches the copy constructor behavior of FUniqueNetIdRepl.
        FUniqueNetIdRepl PlayerIdCopy = *id;

        // Try to get existing persistent data for this player from the manager
        FPlayerPersistentData* DataToUpdate = UDBDPersistentData::GetPlayerPersistentData(this->_persistentData, &PlayerIdCopy);

        // If no data exists yet, point to our local stack variable
        if (DataToUpdate == nullptr)
        {
            DataToUpdate = &LocalPersistentData;
        }

        // ---------------------------------------------------------
        // 2. Populate Data from Profile
        // ---------------------------------------------------------

        // Copy Arrays and Strings
        DataToUpdate->SavedData.SharedData.PlayerName = playerProfileData->PlayerName;
        DataToUpdate->SavedData.SharedData.SteamId = playerProfileData->SteamId;
        
        // Copy Primitive Stats
        DataToUpdate->SavedData.SharedData.SelectedCamperIndex = playerProfileData->SelectedCamperIndex;
        DataToUpdate->SavedData.SharedData.SelectedSlasherIndex = playerProfileData->SelectedSlasherIndex;
        DataToUpdate->SavedData.SharedData.SelectedCharacterCustomization = playerProfileData->SelectedCharacterCustomization;
        DataToUpdate->SavedData.SharedData.SlasherSkulls = playerProfileData->SlasherSkulls;
        DataToUpdate->SavedData.SharedData.CamperSkulls = playerProfileData->CamperSkulls;
        DataToUpdate->SavedData.SharedData.CamperStreak = playerProfileData->CamperStreak;

        // Copy Loadout using specific legacy operator found in ASM
        // ASM: call FLegacySavedPlayerLoadoutData::operator=
        DataToUpdate->CurrentLoadout = *playerLoadout;

        // Synchronize previous skulls
        DataToUpdate->PreviousSlasherSkulls = DataToUpdate->SavedData.SharedData.SlasherSkulls;
        DataToUpdate->PreviousCamperSkulls = DataToUpdate->SavedData.SharedData.CamperSkulls;

        // ---------------------------------------------------------
        // 3. Experience Validation
        // ---------------------------------------------------------

        // Calculate max experience and clamp the local experience value
        int32_t MaxExperience = FPlayerPersistentData::GetMaxExperience(DataToUpdate);
        int32_t LocalExperience = DataToUpdate->SavedData.LocalData._experience;

        if (LocalExperience < 0)
        {
            MaxExperience = 0;
        }
        else if (LocalExperience < MaxExperience)
        {
            MaxExperience = LocalExperience;
        }

        DataToUpdate->PreviousExperience = MaxExperience;

        // ---------------------------------------------------------
        // 4. Save Data to Manager
        // ---------------------------------------------------------

        // Prepare ID for Set operation (Ref count incremented again in ASM)
        FUniqueNetIdRepl SetPlayerId = *id;
        UDBDPersistentData::SetPlayerPersistentData(this->_persistentData, &SetPlayerId, DataToUpdate);

        // ---------------------------------------------------------
        // 5. Sync with Player State
        // ---------------------------------------------------------

        // Iterate through the PlayerArray in the GameState.
        // ASM accesses offset 0x418 on GameState, then offset 0x3b0 (Data) and 0x3b8 (Count).
        // This suggests accessing a TArray<ADBDPlayerState*> located at 0x418 in ADBDGameState.
        
        /* UNDEFINED ELEMENT: Direct access to specific TArray at offset 0x418 in ADBDGameState */
        TArray<APlayerState*>* PlayerArrayPtr = (TArray<APlayerState*>*)((uint8_t*)DBDGameState + 0x418);
        
        if (PlayerArrayPtr)
        {
            // Iterate backwards
            for (int32_t i = PlayerArrayPtr->Num() - 1; i >= 0; --i)
            {
                APlayerState* CurrentPlayerState = (*PlayerArrayPtr)[i];

                // Ensure the object is valid
                if (CurrentPlayerState == nullptr)
                {
                    continue;
                }

                // Dynamic Cast check logic found in ASM (ClassTreeIndex comparison)
                // ASM: call ADBDPlayerState::GetPrivateStaticClass
                ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(CurrentPlayerState);
                if (DBDPlayerState)
                {
                    // Prepare strings for comparison. Use empty string if array is empty.
                    const wchar_t* ProfileNameStr = (playerProfileData->PlayerName.Data.Num() == 0) ? TEXT("") : playerProfileData->PlayerName.Data.GetData();
                    const wchar_t* StateNameStr = (DBDPlayerState->PlayerName.Data.Num() == 0) ? TEXT("") : DBDPlayerState->PlayerName.Data.GetData();

                    // Case-insensitive comparison
                    // ASM: call FGenericPlatformStricmp
                    if (FGenericPlatformStricmp::Stricmp(StateNameStr, ProfileNameStr) == 0)
                    {
                        // Match found: Sync data bi-directionally
                        UDBDPersistentData::CopyData(DBDPlayerState, DataToUpdate, 0);
                        UDBDPersistentData::CopyData(DataToUpdate, DBDPlayerState, 0);

                        // Call undefined virtual function at offset 0x568
                        /* UNDEFINED VTABLE */
                        // typedef void (*FuncType)(ADBDPlayerState*);
                        // FuncType VirtualFunc = *(FuncType*)(*(uint64_t*)DBDPlayerState + 0x568);
                        // VirtualFunc(DBDPlayerState);

                        break; // Stop iterating
                    }
                }
            }
        }

        // ---------------------------------------------------------
        // 6. Update Host Settings
        // ---------------------------------------------------------

        // Retrieve Context System
        UGameFlowContextSystem* ContextSystem = this->_contextSystem;
        if (ContextSystem)
        {
            // Retrieve OnlineSystemHandler
            UOnlineSystemHandler* OnlineHandler = ContextSystem->m_OnlineSystemHandler;

            if (OnlineHandler)
            {
                // Retrieve ServerInstance
                UDBDServerInstance* ServerInstance = OnlineHandler->_serverInstance;
                if (ServerInstance)
                {
                    // Update host settings using an empty string
                    // ASM: mov qword [rsp+0x20], 0 (clears the string's allocator data)
                    gameType.Empty(); 
                    UDBDServerInstance::UpdateHostSettings(ServerInstance, &gameType);
                }
            }
        }
    }

    // ---------------------------------------------------------
    // 7. Cleanup
    // ---------------------------------------------------------
    // Destructors for LocalPersistentData and gameType FString are called implicitly here.
    // ASM explicitly shows FMemory::Free calls for the TArray data buffers.
    
    /* UNREAL AUTO GENERATED FUNCTION */
}




void UDBDGameInstance::BeginLoadingScreen(const FString* MapName)
{
    // Create a local instance of loading screen attributes structure
    // This corresponds to the stack allocation at the beginning of the function
    FLoadingScreenAttributes LoadingScreenAttributes;

    // 14024766c: Set movies to be skippable (0x1)
    LoadingScreenAttributes.bMoviesAreSkippable = true;

    // 14024766c: Set wait for manual stop to false (0x0)
    LoadingScreenAttributes.bWaitForManualStop = false;

    // 140247676: The MoviePaths TArray is zero-initialized (memset in ASM) automatically by the constructor
    
    // 14024768b - 140247693: Set the minimum display time. 
    // The value 0xbf800000 corresponds to float -1.0f
    LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = -1.0f;

    // 140247699: Set playback type to Normal (MT_Normal = 0)
    LoadingScreenAttributes.PlaybackType = EMoviePlaybackType::MT_Normal;

    // 14024769d: Set auto-completion to false
    LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false;

    // 1402476a1 - 14024773d: Create the widget and assign it to the attributes.
    // The massive block of assembly involving FReferenceControllerBase is the 
    // internal inlined code for TSharedPtr assignment/construction.
    LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

    // 140247742: Call GetMoviePlayer function. 
    // The pseudo code indicates it returns a SharedPointer wrapper, checking Object access.
    // 140247752: Call virtual function at offset 0x28 (SetupLoadingScreen) with attributes.
    // /* UNDEFINED ELEMENT */ - GetMoviePlayer appears to be a global helper or static accessor in this context.
    IGameMoviePlayer* MoviePlayer = GetMoviePlayer();
    if (MoviePlayer)
    {
        MoviePlayer->SetupLoadingScreen(LoadingScreenAttributes);
    }

    // 140247781 - End: The function epilogue handles the destruction of local variables.
    // The loops involving FMemory::Free in the disassembly correspond to the 
    // destructor of LoadingScreenAttributes (specifically the MoviePaths TArray) 
    // and the TSharedPtr WidgetLoadingScreen going out of scope.
}




void UDBDGameInstance::ClearOnCloseRequested()
{
    // Unbind the delegate located at offset 0x440.
    // The generic FDelegateBase::Unbind is called to clear the bindings.
    this->_onCloseRequested.Unbind();
}




bool UDBDGameInstance::ConsumeDetectedNetworkConnectionFailure()
{
    // Capture the current state of the network failure flag
    const bool bWasFailureDetected = this->_networkFailureDetected;

    // Reset the internal flag to false (0) so it isn't processed again
    this->_networkFailureDetected = false;

    // Return the state as it was before the reset
    return bWasFailureDetected;
}




bool UDBDGameInstance::ConsumeShowingNetworkError()
{
    // Capture the current state of the network error flag
    // The disassembly reads the byte at offset 0x9b1
    bool bWasShowing = this->_showingNetworkError;

    // Reset the flag to false (0)
    // The disassembly writes 0 to the byte at offset 0x9b1
    this->_showingNetworkError = false;

    // Return the state as it was before the reset
    return bWasShowing;
}




bool UDBDGameInstance::ConsumeTravelToSplash()
{
    // Capture the current value of the member variable before modifying it.
    // Based on the disassembly 'movzx eax, byte [rcx+0x9b0]', this reads a byte/bool value.
    const bool bWasTraveling = this->_travelingToSplash;

    // Reset the member variable to false (0).
    // This effectively "consumes" the flag so it cannot be triggered again immediately.
    this->_travelingToSplash = false;

    // Return the original value found before the reset.
    return bWasTraveling;
}




void UDBDGameInstance::CreateEACInterface()
{
    // Check if the _eac instance already exists and is valid.
    // The disassembly performs a direct check on GUObjectArray flags (specifically checking against pending kill/unreachable),
    // which is the internal implementation of the IsValid() helper in Unreal Engine.
    if (IsValid(this->_eac))
    {
        return;
    }

    // Create a new instance of UDBDEasyAntiCheat.
    // The disassembly calls FObjectInitializer::AssertIfInConstructor and StaticConstructObject_Internal.
    // This is the standard compiled result of calling NewObject<T>() with the Transient Package as the outer.
    // The string "NewObject with empty name..." in the disassembly confirms this was a NewObject call.
    this->_eac = NewObject<UDBDEasyAntiCheat>(GetTransientPackage());

    // Assign 'this' (the UDBDGameInstance) to the member at offset 0xA0 of the created _eac object.
    // We check if creation was successful to be safe, though NewObject usually asserts on failure.
    if (this->_eac)
    {
        // We assume the member at offset 0xA0 is a pointer to the GameInstance (e.g., 'OwningGameInstance').
        this->_eac->GameInstance = this; // Offset: 0xA0
    }
}




void UDBDGameInstance::DBDApplyOfferingModification(float lightModifier, float fogModifier)
{
    // Retrieve the lighting helper.
    // The disassembly shows a call to GetLightingHelper, followed immediately by using the result
    // to call SetOfferingExposureMultiplier, without an explicit null check in between.
    class ULightingHelper* LightingHelper = this->GetLightingHelper();
    LightingHelper->SetOfferingExposureMultiplier(lightModifier);

    // Check if the Builder weak pointer is valid.
    // The disassembly explicitly calls FWeakObjectPtr::IsValid on the member at 0x178.
    if (this->Builder.IsValid())
    {
        // Dereference the weak pointer to get the actual object instance.
        class AProceduralLevelBuilder* LevelBuilder = this->Builder.Get();
        
        // Apply the fog modifier to the level builder.
        LevelBuilder->OfferingAdjustFog(fogModifier);
    }
}




void UDBDGameInstance::DBDDeleteLocalSaveFile()
{
    // 1. Initialize Online Subsystem
    FName SubsystemName;
    FMemory::Memset(&SubsystemName, 0, sizeof(FName)); // memset 0 implies None/Default
    
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(SubsystemName);

    // 2. Access the User Cloud Interface via VTable [0x40]
    if (OnlineSub == nullptr)
    {
        return;
    }

    // VTable Offset 0x40 maps to GetUserCloudInterface()
    TSharedPtr<IOnlineUserCloud> UserCloud = OnlineSub->GetUserCloudInterface();
    if (UserCloud.IsValid())
    {
        // 3. Get the Local Player and their UniqueNetId
        TSharedPtr<const FUniqueNetId> UniqueNetId = nullptr;
        ULocalPlayer* LocalPlayer = this->GetLocalPlayerByIndex(0);
        if (LocalPlayer)
        {
            UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
        }

        if (UniqueNetId.IsValid())
        {
            // The disassembly creates a Hex string from the UniqueID. 
            // Blocks 1-3 use a manual loop implementation, while Blocks 4-7 use GetHexEncodedString.
            // For readability and functionality, we utilize the standard UE4 helper.
            FString HexId = UniqueNetId->GetHexEncodedString();

            // Undefined String Literals (referenced in disassembly data segments)
            // These usually represent file prefixes or suffixes (e.g., "ProfileSave", "Inventory", etc.)
            const FString StringData_1 = TEXT("UNDEFINED_DATA_1"); /* UNDEFINED ELEMENT - data_143585758 */
            const FString StringData_2 = TEXT("UNDEFINED_DATA_2"); /* UNDEFINED ELEMENT - data_1435857a8 */
            const FString StringData_3 = TEXT("UNDEFINED_DATA_3"); /* UNDEFINED ELEMENT - data_143585798 */
            const FString StringData_4 = TEXT("UNDEFINED_DATA_4"); /* UNDEFINED ELEMENT - data_143585788 */
            const FString StringData_5 = TEXT("UNDEFINED_DATA_5"); /* UNDEFINED ELEMENT - data_143585778 */
            const FString StringData_6 = TEXT("UNDEFINED_DATA_6"); /* UNDEFINED ELEMENT - data_143585768 */
            const FString StringData_7 = TEXT("UNDEFINED_DATA_7"); /* UNDEFINED ELEMENT - data_1435857c8 */

            // --- Deletion Block 1 ---
            // Constructs filename: HexId + StringData_1
            FString Filename1 = HexId + StringData_1;
            
            // /* UNDEFINED VTABLE (0xC0) */ - Mapped to IOnlineUserCloud::DeleteUserFile via signature analysis
            // Args: (const FUniqueNetId&, const FString&, bool bCloud, bool bLocal)
            UserCloud->DeleteUserFile(*UniqueNetId, Filename1, true, true);

            // --- Deletion Block 2 ---
            // Constructs filename: HexId + StringData_2
            FString Filename2 = HexId + StringData_2;
            UserCloud->DeleteUserFile(*UniqueNetId, Filename2, true, true);

            // --- Deletion Block 3 ---
            // Constructs filename: HexId + StringData_3
            FString Filename3 = HexId + StringData_3;
            UserCloud->DeleteUserFile(*UniqueNetId, Filename3, true, true);

            // --- Deletion Block 4 ---
            // The disassembly logic shifts here. It concatenates the result of the previous string construction
            // with new data, or re-encodes. Based on flow: HexId + StringData_4
            FString Filename4 = HexId + StringData_4;
            UserCloud->DeleteUserFile(*UniqueNetId, Filename4, true, true);

            // --- Deletion Block 5 ---
            // Calls GetHexEncodedString explicitly again in disassembly
            FString HexIdRe = UniqueNetId->GetHexEncodedString(); 
            FString Filename5 = HexIdRe + StringData_5;
            UserCloud->DeleteUserFile(*UniqueNetId, Filename5, true, true);

            // --- Deletion Block 6 ---
            FString Filename6 = HexIdRe + StringData_6;
            UserCloud->DeleteUserFile(*UniqueNetId, Filename6, true, true);

            // --- Deletion Block 7 ---
            FString Filename7 = HexIdRe + StringData_7;
            UserCloud->DeleteUserFile(*UniqueNetId, Filename7, true, true);
        }
    }
    // SharedPtr ReferenceController destructor handles cleanup automatically in C++
}




void UDBDGameInstance::DBDForceLoad()
{
    // Load the full profile using the facade.
    // The disassembly accesses the _playerDataFacade member (offset 0x3c0) and calls the function.
    if (this->_playerDataFacade)
    {
        this->_playerDataFacade->LoadFullProfile();
    }

    // Retrieve the World from the GameInstance.
    // Assembly calls vtable function at offset 0x108 (index 33). In UGameInstance/UObject, this corresponds to GetWorld().
    UWorld* World = this->GetWorld();
    if (World == nullptr)
    {
        return;
    }

    // Access the global GEngine pointer.
    // The disassembly performs detailed checks against GUObjectArray to ensure GEngine is reachable.
    // In standard C++, checking the pointer validity and IsValid() logic suffices.
    if (GEngine == nullptr)
    {
        return;
    }

    // Attempt to get the first local PlayerController using the Engine and World.
    // Assembly: UEngine::GetFirstLocalPlayerController
    APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(World);
    if (LocalPlayerController == nullptr)
    {
        return;
    }

    // Cast the generic PlayerController to the specific ADBDPlayerControllerBase class.
    // The disassembly compares ClassTreeIndex and ClassTreeNumChildren to verify inheritance (logic of Cast<>).
    ADBDPlayerControllerBase* DBDController = Cast<ADBDPlayerControllerBase>(LocalPlayerController);
    // If the Cast failed, the result is nullptr.
    if (DBDController == nullptr)
    {
        return;
    }

    // Perform validity checks on the Controller.
    // The disassembly checks the Global Object Array flags (Unreachable) and a specific bitfield at offset 0x140 (PendingKill).
    // In UE4 C++, IsPendingKill() covers the object destruction state checks.
    if (DBDController->IsPendingKill())
    {
        return;
    }

    // If the controller is valid, load the profile statistics.
    // Assembly: ADBDPlayerControllerBase::Local_LoadProfileStats
    DBDController->Local_LoadProfileStats();
}




void UDBDGameInstance::DBDForceSave()
{
    // The disassembly performs a "tail call" optimization (JMP instead of CALL+RET).
    // It accesses the _playerDataFacade member variable (offset 0x3c0) to use as the 'this' context for the next call.
    // The register 'edx' is zeroed out (xor edx, edx), passing '0' as the first argument to the function.
    // In UE4, '0' usually represents 'false' for boolean flags or 'nullptr' for delegates.
    this->_playerDataFacade->SaveFullProfile(false);
}




void UDBDGameInstance::DBDLogFindSessions(int32_t searchType, int32_t rank, uint8_t ignoreVersion)
{
    // Retrieve the first local player associated with this game instance.
    // /* UNREAL AUTO GENERATED FUNCTION */
    ULocalPlayer* LocalPlayer = this->GetFirstGamePlayer();
    // Validate the LocalPlayer pointer (retrieved at the start).
    // The disassembly checks this late in the execution flow (address 14024e538).
    // Validate the LocalPlayer internal flags.
    if (LocalPlayer == nullptr || LocalPlayer->IsPendingKill())
    {
        return;
    }

    // Access the custom Context System variable.
    // /* UNDEFINED ELEMENT */ - _contextSystem is likely a custom member of UDBDGameInstance.
    class UGameFlowContextSystem* ContextSystem = this->_contextSystem;
    // Validate ContextSystem. The disassembly performs a Global Object Array lookup here 
    // to check internal flags (specifically RF_PendingKill/Unreachable).
    // Checking if the object is pending kill matches the bitwise operations in the disassembly (shr eax, 0x1d).
    if (ContextSystem == nullptr || ContextSystem->IsPendingKill())
    {
        return;
    }

    // Access the Online System Handler from the context system.
    // /* UNDEFINED ELEMENT */ - m_OnlineSystemHandler is a member of UGameFlowContextSystem.
    // Validate the OnlineSystemHandler internal flags via Global Object Array.
    class UOnlineSystemHandler* OnlineSystemHandler = ContextSystem->m_OnlineSystemHandler;
    // Validate the OnlineSystemHandler pointer.
    if (OnlineSystemHandler == nullptr || OnlineSystemHandler->IsPendingKill())
    {
        return;
    }

    // Double-check the Context System validity before the final call.
    // The disassembly repeats the Global Object check for ContextSystem here. 
    // If it fails now, it nulls the handler (though the previous returns usually catch this).
    if (ContextSystem == nullptr || ContextSystem->IsPendingKill())
    {
        OnlineSystemHandler = nullptr;
    }

    // Safety check: if the handler became null in the step above, we must return to avoid a crash.
    // Note: The assembly zeroes the register RBX (OnlineSystemHandler) and proceeds, 
    // but the following call requires a valid instance.
    if (OnlineSystemHandler == nullptr)
    {
        return;
    }

    // Prepare the UniqueNetId.
    // GetPreferredUniqueNetId returns a TSharedPtr.
    // /* UNREAL AUTO GENERATED FUNCTION */
    TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();

    // Call the logging function on the OnlineSystemHandler.
    // /* UNDEFINED ELEMENT */ - LogFindSessionSearch is a custom function.
    OnlineSystemHandler->LogFindSessionSearch(UniqueNetId, searchType, rank, ignoreVersion);
}




void UDBDGameInstance::DBDToggleLightingLoaded()
{
    // Define the name of the lighting level to toggle.
    // The assembly instantiates FName with "Ambiant01".
    FName LevelName = FName(TEXT("Ambiant01"));

    // Retrieve the streaming level object associated with this name.
    // /* UNREAL AUTO GENERATED FUNCTION */
    class ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);

    // Initialize LatentActionInfo.
    // The assembly explicitly sets the first 8 bytes to -1 (Linkage and UUID)
    // and the remaining bytes (ExecutionFunction and CallbackTarget) to 0.
    struct FLatentActionInfo LatentInfo;
    LatentInfo.Linkage = -1;
    LatentInfo.UUID = -1;
    LatentInfo.ExecutionFunction = NAME_None;
    LatentInfo.CallbackTarget = nullptr;

    // Check the internal state of the streaming level.
    // Assembly: test byte [rax+0xb0], 0x20
    // If the bit is SET (0x20), it falls through to Unload.
    // If the bit is CLEAR (0x00), it jumps to Load.
    // This implies: If (IsLoaded) -> Unload, Else -> Load.
    // Note: accessing bitfields directly is unsafe; using the standard accessor.
    
    bool bIsLevelLoaded = false;
    
    // Safety check: The assembly assumes GetStreamingLevel returns a valid pointer (dereferencing rax+0xb0 immediately).
    // In C++, strictly following the assembly logic:
    if (StreamingLevel)
    {
        bIsLevelLoaded = StreamingLevel->IsLevelLoaded();
    }

    if (bIsLevelLoaded)
    {
        // Path A: The level is loaded/active. Unload it.
        // Assembly passes: this, LevelName, LatentInfo. 
        // The boolean 'bShouldBlockOnUnload' is implicitly false (register r9 is not set).
        // /* UNREAL AUTO GENERATED FUNCTION */
        UGameplayStatics::UnloadStreamLevel(this, LevelName, LatentInfo, false);
    }
    else
    {
        // Path B: The level is not loaded. Load it.
        // Assembly sets r9b = 1 (bShouldBlockOnLoad) and r8d = 1 (bMakeVisibleAfterLoad).
        // /* UNREAL AUTO GENERATED FUNCTION */
        UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, LatentInfo);
    }
}




void UDBDGameInstance::DBD_AddAllItemsAddonsOfferingsToInventory(int32_t amount)
{
    // Define the lookup keys for the databases.
    // In the disassembly, these are passed as data pointers (e.g., data_143585588).
    const FName ItemDBId = FName(TEXT("ItemDB"));       // Placeholder name for data_143585588
    const FName AddonDBId = FName(TEXT("ItemAddonDB"));     // Placeholder name for data_143585598
    const FName OfferingDBId = FName(TEXT("OfferingDB")); // Placeholder name for data_143585608

    // This array will be reused for all three categories to store row names.
    TArray<FName> RowNames;

    // ---------------------------------------------------------
    // 1. Process Items
    // ---------------------------------------------------------
    
    // Retrieve the DataTable for Items from the global design tunables.
    // /* UNDEFINED ELEMENT */ - UDBDDesignTunables::_databases is a TSet<TPair<FName, UDataTable*>>.
    // The assembly performs a FindId and then pointer arithmetic to get the value.
    UDataTable* ItemTable = nullptr;
    if (UDBDDesignTunables::_databases.Contains(ItemDBId))
    {
        ItemTable = UDBDDesignTunables::_databases[ItemDBId];
    }

    // If the table exists, process it.
    if (ItemTable)
    {
        // /* UNREAL AUTO GENERATED FUNCTION */
        RowNames = ItemTable->GetRowNames();

        // Iterate through all item names found in the table.
        for (const FName& ItemID : RowNames)
        {
            // Add the item 'amount' times.
            if (amount > 0)
            {
                int32_t i = amount;
                while (i != 0)
                {
                    // /* UNDEFINED ELEMENT */
                    this->DBD_AddItemToInventory(ItemID);
                    i--;
                }
            }
        }
    }

    // ---------------------------------------------------------
    // 2. Process Addons
    // ---------------------------------------------------------

    // Retrieve the DataTable for Addons.
    UDataTable* AddonTable = nullptr;
    if (UDBDDesignTunables::_databases.Contains(AddonDBId))
    {
        AddonTable = UDBDDesignTunables::_databases[AddonDBId];
    }

    if (AddonTable)
    {
        // Reuse RowNames array. The C++ assignment operator handles the 
        // FMemory::Free logic seen in assembly (14024e804).
        // /* UNREAL AUTO GENERATED FUNCTION */
        RowNames = AddonTable->GetRowNames();

        for (const FName& AddonID : RowNames)
        {
            if (amount > 0)
            {
                int32_t j = amount;
                while (j != 0)
                {
                    // /* UNDEFINED ELEMENT */
                    this->DBD_AddItemAddonToInventory(AddonID);
                    j--;
                }
            }
        }
    }

    // ---------------------------------------------------------
    // 3. Process Offerings
    // ---------------------------------------------------------

    // Retrieve the DataTable for Offerings.
    UDataTable* OfferingTable = nullptr;
    if (UDBDDesignTunables::_databases.Contains(OfferingDBId))
    {
        OfferingTable = UDBDDesignTunables::_databases[OfferingDBId];
    }

    if (OfferingTable)
    {
        // Reuse RowNames array again.
        // /* UNREAL AUTO GENERATED FUNCTION */
        RowNames = OfferingTable->GetRowNames();

        for (const FName& OfferingID : RowNames)
        {
            if (amount > 0)
            {
                int32_t k = amount;
                while (k != 0)
                {
                    // /* UNDEFINED ELEMENT */
                    this->DBD_AddOfferingToInventory(OfferingID);
                    k--;
                }
            }
        }
    }

    // The TArray 'RowNames' goes out of scope here, triggering the final 
    // FMemory::Free logic seen at 14024e988.
}




void UDBDGameInstance::DBD_AddBloodPoints(int32_t count)
{
    // Retrieve the persistent data for the local player.
    // /* UNDEFINED ELEMENT */ - GetLocalPlayerPersistentData is a custom function.
    struct FPlayerPersistentData* PersistentData = this->GetLocalPlayerPersistentData();
    // Verify the data exists before proceeding.
    if (PersistentData == nullptr)
    {
        return;
    }

    // Define the source of the currency addition.
    // The assembly manually constructs a string "Cheat" here.
    FString Source = FString(TEXT("Cheat"));

    // Retrieve the player state.
    // /* UNDEFINED ELEMENT */ - GetLocalPlayerState is a custom function.
    class APlayerState* PlayerState = this->GetLocalPlayerState();

    // Add the experience points (BloodPoints) to the persistent data.
    // /* UNDEFINED ELEMENT */ - AddExperience is a member of FPlayerPersistentData.
    PersistentData->AddExperience(count, Source, PlayerState);

    // Access the Player Data Storage Facade to save the profile.
    // /* UNDEFINED ELEMENT */ - _playerDataFacade is a member of UDBDGameInstance.
    class UPlayerDataStorageFacade* PlayerDataFacade = this->_playerDataFacade;

    // Save the profile immediately.
    // The assembly passes '0' as the second argument.
    // /* UNDEFINED ELEMENT */
    if (PlayerDataFacade)
    {
        PlayerDataFacade->SaveFullProfile(false);
    }
}




void UDBDGameInstance::DBD_AddCamperSkulls(int32_t count)
{
    // Call the generic AddSkulls function.
    // The disassembly passes '0' as the third argument, which represents the Camper role.
    // /* UNDEFINED ELEMENT */
    this->DBD_AddSkulls(count, 0);
}




void UDBDGameInstance::DBD_AddEverythingToInventory()
{
    // Define the lookup keys for the databases based on the distinct data addresses in assembly.
    const FName ItemDBId = FName(TEXT("ItemDB"));       // data_143585588
    const FName AddonDBId = FName(TEXT("ItemAddonDB"));     // data_143585598
    const FName PerkDBId = FName(TEXT("PerkDB"));       // data_1435855b8
    const FName OfferingDBId = FName(TEXT("OfferingDB")); // data_143585608

    // Reusable array to hold row names, matching the memory reuse pattern in assembly.
    TArray<FName> RowNames;
    UDataTable* TableToProcess = nullptr;

    // ---------------------------------------------------------
    // 1. Process Items
    // ---------------------------------------------------------

    // /* UNDEFINED ELEMENT */ - Accessing the custom _databases map.
    if (UDBDDesignTunables::_databases.Contains(ItemDBId))
    {
        TableToProcess = UDBDDesignTunables::_databases[ItemDBId];
    }
    else
    {
        TableToProcess = nullptr;
    }

    if (TableToProcess)
    {
        // /* UNREAL AUTO GENERATED FUNCTION */
        RowNames = TableToProcess->GetRowNames();

        for (const FName& ItemID : RowNames)
        {
            // /* UNDEFINED ELEMENT */
            this->DBD_AddItemToInventory(ItemID);
        }
    }

    // ---------------------------------------------------------
    // 2. Process Addons
    // ---------------------------------------------------------

    if (UDBDDesignTunables::_databases.Contains(AddonDBId))
    {
        TableToProcess = UDBDDesignTunables::_databases[AddonDBId];
    }
    else
    {
        TableToProcess = nullptr;
    }

    if (TableToProcess)
    {
        // Reassigning RowNames frees the previous memory and allocates new for Addons.
        // /* UNREAL AUTO GENERATED FUNCTION */
        RowNames = TableToProcess->GetRowNames();

        for (const FName& AddonID : RowNames)
        {
            // /* UNDEFINED ELEMENT */
            this->DBD_AddItemAddonToInventory(AddonID);
        }
    }

    // ---------------------------------------------------------
    // 3. Process Perks
    // ---------------------------------------------------------
    // Note: This block was absent in the previous function but is present here at 14024ecf7.

    if (UDBDDesignTunables::_databases.Contains(PerkDBId))
    {
        TableToProcess = UDBDDesignTunables::_databases[PerkDBId];
    }
    else
    {
        TableToProcess = nullptr;
    }

    if (TableToProcess)
    {
        // /* UNREAL AUTO GENERATED FUNCTION */
        RowNames = TableToProcess->GetRowNames();

        for (const FName& PerkID : RowNames)
        {
            // /* UNDEFINED ELEMENT */
            this->DBD_AddPerkToInventory(PerkID);
        }
    }

    // ---------------------------------------------------------
    // 4. Process Offerings
    // ---------------------------------------------------------

    if (UDBDDesignTunables::_databases.Contains(OfferingDBId))
    {
        TableToProcess = UDBDDesignTunables::_databases[OfferingDBId];
    }
    else
    {
        TableToProcess = nullptr;
    }

    if (TableToProcess)
    {
        // /* UNREAL AUTO GENERATED FUNCTION */
        RowNames = TableToProcess->GetRowNames();

        for (const FName& OfferingID : RowNames)
        {
            // /* UNDEFINED ELEMENT */
            this->DBD_AddOfferingToInventory(OfferingID);
        }
    }

    // Implicit FMemory::Free occurs here as RowNames goes out of scope.
}




void UDBDGameInstance::DBD_AddFearTokens(int32_t count)
{
    // 1. Get the data structure
    FPlayerPersistentData* playerData = this->GetLocalPlayerPersistentData();
    // Null check (safety)
    if (playerData == nullptr)
    {
        return;
    }

    // 2. Get the hard cap for tokens
    int32_t maxTokens = playerData->GetMaxFearTokens();

    // 3. Get current tokens from offset 0x300
    int32_t currentTokens = playerData->SavedData.LocalData._fearTokens;
    
    // 4. Calculate proposed new amount
    int32_t newAmount = currentTokens + count;

    // 5. Clamping Logic
    // If the addition resulted in a negative number, floor it at 0.
    if (newAmount < 0)
    {
        newAmount = 0;
    }
    // If the new amount is still less than max, keep it.
    // If it is greater than max, cap it at maxTokens.
    else if (newAmount > maxTokens)
    {
        newAmount = maxTokens;
    }

    // 6. Write back to memory
    playerData->SavedData.LocalData._fearTokens = newAmount;

    // 7. Save the profile
    // Offset 0x3C0 is _playerDataFacade
    if (this->_playerDataFacade)
    {
        this->_playerDataFacade->SaveFullProfile(false); // 0 is likely an enum or boolean flag
    }
}




void UDBDGameInstance::DBD_AddItemAddonToInventory(FName ItemAddonID)
{
    // 1. Retrieve the Item Data from Design Tunables
    // Accessing 'DesignTunables' at offset 0xf0 based on disassembly [rcx+0xf0]
    // The return type is inferred as a struct/class containing 'Role' (0xd8) and 'DisplayName' (0x18)
    /* UNDEFINED ELEMENT - UDBDDesignTunables::GetItemAddonData is a custom function */
    FItemAddonProperties* itemAddonData = this->DesignTunables->GetItemAddonData(ItemAddonID);

    // 2. Validate Item Data. If null, handle error.
    if (itemAddonData == nullptr)
    {
        FString errorMessage = FString::Printf(TEXT("Invalid addon ID"));

        // Display error message in Red (FColor::Red)
        if (GEngine)
        {
            // The disassembly calls a function labeled 'AddStructReferencedObjectsOrNot', 
            // but the arguments (GEngine, -1, 5.0f, Red, String) confirm this is AddOnScreenDebugMessage.
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, errorMessage);
        }
        return;
    }

    // 3. Get Local Player State
    /* UNDEFINED ELEMENT - Custom helper function in this GameInstance */
    ADBDPlayerState* localPlayerState = this->GetLocalPlayerState();

    // 4. Determine Player Role
    // The disassembly (14024efaf - 14024f00e) performs a raw inline IsValid check 
    // using GUObjectArray logic and checks specific internal flags (byte+0x140).
    // We translate this to the standard UE4 IsValid check and retrieval of the role.
    EPlayerRole playerGameRole = EPlayerRole::VE_None;

    if (IsValid(localPlayerState))
    {
        // Accessing 'GameRole' at offset 0x750 based on disassembly
        playerGameRole = localPlayerState->GameRole;
    }
    else
    {
        playerGameRole = EPlayerRole::VE_None;
    }

    // 5. Get Saved Profile Data
    /* UNDEFINED ELEMENT - Custom function to retrieve save data */
    FCharacterSavedProfileData* savedProfileData = this->GetCharacterSavedProfileDataForPlayerState(localPlayerState);
    if (savedProfileData == nullptr)
    {
        return;
    }

    // 6. Check Role Compatibility
    // Accessing 'Role' from itemAddonData at offset 0xd8 based on disassembly [rsi+0xd8]
    // Logic: If the item has a specific role (not None/0), it must match the player's role.
    EPlayerRole itemRole = itemAddonData->Role;
    if (itemRole != EPlayerRole::VE_None)
    {
        if (itemRole != playerGameRole)
        {
            return;
        }
    }

    // 7. Add Item to Inventory
    // Call FCharacterSavedProfileData::AddNamedItemToInventory
    savedProfileData->AddNamedItemToInventory(ItemAddonID);

    // 8. Prepare Success Message
    // Accessing 'DisplayName' (FText) at offset 0x18 based on disassembly [rsi+0x18]
    FString itemName = itemAddonData->DisplayName.ToString();
    FString successMessage = FString::Printf(TEXT("Added item addon %s"), *itemName);

    // 9. Display Success Message
    // Display success message in Green (FColor::Green)
    if (GEngine)
    {
        // Using AddOnScreenDebugMessage to match the functionality of the 'AddStructReferencedObjectsOrNot' call in disassembly
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, successMessage);
    }
}




void UDBDGameInstance::DBD_AddItemToInventory(FName ItemID)
{
    // 1. Retrieve the Item Data from Design Tunables
    // Accessing 'DesignTunables' at offset 0xf0.
    // The disassembly sets r8b to 1 before the call (14024f142), indicating a boolean 'true' second argument.
    /* UNDEFINED ELEMENT - UDBDDesignTunables::GetItemData is a custom function */
    FItemData* itemData = this->DesignTunables->GetItemData(ItemID, true);

    // 2. Validate Item Data. If null, handle error.
    if (itemData == nullptr)
    {
        FString errorMessage = FString::Printf(TEXT("Invalid item ID"));

        // Display error message in Red (FColor::Red)
        if (GEngine)
        {
            // The disassembly calls 'AddStructReferencedObjectsOrNot' with color/string args, 
            // representing a symbol mismatch for AddOnScreenDebugMessage.
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, errorMessage);
        }
        return;
    }

    // 3. Get Local Player State
    /* UNDEFINED ELEMENT - Custom helper function in this GameInstance */
    ADBDPlayerState* localPlayerState = this->GetLocalPlayerState();

    // 4. Determine Player Role
    // The disassembly performs manual IsValid checks on the UObjectArray and internal flags.
    EPlayerRole playerGameRole = EPlayerRole::VE_None;

    if (IsValid(localPlayerState))
    {
        // Accessing 'GameRole' at offset 0x750
        playerGameRole = localPlayerState->GameRole;
    }
    else
    {
        playerGameRole = EPlayerRole::VE_None;
    }

    // 5. Get Saved Profile Data
    /* UNDEFINED ELEMENT - Custom function to retrieve save data */
    FCharacterSavedProfileData* savedProfileData = this->GetCharacterSavedProfileDataForPlayerState(localPlayerState);

    if (savedProfileData == nullptr)
    {
        return;
    }

    // 6. Check Role Compatibility
    // Accessing 'Role' from itemData at offset 0xd8 based on disassembly [rsi+0xd8]
    // Logic: If the item has a specific role requirement (not None/0), it must match the player's role.
    EPlayerRole itemRole = itemData->Role;
    if (itemRole != EPlayerRole::VE_None)
    {
        if (itemRole != playerGameRole)
        {
            return;
        }
    }

    // 7. Add Item to Inventory
    // Call FCharacterSavedProfileData::AddNamedItemToInventory
    savedProfileData->AddNamedItemToInventory(ItemID);

    // 8. Prepare Success Message
    // Accessing 'DisplayName' (FText) at offset 0x18 based on disassembly [rsi+0x18]
    FString itemName = itemData->DisplayName.ToString();
    FString successMessage = FString::Printf(TEXT("Added item %s"), *itemName);

    // 9. Display Success Message
    // Display success message in Green (FColor::Green)
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, successMessage);
    }
}




void UDBDGameInstance::DBD_AddOfferingToInventory(FName ItemID)
{
    // 1. Retrieve the Offering Data from Design Tunables
    // Accessing 'DesignTunables' at offset 0xf0.
    // The disassembly sets r8b to 1 before the call (14024f302), indicating a boolean 'true' second argument.
    /* UNDEFINED ELEMENT - UDBDDesignTunables::GetOfferingData is a custom function */
    FOfferingProperties* offeringData = this->DesignTunables->GetOfferingData(ItemID, true);

    // 2. Validate Offering Data. If null, handle error.
    if (offeringData == nullptr)
    {
        FString errorMessage = FString::Printf(TEXT("Invalid item ID"));

        // Display error message in Red (FColor::Red)
        if (GEngine)
        {
            // The disassembly calls 'AddStructReferencedObjectsOrNot' with color/string args, 
            // representing a symbol mismatch for AddOnScreenDebugMessage.
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, errorMessage);
        }
        return;
    }

    // 3. Get Local Player State
    /* UNDEFINED ELEMENT - Custom helper function in this GameInstance */
    ADBDPlayerState* localPlayerState = this->GetLocalPlayerState();

    // 4. Determine Player Role
    // The disassembly performs manual IsValid checks on the UObjectArray and internal flags.
    EPlayerRole playerGameRole = EPlayerRole::VE_None;

    if (IsValid(localPlayerState))
    {
        // Accessing 'GameRole' at offset 0x750
        playerGameRole = localPlayerState->GameRole;
    }
    else
    {
        playerGameRole = EPlayerRole::VE_None;
    }

    // 5. Check Save Data Validity
    // The code first checks if save data exists before checking roles.
    /* UNDEFINED ELEMENT - Custom function to retrieve save data */
    if (this->GetCharacterSavedProfileDataForPlayerState(localPlayerState) == nullptr)
    {
        return;
    }

    // 6. Check Role Compatibility
    // Accessing 'Role' from offeringData at offset 0xd8 based on disassembly [rsi+0xd8]
    // Logic: If the offering has a specific role requirement (not None/0), it must match the player's role.
    EPlayerRole offeringRole = offeringData->Role;
    if (offeringRole != EPlayerRole::VE_None)
    {
        if (offeringRole != playerGameRole)
        {
            return;
        }
    }

    // 7. Re-acquire Data and Add Item
    // Note: The disassembly explicitly calls GetLocalPlayerState and GetCharacterSavedProfileDataForPlayerState 
    // AGAIN here (14024f3bb - 14024f3c6) before adding the item, despite having done so earlier.
    ADBDPlayerState* currentPlayerState = this->GetLocalPlayerState();
    FCharacterSavedProfileData* savedProfileData = this->GetCharacterSavedProfileDataForPlayerState(currentPlayerState);
    
    // Call FCharacterSavedProfileData::AddNamedItemToInventory
    savedProfileData->AddNamedItemToInventory(ItemID);

    // 8. Prepare Success Message
    // Accessing 'DisplayName' (FText) at offset 0x18 based on disassembly [rsi+0x18]
    FString offeringName = offeringData->DisplayName.ToString();
    FString successMessage = FString::Printf(TEXT("Added offering %s"), *offeringName);

    // 9. Display Success Message
    // Display success message in Green (FColor::Green)
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, successMessage);
    }
}




void UDBDGameInstance::DBD_AddPerkToInventory(FName PerkID)
{
    // 1. Retrieve the Perk Data from Design Tunables
    // Accessing 'DesignTunables' at offset 0xf0.
    // The disassembly sets r8b to 1 before the call (14024f4d2), indicating a boolean 'true' second argument.
    /* UNDEFINED ELEMENT - UDBDDesignTunables::GetPerkData is a custom function */
    FPerkProperties* perkData = this->DesignTunables->GetPerkData(PerkID, true);

    // 2. Validate Perk Data. If null, handle error.
    if (perkData == nullptr)
    {
        FString errorMessage = FString::Printf(TEXT("Invalid perk ID"));

        // Display error message in Red (FColor::Red)
        if (GEngine)
        {
            // The disassembly calls 'AddStructReferencedObjectsOrNot' with color/string args, 
            // representing a symbol mismatch for AddOnScreenDebugMessage.
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, errorMessage);
        }
        return;
    }

    // 3. Get Local Player State
    /* UNDEFINED ELEMENT - Custom helper function in this GameInstance */
    ADBDPlayerState* localPlayerState = this->GetLocalPlayerState();

    // 4. Determine Player Role
    // The disassembly performs manual IsValid checks on the UObjectArray and internal flags.
    EPlayerRole playerGameRole = EPlayerRole::VE_None;

    if (IsValid(localPlayerState))
    {
        // Accessing 'GameRole' at offset 0x750
        playerGameRole = localPlayerState->GameRole;
    }
    else
    {
        playerGameRole = EPlayerRole::VE_None;
    }

    // 5. Get Saved Profile Data
    /* UNDEFINED ELEMENT - Custom function to retrieve save data */
    FCharacterSavedProfileData* savedProfileData = this->GetCharacterSavedProfileDataForPlayerState(localPlayerState);

    if (savedProfileData == nullptr)
    {
        return;
    }

    // 6. Check Role Compatibility
    // Accessing 'Role' from perkData at offset 0xd8 based on disassembly [rbx+0xd8]
    EPlayerRole perkRole = perkData->Role;

    if (perkRole != EPlayerRole::VE_None)
    {
        if (perkRole != playerGameRole)
        {
            return;
        }
    }

    // 7. Check Perk Tags (Restriction Check)
    // Accessing 'Tags' array at offset 0x160 based on disassembly [rbx+0x160]
    // The disassembly iterates the array looking for a specific value (data_143621850).
    // We assume this data pointer represents a specific FName, likely "Teachable" or "Unavailable" in context.
    /* UNDEFINED ELEMENT - 'RestrictedTag' represents the unknown global FName being compared against */
    const FName RestrictedTag = FName("Disabled"); // Placeholder name based on logic context
    for (const FName& Tag : perkData->Tags)
    {
        // Comparing raw FName values (pointers in UE4 often optimize to index comparisons)
        if (Tag == RestrictedTag)
        {
            return;
        }
    }

    // 8. Add Perk to Inventory
    // Call FCharacterSavedProfileData::AddNamedItemToInventory
    savedProfileData->AddNamedItemToInventory(PerkID);

    // 9. Prepare Success Message
    // Accessing 'DisplayName' (FText) at offset 0x18 based on disassembly [rbx+0x18]
    FString perkName = perkData->DisplayName.ToString();
    FString successMessage = FString::Printf(TEXT("Added perk %s"), *perkName);

    // 10. Display Success Message
    // Display success message in Green (FColor::Green)
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, successMessage);
    }
}




void UDBDGameInstance::DBD_AddSkulls(int32_t camperSkullsCount, int32_t slasherSkullsCount)
{
    // 1. Get Local Player
    // Retrieving local player at index 0.
    ULocalPlayer* localPlayer = this->GetGameInstance()->GetLocalPlayerByIndex(0);

    // 2. Get Unique Net ID
    // The disassembly retrieves the UniqueNetId as a TSharedPtr/TSharedRef.
    // Note: The disassembly shows manual reference count manipulation, which is handled automatically by TSharedPtr in C++.
    TSharedPtr<const FUniqueNetId> uniqueNetId;
    if (localPlayer != nullptr)
    {
        uniqueNetId = localPlayer->GetPreferredUniqueNetId();
    }

    // 3. Prepare Leaderboard Write Object
    // The disassembly constructs an 'FOnlineLeaderboardWrite' object on the stack (initializing vtable and members).
    // It then calls SetIntStat twice.
    FOnlineLeaderboardWrite writeObject;

    // We assume the data pointers in disassembly correspond to these FNames based on context.
    const FName CamperSkullsStatName("CamperSkulls");
    const FName SlasherSkullsStatName("SlasherSkulls");

    // FOnlineStats::SetIntStat is called for both arguments.
    writeObject.SetIntStat(CamperSkullsStatName, camperSkullsCount);
    writeObject.SetIntStat(SlasherSkullsStatName, slasherSkullsCount);

    // 4. Update Backend via DAL
    // Accessing PlayerProfileDAL at offset 0x120.
    if (this->PlayerProfileDAL)
    {
        // Update stats requires the NetId and the WriteObject.
        // The disassembly passes the TSharedPtr logic for the NetId.
        if (uniqueNetId.IsValid())
        {
            this->PlayerProfileDAL->UpdatePlayerStats(uniqueNetId.ToSharedRef(), writeObject);
        }

        // Upload the changes immediately.
        this->PlayerProfileDAL->UploadCurrentPlayerStats();
    }

    // 5. Update Local Persistent Data
    /* UNDEFINED ELEMENT - Custom function to get persistent data */
    FPlayerPersistentData* persistentData = this->GetLocalPlayerPersistentData();

    if (persistentData)
    {
        // Offsets 0x1a0 and 0x1a4 correspond to SlasherSkulls and CamperSkulls in the SharedData struct.
        persistentData->SavedData.SharedData.SlasherSkulls += slasherSkullsCount;
        persistentData->SavedData.SharedData.CamperSkulls += camperSkullsCount;
    }

    // 6. Update Player Controller
    if (localPlayer)
    {
        APlayerController* playerController = localPlayer->PlayerController;
        if (playerController)
        {
            // The disassembly performs a manual class hierarchy check (IsA) against ADBDPlayerControllerBase.
            ADBDPlayerControllerBase* dbdController = Cast<ADBDPlayerControllerBase>(playerController);

            if (dbdController)
            {
                // Reload profile stats on the controller to reflect changes.
                dbdController->Local_LoadProfileStats();
            }
        }
    }
}