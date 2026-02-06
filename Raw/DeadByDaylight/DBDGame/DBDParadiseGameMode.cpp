void ADBDParadiseGameMode::HandleMatchHasStarted()
{
    // Call the parent class implementation of HandleMatchHasStarted
    /* UNREAL AUTO GENERATED FUNCTION */
    Super::HandleMatchHasStarted();

    // Get the GameInstance associated with this Actor
    /* UNREAL AUTO GENERATED FUNCTION */
    UGameInstance* GenericGameInstance = this->GetGameInstance();

    // Validate if the GameInstance exists
    if (GenericGameInstance == nullptr)
    {
        return;
    }

    // Attempt to cast the generic GameInstance to the specific UDBDGameInstance
    // The disassembly performs a manual check of the ClassTreeIndex here
    /* UNREAL AUTO GENERATED FUNCTION */
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GenericGameInstance);

    // If the cast failed or the instance is invalid, return
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Check if the DBDGameInstance is valid within the Global Object Array (GUObjectArray)
    // The disassembly explicitly checks the flags in GUObjectArray.ObjObjects
    /* UNREAL AUTO GENERATED FUNCTION */
    if (DBDGameInstance->IsValidLowLevel() == false)
    {
        return;
    }

    // Access the FWeakObjectPtr located at offset 0x1B0 inside UDBDGameInstance
    // This pointer is used to retrieve data for ALevelParadise
    // Using pointer arithmetic to access the member as the definition of UDBDGameInstance is not fully provided
    /* UNDEFINED ELEMENT */
    FWeakObjectPtr* ParadiseLevelDataPtr = DBDGameInstance->Paradise;

    // Check if the weak pointer is valid
    /* UNREAL AUTO GENERATED FUNCTION */
    if (ParadiseLevelDataPtr->IsValid() == false)
    {
        return;
    }

    // Retrieve the Local Player State via the Game Instance
    /* UNDEFINED ELEMENT */
    ADBDPlayerState* LocalPlayerState = DBDGameInstance->GetLocalPlayerState();

    if (LocalPlayerState != nullptr)
    {
        // Check if the LocalPlayerState is valid in the object array
        /* UNREAL AUTO GENERATED FUNCTION */
        bool bIsStateValid = LocalPlayerState->IsValidLowLevel();

        // Check bitfield at 0x140, bit 4 (bActorIsBeingDestroyed implies destruction)
        // Offset 0x140 in AActor struct corresponds to the union containing bActorIsBeingDestroyed
        /* UNREAL AUTO GENERATED FUNCTION */
        bool bIsBeingDestroyed = this->IsActorBeingDestroyed();

        if (bIsStateValid == true && bIsBeingDestroyed == false)
        {
            // Verify the object is still valid before the VTable call
            /* UNREAL AUTO GENERATED FUNCTION */
            if (LocalPlayerState->IsValidLowLevel() == true)
            {
                LocalPlayerState->AuthoritySetInParadise(true);
            }
        }
    }

    // Construct FName "Paradise" and reset lighting
    /* UNREAL AUTO GENERATED FUNCTION */
    FName ParadiseName = FName("Paradise");

    // Call ResetLighting on the GameInstance
    /* UNDEFINED ELEMENT */
    DBDGameInstance->ResetLighting(ParadiseName);

    // Resolve the weak pointer to get the actual object
    /* UNREAL AUTO GENERATED FUNCTION */
    UObject* LevelDataObj = ParadiseLevelDataPtr->Get();

    // Reload the level using the resolved data object
    /* UNDEFINED ELEMENT */
    ALevelParadise::ReloadFromData(LevelDataObj);
}




FString ADBDParadiseGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    // 140263c10 - 140263c70: Validate PlayerState casting
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(NewPlayerController->PlayerState);

    // 140263c76 - 140263ca9: Validate GameInstance casting
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // 140263cb5 - 1402646e7: Main Logic Block if PlayerState is valid
    if (DBDPlayerState != nullptr)
    {
        // 140263cbb - 140263d42: Verify PlayerState Internal Validity (Object Array checks)
        // Note: The disassembly performs manual bitwise checks on the UObjectArray to ensure validity.
        // In standard C++, IsValid() or the nullptr check above covers this.
        // We assume valid logic proceeds here.

        // 140263d55 - 140263da8: Parse Option 1 (Likely CloudID or PlayerID)
        // Stored at offset 0x74c
        FString ParsedString1 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585808 */"));
        int32 Value1 = FCString::Atoi(*ParsedString1);
        if (Value1 < 0)
        {
            Value1 = 0;
        }
        DBDPlayerState->SelectedCamperIndex = Value1; // /* UNDEFINED ELEMENT: Offset 0x74c */

        // 140263dc5 - 140263e48: Parse Option 2 (Likely GameRole) and SetGameRole
        // Stored at offset 0x754
        FString ParsedString2 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585818 */"));
        int32 GameRoleVal = 0;

        if (ParsedString2.Len() > 0)
        {
            GameRoleVal = FCString::Atoi(*ParsedString2);
        }

        // Sanity check for GameRole (logic implies < 0 becomes 0)
        if (GameRoleVal < 0)
        {
            GameRoleVal = 0;
        }

        DBDPlayerState->SelectedSlasherIndex = GameRoleVal; // /* UNDEFINED ELEMENT: Offset 0x754 */

        // 140263e0e - 140263e41: Parse Option 3 (Unknown Flag)
        FString ParsedString3 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585828 */"));
        int32 Value3 = 0x10000000;

        if (ParsedString3.Len() > 0)
        {
            Value3 = FCString::Atoi(*ParsedString3);
        }

        if (Value3 < 0)
        {
            Value3 = 0x10000000;
        }
        // This value is possibly a SessionID or MatchFlag stored at 0x754 (Wait, 0x754 is written twice in disassembly logic flow? 
        // Re-evaluating disassembly: 140263e41 writes to 0x754. 140263df4 wrote to 0x74c. 
        // 140263fd6 writes to 0x758. The disassembly reuse of registers suggests sequential writes.)
        // Correction based on flow: 
        // 1. Parse Key1 -> write 0x74c.
        // 2. Parse Key2 -> used for logic, later written? 
        // Actually, disassembly 140263e48 calls SetGameRole using the result of ParseOption 2.

        DBDPlayerState->SetGameRole(DBDPlayerState, (uint8)GameRoleVal); // /* UNDEFINED ELEMENT: SetGameRole */

        // 140263e50: Check if Role == 2 (Likely Killer or specific role)
        if (GameRoleVal == 2)
        {
            // 140263e6e: Parse Option 4 (FName)
            FString ParsedNameString = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585858 */"));
            FName NameValue = NAME_None;

            if (ParsedNameString.Len() > 0)
            {
                NameValue = FName(*ParsedNameString);
            }
            else
            {
                // data_14361e688 points to a default name, likely "None" or empty
                NameValue = NAME_None;
            }

            DBDPlayerState->CamperData.EquipedItemId = NameValue; // /* UNDEFINED ELEMENT: Offset 0x760 (FName) */
        }

        // 140263edd - 140263f03: Parse Option 5 (Int) -> 0x7b8
        FString ParsedString5 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585838 */"));
        int32 Value5 = 1;
        if (ParsedString5.Len() > 0)
        {
            Value5 = FCString::Atoi(*ParsedString5);
        }
        DBDPlayerState->PlayerData.CharacterLevel = Value5; // /* UNDEFINED ELEMENT: Offset 0x7b8 */

        // 140263f22 - 140263f46: Parse Option 6 (Int) -> 0x7bc
        FString ParsedString6 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585848 */"));
        int32 Value6 = 0;
        if (ParsedString6.Len() > 0)
        {
            Value6 = FCString::Atoi(*ParsedString6);
        }
        DBDPlayerState->PlayerData.PrestigeLevel = Value6; // /* UNDEFINED ELEMENT: Offset 0x7bc */

        // 140263f65 - 140263fab: Parse Option 7 (FName) -> 0x7c0
        FString ParsedString7 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585878 */"));
        FName NameValue7 = NAME_None;
        if (ParsedString7.Len() > 0)
        {
            NameValue7 = FName(*ParsedString7);
        }
        DBDPlayerState->PlayerData.EquipedFavorId = NameValue7; // /* UNDEFINED ELEMENT: Offset 0x7c0 (FName) */

        // 140263fbd: GetIntOption -> 0x798
        DBDPlayerState->SlasherData.SlasherSkulls = UGameplayStatics::GetIntOption(Options, TEXT("/* String Key @ data_1435858b8 */"), 0); // /* UNDEFINED ELEMENT: Offset 0x798 */

        // 140263fd6: GetIntOption -> 0x758
        DBDPlayerState->CamperData.CamperSkulls = UGameplayStatics::GetIntOption(Options, TEXT("/* String Key @ data_1435858c8 */"), 0); // /* UNDEFINED ELEMENT: Offset 0x758 */

        // ---------------------------------------------------------
        // Array Parsing Logic (Loops 1, 2, 3)
        // ---------------------------------------------------------

        // 140263ff5: Parse delimited string 1 (Array of FNames) -> 0x7e8
        FString ArrayString1 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_1435858a8 */"));

        // The disassembly contains a loop (140264010) that sanitizes the string 
        // (replacing chars > 0x7f with '?') and splits it.
        // We replicate the functional result using UE4 splitting.

        FString LeftS;
        FString RightS = ArrayString1;

        // Manual sanitization loop simulation based on disassembly (140264053)
        // Note: In pure UE4 C++, we usually assume options are valid, but we will respect the disassembly logic.
        for (int32 i = 0; i < RightS.Len(); i++)
        {
            if ((uint8)RightS[i] > 0x7f)
            {
                RightS[i] = (TCHAR)0x3f; // '?'
                // 140264088: LogBogusChars call would happen here
            }
        }

        while (RightS.Split(TEXT(","), &LeftS, &RightS)) // Assuming delimiter is ',' based on context
        {
            FName NewName = NAME_None;
            if (LeftS.Len() > 0)
            {
                NewName = FName(*LeftS);
            }
            DBDPlayerState->PlayerData.CustomizationMesh.Add(NewName); // /* UNDEFINED ELEMENT: Offset 0x7e8 (TArray<FName>) */
        }
        // Process remainder
        if (RightS.Len() > 0)
        {
            DBDPlayerState->PlayerData.CustomizationMesh.Add(FName(*RightS));
        }


        // 1402641c7: Parse delimited string 2 (Array of FNames) -> 0x7c8
        FString ArrayString2 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585888 */"));
        RightS = ArrayString2;

        // Sanitization Loop 2
        for (int32 i = 0; i < RightS.Len(); i++)
        {
            if ((uint8)RightS[i] > 0x7f)
            {
                RightS[i] = (TCHAR)0x3f;
            }
        }

        while (RightS.Split(TEXT(","), &LeftS, &RightS))
        {
            FName NewName = NAME_None;
            if (LeftS.Len() > 0)
            {
                NewName = FName(*LeftS);
            }
            DBDPlayerState->PlayerData.EquipedPerkIds.Add(NewName); // /* UNDEFINED ELEMENT: Offset 0x7c8 (TArray<FName>) */
        }
        if (RightS.Len() > 0)
        {
            DBDPlayerState->PlayerData.EquipedPerkIds.Add(FName(*RightS));
        }

        // 14026438b: Zero out offset 0x7e0 (Index/Count?)
        // The disassembly zeros out a specific field before the next loop.
        // Assuming 0x7e0 is related to the next array or a counter.
        // DBDPlayerState->MemberVar_7E0 = 0; 

        // 1402643bb: Parse delimited string 3 (Array of Ints) -> 0x7d8
        FString ArrayString3 = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_143585898 */"));
        RightS = ArrayString3;

        // Sanitization Loop 3
        for (int32 i = 0; i < RightS.Len(); i++)
        {
            if ((uint8)RightS[i] > 0x7f)
            {
                RightS[i] = (TCHAR)0x3f;
            }
        }

        while (RightS.Split(TEXT(","), &LeftS, &RightS))
        {
            int32 NewInt = 0;
            if (LeftS.Len() > 0)
            {
                NewInt = FCString::Atoi(*LeftS);
            }
            DBDPlayerState->PlayerData.EquipedPerkLevels.Add(NewInt); // /* UNDEFINED ELEMENT: Offset 0x7d8 (TArray<int32>) */
        }
        if (RightS.Len() > 0)
        {
            DBDPlayerState->PlayerData.EquipedPerkLevels.Add(FCString::Atoi(*RightS));
        }

        // ---------------------------------------------------------
        // Persistent Data and Cache Handling
        // ---------------------------------------------------------

        // 140264551: Restore UniqueId if valid
        // Note: The logic handles FUniqueNetIdRepl. 

        if (DBDGameInstance != nullptr)
        {
            // 14026455f: Check byte at offset 0x240 (boolean flag?)
            // If true, proceed to score cache
            if (DBDGameInstance->HasServerLeftMatch != 0) // /* UNDEFINED ELEMENT: Offset 0x240 */
            {
                // Accessing Shared Pointer for UniqueID
                if (UniqueId.IsValid())
                {
                    // 140264593: Call UDBDPersistentData::GetPlayerScoreCache
                    // Offset 0x3b8 in GameInstance is passed as arg 1

                    void* ScoreCache = UDBDPersistentData::GetPlayerScoreCache(DBDGameInstance->_persistentData, UniqueId); // /* UNDEFINED ELEMENT: Offset 0x3b8 */

                    if (ScoreCache != nullptr)
                    {
                        // 1402645b3: CopyDataFromCache
                        // Construct FPlayerstateDataCache from ScoreCache + 0x98
                        FPlayerstateDataCache DataCache(ScoreCache); // Constructor implied by disassembly
                        this->ADBDPlayerState::CopyDataFromCache(DBDPlayerState, DataCache); // /* UNDEFINED ELEMENT: CopyDataFromCache */
                    }
                }
            }
        }

        // 1402645d4: Parse final Option (String) -> 0x738
        FString FinalString = UGameplayStatics::ParseOption(Options, TEXT("/* String Key @ data_1435858e8 */"));

        // 1402645e6: Set string at 0x738
        if (DBDPlayerState->SteamId != FinalString) // /* UNDEFINED ELEMENT: Offset 0x738 (FString) */
        {
            DBDPlayerState->SteamId = FinalString;
        }
    }

    // 14026471e: Call Base Class Implementation
    return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}