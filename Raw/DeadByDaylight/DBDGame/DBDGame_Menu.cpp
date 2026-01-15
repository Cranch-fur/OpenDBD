TSubclassOf<AGameSession> ADBDGame_Menu::GetGameSessionClass() const
{
    // .text:0000000140257366 lea this, aScriptDeadbyda ; "/Script/DeadByDaylight"
    // .text:0000000140257370 call ?GetPrivateStaticClass@ADBDGameSession...
    // The disassembly calls the internal private static class getter. 
    // In standard C++, this is accessed via the static member function StaticClass().
    return ADBDGameSession::StaticClass();
}




void ADBDGame_Menu::PostInitializeComponents()
{
    // .text:000000014026D99D call ?PostInitializeComponents@AActor@@UEAAXXZ
    // Call the parent class implementation first.
    Super::PostInitializeComponents();

    // .text:000000014026D9A5 call ?GetGameInstance@AActor@@QEBAPEAVUGameInstance@@XZ
    // Retrieve the GameInstance associated with this actor.
    UGameInstance* GameInstance = this->GetGameInstance();

    // .text:000000014026D9B0 jz loc_14026DAD4
    // Check if GameInstance is valid before proceeding.
    if (GameInstance == nullptr)
    {
        return;
    }

    // .text:000000014026D9BD call ?GetPrivateStaticClass@UDBDGameInstance...
    // The disassembly performs a check to ensure the GameInstance is of type UDBDGameInstance.
    // In UE4 C++, this is handled via Cast<>.
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    // .text:000000014026D9D8 ja loc_14026DAD4
    // If the Cast failed, DBDGameInstance will be nullptr.
    if (DBDGameInstance != nullptr)
    {
        // .text:000000014026DA10 mov r8, [rdi+3B0h]
        // Accessing member variable '_contextSystem' at offset 0x3B0.
        // IDA Pseudo: contextSystem = GameInstance->_contextSystem;
        // Note: The pseudo code implies UDBDGameInstance has a member _contextSystem.
        if (DBDGameInstance->_contextSystem != nullptr)
        {
            // .text:000000014026DA3F mov r10, [r8+90h]
            // Accessing member variable 'm_OnlineSystemHandler' from the context system at offset 0x90.
            // IDA Pseudo: m_OnlineSystemHandler = contextSystem->m_OnlineSystemHandler;
            UOnlineSystemHandler* OnlineSystemHandler = DBDGameInstance->_contextSystem->m_OnlineSystemHandler;

            if (OnlineSystemHandler != nullptr)
            {
                // .text:000000014026DA94 call ?EndSessionOnReturnToMenu@UOnlineSystemHandler@@QEAAXXZ
                // Mangled name implies: public: void __cdecl UOnlineSystemHandler::EndSessionOnReturnToMenu(void)
                OnlineSystemHandler->EndSessionOnReturnToMenu();
            }
        }

        // .text:000000014026DAA6 mov r8, [rdi+3B8h]
        // Accessing member variable '_persistentData' at offset 0x3B8.
        // IDA Pseudo: persistentData = GameInstance->_persistentData;
        UDBDPersistentData* PersistentData = DBDGameInstance->_persistentData;

        if (PersistentData != nullptr)
        {
            // .text:000000014026DACF call ?ClearNonLocalPlayerPersistentData@UDBDPersistentData@@QEAAXXZ
            // Mangled name implies: public: void __cdecl UDBDPersistentData::ClearNonLocalPlayerPersistentData(void)
            PersistentData->ClearNonLocalPlayerPersistentData();
        }
    }
}




void ADBDGame_Menu::PostLogin(APlayerController* NewPlayer)
{
    // .text:000000014026E350 call ?PostLogin@AGameMode@@UEAAXPEAVAPlayerController@@@Z
    // Call the parent class implementation first to ensure standard engine logic runs.
    Super::PostLogin(NewPlayer);

    // .text:000000014026E358 call ?SetMatineeIntro@ADBDGame_Menu@@AEAAXXZ
    // Execute the internal function to handle the Matinee Intro logic.
    this->SetMatineeIntro();

    // .text:000000014026E36D jmp ?UpdatePlayerData@ADBDGame_Menu@@AEAAXPEAVAPlayerController@@@Z
    // Execute the internal function to update player data.
    // The disassembly uses a JMP (Tail Call Optimization) here because it is the last instruction.
    this->UpdatePlayerData(NewPlayer);
}




void ADBDGame_Menu::PostSeamlessTravel()
{
    // .text:000000014026E559 call ?PostSeamlessTravel@AGameMode@@UEAAXXZ
    // Call the parent class implementation.
    Super::PostSeamlessTravel();

    // .text:000000014026E561 call ?SetMatineeIntro@ADBDGame_Menu@@AEAAXXZ
    // Initialize the matinee intro sequence.
    this->SetMatineeIntro();

    // .text:000000014026E56C call qword ptr [rax+108h]
    // VTable call at offset 0x108 corresponds to GetWorld().
    UWorld* World = this->GetWorld();

    if (World != nullptr)
    {
        // .text:000000014026E57A call ?GetPlayerControllerIterator@UWorld...
        // .text:000000014026E583 ... loop logic ...
        // Iterate through all PlayerControllers in the world. 
        // In UE4.13, GetPlayerControllerIterator returns a FConstPlayerControllerIterator.
        for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            // .text:000000014026E59B call ?Get@FWeakObjectPtr@@QEBAPEAVUObject@@XZ
            // Dereference the weak pointer from the iterator to get the APlayerController instance.
            APlayerController* PlayerController = Iterator->Get();

            // .text:000000014026E5A6 call ?UpdatePlayerData@ADBDGame_Menu@@AEAAXPEAVAPlayerController@@@Z
            // Update the data for this specific player.
            this->UpdatePlayerData(PlayerController);
        }
    }
}




void ADBDGame_Menu::SetMatineeIntro()
{
    // .text:000000014027347F mov [rsp+48h+matineeActors], 0
    // .text:00000001402734B7 call ?GetAllActorsOfClass@UGameplayStatics...
    // Create an array to hold the search results and populate it with all AMatineeActor instances in the world.
    TArray<AActor*> MatineeActors;
    UGameplayStatics::GetAllActorsOfClass(this, AMatineeActor::StaticClass(), MatineeActors);

    // Iterate through the found actors.
    // The disassembly performs a raw loop over the TArray memory buffer.
    for (AActor* Actor : MatineeActors)
    {
        // .text:00000001402734D6 test rbx, rbx
        // Ensure the actor pointer is valid.
        if (Actor == nullptr)
        {
            continue;
        }

        // .text:00000001402734E6 call ?GetPrivateStaticClass@AMatineeActor...
        // The disassembly checks the class index ensuring it is actually a MatineeActor.
        // In C++, we safely Cast to the target type.
        AMatineeActor* MatineeActor = Cast<AMatineeActor>(Actor);
        if (MatineeActor == nullptr)
        {
            continue;
        }

        // .text:0000000140273538 test byte ptr [rbx+140h], 4
        // .text:000000014027353F jnz loc_1402735EF
        // Offset 0x140 corresponds to AActor bitfields. Bit 4 corresponds to bAutoDestroyWhenFinished.
        // The code jumps to the end (skips assignment) if this bit is set.
        if (MatineeActor->bAutoDestroyWhenFinished == true)
        {
            continue;
        }

        // .text:000000014027356A lea rdx, aOfflinelobbyfa ; "OfflineLobbyFade"
        // .text:0000000140273581 call ?ActorHasTag@AActor@@QEBA_NVFName@@@Z
        // Check if the actor has the "OfflineLobbyFade" tag.
        if (MatineeActor->ActorHasTag(FName("OfflineLobbyFade")))
        {
            // .text:000000014027358A mov [rsi+460h], rbx
            // Assign to _matineeFadeActor (Offset 0x460).
            this->_matineeFadeActor = MatineeActor;
        }
        // .text:0000000140273599 lea rdx, aOfflinelobbyin ; "OfflineLobbyIntro"
        // .text:00000001402735B0 call ?ActorHasTag@AActor@@QEBA_NVFName@@@Z
        // Check if the actor has the "OfflineLobbyIntro" tag.
        else if (MatineeActor->ActorHasTag(FName("OfflineLobbyIntro")))
        {
            // .text:00000001402735B9 mov [rsi+468h], rbx
            // Assign to _matineeIntroActor (Offset 0x468).
            this->_matineeIntroActor = MatineeActor;
        }
        // .text:00000001402735C8 lea rdx, aOfflinelobbylo ; "OfflineLobbyLoop"
        // .text:00000001402735DF call ?ActorHasTag@AActor@@QEBA_NVFName@@@Z
        // Check if the actor has the "OfflineLobbyLoop" tag.
        else if (MatineeActor->ActorHasTag(FName("OfflineLobbyLoop")))
        {
            // .text:00000001402735E8 mov [rsi+470h], rbx
            // Assign to _matineeLoopActor (Offset 0x470).
            this->_matineeLoopActor = MatineeActor;
        }
    }

    // .text:0000000140273609 call ?Free@FMemory@@SAXPEAX@Z
    // The disassembly manually frees the array memory here.
    // In C++, the TArray destructor handles this automatically when 'MatineeActors' goes out of scope.
}




void ADBDGame_Menu::Tick(float DeltaSeconds)
{
    // .text:0000000140279509 call ?Tick@AGameMode@@UEAAXM@Z
    // Call parent implementation first.
    Super::Tick(DeltaSeconds);

    // .text:000000014027950E mov r11, [rbx+460h]
    // Access _matineeFadeActor.
    if (this->_matineeFadeActor == nullptr)
    {
        return;
    }

    // .text:000000014027951E ... loc_14027954A ... test al, 1
    // Validate _matineeFadeActor (check against GUObjectArray implies IsValid/PendingKill check).
    if (this->_matineeFadeActor->IsPendingKill())
    {
        return;
    }

    // .text:0000000140279558 test byte ptr [r11+140h], 4
    // Check AActor::bAutoDestroyWhenFinished (Offset 0x140 bit 4).
    if (this->_matineeFadeActor->bAutoDestroyWhenFinished)
    {
        return;
    }

    // .text:0000000140279586 mov rdx, [rbx+468h]
    // Access _matineeIntroActor.
    if (this->_matineeIntroActor == nullptr)
    {
        return;
    }

    // .text:0000000140279596 ... loc_1402795AC ... test al, 1
    // Validate _matineeIntroActor.
    if (this->_matineeIntroActor->IsPendingKill())
    {
        return;
    }

    // .text:00000001402795B6 test byte ptr [rdx+140h], 4
    // Check AActor::bAutoDestroyWhenFinished on Intro Actor.
    if (this->_matineeIntroActor->bAutoDestroyWhenFinished)
    {
        return;
    }

    // .text:00000001402795D7 movzx eax, byte ptr [rbx+45Ah]
    // Check the current state of _gameIntroSteps (Offset 0x45A).
    if (this->_gameIntroSteps == EGameIntroSteps::Idling)
    {
        // .text:00000001402795E2 test byte ptr [r11+3E0h], 1
        // Check if _matineeFadeActor is currently playing (AMatineeActor::bIsPlaying at offset 0x3E0).
        if (this->_matineeFadeActor->bIsPlaying)
        {
            // .text:00000001402795EC mov byte ptr [rbx+45Ah], 1
            // Advance state to PlayingIntro.
            this->_gameIntroSteps = EGameIntroSteps::PlayingIntro;

            // .text:0000000140279603 jmp qword ptr [rax+5B0h]
            // Play the intro sequence.
            this->_matineeIntroActor->Play();
        }
    }
    // .text:000000014027960A cmp al, 1
    else if (this->_gameIntroSteps == EGameIntroSteps::PlayingIntro)
    {
        // .text:000000014027960E test [rdx+3E0h], al
        // Check if _matineeIntroActor is NOT playing (has finished).
        if (!this->_matineeIntroActor->bIsPlaying)
        {
            // .text:0000000140279616 mov this, [rbx+470h]
            // Access _matineeLoopActor.

            // .text:000000014027961D mov byte ptr [rbx+45Ah], 2
            // Advance state to LoopingCamView.
            this->_gameIntroSteps = EGameIntroSteps::LoopingCamView; // Enum value 2 deduced from disassembly

            // .text:0000000140279627 call qword ptr [rax+5B0h]
            // Play the loop sequence.
            if (this->_matineeLoopActor != nullptr)
            {
                this->_matineeLoopActor->Play();
            }
        }
    }
}




void ADBDGame_Menu::UpdatePlayerData(APlayerController* PlayerController)
{
    // .text:000000014027B7D0 call ?GetGameInstance@AActor@@QEBAPEAVUGameInstance@@XZ
    UGameInstance* GameInstance = this->GetGameInstance();

    // .text:000000014027B7D5 test rax, rax
    // .text:000000014027B7D8 jz loc_14027BA96
    if (GameInstance == nullptr)
    {
        return;
    }

    // .text:000000014027B833 call ?GetPrivateStaticClass@UDBDGameInstance...
    // The disassembly validates the GameInstance class type.
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // .text:000000014027B886 test rsi, rsi
    // Validate PlayerController argument.
    if (PlayerController == nullptr)
    {
        return;
    }

    // .text:000000014027B8B3 test byte ptr [rsi+140h], 4
    // Check if the PlayerController is pending kill (AActor::bAutoDestroyWhenFinished check or similar lifecycle flag).
    if (PlayerController->IsPendingKill())
    {
        return;
    }

    // .text:000000014027B8E5 mov rbp, [rsi+3A0h]
    // Access PlayerController->PlayerState (Offset 0x3A0).
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerController->PlayerState);

    // .text:000000014027B946 test rbp, rbp
    if (DBDPlayerState == nullptr)
    {
        return;
    }

    // --- LOGIC BLOCK 1: SYNC DATA TO PLAYER STATE ---

    // .text:000000014027B9DC call qword ptr [rax+628h]
    // Virtual call at offset 0x628. In UE4 PlayerController, this is IsLocalController().
    if (PlayerController->IsLocalController())
    {
        // .text:000000014027B9EA mov rbx, [r14+3B8h]
        // Access UDBDGameInstance->_persistentData (Offset 0x3B8).
        UDBDPersistentData* PersistentData = DBDGameInstance->_persistentData;

        if (PersistentData != nullptr)
        {
            // .text:000000014027B9F8 call ?ResetPreviousValues@FPlayerPersistentData@@QEAAXXZ
            // Reset cached values in the local persistent data struct.
            // Offset 0x168 indicates accessing _localPlayerPersistentData within UDBDPersistentData.
            PersistentData->_localPlayerPersistentData.ResetPreviousValues();

            // .text:000000014027BA0A call ?CopyData@UDBDPersistentData@@SAXPEAUFPlayerPersistentData@@PEAVADBDPlayerState@@_N@Z
            // Static function call to copy data from struct to PlayerState.
            // Arg3 (r8b) is 1 (true).
            UDBDPersistentData::CopyData(&PersistentData->_localPlayerPersistentData, DBDPlayerState, true);
        }
    }

    // --- LOGIC BLOCK 2: RELOAD PROFILE FACADE ---

    // .text:000000014027BA5D mov eax, [rsi+3BCh]
    // .text:000000014027BA63 shr eax, 1
    // .text:000000014027BA65 test al, 1
    // Checks a specific bitfield flag on the PlayerController at offset 0x3BC.
    // This is likely checking a cached 'IsLocal' or 'IsPrimary' flag before making the virtual call again.
    // We will assume this is a necessary pre-condition for the Facade reload.
    /* UNDEFINED ELEMENT - Raw Flag Check from Disassembly */
    bool bSpecificFlagCheck = PlayerController->bAttachToPawn;

    if (bSpecificFlagCheck)
    {
        // .text:000000014027BA6F call qword ptr [rax+628h]
        // Check IsLocalController() again.
        if (PlayerController->IsLocalController())
        {
            // .text:000000014027BA79 mov this, [r14+3C0h]
            // Access UDBDGameInstance->_playerDataFacade (Offset 0x3C0).
            if (DBDGameInstance->_playerDataFacade != nullptr)
            {
                // .text:000000014027BA82 call ?ConditionalReloadProfile@UPlayerDataStorageFacade@@QEAAX_N@Z
                // Trigger profile reload. Arg (dl) is 1 (true).
                DBDGameInstance->_playerDataFacade->ConditionalReloadProfile(true);
            }
        }
    }
}