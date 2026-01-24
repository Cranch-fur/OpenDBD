UClass* ADBDDebugGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    // Check if the incoming controller is a DBD specific player controller
    ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(InController);

    // Retrieve the Game Instance and cast it to the DBD specific Game Instance
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Ensure both the custom Controller and custom GameInstance are valid before proceeding
    if (DBDController != nullptr)
    {
        if (DBDGameInstance != nullptr)
        {
            // Retrieve the PlayerState from the controller
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(DBDController->PlayerState);

            if (DBDPlayerState != nullptr)
            {
                // Check a specific byte flag in the PlayerState (Offset 0x750).
                // Logic analysis indicates: Value 2 represents a Camper (Survivor), other values represent Slasher (Killer).
                /* UNDEFINED ELEMENT */
                EPlayerRole PlayerRoleRaw = DBDPlayerState->GameRole;

                // If the role is NOT 2 (Slasher/Killer logic)
                if (PlayerRoleRaw != VE_Camper)
                {
                    // Check if the SlasherPawnOverrides array has elements
                    if (this->SlasherPawnOverrides.Num() > 0)
                    {
                        // Generate a random index based on the logic in assembly (converting rand() to normalized float)
                        int32 ArrayNum = this->SlasherPawnOverrides.Num();
                        int32 RandomIndex = 0;

                        // Standard C rand() is used in disassembly, normalized to float [0,1] then multiplied by ArrayNum
                        /* UNDEFINED ELEMENT */
                        float NormalizedRand = (float)std::rand() * (1.0f / (float)RAND_MAX);
                        RandomIndex = (int32)(NormalizedRand * (float)ArrayNum);

                        // Clamp index to be safe (ASM uses cmovle to ensure index <= ArrayNum - 1)
                        if (RandomIndex > ArrayNum - 1)
                        {
                            RandomIndex = ArrayNum - 1;
                        }

                        // Retrieve the class from the array
                        UClass* SlasherClass = this->SlasherPawnOverrides[RandomIndex];

                        // Validate that the class exists and inherits from ASlasherPlayer
                        if (SlasherClass != nullptr)
                        {
                            if (SlasherClass->IsChildOf(ASlasherPlayer::StaticClass()) == true)
                            {
                                return SlasherClass;
                            }
                        }
                    }
                }
                // If the role IS 2 (Camper/Survivor logic)
                else
                {
                    // Check if the CamperPawnOverrides array has elements
                    if (this->CamperPawnOverrides.Num() > 0)
                    {
                        // Use the debug index set in the class
                        int32 CamperIndex = this->_currentCamperIndex;

                        // Ensure the index is valid within the array bounds to prevent crash, 
                        // though ASM primarily checks if the retrieved pointer is null.
                        if (this->CamperPawnOverrides.IsValidIndex(CamperIndex) == true)
                        {
                            // Retrieve the class from the array
                            UClass* CamperClass = this->CamperPawnOverrides[CamperIndex];

                            // Validate that the class exists and inherits from ACamperPlayer
                            if (CamperClass != nullptr)
                            {
                                if (CamperClass->IsChildOf(ACamperPlayer::StaticClass()) == true)
                                {
                                    return CamperClass;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Fallback: Call the parent implementation if no debug overrides were applied or checks failed
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}




APawn* ADBDDebugGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
    // Call the parent class implementation first and store the result.
    // The disassembly shows this is called immediately at the start (14022d64a).
    APawn* ResultPawn = ADBDGameMode::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

    // Attempt to cast the controller to the specific DBD Player Controller.
    ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(NewPlayer);

    // Attempt to cast the Game Instance to the specific DBD Game Instance.
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the Controller cast was successful.
    if (DBDController != nullptr)
    {
        // The disassembly performs low-level checks against the GUObjectArray (checking for Garbage flags)
        // and checks the 'bActorIsBeingDestroyed' bit (0x4 at offset 0x140).
        // In functional UE4 C++, IsPendingKill() checks the generic validity, 
        // and we check the specific flag explicitly as done in ASM.
        if (DBDController->IsPendingKill() == false && DBDController->bActorIsBeingDestroyed == false)
        {
            // Check if the GameInstance cast was successful.
            if (DBDGameInstance != nullptr)
            {
                // Similar validity check for the GameInstance (checking internal flags in GUObjectArray).
                if (DBDGameInstance->IsPendingKill() == false)
                {
                    // Retrieve the PlayerState from the controller and cast it.
                    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(DBDController->PlayerState);

                    if (DBDPlayerState != nullptr)
                    {
                        // Check a specific byte at offset 0x750 of the PlayerState.
                        // Based on context, Value 2 indicates a Camper (Survivor).
                        /* UNDEFINED ELEMENT */
                        EPlayerRole PlayerRole = DBDPlayerState->GameRole;

                        if (PlayerRole == VE_Camper)
                        {
                            // Check if the override array has elements.
                            if (this->CamperPawnOverrides.Num() > 0)
                            {
                                // Increment the current camper index modulo the array size.
                                // Logic: _currentCamperIndex = (_currentCamperIndex + 1) % ArrayNum
                                int32 ArrayNum = this->CamperPawnOverrides.Num();
                                int32 NextIndex = this->_currentCamperIndex + 1;

                                this->_currentCamperIndex = NextIndex % ArrayNum;
                            }
                        }
                    }
                }
            }
        }
    }

    // Return the pawn created by the parent implementation.
    return ResultPawn;
}




void ADBDDebugGameMode::StartNewPlayer(APlayerController* NewPlayer)
{
    // Retrieve the Game Instance and attempt to cast it to the custom DBD Game Instance
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Retrieve the PlayerState from the new controller
    // Accessing offset 0x3a0 in ASM (AController::PlayerState)
    APlayerState* GenericPlayerState = NewPlayer->PlayerState;

    // Check if the custom Game Instance is valid
    if (DBDGameInstance != nullptr)
    {
        // Check if the PlayerState is valid
        if (GenericPlayerState != nullptr)
        {
            // Attempt to cast to the custom DBD Player State
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(GenericPlayerState);

            // Verify the cast was successful
            if (DBDPlayerState != nullptr)
            {
                EPlayerRole Role = VE_Camper;

                // Check the debug flag 'StartWithSlasher' (Offset 0x458 in ASM)
                if (this->StartWithSlasher != false)
                {
                    // Access the GameState to check the player array
                    // ASM accesses offset 0x418 for GameState
                    if (this->GameState != nullptr)
                    {
                        // Check if this is the first player joining the game.
                        // ASM accesses offset 0x3b8 inside GameState which corresponds to PlayerArray.ArrayNum
                        if (this->GameState->PlayerArray.Num() == 1)
                        {
                            Role = VE_Slasher;
                        }
                    }
                }

                // If 'StartWithSlasher' was false, the code jumps directly to setting Role to 2 (VE_Camper).
                // If it was true, it performed the logic above.

                // Set the determined role on the PlayerState
                // The second argument '0' is derived from 'xor r8d, r8d' in ASM.
                DBDPlayerState->SetGameRole(Role, 0);
            }
        }
    }

    // Call the parent class implementation to finish the start process
    // Tailcall in ASM implies return AGameMode::StartNewPlayer(...)
    Super::StartNewPlayer(NewPlayer);
}