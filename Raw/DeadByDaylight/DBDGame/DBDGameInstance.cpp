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
    if (this->Builder.IsValid())
    {
        // Resolve the weak pointer to get the builder instance.
        AProceduralLevelBuilder* levelBuilder = (AProceduralLevelBuilder*)this->Builder.Get();

        // Apply the adjusted fog value.
        levelBuilder->OfferingAdjustFog(adjustedFogValue);
    }
}




bool UDBDGameInstance::AreExtraConditionsMet()
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
    // Retrieve the current GameMode. The disassembly retrieves this via offset 0xF0 from the World/GameInstance context.
    // In UE4, this is typically accessed via the World.
    ADBDGameMode* DBDGameMode = Cast<ADBDGameMode>(this->GetWorld()->GetAuthGameMode());
    // Check if GameMode is valid before proceeding
    if (DBDGameMode == nullptr 
     || DBDGameMode->IsValidLowLevel() == false 
     || DBDGameMode->IsPendingKill()
     || DBDGameMode->bActorIsBeingDestroyed)
    {
        return;
    }

    // Initialize a local instance of persistent data.
    // Disassembly: FPlayerPersistentData::FPlayerPersistentData(&var_3b8)
    FPlayerPersistentData LocalPersistentData;

    // Increment shared reference count for the UniqueNetId if it exists
    if (id->UniqueNetId.SharedReferenceCount.ReferenceController != nullptr)
    {
        id->UniqueNetId.SharedReferenceCount.ReferenceController->SharedReferenceCount++;
    }

    // Try to retrieve existing persistent data for this player ID
    FPlayerPersistentData* ExistingData = UDBDPersistentData::GetPlayerPersistentData(this->_persistentData, id);
    // Determine which data structure to write into (Local stack variable or the retrieved existing pointer)
    FPlayerPersistentData* DataToWrite = &LocalPersistentData;
    if (ExistingData != nullptr)
    {
        DataToWrite = ExistingData;
    }

    // Copy Profile Data into the target structure
    // Disassembly uses TArray::operator= and direct assignments
    DataToWrite->SavedData.SharedData.PlayerName = playerProfileData->PlayerName;
    DataToWrite->SavedData.SharedData.SteamId = playerProfileData->SteamId;
    DataToWrite->SavedData.SharedData.SelectedCamperIndex = playerProfileData->SelectedCamperIndex;
    DataToWrite->SavedData.SharedData.SelectedSlasherIndex = playerProfileData->SelectedSlasherIndex;
    DataToWrite->SavedData.SharedData.SelectedCharacterCustomization = playerProfileData->SelectedCharacterCustomization;
    DataToWrite->SavedData.SharedData.SlasherSkulls = playerProfileData->SlasherSkulls;
    DataToWrite->SavedData.SharedData.CamperSkulls = playerProfileData->CamperSkulls;
    DataToWrite->SavedData.SharedData.CamperStreak = playerProfileData->CamperStreak;

    // Copy Loadout Data using the legacy operator
    // Disassembly: FLegacySavedPlayerLoadoutData::operator=
    DataToWrite->CurrentLoadout = *playerLoadout;

    // Sync "Previous" stats with the current shared data
    DataToWrite->PreviousSlasherSkulls = DataToWrite->SavedData.SharedData.SlasherSkulls;
    DataToWrite->PreviousCamperSkulls = DataToWrite->SavedData.SharedData.CamperSkulls;

    // Calculate Experience Logic
    int32_t MaxExperience = FPlayerPersistentData::GetMaxExperience(DataToWrite);
    int32_t CurrentExperience = DataToWrite->SavedData.LocalData._experience;

    if (CurrentExperience < 0)
    {
        MaxExperience = 0;
    }
    else if (CurrentExperience < MaxExperience)
    {
        MaxExperience = CurrentExperience;
    }

    DataToWrite->PreviousExperience = MaxExperience;

    // Re-increment ref count for the ID before setting (Pseudo code logic matches ID preservation)
    if (id->UniqueNetId.SharedReferenceCount.ReferenceController != nullptr)
    {
        id->UniqueNetId.SharedReferenceCount.ReferenceController->SharedReferenceCount++;
    }

    // Save the modified data back to the persistent storage
    UDBDPersistentData::SetPlayerPersistentData(this->_persistentData, id, DataToWrite);

    // Update the actual PlayerState if the player is currently in the match
    AGameState* GameState = DBDGameMode->GameState;

    // The disassembly loops specifically backwards or checks array bounds
    if (GameState != nullptr && GameState->PlayerArray.Num() > 0)
    {
        for (int32_t i = 0; i < GameState->PlayerArray.Num(); i++)
        {
            // Retrieve PlayerState and Cast to ADBDPlayerState
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(GameState->PlayerArray[i]);

            if (DBDPlayerState == nullptr)
            {
                continue;
            }

            // Compare Player Names (Case Insensitive)
            // Disassembly calls FGenericPlatformStricmp
            const FString& ProfileName = playerProfileData->PlayerName.Data.Num() > 0 ? playerProfileData->PlayerName : TEXT("");
            const FString& StateName = DBDPlayerState->PlayerName.Data.Num() > 0 ? DBDPlayerState->PlayerName : TEXT("");

            if (ProfileName.Equals(StateName, ESearchCase::IgnoreCase))
            {
                // Synchronize data both ways between State and Persistent Data
                UDBDPersistentData::CopyData(DBDPlayerState, DataToWrite, 0);
                UDBDPersistentData::CopyData(DataToWrite, DBDPlayerState, 0);

                // Call ForceNetUpdate on the actor to replicate changes immediately
                // VTable Offset 0x568 resolved to ForceNetUpdate
                DBDPlayerState->ForceNetUpdate();
            }
        }
    }

    // Handle Context System and Host Settings
    UGameFlowContextSystem* ContextSystem = this->_contextSystem;

    if (ContextSystem != nullptr)
    {
        // Check Validity of Context System
        if (ContextSystem->IsValidLowLevel() && ContextSystem->IsPendingKill() == false)
        {
            UOnlineSystemHandler* OnlineHandler = ContextSystem->m_OnlineSystemHandler;
            if (OnlineHandler != nullptr)
            {
                UDBDServerInstance* ServerInstance = OnlineHandler->_serverInstance;

                // Check Validity of Server Instance
                if (ServerInstance != nullptr 
                 && ServerInstance->IsValidLowLevel() 
                 && ServerInstance->IsPendingKill() == false)
                {
                    // Update Host settings, passing nullptr as the second argument (originally DBDGameInstance ptr set to null)
                    UDBDServerInstance::UpdateHostSettings(ServerInstance, nullptr);
                }
            }
        }
    }

    // Local variable 'LocalPersistentData' is destroyed here automatically (C++ RAII).
    // The disassembly shows explicit FMemory::Free calls corresponding to the destruction of 
    // the TArrays inside 'LocalPersistentData' and the input structs.
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
    if (localPlayer)
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




void UDBDGameInstance::DBD_AddToRitual(int32_t index, float value)
{
    // 1. Retrieve the custom Player State.
    // The disassembly calls a specific function "GetLocalPlayerState" which is not 
    // part of the standard UGameInstance API.
    class ADBDPlayerState* PlayerState = this->GetLocalPlayerState(); /* UNDEFINED ELEMENT */
    // 2. Validate the PlayerState pointer.
    if (PlayerState == nullptr)
    {
        return;
    }

    // 3. Check Object Validity and Destruction State.
    // The disassembly performs two distinct checks here:
    // A. It checks the Global Object Array to see if the object is marked 'PendingKill' or 'Unreachable' (Result of (Flags >> 0x1d) & 1).
    // B. It checks the specific boolean flag at offset 0x140 bit 0x04. 
    // bit 0x04 corresponds to 'bActorIsBeingDestroyed'.
    if (PlayerState->IsPendingKill() || PlayerState->bActorIsBeingDestroyed)
    {
        return;
    }

    // 4. Access the Ritual Handler.
    // The assembly performs a redundant validity check on the PlayerState here (checking the GUObjectArray again).
    // This is often a result of using a macro like `IsValid(PlayerState)` or accessing a `TWeakObjectPtr` 
    // immediately before accessing a member.
    if (PlayerState->IsPendingKill())
    {
        // Retrieve the component from offset 0x778.
        class URitualHandlerComponent* RitualHandler = PlayerState->RitualHandler;

        // 5. Execute the Ritual modification.
        // The instruction 'call URitualHandlerComponent::CheatAddToRitual' is executed.
        // Note: The assembly does not show an explicit null check for 'RitualHandler' itself, 
        // relying on the validity of the PlayerState.
        RitualHandler->CheatAddToRitual(index, value); /* UNDEFINED ELEMENT */
    }
}




void UDBDGameInstance::DBD_AddToStat(FName statName, float amount)
{
    // Retrieve the first local player from the Game Instance
    // 14024fa17 call UGameInstance::GetLocalPlayerByIndex
    ULocalPlayer* LocalPlayer = this->GetLocalPlayerByIndex(0);

    // Retrieve the UniqueNetId (TSharedPtr) from the local player
    // 14024fa27 call ULocalPlayer::GetPreferredUniqueNetId
    TSharedPtr<const FUniqueNetId> UniqueNetId = nullptr;
    if (LocalPlayer)
    {
        UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();
    }

    // Create a stack-allocated FOnlineLeaderboardWrite object (identified by vtable initialization at 14024faa3)
    // This object acts as a container for the stats to be written
    FOnlineLeaderboardWrite WriteObject;

    // Convert the FName statName to a string to check for substrings
    // 14024fafe call FName::ToString
    FString StatNameStr = statName.ToString();

    // Check if the stat name contains the substring "_float"
    // 14024fb25 call FString::Find
    // 0xffffffff in assembly corresponds to INDEX_NONE or -1 in C++
    int32 FoundIndex = StatNameStr.Find(TEXT("_float"), ESearchCase::IgnoreCase, ESearchDir::FromStart, -1);

    if (FoundIndex == -1)
    {
        // If "_float" is NOT found, treat the amount as an Integer
        // 14024fb41 cvttss2si (convert float to signed integer)
        // 14024fb46 call FOnlineStats::SetIntStat
        WriteObject.SetIntStat(statName, (int32)amount);
    }
    else
    {
        // If "_float" IS found, treat the amount as a Float
        // 14024fb3a call FOnlineStats::SetFloatStat
        WriteObject.SetFloatStat(statName, amount);
    }

    // Access the PlayerProfileDAL member variable (Offset 0x120 in assembly)
    // 14024fb8e mov rcx, qword [rsi+0x120]
    if (this->PlayerProfileDAL)
    {
        // Update the player stats in the Data Access Layer
        // 14024fb89 call UPlayerProfileDAL::UpdatePlayerStats
        this->PlayerProfileDAL->UpdatePlayerStats(UniqueNetId, &WriteObject);

        // Upload the stats to the backend/service
        // 14024fb95 call UPlayerProfileDAL::UploadCurrentPlayerStats
        this->PlayerProfileDAL->UploadCurrentPlayerStats();
    }

    // Perform checks on the PlayerController to potentially load profile stats
    if (LocalPlayer)
    {
        APlayerController* PlayerController = LocalPlayer->PlayerController;

        if (PlayerController)
        {
            // Verify if the controller is of type ADBDPlayerControllerBase
            // 14024fbaa ADBDPlayerControllerBase::GetPrivateStaticClass (Class hierarchy check)
            ADBDPlayerControllerBase* DBDController = Cast<ADBDPlayerControllerBase>(PlayerController);

            if (DBDController)
            {
                // The assembly performs manual checks against GUObjectArray (14024fbcb - 14024fbfc)
                // checking flags (PendingKill, Unreachable). In C++, IsValid() covers these checks.
                bool bIsValid = DBDController->IsValidLowLevel();

                if (bIsValid)
                {
                    // 14024fc1d call ADBDPlayerControllerBase::Local_LoadProfileStats
                    DBDController->Local_LoadProfileStats();
                }
            }
        }
    }

    // The FOnlineLeaderboardWrite destructor and TSharedPtr destructors 
    // are called automatically here as the stack frame unwinds.
    // (Corresponds to 14024fc4d TSparseArray::Empty and FMemory::Free calls in assembly)
}




void UDBDGameInstance::DBD_AnalyticsTest()
{
    // Initialize a string variable with the value "Test".
    // The assembly at 14024fcec (ResizeGrow) and 14024fd15 (Convert) represents the constructor of FString from a literal.
    FString InProgressType = TEXT("Test");

    // Initialize an empty array of analytics attributes.
    // 14024fd1f: The stack space is zeroed out, representing an empty TArray constructor.
    TArray<FAnalyticsEventAttribute> attr;

    // Call the static function to record client progress.
    // 14024fd2e call UDBDAnalytics::RecordClientProgress
    // RCX = Address of InProgressType
    // RDX = Address of attr
    UDBDAnalytics::RecordClientProgress(InProgressType, attr);

    // End of function. 
    // The code from 14024fd33 to 14024fd85 in disassembly is the automatic destruction (destructors) 
    // of 'attr' and 'InProgressType' as they go out of scope.
    // The loop in disassembly iterates through 'attr' to free potential FStrings inside FAnalyticsEventAttribute,
    // then frees the array buffer, and finally frees the 'InProgressType' string buffer.
}




void UDBDGameInstance::DBD_AssignNewRitual()
{
    // Retrieve the local player state cast to the custom DBD class
    // 14024fd94 call UDBDGameInstance::GetLocalPlayerState
    ADBDPlayerState* PlayerState = this->GetLocalPlayerState();
    // Basic null check
    // 14024fd9c test rax, rax
    if (PlayerState == nullptr)
    {
        return;
    }

    // The assembly block from 14024fda1 to 14024fdf7 performs direct checks against 
    // the global object array (GUObjectArray) and internal bitflags.
    // It verifies if the object index is valid, if the object is not reachable, 
    // or if it is marked as PendingKill (byte at offset 0x140 & 0x4).
    // In standard Unreal Engine C++, this logic is encapsulated in IsValid() or IsValidLowLevel().
    if (PlayerState->IsValidLowLevel() == false)
    {
        return;
    }

    // Retrieve the RitualHandler component stored in the PlayerState
    // 14024fdf9 mov rcx, qword [r8+0x778] (Offset 0x778 corresponds to the RitualHandler pointer)
    URitualHandlerComponent* RitualHandler = PlayerState->RitualHandler;
    // Check if the handler exists before calling (implied safety in C++, though assembly does a direct JMP)
    if (RitualHandler)
    {
        // Call the function to assign a new ritual.
        // 14024fe09 jmp URitualHandlerComponent::AssignNewRitual
        // The assembly uses a "tail call" (JMP instead of CALL) optimization here 
        // because this is the last operation in the function.
        RitualHandler->AssignNewRitual();
    }
}




void UDBDGameInstance::DBD_ChangeSteamOverlayPosition(EPresenceNotificationPosition position, int32 verticalOffset, int32 horizontalOffset)
{
    // Initialize the module name FName
    // 14024fe2d lea rdx, "OnlinePresence"
    FName ModuleName = FName(TEXT("OnlinePresence"));

    // Load the module checked. This retrieves the interface for the plugin.
    // 14024fe42 call FModuleManager::LoadModuleChecked
    IOnlinePresencePlugin& OnlinePresencePlugin = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>(ModuleName);

    // Call the virtual function SetNotificationPosition.
    // The 'position' integer is cast to the enum type.
    // The verticalOffset and horizontalOffset arguments are ignored by the assembly implementation.
    // 14024fe50 call qword [r8+0x158] (Resolved as SetNotificationPosition)
    OnlinePresencePlugin.SetNotificationPosition(position);
}




void UDBDGameInstance::DBD_CheckForNewContent()
{
    // Check if the context system is valid before proceeding.
    if (this->_contextSystem == nullptr)
    {
        return;
    }

    // ---------------------------------------------------------
    // Online Presence / Data Store Logic
    // ---------------------------------------------------------

    // Obtain a shared reference to the DataStore subsystem via the plugin.
    // The disassembly indicates a TSharedRef is returned here.
    TSharedRef<IDataStorePresenceSubsystem> DataStoreRef = IOnlinePresencePlugin::DataStore();

    // Access the interface object.
    IDataStorePresenceSubsystem* DataStoreObj = &DataStoreRef.Get();

    if (DataStoreObj)
    {
        // Call the virtual function at offset 0x128.
        // Based on provided context, this is GetLocalAppBuildId.
        // The return value (int32) is calculated but not stored or used in the subsequent logic.
        DataStoreObj->GetLocalAppBuildId();
    }

    // Note: The disassembly shows manual reference counting decrement logic here.
    // In C++, the TSharedRef destructor handles this automatically when DataStoreRef goes out of scope.

    // ---------------------------------------------------------
    // Game User Settings Logic
    // ---------------------------------------------------------

    // Retrieve the base Game User Settings from the global engine instance.
    UGameUserSettings* GenericSettings = UEngine::GetGameUserSettings(GEngine);
    if (GenericSettings == nullptr)
    {
        return;
    }

    // Attempt to cast the generic settings to the specific UDBDGameUserSettings class.
    // The disassembly uses class tree index comparison, which is the internal implementation of Cast<>.
    UDBDGameUserSettings* DBDSettings = Cast<UDBDGameUserSettings>(GenericSettings);
    if (DBDSettings == nullptr)
    {
        return;
    }

    // ---------------------------------------------------------
    // Validity Check
    // ---------------------------------------------------------

    // The disassembly checks specific internal flags (likely EInternalObjectFlags).
    // "test al, 0x1" usually checks for PendingKill or Unreachable states in the object array.
    if (DBDSettings->IsPendingKill())
    {
        return;
    }

    // ---------------------------------------------------------
    // Content Details Logic
    // ---------------------------------------------------------

    TArray<FNewContentDetails*> NewContentDetailsArray;

    // Retrieve the list of new content details.
    UDBDDesignTunables::GetNewContentDetails(this->DesignTunables, NewContentDetailsArray);

    // Initialize the maximum weight found. initialized to -1 (0xffffffff) in disassembly.
    int32 MaxWeightFound = -1;

    // Check if the array contains any elements.
    if (NewContentDetailsArray.Num() > 0)
    {
        // Iterate through the content details to find the highest weight/ID.
        for (int32 Index = 0; Index < NewContentDetailsArray.Num(); Index++)
        {
            FNewContentDetails* ContentItem = NewContentDetailsArray[Index];
            if (ContentItem)
            {
                /* UNDEFINED ELEMENT */
                // Accessing an int32 property at offset 0x8 of FNewContentDetails.
                // Since the struct definition is not fully provided, we access it via pointer arithmetic.
                // This represents the "Weight" or "News ID" of the content item.
                int32 ItemWeight = *(int32*)((uint8*)ContentItem + 0x8);

                if (ItemWeight >= MaxWeightFound)
                {
                    MaxWeightFound = ItemWeight;
                }
            }
        }
    }

    // ---------------------------------------------------------
    // Comparison and Transition Logic
    // ---------------------------------------------------------

    // Compare the highest weight found in the tunables against the one stored in User Settings.
    // Offset 0x170 is explicitly identified as HighestWeightSeenNews.
    if (MaxWeightFound != DBDSettings->HighestWeightSeenNews)
    {
        UGameFlowContextSystem* ContextSystem = this->_contextSystem;

        // Access the Prompt Context Group.
        // Disassembly implies usage of std::shared_ptr logic here.
        auto& PromptContextGroup = ContextSystem->m_SystemPromptsContextGroup;

        // Ensure the system is valid (simplified logic flow).
        if (ContextSystem)
        {
            /* UNDEFINED ELEMENT */
            // Call RequestTransition on the context group.
            // Arguments derived from registers:
            // RCX (this): PromptContextGroup._Ptr
            // RDX (Out): Return value (shared_ptr ref)
            // R8: 1
            // R9: 0x5C
            // Stack: 0x1 (true)

            CTX::ContextGroup::RequestTransition(PromptContextGroup, 1, 0x5C, true);
        }
    }

    // TArray cleanup (FMemory::Free) occurs automatically here via destructor.
}




void UDBDGameInstance::DBD_ClearInventory()
{
    // Retrieve the local player state associated with this GameInstance.
    // Disassembly: call UDBDGameInstance::GetLocalPlayerState
    class APlayerState* LocalPlayerState = this->GetLocalPlayerState();

    // Retrieve the Saved Profile Data for the specific Player State.
    // Disassembly: call UDBDGameInstance::GetCharacterSavedProfileDataForPlayerState
    struct FCharacterSavedProfileData* CharacterSavedProfileData = this->GetCharacterSavedProfileDataForPlayerState(LocalPlayerState);

    // The code initiates the removal of inventory items using a specific predicate.
    // 
    // Disassembly Notes:
    // 1. The code manually allocates memory (operator new) and sets up a VTable 
    //    (UE4Function_Private::TFunction...) to create a functor.
    // 2. The function pointer passed to this functor is identified by the decompiler as 
    //    'Scaleform::GFx::AS2::AvmButton::ActsAsButton'. This is likely an incorrect symbol 
    //    resolution for the actual internal inventory filtering function.
    // 3. Since the actual logic of the predicate is ambiguous due to the symbol mismatch, 
    //    it is marked as an undefined element below.

    if (CharacterSavedProfileData != nullptr)
    {
        // Calling RemoveAllInventoryNamed with the constructed predicate.
        CharacterSavedProfileData->RemoveAllInventoryNamed( /* UNDEFINED ELEMENT */);
    }
}




void UDBDGameInstance::DBD_DestroySteamInventory()
{
    // Access the persistent data member
    UDBDPersistentData* PersistentData = this->_persistentData;

    // Access the cloud inventory array from persistent data
    // ASM: mov rax, qword [rcx+0x3b8] -> movsxd rbx, dword [rax+0x518]
    TArray<FSteamInventoryItem>& CloudInventory = PersistentData->_cloudInventory;
    int32_t ArrayNum = CloudInventory.Num();
    // Check if the array is empty
    if (ArrayNum == 0)
    {
        return;
    }

    // ---------------------------------------------------------
    // Create a local copy of the array.
    // ASM: TArray::ResizeForCopy followed by j_memcpy.
    // This creates a safe snapshot of the inventory to iterate over.
    // ---------------------------------------------------------
    TArray<FSteamInventoryItem> InventoryCopy;

    // Reserve memory and uninitialized count matching the source
    InventoryCopy.Reserve(ArrayNum);
    InventoryCopy.SetNumUninitialized(ArrayNum);

    // Copy the raw memory from the source allocator to the local copy
    // ASM: call j_memcpy
    FMemory::Memcpy(InventoryCopy.GetData(), CloudInventory.GetData(), ArrayNum * sizeof(FSteamInventoryItem));

    // ---------------------------------------------------------
    // Iterate over the copied array
    // ---------------------------------------------------------
    for (int32_t Index = 0; Index < ArrayNum; Index = Index + 1)
    {
        FSteamInventoryItem* CurrentItem = &InventoryCopy[Index];

        // Retrieve the DataStore subsystem.
        // ASM: call IOnlinePresencePlugin::DataStore
        // The disassembly shows logic for handling a TSharedRef/Ptr (ref counting decrement at end of loop).

        /* UNDEFINED ELEMENT */
        TSharedRef<IDataStorePresenceSubsystem> DataStoreRef = IOnlinePresencePlugin::DataStore();

        IDataStorePresenceSubsystem* DataStoreObject = &DataStoreRef.Get();

        // ---------------------------------------------------------
        // Call DestroyItem (Previously VTable offset 0x168)
        // ---------------------------------------------------------
        // ASM: call qword [rax+0x168]
        // Arg1 (this): DataStoreObject
        // Arg2 (edx): CurrentItem->ItemId (dword [rdi])
        // Arg3 (r8d): 1 (Quantity)
        if (DataStoreObject != nullptr)
        {
            DataStoreObject->DestroyItem(CurrentItem->ItemId, 1);
        }

        // The TSharedRef "DataStoreRef" goes out of scope here.
        // ASM: Logic at 0x14025017c checks reference counts and destroys the object if needed.
        // This is handled automatically by the C++ TSharedRef destructor.
    }

    // The local array "InventoryCopy" goes out of scope here.
    // ASM: Logic at 0x1402501af calls FMemory::Free.
    // This is handled automatically by the TArray destructor.
}




void UDBDGameInstance::DBD_DisplayCurrentUserStat(FName statName)
{
    // Retrieve the local player at index 0
    ULocalPlayer* LocalPlayer = this->GetLocalPlayerByIndex(0);
    // Verify if the LocalPlayer is valid. 
    // The disassembly performs a low-level check against GUObjectArray here (ObjObjects.NumElements and Flags).
    // In C++, IsValid() or a nullptr check typically encapsulates this logic.
    if (LocalPlayer == nullptr)
    {
        return;
    }

    // Prepare variables for the stat retrieval
    int32 Value = 0;
    bool bRetrievalSuccess = false;

    // Access the PlayerProfileDAL member variable (Offset 0x120 in disassembly)
    // The disassembly does not explicitly check for nullptr here, but implies it exists.
    if (this->PlayerProfileDAL)
    {
        // Call the Data Access Layer to get the stat.
        // It takes the statName and a pointer to the integer Value.
        // Returns true (1) on success, false (0) on failure.
        bRetrievalSuccess = this->PlayerProfileDAL->GetCurrentPlayerStat(statName, &Value);
    }

    FString DebugMessage;

    // Branch based on the result of the retrieval (14025024a)
    if (bRetrievalSuccess)
    {
        // Success case: Format "StatName : Value"
        // 14025025a: u"%s : %i"
        DebugMessage = FString::Printf(TEXT("%s : %i"), *statName.ToString(), Value);
    }
    else
    {
        // Failure case: Format "StatName : Could not retrieve."
        // 14025028a: u"%s : Could not retrieve."
        DebugMessage = FString::Printf(TEXT("%s : Could not retrieve."), *statName.ToString());
    }

    // Display the message on screen via GEngine
    // Disassembly loads GEngine (1402502bc)
    if (GEngine)
    {
        // Arguments identified from disassembly:
        // Key (edx): -1 (0xffffffff)
        // TimeToDisplay (xmm2): 10.0f (0x41200000)
        // DisplayColor (r9d): FColor::Cyan
        // DebugMessage: The formatted string
        // bNewerOnTop (stack+0x28): true (0x1)
        // TextScale (stack+0x30): FVector2D::UnitVector

        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, DebugMessage, true, FVector2D::UnitVector);
    }
}




void UDBDGameInstance::DBD_DumpSessions()
{
    // Call virtual function to get the World.
    // Based on the offset 0x108 and usage (accessing AuthorityGameMode), this represents GetWorld().
    class UWorld* World = this->GetWorld();
    if (World == nullptr)
    {
        return;
    }

    // Retrieve AuthorityGameMode.
    // In UE4.13 UWorld, offset 0xF0 corresponds to the AuthorityGameMode member.
    class AGameMode* AuthorityGameMode = World->AuthorityGameMode;
    // Check if AuthorityGameMode is not null.
    if (AuthorityGameMode == nullptr)
    {
        return;
    }

    // Check if AuthorityGameMode is valid (not pending kill / accessible in GUObjectArray).
    // The disassembly checks bit 0x4 at offset 0x140 (bActorIsBeingDestroyed).
    if (IsValid(AuthorityGameMode) == false)
    {
        return;
    }

    // Retrieve GameSession from the GameMode.
    // In AGameMode, offset 0x3C0 corresponds to the GameSession member.
    class AGameSession* GameSession = AuthorityGameMode->GameSession;
    if (GameSession == nullptr)
    {
        return;
    }

    // Verify that the GameSession is of type ADBDGameSession.
    // The disassembly explicitly compares ClassTreeIndex against ADBDGameSession::StaticClass().
    if (GameSession->IsA(ADBDGameSession::StaticClass()) == false)
    {
        return;
    }

    // Check if GameSession is valid (not pending kill / accessible in GUObjectArray).
    // The disassembly repeats the check for bActorIsBeingDestroyed (offset 0x140, bit 0x4).
    if (IsValid(GameSession) == false)
    {
        return;
    }

    // Call the virtual function DumpSessionState.
    // The provided VTable info identifies offset 0x678 as DumpSessionState.
    GameSession->DumpSessionState();
}




void UDBDGameInstance::DBD_GenerateBloodWeb(int32 level)
{
    // Retrieve the local player state. 
    // This appears to be a helper function within the GameInstance to fetch the state of the local player.
    /* UNDEFINED ELEMENT */
    class APlayerState* localPlayerState = this->GetLocalPlayerState();

    // Retrieve the character saved profile data structure for the specific player state.
    // The return type is explicitly identified as FCharacterSavedProfileData pointer.
    /* UNDEFINED ELEMENT */
    struct FCharacterSavedProfileData* characterSavedProfileData = this->GetCharacterSavedProfileDataForPlayerState(localPlayerState);
    // Validate that the profile data exists before proceeding.
    if (characterSavedProfileData == nullptr)
    {
        return;
    }

    // Update the BloodWebLevel in the retrieved profile structure.
    characterSavedProfileData->BloodWebLevel = level;

    // Retrieve the BloodWeb Manager instance to perform operations on the web.
    /* UNDEFINED ELEMENT */
    class UBloodwebManager* bloodWebManager = this->GetBloodWebManager();
    // Request the BloodWeb Manager to build the web in place.
    /* UNDEFINED ELEMENT */
    bloodWebManager->BuildWebInPlace();

    // Access the Player Data Storage Facade to save the full profile.
    // The integer '0' from disassembly is interpreted here as a boolean 'false' (likely for an 'Immediate' or 'Async' flag).
    /* UNDEFINED ELEMENT */
    this->_playerDataFacade->SaveFullProfile(false);
}




void UDBDGameInstance::DBD_GenerateNewRandomRitual()
{
    // 1402504d6: Call GetLocalPlayerState to retrieve the current player state
    class ADBDPlayerState* LocalPlayerState = this->GetLocalPlayerState();
    // 1402504de: Check if the pointer is null
    if (LocalPlayerState == nullptr)
    {
        return;
    }

    // 1402504e3 - 14025051b: Validation Logic
    // The disassembly performs two specific checks here:
    // 1. It looks up the object in the global GUObjectArray and checks the flags (Bit 29 / 0x1d shift) 
    //    to see if the object is Unreachable (EInternalObjectFlags::Unreachable).
    // 2. It checks the boolean bitfield at offset 0x140 for the 3rd bit (0x4).
    //    According to the provided structure image, this corresponds to 'bActorIsBeingDestroyed'.
    if (LocalPlayerState->IsPendingKill() || LocalPlayerState->bActorIsBeingDestroyed)
    {
        return;
    }

    // 140250520 - 140250533: Redundant Validation
    // The disassembly repeats the GUObjectArray check (InternalIndex check) here. 
    // This is likely a compiler artifact or inlined IsValid() macro. 
    // We do not need to repeat the 'if' block in C++ as the previous check covers this logic.

    // 14025053a: Get the current UTC time
    // The disassembly allocates stack space for the return value and calls FDateTime::UtcNow
    FDateTime CurrentTime = FDateTime::UtcNow();

    // 14025053f: Access the RitualHandler component
    // Offset 0x778 in ADBDPlayerState corresponds to the RitualHandler pointer.
    class URitualHandlerComponent* RitualHandler = LocalPlayerState->RitualHandler;
    // 140250550: Call TryGenerateCachedRitual
    // Arguments passed in registers:
    // RDX (Arg 1): CurrentTime.Ticks (rax from UtcNow)
    // R8  (Arg 2): 1 (hardcoded integer/boolean true)
    // R9  (Arg 3): 1 (hardcoded integer/boolean true)
    if (RitualHandler != nullptr)
    {
        RitualHandler->TryGenerateCachedRitual(CurrentTime.GetTicks(), 1, 1);
    }
}




void UDBDGameInstance::DBD_GenerateNewRitual(FName name)
{
    // Call a helper function to retrieve the current LocalPlayerState.
    // Based on the ASM call: UDBDGameInstance::GetLocalPlayerState
    /* UNDEFINED ELEMENT */
    class ADBDPlayerState* LocalPlayerState = this->GetLocalPlayerState();
    // detailed check for nullptr as per instructions
    if (LocalPlayerState == nullptr)
    {
        return;
    }

    /* * Context for the following checks:
     * 1. The ASM accesses GUObjectArray using the object's InternalIndex (offset 0xC).
     * It checks the global flags (specifically bit 29). In UE4, this corresponds to checking
     * if the object is Unreachable or Pending Kill (IsPendingKill).
     * 2. The ASM checks byte offset 0x140 with mask 0x4.
     * Referring to the provided structure image, this corresponds to the bitfield 'bActorIsBeingDestroyed'.
     */

     // Check if the object is pending kill (Standard UE4 IsPendingKill logic found in ASM)
    // Check if the actor is in the process of being destroyed (Bit 2 of the union at offset 0x140)
    if (LocalPlayerState->IsPendingKill() || LocalPlayerState->bActorIsBeingDestroyed)
    {
        return;
    }

    /* * Note: The disassembly performs the IsPendingKill check (GUObjectArray lookup) a second time here.
     * This is likely due to compiler generation of a macro (like ensure(IsValid(obj))) or specific
     * safety checks in the original source code.
     * Since we verified IsPendingKill above, we proceed to the function call.
     */

     // Access the RitualHandler component from the PlayerState.
     // Offset 0x778 in ADBDPlayerState according to ASM comments.
     /* UNDEFINED ELEMENT */
    class URitualHandlerComponent* RitualHandler = LocalPlayerState->RitualHandler;

    // Call the function to try and generate the cached ritual.
    // Function: URitualHandlerComponent::TryGenerateCachedRitualWithKey
    /* UNDEFINED ELEMENT */
    RitualHandler->TryGenerateCachedRitualWithKey(name);
}




void UDBDGameInstance::DBD_GetRegion()
{
    // Access the RegionFinder component at offset 0x100.
    /* UNDEFINED ELEMENT */
    class URegionFinder* RegionFinder = this->RegionFinder;

    // Call GetBestRegion.
    // The ASM allocates stack space (rsp+0x30) for the return value (FName), 
    // but does not use it before the function returns void.
    // The signature 'GetBestRegion(const* this)' implies a const member function.
    const FName BestRegion = RegionFinder->GetBestRegion();
}




void UDBDGameInstance::DBD_GotoSplashScreen(bool showDefaultDialogueOnSplashScreen)
{
    // Access the Context System
    UGameFlowContextSystem* contextSystem = this->_contextSystem;

    // Access m_GameFlowContextGroup.
    // The disassembly (lines 14025063c) explicitly increments the strong reference count (_Uses).
    // This indicates a local std::shared_ptr copy is being created on the stack from the class member.
    std::shared_ptr<CTX::ContextGroup> gameFlowContextGroup = contextSystem->m_GameFlowContextGroup;

    // The ASM calls GetArraySize on the raw pointer (_Ptr) of the shared_ptr.
    // 0xd is hexadecimal for 13.
    // Note: CTX::ContextGroup appears to inherit from or be castable to Scaleform::GFx::AS3::VectorBase<long>.
    /* UNDEFINED ELEMENT - Explicit call to Scaleform VectorBase */
    auto* gameFlowRawPtr = (Scaleform::GFx::AS3::VectorBase<long>*)gameFlowContextGroup.get();

    if (gameFlowRawPtr->GetArraySize() != 13)
    {
        // Access m_OverlayContextGroup.
        // Similar to above, a local std::shared_ptr copy is created (ref count incremented at 14025066f).
        std::shared_ptr<CTX::ContextGroup> overlayContextGroup = this->_contextSystem->m_OverlayContextGroup;

        // Call RequestContextEnd.
        // The disassembly passes the raw pointer of the group.
        // It returns a std::shared_ptr<CTX::TaskHandle> (via hidden return pointer at rsp+0x20), 
        // which is immediately destroyed (ref count decremented/checked at 140250685).
        // We call the function and allow the returned temporary object to go out of scope immediately.
        /* UNDEFINED ELEMENT */
        CTX::ContextGroup::RequestContextEnd(overlayContextGroup.get());

        // Set disconnected state
        this->SetIsDisconnected(false);

        // Update profile offline status
        this->_isProfileOffline = false;

        // Determine transition ID based on the boolean argument.
        // 0x29 = 41, 0x2d = 45.
        uint32_t transitionId = 41;

        if (showDefaultDialogueOnSplashScreen == false)
        {
            transitionId = 45;
        }

        // Perform the travel
        this->TravelToSplashScreen(transitionId);

        // The 'overlayContextGroup' shared_ptr goes out of scope here.
        // This generates the ASM cleanup code (1402506e2 - 14025070d):
        // Decrements _Uses. If 0, calls VTable Dispose. Decrements _Weaks. If 0, calls VTable Delete.
    }

    // The 'gameFlowContextGroup' shared_ptr goes out of scope here.
    // This generates the ASM cleanup code (140250721 - 140250745).
}




void UDBDGameInstance::DBD_LeaveMatch()
{
    // Retrieve the GameFlowContextGroup from the context system via shared pointer logic
    /* UNDEFINED ELEMENT: std::shared_ptr access logic inferred from disassembly */
    auto GameFlowContext = this->_contextSystem->m_GameFlowContextGroup;

    // Check the array size of the GameFlow context.
    // The disassembly explicitly calls Scaleform::GFx::AS3::VectorBase<long>::GetArraySize on the internal pointer.
    /* UNDEFINED ELEMENT: Middleware function call */
    uint32_t ContextArraySize = Scaleform::GFx::AS3::VectorBase<long>::GetArraySize(GameFlowContext._Ptr);

    // Logic Gate: Proceed only if the context array size is 3 or 5
    if (ContextArraySize == 3 || ContextArraySize == 5)
    {
        // Retrieve the local DeadByDaylight Player Controller
        class ADBDPlayerController* LocalPC = this->GetLocalDBDPlayerController();

        // Validate the Player Controller
        if (LocalPC != nullptr)
        {
            // Verify the class type matches ADBDPlayerController (via GetPrivateStaticClass)
            // and perform low-level validity checks (GUObjectArray check in asm corresponds to IsValid())
            if (LocalPC->IsValidLowLevel())
            {
                // Explicit check for the bActorIsBeingDestroyed bitfield flag (Offset 0x140, Bit 0x4)
                if (LocalPC->bActorIsBeingDestroyed == false)
                {
                    // Retrieve the Player State
                    class ADBDPlayerState* LocalPS = this->GetLocalPlayerState();

                    // Validate the Player State (Null check, Low-Level Valid check, and Destruction check)
                    if (LocalPS != nullptr && LocalPS->IsValidLowLevel())
                    {
                        if (LocalPS->bActorIsBeingDestroyed == false)
                        {
                            // Mark the local player state as having left the match
                            LocalPS->Local_SetAsLeftMatch();
                        }
                    }

                    // Notify the server that the player is leaving
                    LocalPC->Server_LeaveGame();

                    // Handle client-side game ending logic
                    LocalPC->Client_GameEnded();

                    // Check the NetMode via the World
                    // The disassembly calls vtable offset 0x108 (Decimal 33). In UObject/UGameInstance, this resolves to GetWorld().
                    /* UNDEFINED VTABLE: Offset 0x108 resolved contextually to GetWorld() */
                    UWorld* World = this->GetWorld();
                    if (World != nullptr)
                    {
                        // Check if we are running as a Listen Server (NM_ListenServer == 2)
                        if (World->InternalGetNetMode() == ENetMode::NM_ListenServer)
                        {
                            // If listen server, force end the game with Reason=1
                            LocalPC->Server_EndGame(true);
                        }
                    }

                    // Trigger the Round Over event
                    this->OnGameRoundOver();
                }
            }
        }
    }

    // --- Context System Transition Logic ---
    // The following section handles UI/State transitions using the custom Context System.
    // It utilizes std::shared_ptr logic heavily in the assembly (checking _Rep and _Use counts).

    // 1. Handle Overlay Context
    if (this->_contextSystem != nullptr)
    {
        auto OverlayContext = this->_contextSystem->m_OverlayContextGroup;
        if (OverlayContext._Ptr != nullptr)
        {
            // Request the end of the current overlay context
            /* UNDEFINED ELEMENT: Custom Context System Call */
            CTX::ContextGroup::RequestContextEnd(OverlayContext._Ptr);
        }
    }

    // 2. Handle Navigation Context
    if (this->_contextSystem != nullptr)
    {
        auto NavigationContext = this->_contextSystem->m_NavigationContextGroup;
        if (NavigationContext._Ptr != nullptr)
        {
            // Request transition for navigation. Arguments: Target=1, Type=0, Unknown=1
            /* UNDEFINED ELEMENT: Custom Context System Call */
            CTX::ContextGroup::RequestTransition(NavigationContext._Ptr, 1, 0, 1);
        }
    }

    // 3. Handle GameFlow Context (Reusing the shared pointer retrieved at the start)
    if (GameFlowContext._Ptr != nullptr)
    {
        // Request transition for GameFlow. Arguments: Target=4, Type=2, Unknown=1
        /* UNDEFINED ELEMENT: Custom Context System Call */
        CTX::ContextGroup::RequestTransition(GameFlowContext._Ptr, 4, 2, 1);
    }
}




void UDBDGameInstance::DBD_LogBloodWebDefinition()
{
    // Create a new instance of UDBDBloodWebDefinition.
    // The disassembly calls GetTransientPackage() to use as the Outer for the new object.
    // The sequence of 'GetTransientPackage', 'AssertIfInConstructor', and 'StaticConstructObject_Internal' 
    // corresponds to the standard NewObject<T> implementation.
    class UDBDBloodWebDefinition* NewBloodWebDefinition = NewObject<UDBDBloodWebDefinition>(GetTransientPackage());

    // Validate that the object was created successfully before using it.
    if (NewBloodWebDefinition != nullptr)
    {
        // Call the LogToConsole method on the newly created instance.
        // In the disassembly, this is performed via a tail jump (JMP), passing the new object instance as 'this'.
        NewBloodWebDefinition->LogToConsole();
    }
}




void UDBDGameInstance::DBD_PrintAudioConfig()
{
    // Retrieve the Wwise Audio Device singleton
    // [140250b6d] call FAkAudioDevice::Get
    FAkAudioDevice* AudioDevice = FAkAudioDevice::Get();
    // Check if the Audio Device is valid
    // [140250b75] test rax, rax; je 0x140250da9
    if (AudioDevice == nullptr)
    {
        return;
    }

    // --- Part 1: Panning Rule ---

    // Determine the Panning Rule (Headphones vs Speakers)
    // [140250b86] call FAkAudioDevice::GetPanningRule
    // [140250b8b] test eax, eax
    const TCHAR* PanningRuleString = TEXT("Headphones");

    // In assembly, 'cmove' (conditional move) is used. 
    // If GetPanningRule returns a non-zero value, it switches to "Speakers".
    if (AudioDevice->GetPanningRule() == AkPanningRule::AkPanningRule_Speakers)
    {
        PanningRuleString = TEXT("Speakers");
    }

    // Format the string for display
    // [140250baa] call FString::Printf__VA
    FString PanningOutput = FString::Printf(TEXT("Panning Rule: %s"), PanningRuleString);

    // Print to screen (GEngine->AddOnScreenDebugMessage)
    // [140250bd0] loads GEngine
    // [140250baf] loads FColor::Red
    // [140250bba] loads 5.0f (0x40a00000)
    if (GEngine != nullptr)
    {
        /* * The decompiler labeled the call at [140250be2] as AddStructReferencedObjectsOrNot.
         * However, based on the arguments (GEngine, Key=-1, Time=5.0f, Color=Red),
         * this is strictly GEngine->AddOnScreenDebugMessage.
         */
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, PanningOutput);
    }

    // --- Part 2: Speaker Configuration (Channel Count) ---

    // Get the Speaker Configuration struct
    // [140250bfc] call FAkAudioDevice::GetSpeakerConfiguration
    AkChannelConfig Config;
    AudioDevice->GetSpeakerConfiguration(Config);

    // Print the number of channels (extracting the first byte of the struct)
    // [140250c01] movzx r8d, byte [rbp+0x18] (Accessing uNumChannels)
    FString NumChannelsOutput = FString::Printf(TEXT("NumChannels: %i"), Config.uNumChannels);

    // Print to screen
    if (GEngine != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, NumChannelsOutput);
    }

    // --- Part 3: Speaker Configuration (Config Type) ---

    // Analyze the raw configuration bits to determine the text label.
    // [140250c55] mov eax, dword [rbp+0x18] (Load full 4-byte struct)
    uint32 RawConfigValue = *reinterpret_cast<uint32*>(&Config);

    // [140250c5c] shr eax, 0xc (Shift right by 12 bits to isolate specific config flags)
    uint32 ConfigTypeIndex = RawConfigValue >> 12;

    const TCHAR* ConfigName = TEXT("None");

    // The assembly performs a series of subtractions/comparisons to switch on the index.
    // [140250c5f] sub eax, 3; je ... (Checks for Stereo)
    if (ConfigTypeIndex == 3)
    {
        // [140250d25] "Stereo"
        ConfigName = TEXT("Stereo");
    }
    // [140250c68] sub eax, 1; je ... (Checks for Mono)
    // Note: The previous subtraction was 3, so effective check is (Index - 3 - 1) == 0 => Index == 4
    else if (ConfigTypeIndex == 4)
    {
        // [140250d01] "Mono"
        ConfigName = TEXT("Mono");
    }
    // [140250c6d] sub eax, 0x60b; je ... 
    // Effective check: Index - 4 - 0x60b == 0 => Index == 0x60F
    else if (ConfigTypeIndex == 0x60F)
    {
        // [140250cdd] Loads data_1428f7aa8
        // Based on string length (8 bytes/4 WCHARS) and context, this is likely "5.1"
        /* UNDEFINED ELEMENT: String at data_1428f7aa8 (Likely "5.1") */
        ConfigName = TEXT("5.1");
    }
    // [140250c76] cmp eax, 0x30
    // Effective check: (Index - 0x60F) == 0x30 => Index == 0x63F
    else if (ConfigTypeIndex == 0x63F)
    {
        // [140250cb9] Loads data_1428f7be8
        // Based on string length (8 bytes/4 WCHARS) and context, this is likely "7.1"
        /* UNDEFINED ELEMENT: String at data_1428f7be8 (Likely "7.1") */
        ConfigName = TEXT("7.1");
    }
    else
    {
        // [140250c94] "None"
        ConfigName = TEXT("None");
    }

    // Format the final config string
    // [140250d53] call FString::Printf__VA
    FString ConfigOutput = FString::Printf(TEXT("Config: %s"), ConfigName);

    // Print to screen
    if (GEngine != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ConfigOutput);
    }

    // [140250da9] Function epilogue and memory cleanup
    // Note: FString logic in C++ automatically handles the FMemory::Free calls seen in ASM.
}




void UDBDGameInstance::DBD_RemoveRitual(FName toRemove)
{
    // 1. Retrieve the local player state.
    // "GetLocalPlayerState" is likely a helper function within UDBDGameInstance.
    ADBDPlayerState* LocalPlayerState = this->GetLocalPlayerState();
    // 2. Null check for the PlayerState.
    // ASM: test rax, rax; je 0x140250e44
    if (LocalPlayerState == nullptr)
    {
        return;
    }

    // 3. Validation Logic.
    // The disassembly performs a complex check involving the Global Object Array (GUObjectArray)
    // and a specific bitfield in the Actor.
    //
    // Check A: Checks the 'PendingKill' flag in the Global Object Array (IsPendingKill()).
    // ASM: Checks GUObjectArray.ObjObjects[InternalIndex] flags >> 0x1d & 1.
    //
    // Check B: Checks the 'bActorIsBeingDestroyed' bitfield at offset 0x140.
    // ASM: test byte [r8+0x140], 0x4
    // Looking at the provided image (union at 0x140):
    // 0x1: bAutoDestroyWhenFinished
    // 0x2: bCanBeDamaged
    // 0x4: bActorIsBeingDestroyed <--- Matches 0x4

    if (LocalPlayerState->IsPendingKill() || LocalPlayerState->bActorIsBeingDestroyed)
    {
        return;
    }

    /* Note: The disassembly repeats the IsPendingKill check here (ASM lines 140250e19 - 140250e2c).
       This is likely a compiler artifact from inlining 'IsValid' logic multiple times
       or a redundant safety check. In clean C++, the check above suffices.
    */

    // 4. Access the RitualHandler and perform the action.
    // ASM: mov rcx, qword [r8+0x778] -> Accesses member at 0x778
    // ASM: call URitualHandlerComponent::DismissRitual
    // We assume offset 0x778 corresponds to the 'RitualHandler' component in ADBDPlayerState.
    if (LocalPlayerState->RitualHandler != nullptr)
    {
        LocalPlayerState->RitualHandler->DismissRitual(toRemove);
    }
}




void UDBDGameInstance::DBD_ResetAllRituals()
{
    // 1. Retrieve the local player state.
    // ASM: call UDBDGameInstance::GetLocalPlayerState
    ADBDPlayerState* LocalPlayerState = this->GetLocalPlayerState();
    // 2. Null check for the PlayerState.
    // ASM: test rax, rax; je 0x140250ed3
    if (LocalPlayerState == nullptr)
    {
        return;
    }

    // 3. Validation Logic.
    // Like the previous function, this checks the Global Object Array for the 'PendingKill' flag
    // and checks the specific 'bActorIsBeingDestroyed' bit in the actor's bitfield.
    //
    // ASM: Checks GUObjectArray.ObjObjects[InternalIndex] flags
    // ASM: test byte [r8+0x140], 0x4 (Checks bActorIsBeingDestroyed)
    if (LocalPlayerState->IsPendingKill() || LocalPlayerState->bActorIsBeingDestroyed)
    {
        return;
    }

    // 4. Access the RitualHandler and call ResetRituals.
    // ASM: mov rcx, qword [r8+0x778] -> Accesses RitualHandler pointer
    // ASM: jmp URitualHandlerComponent::ResetRituals -> Tail call optimization
    //
    // The compiler generated a 'jmp' (Tail Call) because this is the last operation 
    // in the function and the stack frame was already cleaned up.
    if (LocalPlayerState->RitualHandler != nullptr)
    {
        LocalPlayerState->RitualHandler->ResetRituals();
    }
}




void UDBDGameInstance::DBD_ResetCurrentUserStat(FName statName, uint8 andUpload)
{
    // 1. Retrieve the Local Player at index 0.
    // ASM: xor edx, edx (0) -> call UGameInstance::GetLocalPlayerByIndex
    ULocalPlayer* LocalPlayer = this->GetLocalPlayerByIndex(0);

    // 2. Validation Logic.
    // The code checks if the pointer is null, and then validates the object 
    // against the Global Object Array (GUObjectArray) to ensure it is not marked for destruction.
    //
    // ASM: test rax, rax; je ...
    // ASM: Checks GUObjectArray.ObjObjects[InternalIndex] flags (PendingKill check)
    if (LocalPlayer == nullptr || LocalPlayer->IsPendingKill())
    {
        return;
    }

    // 3. Access PlayerProfileDAL and perform the reset.
    // ASM: mov rcx, qword [rbx+0x120] -> Access member at offset 0x120
    // ASM: call UPlayerProfileDAL::ResetCurrentPlayerStat
    // We assume offset 0x120 corresponds to 'PlayerProfileDAL' in UDBDGameInstance.
    if (this->PlayerProfileDAL != nullptr)
    {
        // Note: The disassembly passes 'andUpload' (r8b/dil) as the second argument to the DAL function.
        this->PlayerProfileDAL->ResetCurrentPlayerStat(statName, andUpload);
    }
}




void UDBDGameInstance::DBD_ResetCurrentUserStats(uint8 andUpload, uint8 andUpdateData)
{
    // 1. Retrieve the Local Player at index 0.
    // ASM: call UGameInstance::GetLocalPlayerByIndex
    ULocalPlayer* LocalPlayer = this->GetLocalPlayerByIndex(0);

    // 2. Validate Local Player.
    // ASM: test rax, rax ... checks GUObjectArray for PendingKill
    if (LocalPlayer == nullptr || LocalPlayer->IsPendingKill())
    {
        return;
    }

    // 3. Get the Preferred Unique Net ID.
    // ASM: call ULocalPlayer::GetPreferredUniqueNetId
    // The disassembly shows significant overhead here (ReferenceController, SharedReferenceCount). 
    // This is the raw assembly representation of managing a TSharedPtr. 
    // In C++, the TSharedPtr destructor (RAII) handles the cleanup logic seen at the end of the ASM (1402510cf).
    TSharedPtr<const FUniqueNetId> UniqueNetId = LocalPlayer->GetPreferredUniqueNetId();

    // 4. Call PlayerProfileDAL->ResetCurrentPlayerStats.
    // ASM: mov r8d, 0x1 (3rd arg)
    // ASM: movzx r9d, r15b (4th arg, which is 'andUpload')
    // ASM: lea rdx, [var_28] (Pointer to the TSharedPtr)
    if (this->PlayerProfileDAL != nullptr)
    {
        // Note: The function signature appears to take the UniqueId, a boolean/flag (1), and the upload flag.
        this->PlayerProfileDAL->ResetCurrentPlayerStats(UniqueNetId, true, andUpload);
    }

    // 5. Retrieve and Cast the Player Controller.
    // ASM: mov rdi, qword [rbp+0x30] (LocalPlayer->PlayerController)
    APlayerController* PlayerController = LocalPlayer->PlayerController;
    if (PlayerController != nullptr)
    {
        // 6. Perform Safe Cast to ADBDPlayerControllerBase.
        // ASM: call ADBDPlayerControllerBase::GetPrivateStaticClass
        // ASM: Checks ClassTreeIndex logic. This is the compiled result of Cast<T>() or IsA().
        ADBDPlayerControllerBase* DBDController = Cast<ADBDPlayerControllerBase>(PlayerController);

        // 7. Validate the DBDController.
        // ASM: checks GUObjectArray (PendingKill)
        // ASM: test byte [rdi+0x140], 0x4 (bActorIsBeingDestroyed)
        if (DBDController != nullptr 
         && DBDController->IsPendingKill() == false 
         && DBDController->bActorIsBeingDestroyed == false)
        {
            // 8. Conditional Update.
            // ASM: test r12b, r12b (andUpdateData)
            if (andUpdateData != 0)
            {
                // ASM: call ADBDPlayerControllerBase::Local_LoadProfileStats
                DBDController->Local_LoadProfileStats();
            }
        }
    }

    // End of scope: UniqueNetId (TSharedPtr) is automatically destroyed here.
    // ASM lines 1402510cf - 1402510ee handle the decrement of ReferenceController and deletion if count == 0.
}




void UDBDGameInstance::DBD_ResetInventory()
{
    // 1. Retrieve the Local Player State.
    // Note: GetLocalPlayerState appears to be a helper method within this class.
    class APlayerState* LocalPlayerState = this->GetLocalPlayerState();

    // 2. Retrieve the saved profile data for the specific player state.
    // This returns a pointer to the FCharacterSavedProfileData structure.
    struct FCharacterSavedProfileData* SavedProfileData = this->GetCharacterSavedProfileDataForPlayerState(LocalPlayerState);
    // 3. Validate the profile data pointer. 
    // If the data is null, we cannot proceed with resetting the inventory.
    if (SavedProfileData == nullptr)
    {
        return;
    }

    // 4. Construct the removal predicate.
    // The disassembly (140251137 - 14025115c) sets up a TFunction/Lambda structure.
    // It allocates memory and sets a VTable (UE4Function_Private::TFunction...::vftable).
    // It references a specific function pointer for the logic: 'Scaleform::GFx::AS2::AvmButton::ActsAsButton'.

    // /* UNDEFINED VTABLE */ - The specific TFunction VTable is handled by the compiler's lambda generation.
    auto InventoryRemovalPredicate = [&](/* Parameters unknown, likely const FName& or similar */ void* Item) -> bool
        {
            /* UNDEFINED ELEMENT */
            // The logic here calls the function at address 14025115c.
            // The decompiler labels this: Scaleform::GFx::AS2::AvmButton::ActsAsButton
            // This is likely a boolean predicate returning 'true' to ensure all items are removed.
            return true;
        };

    // 5. Remove inventory items using the predicate.
    // The constructed TFunction (referencing the lambda above) is passed here.
    SavedProfileData->RemoveAllInventoryNamed(InventoryRemovalPredicate);

    // 6. Save the full profile via the Player Data Facade.
    // Accessing member variable _playerDataFacade at offset 0x3c0.
    if (this->_playerDataFacade != nullptr)
    {
        // 14025117e: SaveFullProfile is called with a second argument of 0 (likely false/SyncMode).
        this->_playerDataFacade->SaveFullProfile(false);
    }
}




void UDBDGameInstance::DBD_SendFriendInvite()
{
    // 1. Initialize the subsystem name to None.
    // ASM: xor ebx, ebx (sets to 0/NAME_None)
    FName SubsystemName = NAME_None;

    // 2. Retrieve the static OnlineSubsystem instance.
    // ASM: call IOnlineSubsystem::Get
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(SubsystemName);
    // 3. Validate the subsystem.
    if (OnlineSubsystem == nullptr)
    {
        return;
    }

    // 4. Validate _persistentData.
    // First, check if the pointer itself is null.
    if (this->_persistentData == nullptr)
    {
        return;
    }

    // 5. Perform Low-Level Validity Check on _persistentData.
    // ASM (1402511da - 1402511fd): Checks the InternalIndex against GUObjectArray 
    // to ensure the object is not unreachable or pending kill.
    if (this->_persistentData->IsValidLowLevel() == false)
    {
        return;
    }

    // 6. Retrieve the External UI Interface.
    // ASM: call qword [rax+0x60] (VTable offset 96)
    // Based on the subsequent call signature, this is GetExternalUIInterface.
    IOnlineExternalUIPtr ExternalUIInterface = OnlineSubsystem->GetExternalUIInterface();

    // 7. Check if the interface is valid (TSharedPtr check).
    if (ExternalUIInterface.IsValid())
    {
        // 8. Retrieve necessary data for the invite.
        UOnlineSystemHandler* OnlineSystemHandler = this->GetOnlineSystemHandler();
        ULocalPlayer* LocalPlayer = this->GetLocalPlayer();

        // 9. Open the Invite UI.
        // ASM: call qword [rdi+0x18] (VTable offset 24)
        // Arguments passed: 
        //   EDX: LocalPlayer->ControllerId (int32)
        //   R8:  OnlineSystemHandler->_sessionName (FName)
        // This matches the signature of ShowInviteUI.
        ExternalUIInterface->ShowInviteUI(LocalPlayer->ControllerId, OnlineSystemHandler->_sessionName);
    }

    // 10. TSharedPtr Destructor.
    // ASM (14025124e - 140251289): The destructor for ExternalUIInterface is called here 
    // automatically as the variable goes out of scope, handling reference decrements.
}




void UDBDGameInstance::DBD_SetBonusEventsDate(FString dateString)
{
    // 1. Initialize a default DateTime (0001.01.01-00:00:00).
    // ASM: FDateTime constructor call at 1402512db
    FDateTime ParsedDate = FDateTime(1, 1, 1, 0, 0, 0, 0);

    // 2. Parse the input string into the DateTime struct.
    // ASM: FDateTime::ParseIso8601
    // Note: ParseIso8601 takes the string (buffer) and an output DateTime reference.
    FDateTime::ParseIso8601(*dateString, ParsedDate);

    // 3. Retrieve Bonus Points Events from Design Tunables.
    // ASM: UDBDDesignTunables::GetBonusPointsEvents
    // The result is returned by value (temp array) and assigned to the member variable.
    if (this->DesignTunables != nullptr)
    {
        // The assignment operator (=) copies the data to this->BonusPointsEvents.
        // The temporary array returned by GetBonusPointsEvents is then destructed 
        // (represented by the loop checking offsets 0x30, 0x20, 0x08 in the disassembly).
        this->BonusPointsEvents = this->DesignTunables->GetBonusPointsEvents(ParsedDate);
    }

    // 4. Update the request flag.
    // ASM: mov byte [rbp+0x228], 0x0
    this->ShouldRequestBonusPointsEvents = false;

    // 5. Implicit Destructor for dateString.
    // ASM: 14025139b call FMemory::Free
    // As 'dateString' goes out of scope, its internal data buffer is freed.
}




void UDBDGameInstance::DBD_SetCursorDefaultSpeed(float value)
{
    // 1. Check if the AnalogCursor is valid.
    // Accessing member variable AnalogCursor at offset 0x380.
    // In UE4, if this is a TSharedPtr, we access the internal object.
    if (this->AnalogCursor.IsValid())
    {
        // 2. Forward the call to the Analog Cursor instance.
        // ASM: jne FDBDAnalogCursor::SetCursorDefaultSpeed
        // The compiler optimized this into a tail call because the arguments match.
        this->AnalogCursor->SetCursorDefaultSpeed(value);
    }
}




void UDBDGameInstance::DBD_SetCursorStickIgnore(float value)
{
    // 1. Check if the AnalogCursor is valid.
    // Accessing member variable AnalogCursor at offset 0x380.
    if (this->AnalogCursor.IsValid())
    {
        // 2. Forward the call to the Analog Cursor instance.
        // ASM: jne FDBDAnalogCursor::SetCursorStickIgnore
        // The compiler optimized this into a tail call.
        this->AnalogCursor->SetCursorStickIgnore(value);
    }
}




void UDBDGameInstance::DBD_SetCursorStickySpeed(float value)
{
    // 1. Check if the AnalogCursor is valid.
    // Accessing member variable AnalogCursor at offset 0x380.
    if (this->AnalogCursor.IsValid())
    {
        // 2. Forward the call to the Analog Cursor instance.
        // ASM: jne FDBDAnalogCursor::SetCursorStickySpeed
        // Tail call optimization passes 'this' (the cursor object) and 'value' directly.
        this->AnalogCursor->SetCursorStickySpeed(value);
    }
}




void UDBDGameInstance::DBD_SetStatsUploadEnabled(bool enabled)
{
    // 1. Set the internal flag.
    // ASM: mov byte [rcx+0x60a], dl
    // Accessing member variable _achievementStatsUploadEnabled at offset 0x60a.
    this->_achievementStatsUploadEnabled = enabled;
}




void UDBDGameInstance::DBD_StartQuickPlay()
{
    // Access the Context System
    UGameFlowContextSystem* ContextSystem = this->_contextSystem;

    // Access m_GameFlowContextGroup.
    // In the disassembly, a copy of the shared_ptr is created (reference count incremented: lock inc dword [rdi+0x8]).
    // accessing _Ptr directly suggests we are getting the raw pointer from the shared_ptr.
    std::shared_ptr<CTX::ContextGroup> GameFlowContextGroup = ContextSystem->m_GameFlowContextGroup;
    // Check if the Context Group is valid
    if (GameFlowContextGroup == nullptr)
    {
        return;
    }

    // Check the size of the array within the Context Group.
    // /* UNDEFINED ELEMENT */ - Scaleform function call.
    uint32_t ArraySize = Scaleform::GFx::AS3::VectorBase<long>::GetArraySize(GameFlowContextGroup.get());

    // The disassembly performs a specific check logic:
    // It checks if size is 14. If not, it calls GetArraySize again and checks if it is 1.
    bool bShouldProceed = false;

    if (ArraySize == 14)
    {
        bShouldProceed = true;
    }
    else
    {
        // /* UNDEFINED ELEMENT */
        if (Scaleform::GFx::AS3::VectorBase<long>::GetArraySize(GameFlowContextGroup.get()) == 1)
        {
            bShouldProceed = true;
        }
    }

    if (bShouldProceed)
    {
        // --- Handle Overlay Context ---
        UGameFlowContextSystem* LocalContextSystem = this->_contextSystem;
        std::shared_ptr<CTX::ContextGroup> OverlayContextGroup = LocalContextSystem->m_OverlayContextGroup;
        if (OverlayContextGroup != nullptr)
        {
            // Request the end of the current context.
            // The function returns a shared_ptr (TaskHandle) which is immediately destructed in the ASM (lock xadd ... _Uses).
            // /* UNDEFINED ELEMENT */
            CTX::ContextGroup::RequestContextEnd(OverlayContextGroup.get());
        }

        // --- Handle System Prompts Context ---
        std::shared_ptr<CTX::ContextGroup> SystemPromptsContextGroup = LocalContextSystem->m_SystemPromptsContextGroup;
        if (SystemPromptsContextGroup != nullptr)
        {
            // /* UNDEFINED ELEMENT */
            CTX::ContextGroup::RequestContextEnd(SystemPromptsContextGroup.get());
        }

        // --- Handle Game Flow Transition ---
        // Request a transition on the GameFlowContextGroup (stored in r12 in ASM).
        // Arguments: ContextPtr, TransitionID (1), Payload (0), UnknownFlag (1).
        // /* UNDEFINED ELEMENT */
        CTX::ContextGroup::RequestTransition(GameFlowContextGroup.get(), 1, 0, 1);

        // --- Handle Navigation Context ---
        std::shared_ptr<CTX::ContextGroup> NavigationContextGroup = LocalContextSystem->m_NavigationContextGroup;
        if (NavigationContextGroup != nullptr)
        {
            // Request a transition on the NavigationContextGroup.
            // Arguments: ContextPtr, TransitionID (2), Payload (0x47 = 71), UnknownFlag (1).
            // /* UNDEFINED ELEMENT */
            CTX::ContextGroup::RequestTransition(NavigationContextGroup.get(), 2, 0x47, 1);
        }
    }

    // Local shared_ptr variables (GameFlowContextGroup, OverlayContextGroup, etc.) go out of scope here.
    // The compiler generates the code to decrement the reference counts (_Uses, _Weaks) and delete if necessary.
}




void UDBDGameInstance::DBD_TestDiceRoll(float BaseProbability, float MultiplicativeModifier, float AdditiveModifier)
{
    // The disassembly clears the first argument (ECX = 0) and performs a jump (tail call) to the dice roller.
    // This indicates that the DiceRoller function accepts a context object as the first parameter, 
    // which is explicitly set to nullptr in this test function.

    /* UNDEFINED ELEMENT */
    UDBDDiceRoller::CalculateRollResult(nullptr, BaseProbability, MultiplicativeModifier, AdditiveModifier);
}




void UDBDGameInstance::DBD_TestRegion()
{
    // Access the RegionFinder member variable.
    // The assembly performs a tail call (JMP) to PingRegions, effectively passing
    // 'this->RegionFinder' as the 'this' pointer for the function call.
    // No null check is performed on the pointer before the call.

    /* UNDEFINED ELEMENT */
    this->RegionFinder->PingRegions();
}




void UDBDGameInstance::DBD_TestSaveFile(FString name)
{
    // Calculate the absolute path for the long JSON test file.
    // The assembly constructs this by concatenating the save directory with the filename.
    FString LongFileName = FPaths::GameSavedDir() + TEXT("Test/LongJson.txt");

    FString JsonString;

    // Convert the local player's persistent data to a verbose JSON string.
    // The 3rd argument '0' likely represents a flag or indentation level.
    /* UNDEFINED ELEMENT */
    DBDSaveUtils::DBDPersistentDataToJsonString(this->_persistentData->_localPlayerPersistentData.SavedData, JsonString, 0);

    // Save the verbose JSON string to the file.
    // EncodingOptions::AutoDetect (0) is used.
    FFileHelper::SaveStringToFile(JsonString, *LongFileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());

    // Calculate the absolute path for the short (condensed) JSON test file.
    FString ShortFileName = FPaths::GameSavedDir() + TEXT("Test/ShortJson.txt");

    // Convert the local player's persistent data to a condensed JSON string.
    // This call reuses the 'JsonString' variable.
    /* UNDEFINED ELEMENT */
    DBDSaveUtils::DBDPersistentDataToJsonStringCondensed(this->_persistentData->_localPlayerPersistentData.SavedData, JsonString);

    // Save the condensed JSON string to the file.
    FFileHelper::SaveStringToFile(JsonString, *ShortFileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
}




void UDBDGameInstance::DBD_TestTickedDiceRoll(float BaseProbability, float ticks, float MultiplicativeModifier, float AdditiveModifier)
{
    // The disassembly clears the first argument (ECX = 0) and performs a jump (tail call) to the dice roller.
    // This indicates that the DiceRoller function accepts a context object as the first parameter, 
    // which is explicitly set to nullptr in this test function.

    /* UNDEFINED ELEMENT */
    UDBDDiceRoller::CalculateTickedRollResult(nullptr, BaseProbability, ticks, MultiplicativeModifier, AdditiveModifier);
}




void UDBDGameInstance::DBD_ToggleOnlineRole()
{
    /* UNDEFINED ELEMENT */
    ADBDPlayerState* LocalPlayerState = this->GetLocalPlayerState();

    // The assembly performs explicit checks against the Global Unreal Object Array (GUObjectArray)
    // and internal flags (PendingKill/Unreachable). In C++, this is equivalent to IsValid().
    if (IsValid(LocalPlayerState) == false)
    {
        return;
    }

    // Toggle logic: 
    // If GameRole is 1, (1 == 1) is true (1), + 1 = 2.
    // If GameRole is 2, (2 == 1) is false (0), + 1 = 1.
    // This switches the role between 1 and 2.
    EPlayerRole NewRole = (LocalPlayerState->GameRole == EPlayerRole::VE_Slasher) ? EPlayerRole::VE_Camper : EPlayerRole::VE_Slasher;

    // Perform the server call to update the role.
    /* UNDEFINED ELEMENT */
    LocalPlayerState->Server_SetGameRole(NewRole);
}




void UDBDGameInstance::DBD_UpdateSteamInventory()
{
    // Retrieve the DataStore subsystem from the OnlinePresencePlugin.
    // The disassembly indicates this returns a TSharedRef (or TSharedPtr assumed valid),
    // as it is accessed immediately without a null check.
    /* UNDEFINED ELEMENT */
    TSharedRef<IDataStorePresenceSubsystem> DataStore = IOnlinePresencePlugin::DataStore();

    // Call the virtual function at offset 0x178 (UpdateUserInventory).
    DataStore->UpdateUserInventory();

    // The TSharedRef goes out of scope here, triggering the reference counting cleanup 
    // observed in the disassembly (lines 14027b4fc - 14027b52d).
}




void UDBDGameInstance::EndAuthentication()
{
    // access _auth member
    // The assembly performs a null check followed by a validation of internal object flags 
    // via GUObjectArray. This is equivalent to the standard UE4 IsValid() check.
    if (IsValid(this->_auth))
    {
        /* UNDEFINED ELEMENT */
        this->_auth->CancelTickets();

        this->_auth = nullptr;
    }
}




void UDBDGameInstance::FinalNetworkConnectionCheck()
{
    // Check if the current status is NOT "NoNetworkConnection".
    // If we think we might be connected (or in any state other than explicitly no network),
    // we verify it against the platform using the OnlinePresence plugin.
    if (this->_ConnectionStatus != EOnlineServerConnectionStatus::Type::NoNetworkConnection)
    {
        // Load the OnlinePresence module interface.
        /* UNDEFINED ELEMENT */
        IOnlinePresencePlugin* OnlinePresence = FModuleManager::LoadModuleChecked<IOnlinePresencePlugin>("OnlinePresence");

        // Check if the platform reports we are actually connected AND the local profile is signed in.
        // We use the virtual function IsConnected() (Offset 0xA0).
        if (OnlinePresence->IsConnected() && this->IsMainProfileSignedIn(true))
        {
            // If we are detected as online by the platform, log a warning and force the internal status to Connected.
            UE_LOG(GameFlow, Warning, TEXT("FinalNetworkConnectionCheck: we are somehow still connected to a network. Reset Connection status from %d to %d"),
                (int32)this->_ConnectionStatus, (int32)EOnlineServerConnectionStatus::Type::Connected);

            this->_ConnectionStatus = EOnlineServerConnectionStatus::Type::Connected;
            return;
        }
    }

    // Fallback/Cleanup path:
    // If we were already disconnected (Status == NoNetworkConnection) or the actual platform connection check failed.
    this->_initializedFromCloud = false;

    this->ShowNetworkErrorPrompt();

    UE_LOG(GameFlow, Verbose, TEXT("vvv OnLeavingOnlineMultiplayer vvv"));
}




void UDBDGameInstance::FireSoundEvent(AActor* instigator, const FVector* location, uint8_t shouldTrack, float audibleRange, ADBDPlayer* instigatingPlayer, uint8_t isQuickAction)
{
    float finalAudibleRange = audibleRange;

    // Check if the instigating player is valid
    if (instigatingPlayer != nullptr)
    {
        // Based on assembly 14025510f-14025514b:
        // Checks Global Object Array for PendingKill flag (Bit 29) 
        // AND checks the bActorIsBeingDestroyed flag (offset 0x140, bit mask 0x4)
        if (instigatingPlayer->IsPendingKill() == false && instigatingPlayer->bActorIsBeingDestroyed == false)
        {
            // Call virtual function at offset 0x1070 (defined in _V as IsValidImpl)
            /* UNDEFINED VTABLE */
            if (instigatingPlayer->IsValidImpl())
            {
                // Access member at 0x7a8 (PerkManager)
                // Assembly: 140255175 lea rcx, [rbx+0x7a8]; mov rax, [rcx]
                /* UNDEFINED ELEMENT */
                UPerkManager* perkManager = instigatingPlayer->PerkManager;

                if (perkManager != nullptr)
                {
                    // Determine the modifier type based on action speed
                    // 0xA8 = VE_ModifyQuickActionLoudNoiseRange
                    // 0x54 = VE_ModifyLoudNoiseRangeFromSelf
                    EGameplayModifierType modifierType = VE_ModifyQuickActionLoudNoiseRange;

                    if (isQuickAction == false)
                    {
                        modifierType = VE_ModifyLoudNoiseRangeFromSelf;
                    }

                    // Call virtual function at offset 0x10 of the PerkManager
                    // Assembly: 140255193 call UPerkManager::GetMultiplicativeModifierValue
                    /* UNDEFINED ELEMENT */
                    float modifierValue = perkManager->GetMultiplicativeModifierValue(modifierType);

                    // Apply the modifier to the range (mulss instruction)
                    finalAudibleRange = finalAudibleRange * modifierValue;
                }
            }
        }
    }

    // Access _soundEventSystem at offset 0x3C8
    if (this->_soundEventSystem == nullptr)
    {
        return;
    }

    // Create a local copy of the location vector (Assembly 1402551d0 - 1402551dc)
    FVector locationCopy;
    locationCopy.X = location->X;
    locationCopy.Y = location->Y;
    locationCopy.Z = location->Z;

    // Forward the event to the Sound Event System
    /* UNDEFINED ELEMENT */
    this->_soundEventSystem->FireSoundEvent(instigator, &locationCopy, shouldTrack, finalAudibleRange);
}




void UDBDGameInstance::ForceCloseGame()
{
    // Access the global Slate Application instance and close all windows immediately
    // Assembly 140255200: Loads the CurrentApplication global pointer
    // Assembly 140255207: Tail call jump to CloseAllWindowsImmediately
    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().CloseAllWindowsImmediately();
    }
}




void UDBDGameInstance::GenerateBloodWebForCurrentCharacter()
{
    // Retrieve the pointer to the BloodWebManager from the current GameInstance.
    // Based on the disassembly, the result is moved to RCX, which acts as the 'this' pointer 
    // for the subsequent function call.
    /* UNDEFINED ELEMENT */
    UBloodwebManager* BloodWebManager = this->GetBloodWebManager();

    // Call the function to build the web.
    // The disassembly shows a 'jmp' instruction (tail call optimization) instead of a 'call' + 'ret'.
    // This means the function returns immediately after executing BuildWebInPlace.
    /* UNDEFINED ELEMENT */
    BloodWebManager->BuildWebInPlace();
}




AActor* UDBDGameInstance::GenerateLighting(FName mapTheme)
{
    // Call virtual function at index 0x21 (offset 0x108).
    // Based on the pseudo code return type, this retrieves the World context.
    /* UNDEFINED VTABLE */
    UWorld* World = this->GetWorld();

    // Retrieve the helper object responsible for lighting generation.
    /* UNDEFINED ELEMENT */
    ULightingHelper* LightingHelper = this->GetLightingHelper();

    // Generate the lighting actor using the helper, passing the World and mapTheme.
    // The result is saved to be returned later (stored in RBX in assembly).
    /* UNDEFINED ELEMENT */
    AActor* Result = LightingHelper->GenerateLighting(World, mapTheme);

    // Apply modifications from offerings. This function does not take arguments
    // other than 'this' and does not use the 'Result' variable.
    /* UNDEFINED ELEMENT */
    this->ApplyOfferingModification();

    // Return the actor generated by the lighting helper.
    return Result;
}




UDBDAuthentication* UDBDGameInstance::GetAuthentication()
{
    // Check if the current authentication object is null or invalid (e.g. marked as Unreachable/Garbage Collected).
    // The disassembly explicitly checks the GUObjectArray flags (bit 29: RF_Unreachable), which corresponds 
    // to the standard IsValid() check in Unreal Engine.
    if (this->_auth == nullptr || IsValid(this->_auth) == false)
    {
        // Define the name for the new object.
        FName AuthName = FName("authentication");

        // Create a new instance of UDBDAuthentication.
        // The disassembly calls StaticConstructObject_Internal, which is the backend for NewObject.
        this->_auth = NewObject<UDBDAuthentication>(this, UDBDAuthentication::StaticClass(), AuthName);

        // Initialize the new authentication object, passing the current GameInstance as the owner/context.
        /* UNDEFINED ELEMENT */
        this->_auth->Initialize(this);
    }

    return this->_auth;
}




FBloodWebPersistentData* UDBDGameInstance::GetBloodWebData()
{
    // Retrieve the state of the local player.
    // The result is passed as an argument to the next function call.
    /* UNDEFINED ELEMENT */
    class APlayerState* PlayerState = this->GetLocalPlayerState();

    // Get the character's saved profile data based on the retrieved player state.
    /* UNDEFINED ELEMENT */
    struct FCharacterSavedProfileData* SavedProfileData = this->GetCharacterSavedProfileDataForPlayerState(PlayerState);
    // Check if the profile data was successfully retrieved.
    if (SavedProfileData == nullptr)
    {
        return nullptr;
    }

    // Return the pointer to the BloodWebData member within the saved profile data struct.
    // The disassembly performs an addition (add rax, 0x20) to the base pointer to access this member.
    return &SavedProfileData->BloodWebData;
}




UBloodwebManager* UDBDGameInstance::GetBloodWebManager()
{
    // Check if the current BloodwebManager is null or if the object is invalid (marked as Unreachable/Garbage Collected).
    // The disassembly checks the GUObjectArray flags directly, which corresponds to the standard IsValid() check.
    if (this->BloodwebManager == nullptr || IsValid(this->BloodwebManager) == false)
    {
        // Define the name for the new manager instance.
        FName ManagerName = FName("BloodwebManager");

        // Create a new UBloodwebManager instance.
        // The disassembly invokes StaticConstructObject_Internal, which is the backend for NewObject.
        this->BloodwebManager = NewObject<UBloodwebManager>(this, UBloodwebManager::StaticClass(), ManagerName);
    }

    return this->BloodwebManager;
}




FName UDBDGameInstance::GetCharacterDefaultItem(int32 inCharacterIndex)
{
    // Retrieve the DesignTunables object stored in the GameInstance (offset 0xF0).
    /* UNDEFINED ELEMENT */
    UDBDDesignTunables* Tunables = this->DesignTunables;

    // Call the function to get the character description based on the index.
    // The DesignTunables object becomes the 'this' pointer for this call.
    /* UNDEFINED ELEMENT */
    struct FCharacterDescription* CharacterDescription = Tunables->GetCharacterDescription(inCharacterIndex);
    // Check if the pointer returned is null.
    if (CharacterDescription == nullptr)
    {
        // If no description is found, return "None".
        // The assembly invokes the FName constructor with "None" directly into the return memory.
        return FName("None");
    }

    // Return the DefaultItem field from the character description structure (offset 0xF0).
    /* UNDEFINED ELEMENT */
    return CharacterDescription->DefaultItem;
}




FCharacterSavedProfileData* UDBDGameInstance::GetCharacterSavedProfileDataForPlayerState(const class UDBDGameInstance* this, class ADBDPlayerState* playerState)
{
    // Check if the playerState pointer is null
    if (playerState == nullptr)
    {
        return nullptr;
    }

    // Check if the Object is valid within the global object array (GUObjectArray).
    // The disassembly checks the internal index against the number of elements and the validity flags.
    // In Unreal Engine, this logic is encapsulated in IsValidLowLevel() or IsValid().
    if (playerState->IsValidLowLevel() == false)
    {
        return nullptr;
    }

    // Check specific bit flag at offset 0x140.
    // According to the provided structure image and disassembly (test byte [rdi+0x140], 0x4),
    // 0x4 corresponds to the 3rd bit field: bActorIsBeingDestroyed.
    if (playerState->bActorIsBeingDestroyed)
    {
        return nullptr;
    }

    // Retrieve the persistent data for the local player
    struct FPlayerPersistentData* persistentData = this->GetLocalPlayerPersistentData();
    // Verify if persistent data exists
    if (persistentData == nullptr)
    {
        return nullptr;
    }

    // Get the current character ID from the player state
    int32_t characterId = playerState->GetCurrentCharacterID();
    // Ensure the ID is valid (Assembly checks for negative values via 'js')
    if (characterId < 0)
    {
        return nullptr;
    }

    // The logic below represents a TMap lookup.
    // The disassembly attempts to find the data by hash. If it fails, it constructs a default
    // FCharacterSavedProfileData, adds it to the container, and returns a pointer to the new element.
    // This functionality is equivalent to TMap::FindOrAdd.

    // Accessing hierarchy: SavedData -> LocalData -> CharacterData
    // 'CharacterData' is likely a TMap<int32, FCharacterSavedProfileData> based on the TPairInitializer usage in pseudo code.

    return &persistentData->SavedData.LocalData.CharacterData.FindOrAdd(characterId);
}




bool UDBDGameInstance::GetCurrentGametypeAffectsProgression()
{
    // Access the persistent data storage from the instance
    // Disassembly: mov rax, qword [rcx+0x3b8]
    const class UDBDPersistentData* persistentData = this->_persistentData;

    // Check specific bits of the GameType variable.
    // Disassembly: test byte [rax+0x30], 0xfd
    // The offset 0x30 points to _gamePersistentData.SessionInfos.GameType inside the persistentData class.
    // The mask 0xFD (binary 1111 1101) is applied.
    // The 'sete' instruction returns 1 (true) if the result of the AND operation is 0 (Zero Flag set).
    // This implies that the function returns true only if the GameType has no bits set that overlap with 0xFD.
    // Effectively, this allows GameType values of 0 (None/Ranked) and 2 (Unknown/Specific Mode), while excluding others.
    // Maybe == 0 means EGameType::Online? 
    if ((persistentData->_gamePersistentData.SessionInfos.GameType & 0xFD) == EGameType::Online)
    {
        return true;
    }

    return false;
}




bool UDBDGameInstance::GetCurrentGametypeOnFriendInvite()
{
    // Access the persistent data storage
    // Disassembly: mov rax, qword [rcx+0x3b8]
    const class UDBDPersistentData* persistentData = this->_persistentData;

    // Retrieve the GameType byte
    // Disassembly: movzx ecx, byte [rax+0x30]
    uint8_t gameType = persistentData->_gamePersistentData.SessionInfos.GameType;

    // Check if the GameType is valid for friend invites.
    // The assembly uses a standard compiler optimization for checking a range: (Value - LowerBound) <= (UpperBound - LowerBound).
    //
    // Disassembly:
    // dec cl       ; gameType - 1
    // cmp cl, 0x1  ; Compare result with 1
    // setbe al     ; Set result if Below or Equal (unsigned)
    //
    // Logic: (gameType - 1) <= 1
    //
    // Verification:
    // If gameType == 0: (0 - 1) wraps to 255 (uint8). 255 <= 1 is False.
    // If gameType == 1: (1 - 1) = 0. 0 <= 1 is True.
    // If gameType == 2: (2 - 1) = 1. 1 <= 1 is True.
    // If gameType >= 3: (3 - 1) = 2. 2 <= 1 is False.
    //
    // Therefore, this function returns true only if GameType is 1 or 2.

    if (gameType == EGameType::PartyMode || gameType == EGameType::SurvivorGroup)
    {
        return true;
    }

    return false;
}




int32 UDBDGameInstance::GetDeadOrDisconnectedCampersCount(ADBDPlayerState* exception)
{
    // Check if the exception pointer is valid using standard GUObjectArray checks (implied by IsValid)
    // and specifically check the bActorIsBeingDestroyed flag as seen in the disassembly (0x140 check).
    if (exception == nullptr || exception->bActorIsBeingDestroyed)
    {
        return 0;
    }

    FString ExceptionIdStr;

    // Access the UniqueNetId from the PlayerState
    if (exception->UniqueId.UniqueNetId.IsValid())
    {
        // Get the raw bytes from the UniqueNetId
        /* UNDEFINED VTABLE: Calling GetSize (+0x10) and GetBytes (+0x8) from FUniqueNetId vtable */
        const int32 IdSize = exception->UniqueId.UniqueNetId->GetSize();
        const uint8* IdBytes = exception->UniqueId.UniqueNetId->GetBytes();

        if (IdSize > 0 && IdBytes != nullptr)
        {
            // The disassembly creates a Hex string from the bytes. 
            // Unreal Engine's BytesToHex is the functional equivalent of the manual loop seen in disassembly.
            ExceptionIdStr = BytesToHex(IdBytes, IdSize);
        }
    }

    int32 ResultCount = 0;

    // Iterate through the _cachePlayerStatus map.
    // The disassembly performs a low-level TMap iteration (checking allocation flags and iterating sparse array).
    for (auto It = this->_cachePlayerStatus.CreateConstIterator(); It; ++It)
    {
        const FString& PlayerId = It.Key();
        const uint8 PlayerStatus = It.Value();

        // Compare the Map Key (PlayerID) with the Exception's ID.
        // The disassembly uses 'towlower', indicating a case-insensitive comparison.
        if (PlayerId.Equals(ExceptionIdStr, ESearchCase::IgnoreCase))
        {
            continue;
        }

        // Check the status value.
        // Disassembly: cmp al, 0x2 (Equal to 2)
        // Disassembly: sub al, 0x6 -> cmp al, 0x1 (Checks if value is 6 or 7)
        // 2: Likely Disconnected/Dead
        // 6, 7: Likely Escaped or Left Match
        if (PlayerStatus == 2)
        {
            ResultCount++;
        }
        else if (PlayerStatus == 6 || PlayerStatus == 7)
        {
            ResultCount++;
        }
    }

    return ResultCount;
}




UDBDEasyAntiCheat* UDBDGameInstance::GetEAC()
{
    // Retrieve the pointer to the EasyAntiCheat instance stored at offset 0x500
    // Returns the member variable _eac
    return this->_eac;
}




ANetworkFenceActor* UDBDGameInstance::GetFence()
{
    // Access the member variable at offset 0x390 which is a WeakObjectPtr.
    // The disassembly performs a tail call to FWeakObjectPtr::Get, passing the address of _localFence.
    // We explicitly cast the result to the expected return type.
    return (ANetworkFenceActor*)this->_localFence.Get();
}




UClass* UDBDGameInstance::GetGameCharacterSynchronous(int32_t InCharacterIndex)
{
    // 1. Retrieve the data structure defining the character (Name, Assets, Tunables)
    FCharacterDescription* Desc = UDBDDesignTunables::GetCharacterDescription(this->DesignTunables, InCharacterIndex);
    if (Desc == nullptr)
    {
        return nullptr;
    }

    // 2. Access the Soft Pointer to the Pawn Blueprint (TSoftClassPtr<APawn>)
    // The raw code interacts with FWeakObjectPtr and FStringAssetReference internals here.
    UObject* LoadedAsset = Desc->GamePawn.Get();
    // 3. If the pointer is null or stale, attempt to resolve it
    if (LoadedAsset == nullptr)
    {
        // Check if the Asset Tag counter has changed since we last tested (Optimization)
        if (FStringAssetReference::CurrentTag.Counter != Desc->GamePawn.TagAtLastTest)
        {
            // Update the Weak Pointer by resolving the Object ID
            Desc->GamePawn.WeakPtr = FStringAssetReference::ResolveObject(Desc->GamePawn.ObjectID);

            // Sync the tag counters
            Desc->GamePawn.TagAtLastTest = FStringAssetReference::CurrentTag.Counter;

            // Try getting the object again after resolution
            LoadedAsset = Desc->GamePawn.WeakPtr.Get();
        }
    }

    // 4. Hard Fallback: Synchronous Load
    // If resolution failed (asset is not in memory), force load it from disk.
    if (LoadedAsset == nullptr)
    {
        LoadedAsset = this->AssetLoader.SynchronousLoad(Desc->GamePawn.ObjectID);
    }

    // 5. Validation: Ensure we loaded a Class that is actually a Pawn
    if (LoadedAsset)
    {
        // Check 1: Is the loaded object a UClass? (Inherits from /Script/CoreUObject.Class)
        UClass* AsClass = Cast<UClass>(LoadedAsset);

        if (AsClass)
        {
            // Check 2: Does this class inherit from APawn?
            // The assembly loops through SuperStructs (offset 0x30) to verify inheritance.
            if (AsClass->IsChildOf(APawn::StaticClass()))
            {
                return AsClass;
            }
        }
    }

    return nullptr;
}




ADBDGameState* UDBDGameInstance::GetGameState()
{
    // Call the virtual function GetWorld located at VTable index 33 (Offset 0x108).
    class UWorld* World = this->GetWorld();
    // Verify the World pointer is not null.
    if (World != nullptr)
    {
        // The assembly performs a manual check against the Global Object Array (GUObjectArray) 
        // to ensure the object flags do not indicate it is Unreachable or Pending Kill.
        // In UE4 C++, IsValidLowLevel() encapsulates these validity checks.
        if (World->IsValidLowLevel())
        {
            // Retrieve the GameState member variable from the World object.
            // Offset 0x58 in UWorld corresponds to 'GameState'.
            class AGameState* GameState = World->GameState;
            if (GameState != nullptr)
            {
                // Attempt to cast the generic GameState to the specific ADBDGameState class.
                // The assembly implements this via ADBDGameState::GetPrivateStaticClass() 
                // and a ClassTreeIndex comparison, which is the internal logic of the Cast<T> function.
                class ADBDGameState* DBDGameState = Cast<ADBDGameState>(GameState);
                if (DBDGameState != nullptr)
                {
                    return DBDGameState;
                }
            }

            // Return nullptr if the GameState is not of the expected type or is null.
            return nullptr;
        }
    }

    // Return nullptr if the World object is invalid.
    return nullptr;
}




TArray<ASlasherPlayer*> UDBDGameInstance::GetInGameKillers()
{
    // Initialize the result array
    TArray<ASlasherPlayer*> InGameKillers;

    // Iterate through the players array
    for (int32 Index = 0; Index < this->_players.Num(); Index++)
    {
        // Retrieve the player pointer from the array
        ADBDPlayer* PlayerCandidate = this->_players[Index];
        // Basic null check
        if (PlayerCandidate == nullptr)
        {
            continue;
        }

        // Cast check
        // The disassembly calls ASlasherPlayer::GetPrivateStaticClass and checks the ClassTreeIndex.
        // This corresponds to the Unreal Engine Cast<T> operation.
        ASlasherPlayer* SlasherPlayer = Cast<ASlasherPlayer>(PlayerCandidate);
        // If the player is not an ASlasherPlayer, skip to the next iteration
        if (SlasherPlayer == nullptr)
        {
            continue;
        }

        // Validity Check 1: Internal Object Flags
        // The disassembly checks GUObjectArray flags (specifically bit 29, which usually indicates PendingKill).
        if (SlasherPlayer->IsPendingKill())
        {
            continue;
        }

        // Validity Check 2: Actor Destruction Flag (Offset 0x140)
        // ASM: test byte [rbx+0x140], 0x4
        // Based on the provided structure, bit 0x4 (3rd bit) corresponds to bActorIsBeingDestroyed.
        if (SlasherPlayer->bActorIsBeingDestroyed)
        {
            continue;
        }

        // Validity Check 3: IsValidImpl (VTable Offset 0x1070)
        // ASM: call qword [rax+0x1070]; test al, al; je ...
        // Logic: Call the virtual function. If it returns false (0), skip adding the player.
        if (SlasherPlayer->IsValidImpl() == false)
        {
            continue;
        }

        // Validity Check 4: Paradise Status
        // ASM: call ADBDPlayer::IsInParadise; test al, al; jne ...
        // Logic: Call IsInParadise. If it returns true (1), skip adding the player.
        if (SlasherPlayer->IsInParadise())
        {
            continue;
        }

        // If all checks pass, add the SlasherPlayer to the list
        InGameKillers.Add(SlasherPlayer);
    }

    return InGameKillers;
}




TArray<ADBDPlayer*> UDBDGameInstance::GetInGamePlayers()
{
    // Initialize the return array
    // ASM: 140257a8a mov qword [rdx], 0x0 (Zeroing the array structure)
    TArray<ADBDPlayer*> InGamePlayers;

    // Iterate through the _players array stored in the GameInstance
    // ASM: 140257aa2 lea rcx, [r14+rax*8] (Loop bounds calculation)
    for (int32 Index = 0; Index < this->_players.Num(); Index++)
    {
        // Retrieve the player pointer from the array
        ADBDPlayer* PlayerCandidate = this->_players[Index];
        // Basic null check
        // ASM: 140257ad3 test rbx, rbx; je ...
        if (PlayerCandidate == nullptr)
        {
            continue;
        }

        // Validity Check 1: Internal Object Flags (Pending Kill)
        // ASM: 140257b00 shr eax, 0x1d; test al, 0x1
        // Checks the Global Object Array flags. Bit 29 is PendingKill.
        if (PlayerCandidate->IsPendingKill())
        {
            continue;
        }

        // Validity Check 2: Actor Destruction Flag (Offset 0x140)
        // ASM: 140257b07 test byte [rbx+0x140], 0x4
        // Based on the previously provided structure, bit 0x4 (3rd bit) corresponds to bActorIsBeingDestroyed.
        if (PlayerCandidate->bActorIsBeingDestroyed)
        {
            continue;
        }

        // Validity Check 3: IsValidImpl (VTable Offset 0x1070)
        // ASM: 140257b3c call qword [rax+0x1070]
        // ASM: 140257b44 je ... (Jump if result is 0/false)
        if (PlayerCandidate->IsValidImpl() == false)
        {
            continue;
        }

        // Validity Check 4: Paradise Status
        // ASM: 140257b49 call ADBDPlayer::IsInParadise
        // ASM: 140257b50 jne ... (Jump if result is 1/true)
        // Logic: If the player is in paradise (likely dead/spectating), they are not considered "InGame".
        if (PlayerCandidate->IsInParadise())
        {
            continue;
        }

        // If all checks pass, add the ADBDPlayer to the result list
        // ASM: 140257b52 - 140257b77 (Array Add/Grow logic)
        InGamePlayers.Add(PlayerCandidate);
    }

    return InGamePlayers;
}




TArray<ACamperPlayer*> UDBDGameInstance::GetInGameSurvivors()
{
    // Initialize the return array
    // ASM: 140257bc0 mov qword [rdx], 0x0
    TArray<ACamperPlayer*> InGameSurvivors;

    // Iterate through the _players array stored in the GameInstance
    // ASM: 140257bd9 movsxd rax, dword [rcx+0x5f0] (ArrayNum)
    for (int32 Index = 0; Index < this->_players.Num(); Index++)
    {
        // Retrieve the player pointer from the array
        ADBDPlayer* PlayerCandidate = this->_players[Index];
        // Basic null check
        // ASM: 140257c03 test rbx, rbx
        if (PlayerCandidate == nullptr)
        {
            continue;
        }

        // Cast check
        // ASM: 140257c13 call ACamperPlayer::GetPrivateStaticClass
        // ASM: 140257c28 cmp ecx, dword [rax+0x8c] (ClassTree comparison)
        // This logic confirms we are casting to ACamperPlayer.
        ACamperPlayer* CamperPlayer = Cast<ACamperPlayer>(PlayerCandidate);

        // If the player is not an ACamperPlayer, skip to the next iteration
        if (CamperPlayer == nullptr)
        {
            continue;
        }

        // Validity Check 1: Internal Object Flags (Pending Kill)
        // ASM: 140257c5a shr eax, 0x1d; test al, 0x1
        if (CamperPlayer->IsPendingKill())
        {
            continue;
        }

        // Validity Check 2: Actor Destruction Flag (Offset 0x140)
        // ASM: 140257c61 test byte [rbx+0x140], 0x4
        // Logic: If bit 0x4 is set, the actor is being destroyed.
        if (CamperPlayer->bActorIsBeingDestroyed)
        {
            continue;
        }

        // Validity Check 3: IsValidImpl (VTable Offset 0x1070)
        // ASM: 140257c8b call qword [rax+0x1070]
        // Logic: If the virtual function returns false, skip.
        if (CamperPlayer->IsValidImpl() == false)
        {
            continue;
        }

        // Validity Check 4: Paradise Status
        // ASM: 140257c98 call ADBDPlayer::IsInParadise
        // Logic: If IsInParadise returns true (e.g. sacrificed/escaped/dead), skip.
        if (CamperPlayer->IsInParadise())
        {
            continue;
        }

        // If all checks pass, add the CamperPlayer to the result list
        // ASM: 140257ca1 - 140257cb5 (Array Add logic)
        InGameSurvivors.Add(CamperPlayer);
    }

    return InGameSurvivors;
}




ADBDPlayerState* UDBDGameInstance::GetKillerPlayerState()
{
    // Retrieve the current GameState.
    // The assembly explicitly calls UDBDGameInstance::GetGameState.
    class ADBDGameState* GameState = this->GetGameState();

    // 1. Check if GameState pointer is null.
    if (GameState == nullptr)
    {
        return nullptr;
    }

    // 2. Validate GameState against the Global Object Array.
    // The assembly manually checks GUObjectArray flags (Unreachable/PendingKill).
    // In UE4, this is encapsulated by the IsValid() function.
    if (IsValid(GameState) == false)
    {
        return nullptr;
    }

    // 3. Check explicit Actor destruction flag on GameState.
    // Offset 0x140 is checked for bit 0x4.
    // According to the provided structure, this corresponds to bActorIsBeingDestroyed.
    if (GameState->bActorIsBeingDestroyed)
    {
        return nullptr;
    }

    // Access the PlayerArray from the GameState.
    // In the disassembly (140257fde), 'rdi' is loaded from [GameState + 0x3b0].
    // 0x3b0 is the offset for the PlayerArray in this version of AGameState.
    // 'rsi' is calculated as EndPtr, indicating a standard TArray iteration.
    auto& PlayerArray = GameState->PlayerArray;

    for (int32 i = 0; i < PlayerArray.Num(); i++)
    {
        class APlayerState* CurrentPlayerState = PlayerArray[i];

        // Basic null check for the array element.
        if (CurrentPlayerState == nullptr)
        {
            continue;
        }

        // Attempt to cast to ADBDPlayerState.
        // The assembly calls GetPrivateStaticClass and performs a class hierarchy check (Cast<T>).
        class ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(CurrentPlayerState);
        if (DBDPlayerState == nullptr)
        {
            continue;
        }

        // Validate the specific PlayerState against Global Object Array (IsValid).
        if (IsValid(DBDPlayerState) == false)
        {
            continue;
        }

        // Check explicit Actor destruction flag on the PlayerState.
        // Identical check to the GameState check above (offset 0x140, bit 0x4).
        if (DBDPlayerState->bActorIsBeingDestroyed)
        {
            continue;
        }

        // Check the Player's Role.
        // The assembly compares byte at offset 0x750 to 0x1.
        // Context identifies this offset as 'GameRole'.
        // We assume enum value 1 corresponds to the Killer role.
        if (DBDPlayerState->GameRole == EPlayerRole::VE_Slasher)
        {
            return DBDPlayerState;
        }
    }

    // No Killer found.
    return nullptr;
}




ABaseSky* UDBDGameInstance::GetLevelLighting()
{
    /**
     * Retrieve the LightingHelper instance via the member function.
     * Access the _levelLighting member variable directly from the returned object.
     * Offset reference: [rax+0x28]
     */
    return this->GetLightingHelper()->_levelLighting;
}




ULightingInterpolator* UDBDGameInstance::GetLightInterpolator()
{
    /**
     * Retrieve the LightingHelper instance and immediately call its GetLightInterpolator method.
     * The disassembly utilizes a tail call optimization (JMP), using the pointer returned
     * from GetLightingHelper() as the 'this' context for the subsequent call.
     */
    return this->GetLightingHelper()->GetLightInterpolator();
}




ULightingHelper* UDBDGameInstance::GetLightingHelper()
{
    // The disassembly performs a manual check against GUObjectArray and internal object flags 
    // to verify if the object is reachable. In standard UE4 C++, this is encapsulated by IsValid().
    if (this->_ligthingHelper == nullptr || !IsValid(this->_ligthingHelper))
    {
        // Define the name for the new object
        FName InName = FName("LightingHelper");

        /**
         * Create the new ULightingHelper object.
         * The disassembly calls StaticConstructObject_Internal directly with specific flags.
         * The logic matches the standard NewObject<T> implementation with a specific name and RF_NoFlags.
         * * Arguments detected in ASM:
         * Class: ULightingHelper::StaticClass()
         * Outer: this
         * Name: "LightingHelper"
         * SetFlags: RF_NoFlags (0)
         */
        this->_ligthingHelper = NewObject<ULightingHelper>(this, ULightingHelper::StaticClass(), InName, RF_NoFlags);
    }

    return this->_ligthingHelper;
}




FCharacterSavedProfileData* UDBDGameInstance::GetLocalCharacterSavedProfileData()
{
    /**
     * Retrieve the local player state and delegate to the generalized function.
     * The disassembly performs a tail call optimization (JMP).
     */
    return this->GetCharacterSavedProfileDataForPlayerState(this->GetLocalPlayerState());
}




ADBDPlayerControllerBase* UDBDGameInstance::GetLocalDBDPlayerController()
{
    /**
     * Retrieve the generic player controller from the base UGameInstance class.
     * Note: GetFirstLocalPlayerController is a standard UGameInstance method.
     */
    class APlayerController* LocalController = this->GetFirstLocalPlayerController();

    /**
     * Downcast the generic controller to the specific ADBDPlayerControllerBase.
     * The disassembly shows explicit logic checking the ClassTreeIndex (RTTI) and
     * validating internal object flags (checking GUObjectArray and IsPendingKill).
     * In standard UE4 C++, this logic is encapsulated entirely within the Cast<T> template.
     */
    return Cast<ADBDPlayerControllerBase>(LocalController);
}




ULocalPlayer* UDBDGameInstance::GetLocalPlayer()
{
    // Iterate through the standard LocalPlayers array inherited from UGameInstance
    for (int32 i = 0; i < this->LocalPlayers.Num(); ++i)
    {
        class ULocalPlayer* LocalPlayer = this->LocalPlayers[i];

        // Validate the LocalPlayer instance (ASM checks GUObjectArray directly)
        if (LocalPlayer && IsValid(LocalPlayer))
        {
            /**
             * Access the PlayerController.
             * Offset 0x30 in the disassembly typically corresponds to the PlayerController member in ULocalPlayer.
             */
            class APlayerController* PlayerController = LocalPlayer->PlayerController;

            /**
             * Check if the PlayerController is valid.
             * The disassembly explicitly checks:
             * 1. Not Null
             * 2. Valid in GUObjectArray
             * 3. Bitmask check at offset 0x140 (IsPendingKill check)
             * These combined checks are equivalent to the standard IsValid() helper.
             */
            if (PlayerController && IsValid(PlayerController))
            {
                return LocalPlayer;
            }
        }
    }

    return nullptr;
}




APlayerController* UDBDGameInstance::GetLocalPlayerController()
{
    /**
     * Retrieve the first local player controller.
     * This is a standard UGameInstance method.
     */
    class APlayerController* LocalController = this->GetFirstLocalPlayerController();

    /**
     * Validate the controller.
     * The disassembly performs explicit checks against GUObjectArray and internal flags
     * (specifically checking for pending kill status).
     * These checks are standard IsValid() logic.
     */
    if (LocalController && IsValid(LocalController))
    {
        return LocalController;
    }

    return nullptr;
}




FPlayerPersistentData* UDBDGameInstance::GetLocalPlayerPersistentData()
{
    /**
     * Retrieve the first local player.
     * UGameInstance::GetFirstGamePlayer returns a ULocalPlayer*.
     */
    ULocalPlayer* LocalPlayerPtr = this->GetFirstGamePlayer();

    /**
     * Validate the player using a weak pointer wrapper.
     * The disassembly explicitly creates a FWeakObjectPtr on the stack (rsp+0x68),
     * assigns the player to it, and performs validity checks.
     * This ensures the object is not pending kill or garbage collected.
     */
    TWeakObjectPtr<ULocalPlayer> WeakPlayer(LocalPlayerPtr);

    if (WeakPlayer.IsValid() && WeakPlayer.Get()->IsValidLowLevel())
    {
        /**
         * Get the UniqueNetId from the local player.
         * Function returns: TSharedPtr<const FUniqueNetId>
         */
        TSharedPtr<const FUniqueNetId> UniqueNetId = WeakPlayer.Get()->GetPreferredUniqueNetId();

        /**
         * Wrap the ID in FUniqueNetIdRepl structure.
         * The disassembly constructs this struct at [rsp+0x30] and increments the shared reference count,
         * which matches the constructor FUniqueNetIdRepl(const TSharedPtr<const FUniqueNetId>& InUniqueNetId).
         */
        FUniqueNetIdRepl PlayerIdRepl(UniqueNetId);

        /**
         * Retrieve the persistent data using the ID.
         * Delegates to the _persistentData member (UDBDPersistentData*).
         */
        return this->_persistentData->GetPlayerPersistentData(PlayerIdRepl);
    }

    return nullptr;
}




ADBDPlayerState* UDBDGameInstance::GetLocalPlayerState()
{
    /**
     * First, ensure the local player controller is the correct DBD specific type.
     * The disassembly calls UDBDGameInstance::GetLocalPlayerController() and validates the result.
     */
    if (this->GetLocalPlayerController())
    {
        /**
         * Retrieve the generic player controller.
         * The disassembly explicitly calls GetFirstLocalPlayerController here again.
         * While redundant (since GetLocalPlayerController wraps this), it ensures
         * we are working with the valid base pointer for the PlayerState lookup.
         */
        APlayerController* LocalController = this->GetFirstLocalPlayerController();

        // Perform standard validity checks (ASM checks GUObjectArray and PendingKill flags)
        if (LocalController && IsValid(LocalController))
        {
            /**
             * Access the PlayerState.
             * Offset 0x3a0 corresponds to the PlayerState member in AController/APlayerController.
             */
            APlayerState* PlayerState = LocalController->PlayerState;

            /**
             * Cast the generic state to the specific ADBDPlayerState.
             * The disassembly performs an RTTI check (ClassTreeIndex comparison).
             */
            return Cast<ADBDPlayerState>(PlayerState);
        }
    }

    return nullptr;
}




ADBDPlayer* UDBDGameInstance::GetLocallyControlledCharacter()
{
    // Whatever Binary Ninja & IDA decompiled here, it doesn't make any sense!
    // Instead of iterating through every LocalPlayer checking if it's IsLocallyControlled, we only check the last one from the array O_o

    class ULocalPlayer* FoundLocalPlayer = nullptr;

    // Iterate the LocalPlayers array to find the first valid instance.
    // The IDA code performs explicit low-level checks against GUObjectArray to ensure validity.
    for (int32 i = 0; i < this->LocalPlayers.Num(); ++i)
    {
        class ULocalPlayer* CurrentPlayer = this->LocalPlayers[i];
        if (IsValid(CurrentPlayer))
        {
            FoundLocalPlayer = CurrentPlayer;
            break;
        }
    }

    // If a valid LocalPlayer was found, verify their Pawn
    if (FoundLocalPlayer)
    {
        // Helper context to retrieve the pawn safely
        FLocalPlayerContext PlayerContext(FoundLocalPlayer, nullptr);
        class APawn* Pawn = PlayerContext.GetPawn();

        // Attempt to cast to the specific character class
        class ADBDPlayer* DBDPlayer = Cast<ADBDPlayer>(Pawn);

        // Validate the character
        // IDA checks: Cast success && IsValidLowLevel (Flags) && Not PendingKill
        if (DBDPlayer && IsValid(DBDPlayer))
        {
            // 1. First Virtual Function Check (Offset 0x1070)
            // IDA misidentifies this as NotifyActorEndOverlap, but the disassembly shows a bool return 
            // and no complex arguments. It acts as a gate before checking IsLocallyControlled.
            bool bCheckPassed = ((bool(*)(class ADBDPlayer*))(*(void***)DBDPlayer)[0x20E])(DBDPlayer); // 0x1070 / 8 = 0x20E

            if (bCheckPassed)
            {
                // 2. Second Virtual Function Check (Offset 0x630)
                // IDA explicitly identifies this as IsLocallyControlled.
                if (DBDPlayer->IsLocallyControlled())
                {
                    return DBDPlayer;
                }
            }
        }
    }

    return nullptr;
}




int32 UDBDGameInstance::GetMaxSurvivorCount()
{
    // Check if the weak pointer to the Builder is valid.
    // Offsets indicate 'Builder' is at 0x178 in UDBDGameInstance.
    if (this->Builder.IsValid() == false)
    {
        return 4;
    }

    // Retrieve the raw pointer to the AProceduralLevelBuilder.
    AProceduralLevelBuilder* BuilderPtr = this->Builder.Get();

    // Access the ProceduralGenerationData member (offset 0x3d8).
    UProceduralGenerationData* GenerationData = BuilderPtr->ProceduralGenerationData;

    // Ensure the data pointer is not null before checking validity.
    if (GenerationData == nullptr)
    {
        return 4;
    }

    // The disassembly (lines 14025ab0d to 14025ab34) performs a direct check against 
    // GUObjectArray to verify the object flags (checking for PendingKill/Garbage).
    // In UE4, this logic is encapsulated by the global IsValid() function.
    if (IsValid(GenerationData) == false)
    {
        return 4;
    }

    // Return the MaxSurvivorCount (offset 0x58).
    return GenerationData->MaxSurvivorCount;
}




UClass* UDBDGameInstance::GetMenuCharacterSynchronous(int32_t inCharacterIndex)
{
    // Retrieve the character description from the Design Tunables based on the index.
    FCharacterDescription* Description = UDBDDesignTunables::GetCharacterDescription(this->DesignTunables, inCharacterIndex);

    // If the description is not found, return nullptr.
    if (Description == nullptr)
    {
        return nullptr;
    }

    // Try to get the asset from the Weak Pointer (Fast Path: Object is already loaded in memory).
    // The disassembly performs a manual expansion of TAssetPtr/TSoftClassPtr logic here.
    UClass* LoadedAsset = Description->MenuPawn.Get();

    if (LoadedAsset != nullptr)
    {
        // Verify that the loaded object is a UClass.
        if (LoadedAsset->GetClass()->IsChildOf(UClass::StaticClass()))
        {
            // Verify that the loaded class inherits from APawn.
            if (LoadedAsset->IsChildOf(APawn::StaticClass()))
            {
                // In the disassembly, there is a second 'Get' sequence (0x14025ac8f) here.
                // This updates the TagAtLastTest/internal state of the AssetPtr to mark it as recently verified.
                // Calling .Get() conceptually covers this functional requirement.
                return Description->MenuPawn.Get();
            }
        }
    }

    // Slow Path: The object was not in memory or the weak pointer was stale.
    // We must load it synchronously using the StreamableManager.

    // 0x14025ac36: Prepare the ObjectID (FStringAssetReference/FSoftObjectPath) for loading.
    // 0x14025ac44: Call SynchronousLoad.
    UObject* SynchronousLoadedObject = this->AssetLoader.SynchronousLoad(Description->MenuPawn.ToStringReference());

    // Check if the loading was successful.
    if (SynchronousLoadedObject != nullptr)
    {
        // 0x14025ac58 - 0x14025ac73: The disassembly checks if the Class of the loaded object 
        // inherits from UClass (i.e., ensure the loaded object is itself a UClass, not a texture or mesh).
        // Unlike the Fast Path, the disassembly does not strictly loop to check for APawn inheritance 
        // in this specific block, it only validates it is a UClass type.

        if (SynchronousLoadedObject->GetClass()->IsChildOf(UClass::StaticClass()))
        {
            // Return the loaded object cast to UClass.
            return static_cast<UClass*>(SynchronousLoadedObject);
        }
    }

    // If loading failed or the type was invalid.
    return nullptr;
}




FOfferingProperties* UDBDGameInstance::GetOffering(uint8_t index)
{
    // Retrieve the FName identifier for the offering at the given index.
    // The disassembly sets up a stack location (rsp+0x30) to receive the FName,
    // which indicates GetOfferingName returns an FName struct (likely 8 bytes in this version)
    // via a hidden return parameter, or the compiler optimized the local variable storage.
    const FName OfferingName = this->GetOfferingName(index);

    // Access the DesignTunables member variable (offset 0xf0).
    // Retrieve the Offering Data associated with the OfferingName.
    // 
    // Disassembly analysis for GetOfferingData call:
    // RCX: this->DesignTunables
    // RDX: OfferingName (Passed by Value: 'mov rdx, qword [rax]' loads the 8-byte FName content directly into the register)
    // R8 : 0x1 (Boolean 'true', likely a flag like bIsValid or bWarnIfNotFound)
    return this->DesignTunables->GetOfferingData(OfferingName, true);
}




UOfferingHandler* UDBDGameInstance::GetOfferingHandler()
{
    // Check if the cached offering handler is null or if it has been marked as PendingKill (invalidated).
    // 0x14025b966: Checks for nullptr.
    // 0x14025b991: Checks the global object array (GUObjectArray) for the PendingKill flag (0x20000000).
    if (this->_offeringHandler == nullptr || this->_offeringHandler->IsPendingKill())
    {
        // 0x14025b9a0: Prepare the name for the new object.
        FName HandlerName = FName("OfferingHandler");

        // Create the new OfferingHandler instance.
        // 0x14025ba02: The disassembly calls StaticConstructObject_Internal. 
        // In standard UE4 C++, this is wrapped by NewObject. The disassembly also shows the 
        // FObjectInitializer::AssertIfInConstructor check, which is part of the NewObject template inline logic.
        this->_offeringHandler = NewObject<UOfferingHandler>(this, UOfferingHandler::StaticClass(), HandlerName);
    }

    return this->_offeringHandler;
}




FName UDBDGameInstance::GetOfferingName(uint8_t index)
{
    // Retrieve the current GameState.
    class ADBDGameState* GameState = this->GetGameState();

    // Verify the GameState is valid.
    // 0x14025ba44: Checks for nullptr.
    // 0x14025ba4e: Checks the global object array to ensure the object is not marked as PendingKill.
    if (GameState != nullptr && GameState->IsPendingKill() == false)
    {
        // 0x14025ba78: Check the 'bActorIsBeingDestroyed' flag at offset 0x140.
        // The disassembly tests bit 3 (0x4). If it is set, the function aborts.
        if (GameState->bActorIsBeingDestroyed == false)
        {
            // Access the 'Offerings' array within the 'FOfferingData' struct.
            // Disassembly access:
            // r9+0x670 (ArrayNum) -> Corresponds to Offerings.Num()
            // r9+0x668 (DataPtr)  -> Corresponds to Offerings.GetData()

            if (index < GameState->_levelOfferings.Offerings.Num())
            {
                // Retrieve the specific offering from the array using the index.
                // The sizeof(FSelectedOffering) is 16 bytes, matching the disassembly stride.
                return GameState->_levelOfferings.Offerings[index].OfferingName;
            }
        }
    }

    // Return NAME_None if the GameState is invalid, being destroyed, or the index is out of bounds.
    return NAME_None;
}




int32 UDBDGameInstance::GetOfferingOfTypeCount(EOfferingEffectType type)
{
    // 14025bb17: Call internal helper to check if the offering exists.
    // If this returns false (0), the function returns 0 immediately.
    if (this->HasOfferingOfType(type) == false)
    {
        // 14025bbda: Return 0 (ebx is initialized to 0 at 14025bb15)
        return 0;
    }

    /** * The disassembly from 14025bb24 to 14025bb99 represents the inlined logic
     * of a TMap::Find or TMap::operator[] lookup.
     * * It performs the following steps:
     * 1. Checks ArrayNum vs NumFreeIndices (validity check).
     * 2. Hashes the input 'type'.
     * 3. Traverses the linked list in the hash bucket.
     * 4. Compares the key (type) at offset 0 of the element.
     */

     // Attempt to find the entry in the map.
    const FOfferingContext* FoundEntry = this->_resultEffects01.Find(type);

    /**
     * 14025bbc4: If the entry is found (which is implied to be true if HasOfferingOfType is true),
     * the code performs a subtraction of two values within the element structure.
     * * Assembly:
     * mov eax, dword [rbx+0x10]  ; Load value at Offset 0x10
     * sub eax, dword [rbx+0x3c]  ; Subtract value at Offset 0x3C
     * * Note: In the raw assembly, if the loop finishes without finding the item (rbx == 0),
     * it would crash at 0x10. The code relies on HasOfferingOfType ensuring existence.
     */
    if (FoundEntry != nullptr)
    {
        // Calculate the remaining amount based on the offsets.
        // We access the members of the struct which map to 0x10 and 0x3C.
        return FoundEntry->TotalCount - FoundEntry->ConsumedCount;
    }

    // Default return if logic fails (though technically unreachable based on assembly flow checks).
    return 0;
}




int32 UDBDGameInstance::GetOfferingOwner(uint8 index)
{
    // 14025bbfd: Retrieve the GameState.
    ADBDGameState* GameState = this->GetGameState();

    /**
     * 14025bc05 - 14025bc5b: Validation Logic.
     * The assembly performs a complex check involving the GUObjectArray (Global Object Array)
     * and specific flags (0x20000000 / PendingKill) and a specific byte offset [0x140] on the actor.
     * In high-level UE4 C++, this sequence represents the standard IsValid() check,
     * which verifies the object pointer is not null and is not pending kill/garbage collection.
     */
    if (IsValid(GameState) == false)
    {
        // 14025bc84: If GameState is null or invalid, return -1 (0xFFFFFFFF).
        return -1;
    }

    /**
     * 14025bc61: Array Bounds Check.
     * Checks if the requested index is within the valid range of the Offerings array.
     * Note: Assembly explicitly casts the uint8 index to int32/uint32 for comparison.
     */
    if ((int32)index >= GameState->_levelOfferings.Offerings.Num())
    {
        // 14025bc84: If index is out of bounds, return -1.
        return -1;
    }

    /**
     * 14025bc76: Data Retrieval.
     * 1. Accesses the AllocatorInstance.Data pointer of the TArray.
     * 2. Calculates offset: index * 16 (0x10).
     * 3. Returns the first 4 bytes (int32) at that offset.
     */
    return GameState->_levelOfferings.Offerings[index].OwnerID;
}




int32 UDBDGameInstance::GetOfferingsCount()
{
    // 14025bca6: Retrieve the GameState instance.
    ADBDGameState* GameState = this->GetGameState();

    /**
     * 14025bcb1 - 14025bd04: Validation Logic.
     * The assembly performs a sequence of checks:
     * 1. Checks if GameState pointer is null.
     * 2. Checks against GUObjectArray (Global Object Table) using InternalIndex.
     * 3. Checks specific flags (0x20000000 - PendingKill).
     * 4. Checks internal actor flags (likely IsBeingDestroyed).
     * * In UE4 C++, this entire block is encapsulated by the IsValid() function.
     */
    if (IsValid(GameState) == false)
    {
        // 14025bd13: If the GameState is null or invalid, return 0.
        return 0;
    }

    /**
     * 14025bd06: Retrieve Array Size.
     * Accesses `_levelOfferings.Offerings.ArrayNum`.
     * In C++, TArray.Num() returns the ArrayNum member.
     */
    return GameState->_levelOfferings.Offerings.Num();
}




float GetOfferingsModification(EOfferingEffectType type, int32_t playerId)
{
    int64_t playerId_64 = (int64_t)playerId;
    float result = 0.0f;

    // Check if the outer map is valid/initialized (Pseudo: ArrayNum != NumFreeIndices check implies IsEmpty check)
    if (this->_resultEffects01.Num() == 0)
    {
        return result;
    }

    // ---------------------------------------------------------
    // Step 1: Find the effect entry for the specific Offering Type
    // ---------------------------------------------------------

    // This block replicates the manual hash lookup seen in disassembly (0x14025bd79 - 0x14025bda9)
    TMap<int32, FOfferingModifierData>* InnerMapPtr = this->_resultEffects01.Find((uint8)type);
    if (InnerMapPtr == nullptr)
    {
        return 0.0f;
    }

    // ---------------------------------------------------------
    // Step 2: Retrieve Base/Global Modification (Key: -1)
    // ---------------------------------------------------------

    // The disassembly checks key against 0xffffffff (-1) at 0x14025be8c.
    // This implies a "Global" or "Default" modifier is stored under key -1.
    if (InnerMapPtr->Num() > 0)
    {
        const int32 GlobalKey = -1;

        // Manual iteration logic in ASM replaced by TMap::Find for readability/functionality
        FOfferingModifierData* GlobalData = InnerMapPtr->Find(GlobalKey);
        if (GlobalData != nullptr)
        {
            // ASM: addss xmm0, dword [rdi+0x28] (0x14025c1cd)
            result += GlobalData->Modifier;
        }
    }

    // ---------------------------------------------------------
    // Step 3: Retrieve Player Specific Modification (Key: playerId)
    // ---------------------------------------------------------

    // ASM: cmp ebp, 0xffffffff (0x14025bf94)
    if (playerId != -1)
    {
        // The disassembly repeats the outer lookup here (common in unoptimized builds),
        // effectively getting 'InnerMapPtr' again.

        if (this->_resultEffects01.Num() > 0) // Re-check outer map (0x14025bfab)
        {
            // We already have InnerMapPtr from Step 1, but to strictly follow the 
            // disassembly flow which re-acquires the pointer:
            TMap<int32, FOfferingModifierData>* ReAcquiredInnerMap = this->_resultEffects01.Find((uint8)type);

            if (ReAcquiredInnerMap != nullptr)
            {
                // Look for the specific player ID
                // ASM: cmp dword [rdx+r8], ebp (0x14025c067)
                FOfferingModifierData* PlayerData = ReAcquiredInnerMap->Find(playerId);
                if (PlayerData != nullptr)
                {
                    // ASM: addss xmm0, dword [rdi+0x28] (0x14025c1cd logic combined)
                    result += PlayerData->Modifier;
                }
            }
        }
    }

    return result;
}




float GetOfferingsScoreModification(FScoreValue* scoreVal, int32_t playerID, EPlayerRole playerRole)
{
    // 14025c1f0: Initialize accumulator (xmm6 in ASM)
    float totalModification = 0.0f;

    // 14025c210: Validation check
    if (scoreVal == nullptr)
    {
        return 0.0f;
    }

    // 14025c22d: Initialize a TArray to store effect types. 
    // ASM allocates space on stack and clears memory.
    TArray<EOfferingEffectType> EffectTypes;

    // 14025c23d: Call static helper to populate the array
    /* UNDEFINED ELEMENT - FOffering::GetOfferingEffects is likely a static game-specific function */
    FOffering::GetOfferingEffects(scoreVal, &EffectTypes, playerRole);

    // 14025c24f: Check if array has elements before iterating
    if (EffectTypes.Num() > 0)
    {
        // Iterate through the collected EffectTypes
        // ASM: 14025c6d4 (loop start) to 14025c6d1 (cmp rsi, rbp)
        for (const EOfferingEffectType& Type : EffectTypes)
        {
            // Check if the outer map has data
            if (this->_resultEffects01.Num() == 0)
            {
                continue;
            }

            // ---------------------------------------------------------
            // Step 1: Find the inner map for the current EffectType
            // ---------------------------------------------------------
            // ASM: 14025c2a2 (Hash calculation for outer map)
            TMap<int32, FOfferingModifierData>* InnerMapPtr = this->_resultEffects01.Find((uint8)Type);

            if (InnerMapPtr != nullptr)
            {
                // ---------------------------------------------------------
                // Step 2: Retrieve Base/Global Modification (Key: -1)
                // ---------------------------------------------------------
                // ASM: 14025c3ac - cmp ..., 0xffffffff
                const int32 GlobalKey = -1;
                FOfferingModifierData* GlobalData = InnerMapPtr->Find(GlobalKey);

                if (GlobalData != nullptr)
                {
                    // ASM: 14025c3ae (accessing data) -> accumulated later
                    totalModification += GlobalData->Modifier;
                }

                // ---------------------------------------------------------
                // Step 3: Retrieve Player Specific Modification (Key: playerId)
                // ---------------------------------------------------------
                // ASM: 14025c4b6 - cmp playerID, -1
                if (playerID != -1)
                {
                    // Note: The assembly actually performs the outer map lookup *again* here 
                    // (see ASM 14025c4d0). In optimized C++, we would reuse 'InnerMapPtr', 
                    // but strictly following the logic flow implies checking the structure again.
                    // We will use the existing pointer for readability as the result is identical.

                    // ASM: 14025c58b - cmp [rdx+r8], playerID
                    FOfferingModifierData* PlayerData = InnerMapPtr->Find(playerID);

                    if (PlayerData != nullptr)
                    {
                        // ASM: 14025c6c9 - addss xmm6, dword [rax+0x28]
                        totalModification += PlayerData->Modifier;
                    }
                }
            }
        }
    }

    // 14025c6e9: Clean up TArray memory.
    // In C++, the TArray destructor is called automatically here as EffectTypes goes out of scope.
    // FMemory::Free(Data_2); 

    return totalModification;
}




FName GetOfferingsValue(EOfferingEffectType type, int32_t playerId)
{
    // 14025c7eb: Default return value is NAME_None (0)
    FName Result = NAME_None;

    // Check if the outer map has data (ASM: 14025c77d)
    if (this->_resultEffects01.Num() == 0)
    {
        return Result;
    }

    // ---------------------------------------------------------
    // Step 1: Find the inner map for the specific Offering Type
    // ---------------------------------------------------------
    // ASM: 14025c798 - Hash lookup for Type
    TMap<int32, FOfferingModifierData>* InnerMapPtr = this->_resultEffects01.Find((uint8)type);

    if (InnerMapPtr != nullptr)
    {
        // Note: The assembly at 14025c81d performs a redundant outer map lookup here 
        // (likely due to compiler inlining or macro expansion in the original code), 
        // but effectively it just proceeds to look up the playerId in the inner map.

        // ---------------------------------------------------------
        // Step 2: Retrieve Value for specific Player (Key: playerId)
        // ---------------------------------------------------------
        // ASM: 14025c8ad - Hashing playerId (rbp)
        // ASM: 14025c8b3 - Inner Map Lookup
        FOfferingModifierData* FoundData = InnerMapPtr->Find(playerId);

        if (FoundData != nullptr)
        {
            // ASM: 14025ca07 - mov rax, qword [rsi+0x20]
            // Reads the FName at offset 0x20 relative to the element start
            Result = FoundData->NameValue;
        }
    }

    return Result;
}




TSubclassOf<class UOnlineSession> UDBDGameInstance::GetOnlineSessionClass()
{
    // The disassembly explicitly loads the string "/Script/DeadByDaylight" 
    // and calls GetPrivateStaticClass on UDBDOnlineSessionClient.
    // In standard UE4 development, this is often wrapped in UDBDOnlineSessionClient::StaticClass(), 
    // but here we call the underlying static function directly as seen in the assembly.

    class UClass* ResultClass = UDBDOnlineSessionClient::GetPrivateStaticClass(TEXT("/Script/DeadByDaylight"));

    // Return the result (The compiler handles the implicit conversion to TSubclassOf<UOnlineSession>)
    return ResultClass;
}




class UOnlineSystemHandler* UDBDGameInstance::GetOnlineSystemHandler()
{
    // Retrieve the stored Context System pointer
    class UGameFlowContextSystem* ContextSystem = this->_contextSystem;

    /**
     * The disassembly performs an inlined validity check here:
     * 1. Checks if the pointer is not null.
     * 2. Looks up the object in the global `GUObjectArray` using `InternalIndex`.
     * 3. Checks the object flags (specifically bit 29, 0x20000000) to ensure it is not Unreachable/PendingKill.
     * * In standard Unreal Engine C++, this logic is encapsulated by the global IsValid() function.
     */
    if (IsValid(ContextSystem) == false)
    {
        return nullptr;
    }

    // Return the OnlineSystemHandler stored within the valid ContextSystem
    return ContextSystem->m_OnlineSystemHandler;
}




FPlayerPersistentData* UDBDGameInstance::GetPlayerPersistentDataForPlayerState(class ADBDPlayerState* playerState)
{
    // The disassembly performs a strict validity check on the playerState argument.
    // It checks for nullptr, verifies the object exists in the global GUObjectArray, 
    // and checks internal flags (likely EInternalObjectFlags::Unreachable or PendingKill).
    // In UE4, this logic is encapsulated by the standard IsValid() function.
    if (IsValid(playerState))
    {
        // 1. A copy of the player's UniqueId (FUniqueNetIdRepl) is constructed on the stack.
        //    (The assembly explicitly sets the vtable and increments the shared reference count).
        // 2. The function accesses the _persistentData member (likely a UDBDPersistentData*).
        // 3. It calls the GetPlayerPersistentData function, passing the ID by value (implied by the local copy construction).

        return UDBDPersistentData::GetPlayerPersistentData(this->_persistentData, playerState->UniqueId);
    }

    // Return nullptr if the player state is invalid
    return nullptr;
}




ADBDPlayerState* UDBDGameInstance::GetPrimaryPlayerState()
{
    // Call the base UGameInstance function to retrieve the Primary Player Controller
    class APlayerController* PrimaryPlayerController = this->GetPrimaryPlayerController();

    /**
     * The disassembly performs an inlined validity check:
     * 1. Checks if PrimaryPlayerController is not null.
     * 2. Verifies the object exists in GUObjectArray.
     * 3. Checks for EInternalObjectFlags::Unreachable and PendingKill flags.
     * This is standard Unreal Engine safety encapsulated by IsValid().
     */
    if (IsValid(PrimaryPlayerController))
    {
        // Retrieve the PlayerState (offset 0x3a0 in AController) and safely cast it to ADBDPlayerState.
        // The assembly uses a tail call to 'SafeCast', which is functionally equivalent to UE4's Cast<T>.
        return Cast<class ADBDPlayerState>(PrimaryPlayerController->PlayerState);
    }

    return nullptr;
}




int32 UDBDGameInstance::GetRandomCamperIndex()
{
    // Retrieve the Design Tunables instance
    class UDBDDesignTunables* Tunables = this->DesignTunables;

    // Initialize a local array to store character descriptions.
    // The disassembly shows manual stack allocation and initialization to 0, 
    // which corresponds to the default constructor of TArray.
    TArray<const class FCharacterDescription*> CharacterList;

    /**
     * Call GetCharactersByRole.
     * Argument 2 (Role): 0x2 corresponds to VE_Camper (Survivor).
     * Argument 3 (UnkBool): 0 (False).
     */
    UDBDDesignTunables::GetCharactersByRole(Tunables, CharacterList, 0x2 /* VE_Camper */, false);

    int32 ArrayNum = CharacterList.Num();
    int32 SelectedIndex = 0;

    // If we have characters, pick a random one
    if (ArrayNum > 0)
    {
        // The disassembly uses std::rand() explicitly with float normalization.
        // Logic: Index = floor( (rand() / RAND_MAX) * ArrayNum )
        // 0.000030518509f is equivalent to (1.0 / 32767.0).
        int32 RandomVal = std::rand();
        float NormalizedRandom = (float)RandomVal * 0.000030518509f;
        int32 CalculatedIndex = (int32)(NormalizedRandom * (float)ArrayNum);

        // Clamp logic seen in assembly: if (CalculatedIndex <= ArrayNum - 1) SelectedIndex = CalculatedIndex;
        // This effectively ensures we don't go out of bounds if float precision acts up.
        int32 MaxIndex = ArrayNum - 1;
        if (CalculatedIndex <= MaxIndex)
        {
            SelectedIndex = CalculatedIndex;
        }
        else
        {
            SelectedIndex = MaxIndex;
        }
    }
    else
    {
        // Fallback index is 0 if array is empty (Note: This risks a crash if Data is null, 
        // implies DesignTunables is expected to always return valid data).
        SelectedIndex = 0;
    }

    // Retrieve the pointer from the array
    const class FCharacterDescription* SelectedCharDesc = CharacterList[SelectedIndex];

    // Access the integer at Offset 0x8 from the struct.
    // Assuming this field is the Character Index or ID.
    // mov ebx, dword [rax+0x8]
    int32 CharacterIndex = SelectedCharDesc->CharacterIndex;

    // The TArray destructor will automatically be called here, invoking FMemory::Free on CharacterList.Data.
    return CharacterIndex;
}