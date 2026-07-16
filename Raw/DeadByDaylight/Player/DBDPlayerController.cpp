// starts at line 41
ADBDPlayerController::ADBDPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    this->_didSetLocalPawn = false;

    this->_resettingYawScaling = true;
    this->_resettingPitchScaling = true;

    // GetPrivateStaticClass(L"/Script/DeadByDaylight") is the generated internal 
    // implementation behind the standard Unreal Engine StaticClass() macro.
    this->PlayerCameraManagerClass = ADBDPlayerCameraManager::StaticClass();
}

// starts at line 46
void ADBDPlayerController::InitPlayerState()
{
    Super::InitPlayerState();
}

// starts at line 51
void ADBDPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Call the base class implementation to ensure parent replicated properties are included in the array
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Register the '_theFence' variable for network replication.
    DOREPLIFETIME(ADBDPlayerController, _theFence);
}

// starts at line 58
void ADBDPlayerController::OnGameRoundStarted()
{
    // Ensure GameInstance and PlayerState are valid and of the expected DeadByDaylight classes.
    UDBDGameInstance* OwningGameInstance = Cast<UDBDGameInstance>(GetWorld()->OwningGameInstance);
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);

    // Explicit checks for validity (avoiding early exits with '!' as requested).
    if (OwningGameInstance == nullptr || DBDPlayerState == nullptr)
    {
        return;
    }

    // IsValid ensures the object isn't pending kill (checking GUObjectArray flags in the disassembly).
    if (IsValid(OwningGameInstance) == false || IsValid(DBDPlayerState) == false)
    {
        return;
    }

    // Perform state updates and log operations
    DBDPlayerState->FlushScoreEvents();
    DBDPlayerState->FullUpdateUserGameStatsCache();

    if (this->IsLocalController() == true)
    {
        // Obtain persistent data associated with the current player
        FUniqueNetIdRepl PlayerNetId = DBDPlayerState->UniqueId;
        FPlayerPersistentData* PersistentData = OwningGameInstance->_persistentData->GetPlayerPersistentData(PlayerNetId);

        // Check if data is valid and the game type supports progression
        if (PersistentData != nullptr)
        {
            if (OwningGameInstance->GetCurrentGametypeAffectsProgression() == true)
            {
                // Retrieve the disconnect leave penalty tunable value 
                float DisconnectPenalty = OwningGameInstance->DesignTunables->GetTunableValue(TunableValues::DISCONNECT_LEAVE_PENALTY, false);
                
                // Calculate penalty time and apply it to the saved data
                int32 PenaltyMinutes = static_cast<int32>((DisconnectPenalty * 2.0f) - 0.5f) / 2;
                PenaltyTime.Assign(0, 0, FTimespan::FromMinutes(PenaltyMinutes), 0, 0, 0);

                PersistentData->SavedData.LocalData.DisconnectPenaltyTime += PenaltyTime;
                
                // Update loadout and character index from current state
                PersistentData->SavedData.LocalData.LastConnectedLoadout = PersistentData->CurrentLoadout;
                PersistentData->SavedData.LocalData.LastConnectedCharacterIndex = PersistentData->GetCurrentCharacterID();

                // Set ongoing game time to current UTC time
                PersistentData->SavedData.LocalData.SetOngoingGameTime(FDateTime::UtcNow());
                
                // Save the modified profile data
                OwningGameInstance->_playerDataFacade->SaveFullProfile(false);
            }
        }

        // Try to retrieve and reset the achievement handler component
        UAchievementHandlerComponent* AchievementHandler = Cast<UAchievementHandlerComponent>(DBDPlayerState->GetComponentByClass(UAchievementHandlerComponent::StaticClass()));        
        if (IsValid(AchievementHandler) == true)
        {
            AchievementHandler->Reset();
        }
        
        // Finalize state by logging current loadout
        DBDPlayerState->LogCurrentLoadout();
    }
}

// starts at line 94
void ADBDPlayerController::PostInitializeComponents()
{
    // Call the parent class implementation
    Super::PostInitializeComponents();

    UWorld* World = this->GetWorld();
    
    // Ensure the world context is valid
    if (World == nullptr)
    {
        return;
    }

    // Attempt to retrieve and cast the game instance
    UDBDGameInstance* OwningGameInstance = Cast<UDBDGameInstance>(World->OwningGameInstance);

    // Verify the game instance is not pending kill
    if (IsValid(OwningGameInstance) == false)
    {
        return;
    }

    // Check if the current world type is Play-In-Editor (1) or Game (3)
    // and verify if this player controller has network authority (ROLE_Authority == 3)
    if ((World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::Game) && this->Role == ROLE_Authority)
    {
        // Setup parameters for spawning the network fence actor
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;

        // Spawn the ANetworkFenceActor in the world and assign it to the internal pointer
        // Passing nullptr for Location and Rotation falls back to origin/zero rotation
        this->_theFence = World->SpawnActor<ANetworkFenceActor>(ANetworkFenceActor::StaticClass(), nullptr, nullptr, SpawnParameters);
    }

    // Explicitly call the replication function for the fence
    this->OnRep_Fence();
}

// starts at line 112
bool ADBDPlayerController::Server_SetSpectatorMode_Validate(bool)
{
    return true;
}

// starts at line 121
void ADBDPlayerController::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
    // Reset the flag indicating if there was any movement input during this frame
    this->_hasMoveInputThisFrame = false;

    // Call the parent class implementation to ensure standard actor ticking logic is processed.
    // The disassembly explicitly calls APlayerController instead of ADBDPlayerControllerBase.
    Super::TickActor(DeltaTime, TickType, ThisTickFunction);
}

// starts at line 127
void ADBDPlayerController::UpdateAxisScaling(float DeltaTime, bool force)
{
    ADBDPlayer* ControlledPlayer = this->GetControlledPlayer();

    // Checking GUObjectArray flags translates to standard UE IsValid check
    if (IsValid(ControlledPlayer) == false)
    {
        return;
    }

    // Check if Yaw scaling needs to be updated
    if (force == true || 
        this->_yawInputScalingTimer.IsDone() == false || 
        this->GetOriginalYawScale(true) != this->_originalConstantYawScale || 
        this->GetOriginalYawScale(false) != this->_originalAbsoluteYawScale)
    {
        this->_yawInputScalingTimer.Update(DeltaTime);
        
        // The boolean parameter represents 'isUsingGamepad'
        this->_originalConstantYawScale = this->GetOriginalYawScale(true);
        this->_originalAbsoluteYawScale = this->GetOriginalYawScale(false);

        float PercentTimeElapsed = this->_yawInputScalingTimer.GetPercentTimeElapsed();

        if (this->_resettingYawScaling == false)
        {
            // Interpolate towards the target input scaling
            this->_constantYawScale = this->_previousConstantYawScale + ((this->_targetYawInputScaling - this->_previousConstantYawScale) * PercentTimeElapsed);
            this->_absoluteYawScale = this->_previousAbsoluteYawScale + ((this->_targetYawInputScaling - this->_previousAbsoluteYawScale) * PercentTimeElapsed);
        }
        else
        {
            // Interpolate back to the original input scaling
            this->_constantYawScale = this->_previousConstantYawScale + ((this->_originalConstantYawScale - this->_previousConstantYawScale) * PercentTimeElapsed);
            this->_absoluteYawScale = this->_previousAbsoluteYawScale + ((this->_originalAbsoluteYawScale - this->_previousAbsoluteYawScale) * PercentTimeElapsed);
        }
    }

    // Check if Pitch scaling needs to be updated
    if (force == true || 
        this->_pitchInputScalingTimer.IsDone() == false || 
        this->GetOriginalPitchScale(true) != this->_originalConstantPitchScale || 
        this->GetOriginalPitchScale(false) != this->_originalAbsolutePitchScale)
    {
        this->_pitchInputScalingTimer.Update(DeltaTime);
        
        // The boolean parameter represents 'isUsingGamepad'
        this->_originalConstantPitchScale = this->GetOriginalPitchScale(true);
        this->_originalAbsolutePitchScale = this->GetOriginalPitchScale(false);

        float PercentTimeElapsed = this->_pitchInputScalingTimer.GetPercentTimeElapsed();

        if (this->_resettingPitchScaling == false)
        {
            // Interpolate towards the target input scaling
            this->_constantPitchScale = this->_previousConstantPitchScale + ((this->_targetPitchInputScaling - this->_previousConstantPitchScale) * PercentTimeElapsed);
            this->_absolutePitchScale = this->_previousAbsolutePitchScale + ((this->_targetPitchInputScaling - this->_previousAbsolutePitchScale) * PercentTimeElapsed);
        }
        else
        {
            // Interpolate back to the original input scaling
            this->_constantPitchScale = this->_previousConstantPitchScale + ((this->_originalConstantPitchScale - this->_previousConstantPitchScale) * PercentTimeElapsed);
            this->_absolutePitchScale = this->_previousAbsolutePitchScale + ((this->_originalAbsolutePitchScale - this->_previousAbsolutePitchScale) * PercentTimeElapsed);
        }
    }
}

// starts at line 172
void ADBDPlayerController::Tick(float DeltaTime)
{
    // Call the parent Tick function
    Super::Tick(DeltaTime);

    // Update the axis scaling, passing 'false' for the force parameter
    this->UpdateAxisScaling(DeltaTime, false);

    // Check if the local pawn has already been set
    if (this->_didSetLocalPawn == false)
    {
        APawn* CurrentPawn = this->Pawn;
        if (IsValid(CurrentPawn) == true)
        {
            // Check if the pawn is locally controlled
            if (CurrentPawn->IsLocallyControlled() == true)
            {
                UDBDGameInstance* OwningGameInstance = Cast<UDBDGameInstance>(this->GetWorld()->OwningGameInstance);
                if (IsValid(OwningGameInstance) == true)
                {
                    this->_didSetLocalPawn = true;
                }
            }
        }
    }

    // Handle spectating logic for the server/authority
    if (this->Role == ROLE_Authority && this->_spectating == true)
    {
        ADBDPlayerState* SpectatedPlayer = this->GetSpectatedPlayerState();
        if (IsValid(SpectatedPlayer) == true)
        {
            AGameMode* AuthorityGameMode = this->GetWorld()->AuthorityGameMode;
            if (IsValid(AuthorityGameMode) == true)
            {
                // If the game mode no longer allows spectating the current player, switch to the next player
                if (AuthorityGameMode->CanSpectate(this, SpectatedPlayer) == false)
                {
                    this->ServerViewNextPlayer();
                }
            }
        }
    }

    // Check the console variable for showing pings
    static IConsoleVariable* CVarShowPings = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.ShowPings"));
    if (CVarShowPings != nullptr && CVarShowPings->GetInt() != 0)
    {
        // Execute a lambda if the GameState is valid to update/show pings.
        UDBDUtilities::CallIfGameStateValid(this, [this](ADBDGameState* GameState)
        {
            // Ensure the GameState pointer is valid
            if (GameState == nullptr)
            {
                return;
            }

            // Loop through the PlayerArray (Data is at offset 0x3B0, Num is at 0x3B8)
            for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
            {
                APlayerState* CurrentPlayerState = GameState->PlayerArray[i];

                if (CurrentPlayerState != nullptr)
                {
                    // Retrieve the player's UObject name 
                    // (Offset 0x18 corresponds to UObject::NamePrivate, accessed via GetFName())
                    FString PlayerName = CurrentPlayerState->GetFName().ToString();

                    // Retrieve the player's exact ping 
                    // (Offset 0x3C0 corresponds to APlayerState::ExactPing)
                    float PingValue = CurrentPlayerState->ExactPing;

                    // Format the debug string
                    FString DebugMessage = FString::Printf(TEXT("%s : Ping: %f"), *PlayerName, PingValue);

                    if (GEngine != nullptr)
                    {
                        // Retrieve the player's unique PlayerId to use as the message Key
                        // (Offset 0x3A8 corresponds to APlayerState::PlayerId)
                        // Using the PlayerId as the Key ensures the text overwrites itself on screen
                        // rather than creating a new line every tick.
                        uint64 Key = static_cast<uint64>(CurrentPlayerState->PlayerId);

                        GEngine->AddOnScreenDebugMessage(
                            Key, 
                            -1.0f, 
                            FColor::White, 
                            DebugMessage, 
                            true, 
                            FVector2D::UnitVector
                        );
                    }
                }
            }
        });
    }
}

// starts at line 239
void ADBDPlayerController::GetAudioListenerPosition(FVector& OutLocation, FVector& OutFrontDir, FVector& OutRightDir)
{
    // Check if we have a valid Character and CameraManager with an active camera cache
    if (this->Character != nullptr && 
        this->PlayerCameraManager != nullptr && 
        this->PlayerCameraManager->CameraCache.TimeStamp > 0.0f)
    {
        // Console variable to override AudioListener position logic
        static IConsoleVariable* CVarAudioListenerOnActor = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.AudioListenerOnActor"));
        bool bShouldUseActorListener = false;
        
        if (CVarAudioListenerOnActor != nullptr)
        {
            // Thread safety check matching the disassembly logic before accessing CVar values
            if (GIsGameThreadIdInitialized == false || 
                FPlatformTLS::GetCurrentThreadId() == GGameThreadId || 
                FPlatformTLS::GetCurrentThreadId() == GSlateLoadingThreadId)
            {
                bShouldUseActorListener = CVarAudioListenerOnActor->GetInt() != 0;
            }
            else
            {
                bShouldUseActorListener = false;
            }
        }

        if (bShouldUseActorListener)
        {
            // Get the current camera rotation
            FRotator CameraRotation = this->PlayerCameraManager->GetCameraRotation();
            FVector ActorLocation = FVector::ZeroVector;

            // Get the character's root component location
            USceneComponent* RootComp = this->Character->GetRootComponent();
            if (RootComp != nullptr)
            {
                ActorLocation = RootComp->GetComponentTransform().GetTranslation();
            }

            OutLocation = ActorLocation;

            // Convert rotation to a transformation matrix to extract directional vectors.
            // The massive block of SSE instructions in the disassembly is the inline expansion 
            // of Unreal Engine's FRotationMatrix creation and subsequent unit axis extraction.
            FRotationMatrix ViewRotationMatrix(CameraRotation);

            OutFrontDir = ViewRotationMatrix.GetUnitAxis(EAxis::X); // ForOneFrame/ForDuration maps to axis indices in UE
            OutRightDir = ViewRotationMatrix.GetUnitAxis(EAxis::Y);

            return;
        }
    }

    // Fallback to standard PlayerController behavior if conditions aren't met
    Super::GetAudioListenerPosition(OutLocation, OutFrontDir, OutRightDir);
}

// starts at line 257
void ADBDPlayerController::Server_SetReadyToTravel_Implementation()
{
    // Check if the AuthorityGameMode exists in the current world context
    AGameModeBase* CurrentGameMode = this->GetWorld()->GetAuthGameMode();

    if (CurrentGameMode != nullptr)
    {
        // Try casting the player state to ADBDPlayerState
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        if (DBDPlayerState != nullptr)
        {
            // Verify if the player state is valid using standard Unreal checks
            if (IsValid(DBDPlayerState) == true)
            {
                // In the disassembly, offset 0x820 is accessed and set to 1.
                DBDPlayerState->IsReadyToTravel = true;
            }
        }

        // Check if the current game mode is ADBDGameMode and start the travel process
        ADBDGameMode* DBDGameMode = Cast<ADBDGameMode>(CurrentGameMode);

        if (DBDGameMode != nullptr)
        {
            DBDGameMode->StartTravel();
        }
    }
}

// starts at line 280
void ADBDPlayerController::Server_LeaveGame_Implementation()
{
    // Ensure the controller has Authority
    if (this->Role == ROLE_Authority)
    {
        // Handle Server (Host) leaving the game
        AGameModeBase* AuthGameMode = this->GetWorld()->GetAuthGameMode();
        if (AuthGameMode != nullptr)
        {
            ADBDGameState* GameState = Cast<ADBDGameState>(AuthGameMode->GameState);
            if (IsValid(GameState) == true)
            {
                // If the current NetMode is ListenServer and this is the local controller (the host)
                if (this->GetNetMode() == NM_ListenServer && this->IsLocalController() == true)
                {
                    GameState->Multicast_SetServerLeftGame(true);
                }
            }
        }

        // Handle Camper (Client) leaving the game
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
        if (IsValid(DBDPlayerState) == true)
        {
            // In the disassembly, offset 0x9A1 is accessed and compared to sil (0).
            if (DBDPlayerState->_inParadise == false)
            {
                ACamperPlayer* CamperCharacter = Cast<ACamperPlayer>(this->GetCharacter());
                if (IsValid(CamperCharacter) == true && CamperCharacter->IsActorBeingDestroyed() == false)
                {
                    if (CamperCharacter->IsValidImpl() == true)
                    {
                        // Force kill the camper if they disconnect while alive
                        CamperCharacter->Kill(false);
                        this->AuthorityOnCamperDisconnect();
                    }
                }

                // Update player state to reflect leaving the match
                DBDPlayerState->SetPlayerGameState(EGameState::VE_ManuallyLeftMatch);
                DBDPlayerState->FlushScoreEvents();
                DBDPlayerState->Multicast_SetAsLeftMatch();

                // Fire a game event to notify the game tracker (typically for Slasher scoring)
                UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
                if (IsValid(GameInstance) == true)
                {
                    // The offset 0x108 on GameInstance maps to the UGameEventTracker component in the disassembly.
                    UGameEventTracker* EventTracker = GameInstance->GameEventTracker;
                    if (EventTracker != nullptr)
                    {
                       EventTracker->FireGameEvent(EDBDScoreTypes::DBDSlasherScore_CamperDisconnectedDuringMatch, 1.0f, nullptr, CamperCharacter);
                    }
                }
            }
        }
    }
}

// starts at line 319
void ADBDPlayerController::Server_EndGame_Implementation(bool serverHasLeftTheGame)
{
    // Ensure the controller has Authority
    if (this->Role == ROLE_Authority)
    {
        // 1. Process Player State: Flush any pending score events before the match terminates
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
        if (DBDPlayerState != nullptr)
        {
            if (IsValid(DBDPlayerState) == true)
            {
                DBDPlayerState->FlushScoreEvents();
            }
        }

        // 2. Process Game Mode and Game State
        ADBDGameMode* AuthGameMode = Cast<ADBDGameMode>(this->GetWorld()->GetAuthGameMode());
        if (AuthGameMode != nullptr)
        {
            if (IsValid(AuthGameMode) == true)
            {
                ADBDGameState* DBDGameState = Cast<ADBDGameState>(AuthGameMode->GameState);
                if (IsValid(DBDGameState) == true)
                {
                    // Notify all clients whether the server has left the game
                    DBDGameState->Multicast_SetServerLeftGame(serverHasLeftTheGame);

                    // Update the survivor count to zero as the game is ending
                    DBDGameState->Authority_SetSurvivorLeft(0);

                    // Set an internal flag on the GameState
                    DBDGameState->IsGameEnded = true;
                }

                // Call the game mode handler for match termination logic
                AuthGameMode->HandleMatchEnded(serverHasLeftTheGame);
            }
        }
    }
}

// starts at line 350
void ADBDPlayerController::Server_CompleteEscapeRequirements_Implementation()
{
    // Retrieve the Authority Game Mode from the current World
    AGameModeBase* AuthGameMode = this->GetWorld()->GetAuthGameMode();

    // Ensure the Game Mode is valid
    if (AuthGameMode != nullptr)
    {
        // Cast the Game Mode to the specific Dead by Daylight Game Mode class
        ADBDGameMode* DBDGameMode = Cast<ADBDGameMode>(AuthGameMode);

        // Verify the cast was successful (matches the class tree index checks in disassembly)
        if (DBDGameMode != nullptr)
        {
            // Trigger the completion of escape requirements on the server
            DBDGameMode->CompleteEscapeRequirements();
        }
    }
}

// starts at line 368
void ADBDPlayerController::Client_SetSpectatorMode_Implementation(bool on)
{
    // Check if this is the locally controlled player controller
    if (this->IsLocalController() == true)
    {
        this->_spectating = on;

        APlayerState* CurrentPlayerState = this->PlayerState;
        
        // Ensure PlayerState is valid before modifying flags
        if (CurrentPlayerState != nullptr)
        {
            if (on == true)
            {
                // In the disassembly, rax+3ACh is modified. This is inside APlayerState.
                CurrentPlayerState->bIsSpectator = true;

                // Change state to Spectating (index 322/0x142 in the disassembly)
                this->ChangeState(NAME_Spectating);
            }
            else
            {
                // Clear the spectator flag
                CurrentPlayerState->bIsSpectator = false;

                // Change state back to Inactive or standard state (index 320/0x140 in the disassembly)
                this->ChangeState(NAME_Playing);
            }
        }

        // Notify the server of the change in spectator mode
        this->Server_SetSpectatorMode(on);
    }
}

// starts at line 381
void ADBDPlayerController::OnRep_Fence()
{
    // Standard Unreal Engine validity check (expanded GUObjectArray and pending kill checks in disassembly)
    if (IsValid(this->_theFence) == true)
    {
        UWorld* World = this->GetWorld();
        
        if (World != nullptr)
        {
            // Retrieve and cast the owning game instance to UDBDGameInstance
            UDBDGameInstance* OwningGameInstance = Cast<UDBDGameInstance>(World->OwningGameInstance);
            
            // Standard Unreal Engine validity check for the game instance
            if (IsValid(OwningGameInstance) == true)
            {
                // Check if this object is explicitly a player controller and is controlled locally
                if (this->bIsPlayerController == true && this->IsLocalController() == true)
                {
                    // Pass the valid fence actor to the game instance for initialization
                    OwningGameInstance->InitializeFence(this->_theFence);
                }
            }
        }
    }
}

// starts at line 417
void ADBDPlayerController::Server_SetSpectatorMode_Implementation(bool on)
{
    // Update the local spectating flag
    this->_spectating = on;
    
    APlayerState* CurrentPlayerState = this->PlayerState;

    if (on == true)
    {
        // Set the spectator flag in PlayerState.
        CurrentPlayerState->bIsSpectator = true;

        // Change state to Spectating (FName index 322/0x142)
        this->ChangeState(NAME_Spectating);

        // Try to find a valid target to spectate
        UWorld* World = this->GetWorld();
        if (World != nullptr)
        {
            AGameStateBase* GameState = World->GetGameState();
            if (GameState != nullptr)
            {
                AGameModeBase* AuthGameMode = World->GetAuthGameMode();
                
                if (AuthGameMode != nullptr)
                {
                    // Iterate through the PlayerArray
                    for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
                    {
                        APlayerState* TargetPlayerState = GameState->PlayerArray[i];

                        if (TargetPlayerState != nullptr)
                        {
                            // Ask the GameMode if we can spectate this player
                            if (AuthGameMode->CanSpectate(this, TargetPlayerState) == true)
                            {
                                // Prepare the transition parameters
                                FViewTargetTransitionParams TransitionParams;
                                TransitionParams.BlendTime = 0.0f;
                                TransitionParams.BlendExp = 2.0f;
                                TransitionParams.BlendFunction = VTBlend_Cubic; // VTBlend_Cubic is 1
                                TransitionParams.bLockOutgoing = false;

                                // Set the view target to the spectated player
                                this->SetViewTarget(TargetPlayerState, TransitionParams);
                                
                                // Found a target, break out of the loop
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        // Clear the spectator flag
        CurrentPlayerState->bIsSpectator = false;

        // Change state back to Playing (FName index 320/0x140)
        this->ChangeState(NAME_Playing);
    }
}

// starts at line 436
bool ADBDPlayerController::IsSpectating() const
{
    return this->_spectating;
}

// starts at line 442
ADBDPlayerState* ADBDPlayerController::GetSpectatedPlayerState() const
{
    // Return early if not in spectating mode
    if (this->_spectating == false)
    {
        return nullptr;
    }

    // Attempt to get the current view target actor
    AActor* ViewTarget = this->GetViewTarget();

    // Standard check to ensure ViewTarget is valid and not pending kill
    if (IsValid(ViewTarget) == false || ViewTarget->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Try casting directly to ADBDPlayerState
    ADBDPlayerState* SpectatedState = Cast<ADBDPlayerState>(ViewTarget);

    if (IsValid(SpectatedState) == true)
    {
        return SpectatedState;
    }

    // If it's a Controller or a Pawn, attempt to retrieve the PlayerState from it
    AController* TargetController = Cast<AController>(ViewTarget);
    APawn* TargetPawn = Cast<APawn>(ViewTarget);

    if (TargetController != nullptr || TargetPawn != nullptr)
    {
        // Extract PlayerState.
        APlayerState* ActorPlayerState = TargetController ? TargetController->PlayerState : TargetPawn->PlayerState;

        if (IsValid(ActorPlayerState) == true && ActorPlayerState->IsActorBeingDestroyed() == false)
        {
            if (ActorPlayerState->IsA<ADBDPlayerState>() == true)
            {
                return Cast<ADBDPlayerState>(ActorPlayerState);
            }
        }
    }

    return nullptr;
}

// starts at line 476
ADBDPlayer* ADBDPlayerController::GetControlledPlayer() const
{
    // If the controller is in spectating mode, it does not have a directly controlled player
    if (this->_spectating == true)
    {
        return nullptr;
    }

    // Retrieve the current view target actor
    AActor* ViewTarget = this->GetViewTarget();

    // Verify the view target is valid and not pending kill
    if (IsValid(ViewTarget) == false || ViewTarget->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Try to cast the view target directly to an ADBDPlayer
    ADBDPlayer* ControlledPlayer = Cast<ADBDPlayer>(ViewTarget);

    if (ControlledPlayer != nullptr)
    {
        return ControlledPlayer;
    }

    // If the view target isn't a player, check if it's a camera actor
    ACameraActor* CameraTarget = Cast<ACameraActor>(ViewTarget);

    if (CameraTarget != nullptr)
    {
        // If the view target is a camera, the controlled player might be the owner of the camera
        AActor* CameraOwner = CameraTarget->GetOwner();
        
        return Cast<ADBDPlayer>(CameraOwner);
    }

    // If the view target is neither an ADBDPlayer nor an ACameraActor owned by one, return nullptr
    return nullptr;
}

// starts at line 499
ADBDPlayer* ADBDPlayerController::GetSpectatedPlayer() const
{
    // If the controller is not in spectating mode, it does not have a spectated player
    if (this->_spectating == false)
    {
        return nullptr;
    }

    // Retrieve the current view target actor
    AActor* ViewTarget = this->GetViewTarget();

    // Verify the view target is valid and not pending kill
    if (IsValid(ViewTarget) == false || ViewTarget->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Try to cast the view target directly to an ADBDPlayer
    ADBDPlayer* SpectatedPlayer = Cast<ADBDPlayer>(ViewTarget);

    if (SpectatedPlayer != nullptr)
    {
        return SpectatedPlayer;
    }

    // If the view target isn't a player, check if it's a camera actor
    ACameraActor* CameraTarget = Cast<ACameraActor>(ViewTarget);

    if (CameraTarget != nullptr)
    {
        // If the view target is a camera, the spectated player might be the owner of the camera
        AActor* CameraOwner = CameraTarget->GetOwner();
        
        return Cast<ADBDPlayer>(CameraOwner);
    }

    // If the view target is neither an ADBDPlayer nor an ACameraActor owned by one, return nullptr
    return nullptr;
}

// starts at line 522
void ADBDPlayerController::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
    // Pass the transition parameters explicitly to the parent class implementation
    Super::SetViewTarget(NewViewTarget, TransitionParams);

    // Call local setup for observing the new view target
    this->SetupViewTargetObservation(NewViewTarget);

    // If this is the locally controlled player, update the audio engine RTPC values
    if (this->IsLocalController() == true)
    {
        FAkAudioDevice* AudioDevice = FAkAudioDevice::Get();

        // The disassembly provides the string literals and float values directly
        if (AudioDevice != nullptr)
        {
            AudioDevice->SetRTPCValue(TEXT("AudioRTPC_Proximity_Survivor_Killer"), 3200.0f, 0, nullptr);
            AudioDevice->SetRTPCValue(TEXT("AudioRTPC_Chase_Survivor"), 0.0f, 0, nullptr);
            AudioDevice->SetRTPCValue(TEXT("AudioRTPC_Chase_Killer"), 0.0f, 0, nullptr);
        }

        // Broadcast the OnLocallyObservedChanged delegate if there are bound listeners
        if (this->OnLocallyObservedChanged.IsBound() == true)
        {
            this->OnLocallyObservedChanged.Broadcast();
        }
    }
}

// starts at line 541
void ADBDPlayerController::SetupViewTargetObservation(AActor* NewViewTarget)
{
    // Check if this controller is a local player controller (bIsPlayerController bit 1 && IsLocalController)
    if (this->bIsPlayerController == true && this->IsLocalController() == true)
    {
        // Ensure the new view target is a valid Dead by Daylight Player State (or similar class derived from it)
        // The disassembly checks the class tree index of NewViewTarget against ADBDPlayerState.
        if (NewViewTarget->IsA(ADBDPlayerState::StaticClass()) == false)
        {
            return;
        }
        
        UWorld* World = this->GetWorld();
        if (World != nullptr)
        {
            ADBDGameState* GameState = Cast<ADBDGameState>(World->GameState);

            if (IsValid(GameState) == true && GameState->IsActorBeingDestroyed() == false)
            {
                if (IsValid(NewViewTarget) == true && NewViewTarget->IsActorBeingDestroyed() == false)
                {
                    // Iterate through all pawns in the world
                    for (FConstPawnIterator It = World->GetPawnIterator(); It; ++It)
                    {
                        APawn* CurrentPawn = It->Get();

                        if (CurrentPawn != nullptr)
                        {
                            // Check if the pawn is a Slasher Player
                            ASlasherPlayer* Slasher = Cast<ASlasherPlayer>(CurrentPawn);

                            if (IsValid(Slasher) == true && Slasher->IsActorBeingDestroyed() == false && Slasher->IsValidImpl())
                            {
                                // Retrieve the Slasher's current target camera/head actor
                                APlayerState* SlasherPlayerState = Slasher->PlayerState;

                                if (IsValid(SlasherPlayerState) == true && SlasherPlayerState->IsActorBeingDestroyed() == false)
                                {
                                    // Check if the new view target is the same as the Slasher's target
                                    bool bIsViewTargetKillerHead = (SlasherPlayerState == NewViewTarget);

                                    // Check if the hidden state has changed (offset 0x1164 in pseudo-C)
                                    if (Slasher->_isHeadHidden != bIsViewTargetKillerHead)
                                    {
                                        Slasher->_isHeadHidden = bIsViewTargetKillerHead;
                                        Slasher->OnHeadHidden(bIsViewTargetKillerHead);
                                    }

                                    // Update a value on the GameState (offset 0x620)
                                    float TrackingValue = bIsViewTargetKillerHead ? 1.0f : 0.0f;
                                    GameState->_killerIntroCompleted = TrackingValue;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// starts at line 573
void ADBDPlayerController::SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds)
{
    // Check if the controller is in spectating mode and if the TargetPawn is valid and is a DBDPlayer
    if (this->_spectating == true)
    {
        if (TargetPawn->IsA(ADBDPlayer::StaticClass()) == true)
        {
            ADBDPlayer* DBDPlayerTarget = Cast<ADBDPlayer>(TargetPawn);

            FRotator TargetRotation;

            // Get control rotation from the pawn if it is locally controlled or has server authority,
            // otherwise use the replicated control rotation from player data.
            if (DBDPlayerTarget->IsLocallyControlled() == true || DBDPlayerTarget->Role == ROLE_Authority)
            {
                TargetRotation = DBDPlayerTarget->GetControlRotation();
            }
            else
            {
                TargetRotation = DBDPlayerTarget->_playerData->_controlRotation;
            }

            // Normalize the rotation using standard FMath clamping functions
            // (The SSE block in the disassembly normalizes pitch, yaw, and roll to be within -180 to 180 degrees)
            TargetRotation.Normalize();

            this->BlendedTargetViewRotation = FMath::RInterpTo(this->BlendedTargetViewRotation, TargetRotation, DeltaSeconds, GSpectateSmoothSpeed);

            // UE_LOG statement generated if verbosity allows
            // Note: The GUI log category and verbosity checks are typical UE4 macro expansions.
            // Using standard UE_LOG syntax here.
            UE_LOG(GUI, Verbose, TEXT("Starting smooth from [%f, %f, %f] to [%f, %f, %f]"), 
                   this->BlendedTargetViewRotation.Pitch, this->BlendedTargetViewRotation.Yaw, this->BlendedTargetViewRotation.Roll,
                   TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll);

            return;
        }
    }

    // Fall back to the parent implementation if not spectating or target is invalid
    Super::SmoothTargetViewRotation(TargetPawn, DeltaSeconds);
}

// starts at line 612
void ADBDPlayerController::Destroyed()
{
    // Call the parent implementation to handle standard destruction logic
    Super::Destroyed();

    // Check if this controller has Authority (is running on the server)
    if (this->Role == ROLE_Authority)
    {
        // Verify the fence is valid and not pending kill
        if (IsValid(this->_theFence) == true)
        {
            // Call Destroy on the fence actor.
            // The parameters (false, true) correspond to bNetForce and bShouldModifyLevel respectively.
            this->_theFence->Destroy(false, true);

            // Nullify the pointer to prevent dangling references
            this->_theFence = nullptr;
        }
    }
}

// starts at line 621
void ADBDPlayerController::BeginDestroy()
{
    // Call the parent class implementation of BeginDestroy
    Super::BeginDestroy();

    // Check if the current controller has Authority
    if (this->Role == ROLE_Authority)
    {
        // Verify the object exists and is not pending kill
        if (IsValid(this->_theFence) == true)
        {
            // Call Destroy on the fence actor. 
            // Using false for bNetForce and true for bShouldModifyLevel matching the disassembly.
            this->_theFence->Destroy(false, true);

            // Nullify the pointer to prevent dangling references
            this->_theFence = nullptr;
        }
    }
}

// starts at line 630
void ADBDPlayerController::AuthorityEvaluateGameEndInternalState()
{
    // Check if the current controller has Authority
    if (this->Role == ROLE_Authority)
    {
        // Try casting the player state to ADBDPlayerState
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        // Standard Unreal Engine validity check
        if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
        {
            // In the disassembly, the byte at offset 0x750 is checked to see if it equals 2.
            if (DBDPlayerState->GameRole == EPlayerRole::VE_Camper)
            {
                this->EvaluateGameEndCamperInternalState(DBDPlayerState);
            }
        }
    }
}

// starts at line 662
void ADBDPlayerController::EvaluateGameEndCamperInternalState(ADBDPlayerState* playerState)
{
    // Try to get the current pawn and cast to a CamperPlayer
    ACamperPlayer* CamperPawn = Cast<ACamperPlayer>(this->GetPawn());

    // Standard check to ensure the camper pawn is valid and not pending kill
    if (IsValid(CamperPawn) == true && CamperPawn->IsActorBeingDestroyed() == false && CamperPawn->IsValidImpl() == true)
    {
        // Verify we have a valid GameState
        ADBDGameState* GameState = Cast<ADBDGameState>(this->GetWorld()->GameState);

        if (IsValid(GameState) == true && GameState->IsActorBeingDestroyed() == false)
        {
            // Extract a byte from offset 0x7F9 in playerState.
            // In the pseudo-C, it subtracts 2 and checks if the result is <= 1 (meaning it was 2 or 3).
            // It then later subtracts 6 and checks if the result is <= 1 (meaning it was 6 or 7).
            // This corresponds to EGameState values.
            // Assuming 2 = VE_Escaped, 3 = VE_EscapedInjured
            // Assuming 6 = VE_Disconnected, 7 = VE_ManuallyLeftMatch

            // In the disassembly, we verify a sub-object or condition on GameState (offset 0x5F0).
            ASlasherPlayer* Slasher = GameState->Slasher;
            
            if (IsValid(Slasher) == true && Slasher->IsActorBeingDestroyed() == false && Slasher->IsValidImpl() == true)
            {
                // Check if the state is NOT already escaped or disconnected
                // If the player is not already marked as Sacrificed or Dead
                if (playerState->PlayerData._playerGameState != EGameState::VE_Escaped && 
                    playerState->PlayerData._playerGameState != EGameState::VE_EscapedInjured &&
                    playerState->PlayerData._playerGameState != EGameState::VE_Disconnected && 
                    playerState->PlayerData._playerGameState != EGameState::VE_ManuallyLeftMatch)
                {
                    EGameState NewState = EGameState::VE_None;

                    if (CamperPawn->IsSacrificed() == true)
                    {
                        NewState = EGameState::VE_Sacrificed; // 4 in pseudo-C, mapped to VE_Sacrificed
                    }
                    else if (CamperPawn->IsDead() == true)
                    {
                        NewState = EGameState::VE_Dead; // 5 in pseudo-C, mapped to VE_Dead
                    }

                    playerState->SetPlayerGameState(NewState);
                }

                // Update flags to indicate the player is in the post-game "paradise" state
                playerState->bIsInactive = true;
                
                // Offset 0x750 is a virtual function call AuthoritySetInParadise
                playerState->AuthoritySetInParadise(true);
            }
        }
    }
}

// starts at line 677
void ADBDPlayerController::AuthorityEvaluateGameEndScoreEvents()
{
    // Check if the current controller has Authority
    if (this->Role == ROLE_Authority)
    {
        // Try casting the player state to ADBDPlayerState
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        // Standard Unreal Engine validity check
        if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
        {
            // In the disassembly, the byte at offset 0x750 is checked to see if it equals 1.
            if (DBDPlayerState->GameRole == EPlayerRole::VE_Slasher)
            {
                this->EvaluateGameEndSlasherScoreEvents(DBDPlayerState);
            }

            // Fire the general EndGame score event for the player
            // The score type parameter is 0x9C, mapping to DBDPlayerScore_EndGame
            DBDPlayerState->FireScoreEvent(EDBDScoreTypes::DBDPlayerScore_EndGame, 1.0f, true);
        }
    }
}

// starts at line 706
void ADBDPlayerController::EvaluateGameEndSlasherScoreEvents(ADBDPlayerState* playerState)
{
    if (IsValid(playerState) == false || playerState->IsActorBeingDestroyed() == true)
    {
        return;
    }

    int32 TotalCount = 0;
    int32 DeadCount = 0;
    int32 SacrificedCount = 0;  
    int32 NoneCount = 0;  
    int32 DisconnectedCount = 0;

    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (IsValid(GameInstance) == true)
    {
        // The massive block of code involving _cachedPlayerScoreMap and BitScanReverse
        // is the expanded form of iterating over a TMap in Unreal Engine.
        // It loops through all cached player scores to count the outcomes of the survivors.
        for (auto It = GameInstance->_persistentData->_cachedPlayerScoreMap.CreateConstIterator(); It; ++It)
        {
            const FCachedPlayerScoreData& ScoreData = It.Value();
            
            // Offset 0x4D from the start of FCachedPlayerScoreData corresponds to role (2 = Camper)
            if (ScoreData.Role == EPlayerRole::VE_Camper)
            {
                TotalCount++;

                // Offset 0x4C from the start of FCachedPlayerScoreData corresponds to the end game state
                EGameState EndState = ScoreData.PlayerGameState;

                switch (EndState)
                {
                    case EGameState::VE_Dead:
                        DeadCount++;
                        break;
                    
                    case EGameState::VE_Sacrificed:
                        SacrificedCount++;
                        break;
                    
                    case EGameState::VE_None:
                        NoneCount++;
                        break;
                    
                    case EGameState::VE_Disconnected:
                    case EGameState::VE_ManuallyLeftMatch:
                        DisconnectedCount++;
                        break;
                }
            }
        }
    }

    // Fire Slasher Score Events based on the counts
    if (TotalCount > 0 && (DeadCount + DisconnectedCount + SacrificedCount) >= TotalCount)
    {
        // Offset 0x60 in pseudo-C translates to DBDSlasherScore_NoEscape
        playerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_NoEscape, 1.0f, true);
    }
    
    // Offset 0x69 translates to DBDSlasherScore_SacrificedCount
    playerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_SacrificedCount, static_cast<float>(SacrificedCount), true);

    // Offset 0x5E translates to DBDSlasherScore_CamperDisconnect
    playerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_CamperDisconnect, static_cast<float>(NoneCount + DisconnectedCount), true);

    // Offset 0x61 translates to DBDSlasherScore_SacrificePercent (using NoneCount as value in disassembly)
    playerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_SacrificePercent, static_cast<float>(NoneCount), true);

    // Evaluate character-specific end game events
    ADBDPlayer* SlasherCharacter = Cast<ADBDPlayer>(this->GetCharacter());

    if (SlasherCharacter != nullptr)
    {
        if (IsValid(SlasherCharacter) == true)
        {
            // Check if all players are dead, disconnected, or sacrificed
            if ((DeadCount + DisconnectedCount + SacrificedCount) == GameInstance->GetMaxSurvivorCount())
            {
                // Call GetPerkManager on the Slasher
                UPerkManager* PerkManager = SlasherCharacter->GetPerkManager();

                if (IsValid(PerkManager) == true)
                {
                    if (PerkManager->HasAllCharacterPerksEquipped() == true)
                    {
                        // Offset 0x86 translates to DBDPlayerScore_FinishWithPerks
                        playerState->FireScoreEvent(EDBDScoreTypes::DBDPlayerScore_FinishWithPerks, 1.0f, true);
                    }
                }
            }

            // Check for Stalker Component achievements/scores (like Michael Myers)
            UStalkerComponent* StalkerComponent = Cast<UStalkerComponent>(SlasherCharacter->GetComponentByClass(UStalkerComponent::StaticClass()));

            if (IsValid(StalkerComponent) == true)
            {
                // Check a specific flag on the StalkerComponent (Offset 0x148 in pseudo-C, value 4)
                if (StalkerComponent->_campersKilled == 4)
                {
                    // Offset 0x73 translates to DBDSlasherScore_StalkerKillAllCampers
                    playerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_StalkerKillAllCampers, 1.0f, true);
                }
            }
        }
    }
}

// starts at line 801
void ADBDPlayerController::PawnLeavingGame()
{
    // Check if the current controller has Authority
    if (this->Role == ROLE_Authority)
    {
        // Handle network fence destruction
        if (this->_theFence != nullptr)
        {
            if (IsValid(this->_theFence) == true && this->_theFence->IsActorBeingDestroyed() == false)
            {
                // Call Destroy on the fence actor. 
                // Using false for bNetForce and true for bShouldModifyLevel matching the disassembly.
                this->_theFence->Destroy(false, true);
                
                // Nullify the pointer to prevent dangling references
                this->_theFence = nullptr;
            }
        }

        // Retrieve and cast the player state and character
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
        ADBDPlayer* DBDCharacter = Cast<ADBDPlayer>(this->Character);

        // Process delegates notifying that the pawn is leaving
        // The first delegate at offset 0x8C8 in pseudo-C corresponds to OnPawnLeavingGame
        if (this->OnPawnLeavingGame.IsBound() == true)
        {
            this->OnPawnLeavingGame.Broadcast(DBDCharacter);
        }

        // The second delegate at offset 0x8F0 corresponds to OnPawnLeavingGameRaw
        if (this->OnPawnLeavingGameRaw.IsBound() == true)
        {
            this->OnPawnLeavingGameRaw.Broadcast(DBDCharacter);
        }

        // Check if the player state is valid
        if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
        {
            // Offset 0x9A1 corresponds to !BYTE1(PlayerState[2].TimerHandle_LifeSpanExpired.Handle)
            // This checks if the player has not already left the match (bHasLeftMatch flag).
            if (DBDPlayerState->_inParadise == false)
            {
                this->AuthorityOnCamperDisconnect();
            }
        }
    }

    // Always call the base class implementation at the end
    Super::PawnLeavingGame();
}

// starts at line 825
void ADBDPlayerController::AuthorityOnCamperDisconnect()
{
    // Check if the current controller has Authority
    if (this->Role == ROLE_Authority)
    {
        // Try to cast the player state to ADBDPlayerState
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
        {
            // Try to cast the character to ACamperPlayer
            ACamperPlayer* CamperCharacter = Cast<ACamperPlayer>(this->GetCharacter());

            if (IsValid(CamperCharacter) == true && CamperCharacter->IsActorBeingDestroyed() == false && CamperCharacter->IsValidImpl())
            {
                // Check if a specific condition is met, offset 0x9A1 in pseudo-C, likely related to disconnection/abandonment status
                if (DBDPlayerState->_inParadise == false && CamperCharacter->DisconnectScoringDone == false)
                {
                    // 1. Calculate the Drop Rotation from the Root Component (Offset 0x160 -> 0x130 FQuat)
                    FRotator DropRotation = FRotator::ZeroRotator;

                    USceneComponent* RootComp = CamperCharacter->GetRootComponent();
                    if (RootComp != nullptr)
                    {
                        DropRotation = RootComp->GetComponentRotation();
                    }

                    // 2. Calculate the Drop Location using the _itemDropOffPosition component (Offset 0x0E68 -> 0x140 FVector)
                    FVector DropLocation = FVector::ZeroVector;
                    
                    USceneComponent* DropOffPosition = CamperCharacter->_itemDropOffPosition;
                    if (DropOffPosition != nullptr)
                    {
                        DropLocation = DropOffPosition->GetComponentLocation();
                    }
                    
                    // Try to drop the current item the player is holding at their death/disconnect location
                    CamperCharacter->TryDropCurrentItemOnDeath(DropLocation, DropRotation);

                    // Get GameState to verify conditions
                    ADBDGameState* GameState = this->GetWorld()->GetGameState<ADBDGameState>();
                    
                    if (IsValid(GameState) == true && GameState->IsActorBeingDestroyed() == false)
                    {
                        // In the pseudo-C, offset 0x5F0 points to Slasher or some endgame tracking component in the GameState.
                        ASlasherPlayer* Slasher = GameState->Slasher;
                        if (IsValid(Slasher) == true && Slasher->IsActorBeingDestroyed() == false && Slasher->IsValidImpl())
                        {
                            APlayerState* SlasherPlayerState = Slasher->PlayerState;
                            if (IsValid(SlasherPlayerState) == true && SlasherPlayerState->IsActorBeingDestroyed() == false)
                            {
                                // Get the remaining drain timer percent (e.g., entity progress or bleed-out on hook)
                                float DrainPercent = CamperCharacter->GetPercentDrainTimer();
                                
                                // Fire a score event for the Slasher based on the remaining drain percent
                                // The scoreType is 0x5E which maps to DBDSlasherScore_SacrificePercent
                                DBDPlayerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_SacrificePercent, DrainPercent, true);
                                
                                // Reset the drain timer percent left to 0.0
                                CamperCharacter->Authority_SetDrainTimerPercentLeft(0.0f);
                                
                                // Set the flag indicating the disconnect has been fully processed
                                CamperCharacter->DisconnectScoringDone = true;
                            }
                        }
                    }
                }
            }
        }

        // Notify the GameInstance that the player has disconnected
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (IsValid(GameInstance) == true)
        {
            ADBDPlayer* DBDPlayer = Cast<ADBDPlayer>(this->Character);

            if (IsValid(DBDPlayer) == true && DBDPlayer->IsActorBeingDestroyed() == false && DBDPlayer->IsValidImpl())
            {
                GameInstance->NotifyPlayerDisconnect(DBDPlayer);
            }
        }
    }
}

// starts at line 865
void ADBDPlayerController::SpectateKillerInEditor()
{
    // Ensure we are in a valid world with a valid GameState
    UWorld* World = this->GetWorld();
    if (World == nullptr)
    {
        return;
    }

    AGameStateBase* GameState = World->GetGameState();
    if (GameState == nullptr)
    {
        return;
    }

    // Iterate through all PlayerStates in the game
    for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
    {
        ADBDPlayerState* CurrentPlayerState = GameState->PlayerArray[i];

        if (CurrentPlayerState != nullptr)
        {
            // In the pseudo-C, offset 0x750 is checked against the value 2.
            // As seen in previous functions, this offset relates to the player's role, and 2 likely corresponds to the VE_Camper.
            if (CurrentPlayerState->GameRole == EPlayerRole::VE_Camper)
            {
                // In the pseudo-C, offset 0x90 retrieves the player controller associated with the PlayerState.
                // In standard UE4, this would be retrieved via APlayerState->GetOwner() or looking up the controller via ID.
                // Assuming standard accessor for this context.
                ADBDPlayerController* CamperController = Cast<ADBDPlayerController>(CurrentPlayerState->GetOwner());

                // Check validity of the cast explicitly
                if (IsValid(CamperController) == true)
                {
                    // Notify the server that this controller wants to spectate the killer
                    ADBDPlayerState* MyPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
                    CamperController->Server_SetSpectatorModeInEditor(true, MyPlayerState);

                    // Setup transition parameters for setting the view target
                    FViewTargetTransitionParams TransitionParams;
                    TransitionParams.BlendTime = 0.0f;
                    TransitionParams.BlendExp = 2.0f;
                    TransitionParams.BlendFunction = VTBlend_Cubic; // VTBlend_Cubic is enum value 1
                    TransitionParams.bLockOutgoing = false;

                    // Set the view target of the killer's controller to this controller's character
                    // The disassembly (offset 0xC18) points to the SetViewTarget virtual function in APlayerController
                    CamperController->SetViewTarget(this->GetCharacter(), TransitionParams);
                    
                    // We found the killer and processed the logic, break out of the loop
                    return;
                }
            }
        }
    }
}

// starts at line 883
void ADBDPlayerController::SpectateSurvivorInEditor()
{
    // Retrieve the GameState from the current World
    UWorld* World = this->GetWorld();
    if (World == nullptr)
    {
        return;
    }

    AGameStateBase* GameState = World->GetGameState();
    if (GameState == nullptr)
    {
        return;
    }

    ADBDPlayerController* FirstSurvivorController = nullptr;
    ADBDPlayerController* SecondSurvivorController = nullptr;
    ADBDPlayerState* TargetPlayerState = nullptr;

    // Iterate through the PlayerArray to find two survivors
    // The pseudo-C loops to find a player with role == VE_Escaped (which seems like an incorrect translation for VE_Camper or a similar role enum value, likely value 2)
    for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
    {
        ADBDPlayerState* CurrentPlayerState = Cast<ADBDPlayerState>(GameState->PlayerArray[i]);

        if (CurrentPlayerState != nullptr)
        {
            // Assuming the check is for VE_Camper (Survivor) based on the function name, though pseudo-C says VE_Escaped.
            if (CurrentPlayerState->GameRole == EPlayerRole::VE_Camper)
            {
                if (FirstSurvivorController == nullptr)
                {
                    // Found the first survivor, store their controller
                    FirstSurvivorController = Cast<ADBDPlayerController>(CurrentPlayerState->GetOwner());
                }
                else
                {
                    // Found a second survivor, store their controller and player state, then break
                    SecondSurvivorController = Cast<ADBDPlayerController>(CurrentPlayerState->GetOwner());
                    TargetPlayerState = CurrentPlayerState;
                    break;
                }
            }
        }
    }

    // Check if both controllers were found and are valid
    if (IsValid(FirstSurvivorController) == true && IsValid(SecondSurvivorController) == true)
    {
        // Notify the server that the first survivor's controller wants to spectate the second survivor
        FirstSurvivorController->Server_SetSpectatorModeInEditor(true, TargetPlayerState);

        // Prepare transition parameters for SetViewTarget
        FViewTargetTransitionParams TransitionParams;
        TransitionParams.BlendTime = 0.0f;
        TransitionParams.BlendExp = 2.0f;
        TransitionParams.BlendFunction = VTBlend_Cubic; // Enum value 1
        TransitionParams.bLockOutgoing = false;

        // Set the view target of the first survivor's controller to the second survivor's pawn/character
        AActor* TargetActor = SecondSurvivorController->GetCharacter();        
        FirstSurvivorController->SetViewTarget(TargetActor, TransitionParams);
    }
}

// starts at line 912
void ADBDPlayerController::Client_SetSpectatorModeInEditor_Implementation(bool on, ADBDPlayerState* spectatingPlayerState)
{
    APlayerState* CurrentPlayerState = this->PlayerState;
    
    // Set the internal spectating flag
    this->_spectating = on;

    if (CurrentPlayerState != nullptr)
    {
        // The disassembly modifies a bitfield at offset 0x3AC (235 * 4).
        // This toggles the first bit (which usually corresponds to bIsSpectator) to match the 'on' value.
        // It uses an XOR trick to set the specific bit.
        CurrentPlayerState->bIsSpectator = on;
    }
}

// starts at line 918
void ADBDPlayerController::Server_SetSpectatorModeInEditor_Implementation(bool on, ADBDPlayerState* spectatingPlayerState)
{
    // First, call the client-side RPC to set spectator mode on the client
    this->Client_SetSpectatorModeInEditor(on, spectatingPlayerState);

    // Get the controller's PlayerState
    APlayerState* CurrentPlayerState = this->PlayerState;

    // Update the local controller's spectating state
    this->_spectating = on;

    if (CurrentPlayerState != nullptr)
    {
        // Modify the same bitfield as the client (offset 0x3AC or 235 * 4) using the XOR trick
        // This corresponds to setting the bIsSpectator flag on the server's copy of the PlayerState.
        CurrentPlayerState->bIsSpectator = on;
    }
}

// starts at line 931
void ADBDPlayerController::Local_WriteGameEndStats()
{
    // Ensure the function only runs on the local player controller
    if (this->IsLocalController() == true)
    {
        // Retrieve the GameInstance and check its validity
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (IsValid(GameInstance) == true)
        {
            // Retrieve the PlayerState and check its validity
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

            if (IsValid(DBDPlayerState) == true && DBDPlayerState->IsActorBeingDestroyed() == false)
            {
                // Check if the game end stats have already been written (using offset 0x241)
                if (GameInstance->HasWrittenGameEndStats == false)
                {
                    // Mark that stats are now being written to prevent duplicate processing
                    GameInstance->HasWrittenGameEndStats = true;

                    // Verify if the current gametype affects progression
                    if (GameInstance->GetCurrentGametypeAffectsProgression() == false)
                    {
                        // The gametype doesn't grant progression, so abort saving stats
                        UE_LOG(GameFlow, Log, TEXT("WriteGameEndStats: Gametype does not affect progression"));
                        return;
                    }

                    // Evaluate badges locally
                    this->Local_EvaluateBadges();

                    // Update the user's game stats cache in the PlayerState
                    DBDPlayerState->FullUpdateUserGameStatsCache();

                    // Access the UserGameStats from the PlayerState (offset 0x888 + 8)
                    FUserGameStats& UserStats = DBDPlayerState->_cachedUserGameStats;
                    int32 SkullsToAdd = UserStats.Skulls;

                    // Log the score events recorded during the game
                    DBDPlayerState->LogScoreEvents();

                    // Prepare an object to write to the online leaderboard
                    FOnlineLeaderboardWrite WriteObject;
                    WriteObject.UpdateMethod = ELeaderboardUpdateMethod::KeepBest;

                    // Get the unique network ID
                    FUniqueNetIdRepl UniqueId = DBDPlayerState->GetUniqueId();

                    // Get persistent data for the player
                    FPlayerPersistentData* PersistentData = GameInstance->_persistentData->GetPlayerPersistentData(UniqueId);

                    if (PersistentData != nullptr)
                    {
                        // Add fear tokens gathered from the match
                        /* UNDEFINED ELEMENT */
                        PersistentData->AddFearTokens(UserStats->FearTokens);

                        // Extract total post-game experience and add it to persistent data
                        int32 TotalExp = UserStats->GetTotalExperiencePostGame();
                        PersistentData->AddExperience(TotalExp, TEXT("GameEnd"), DBDPlayerState);

                        // Get maximum allowed skulls from the tunable settings
                        int32 MaxSkulls = GameInstance->DesignTunables->GetMaxSkulls();

                        // Check the player's role (using offset 0x750)
                        if (DBDPlayerState->GameRole == EPlayerRole::VE_Slasher) // 1 = Slasher
                        {
                            int32 CurrentSkulls = PersistentData->SavedData.SharedData.SlasherSkulls;
                            // Clamp skulls to prevent falling below zero or exceeding max
                            if (CurrentSkulls + SkullsToAdd > MaxSkulls)
                            {
                                SkullsToAdd = MaxSkulls - CurrentSkulls;
                            }
                            else if (CurrentSkulls + SkullsToAdd < 0)
                            {
                                SkullsToAdd = -CurrentSkulls;
                            }
                            
                            WriteObject.SetIntStat(DBDProfileStats::DBD_KILLER_SKULLS, SkullsToAdd);
                            PersistentData->SavedData.SharedData.SlasherSkulls += SkullsToAdd;
                        }
                        else if (Role == EPlayerRole::VE_Camper) // 2 = Camper
                        {
                            int32 CurrentSkulls = PersistentData->SavedData.SharedData.CamperSkulls;
                            // Clamp skulls
                            if (CurrentSkulls + SkullsToAdd > MaxSkulls)
                            {
                                SkullsToAdd = MaxSkulls - CurrentSkulls;
                            }
                            else if (CurrentSkulls + SkullsToAdd < 0)
                            {
                                SkullsToAdd = -CurrentSkulls;
                            }
                        
                            WriteObject.SetIntStat(DBDProfileStats::DBD_CAMPER_SKULLS, SkullsToAdd);
                            PersistentData->SavedData.SharedData.CamperSkulls += SkullsToAdd;
                        }

                        // Update local cache
                        UDBDPersistentData::CopyLocalCacheData(PersistentData, DBDPlayerState);

                        // Fire an event to track the awarded skulls
                        GameInstance->GameEventTracker->FireGameflowEvent(EDBDScoreTypes::DBDPlayerScore_AwardSkulls, static_cast<float>(SkullsToAdd), DBDPlayerState);

                        // Set up a boolean to track if all categories maxed out
                        bool bMaxedAllCategories = (UserGameStats->_experiences.Num() == 4);

                        // Iterate over the _experiences map inside UserGameStats
                        for (auto It = UserGameStats->_experiences.CreateConstIterator(); It; ++It)
                        {
                            int32 ExperienceScore = It.Value();
                            
                            // Check if the score is less than the max required for a category (e.g., 5000)
                            if (ExperienceScore < 5000)
                            {
                                bMaxedAllCategories = false;
                            }
                            
                            // Compare the actual score with the tunable max value for the category
                            float MaxCategoryScore = GameInstance->DesignTunables->GetTunableValue(TunableValues::SCORE_CATEGORY_MAX);
                            if (static_cast<float>(ExperienceScore) >= MaxCategoryScore)
                            {
                                // Fire a gameflow event if the category experience is maxed out
                                GameInstance->GameEventTracker->FireGameflowEvent(
                                    EDBDScoreTypes::DBDPlayerScore_ExperienceOneCategory, 
                                    static_cast<float>(ExperienceScore), 
                                    DBDPlayerState
                                );
                            }
                        }

                        if (bMaxedAllCategories == true)
                        {
                            GameInstance->GameEventTracker->FireGameflowEvent(
                                EDBDScoreTypes::DBDPlayerScore_MaxExperienceAllCategories, 
                                1.0f, 
                                DBDPlayerState
                            );
                        }

                        // --- Disconnect Penalties and Rewards calculation logic block ---
                        // Note: The disassembly contains floating-point logic dealing with disconnect tunable values, converting them into FTimespan.
                        float Penalty = GameInstance->DesignTunables->GetTunableValue(TunableValues::DISCONNECT_LEAVE_PENALTY);
                        float Reward = GameInstance->DesignTunables->GetTunableValue(TunableValues::DISCONNECT_FINISH_GAME_REWARD);
                        int32 totalMinutes = static_cast<int32>(Penalty * 2.0f - 0.5f) / 2 + static_cast<int32>(Reward * 2.0f - 0.5f) / 2;
                        FTimespan DisconnectTime = FTimespan::FromMinutes(totalMinutes);
                        
                        // Apply time penalty or clear certain flags/loadout data
                        PersistentData->SavedData.LocalData.LastConnectedLoadout.Clear();
                        PersistentData->SavedData.LocalData.DisconnectPenaltyTime -= DisconnectTime;
                        PersistentData->SavedData.LocalData._ongoingGameTime = FDateTime::MinValue();

                        FString GameHashStr = FString::Printf(TEXT("%llu"), 0ULL);
                        BHVRCrashUtils::SetGameHashValue(GameHashStr);
                        // --------------------------------------------------------------

                        // Final logging (if Verbosity level allows)
                        UE_LOG(GameFlow, Log, TEXT("WriteGameEndStats: Skulls: %i BloodPoints: %i FearTokens: %i"), SkullsToAdd, TotalExp, PersistentData->FearTokens);

                        // Send replicated skulls count to server
                        this->Server_UpdateReplicatedSkulls(SkullsToAdd);

                        // Write profile stats locally and to the backend leaderboard system
                        this->LogProfileStatWrite(WriteObject);
                        this->WritePlayerStats(WriteObject);

                        // Flag that the stats need to be uploaded
                        this->_uploadStatsNeeded = true;

                        // Force save full player profile via backend
                        GameInstance->_playerDataFacade->SaveFullProfile(false);
                    }
                }
            }
        }
    }
}

// starts at line 1091
void ADBDPlayerController::Local_EvaluateBadges()
{
    // Ensure this is executing on the locally controlled player
    if (this->IsLocalController() == true)
    {
        // Try to retrieve the local PlayerState
        ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        if (IsValid(DBDPlayerState) == true)
        {
            // In the disassembly, it retrieves an object at an offset (e.g., offset 0x790 in pseudo-C)
            // This corresponds to the UDBDBadgeEvaluatorComponent attached to the PlayerState
            UDBDBadgeEvaluatorComponent* BadgeEvaluator = DBDPlayerState->BadgeEvaluator;

            if (IsValid(BadgeEvaluator) == true)
            {
                // Tell the component to evaluate all badges based on the match's events
                BadgeEvaluator->EvaluateAllBadges();

                // Retrieve the results of the evaluation
                UDBDBadgeRewardData BadgeRewardData;
                BadgeEvaluator->Local_GetRewardedBadges(BadgeRewardData);

                // Build a debug string concatenating all awarded badges
                FString AllBadgesDebugStr = TEXT("All Badges: ");

                if (BadgeRewardData.AllRewardedBadges.Num() <= 0)
                {
                    AllBadgesDebugStr += TEXT("None");
                }
                else
                {
                    for (int32 i = 0; i < BadgeRewardData.AllRewardedBadges.Num(); ++i)
                    {
                        // Assuming AllRewardedBadges is an array of UDBDPlayerBadge* or similar structs
                        // and Name is an FText property
                        FString BadgeName = BadgeRewardData.AllRewardedBadges[i]->Name.ToString();

                        AllBadgesDebugStr += BadgeName;

                        if (i != BadgeRewardData.AllRewardedBadges.Num() - 1)
                        {
                            AllBadgesDebugStr += TEXT(", ");
                        }
                    }
                }

                // Determine the secondary badge name and pips
                FString SecondaryBadgeName = TEXT("None");
                int32 SecondaryPips = 0;

                if (BadgeRewardData.SecondaryBadge != nullptr)
                {
                    SecondaryBadgeName = BadgeRewardData.SecondaryBadge->Name.ToString();
                    SecondaryPips = BadgeRewardData.SecondaryBadge->PipsToAward;
                }

                // Build the debug string for the secondary badge
                FString SecondaryBadgeDebugStr = FString::Printf(TEXT("Secondary Badge (%d Pips): %s"), SecondaryPips, *SecondaryBadgeName);

                // Determine the primary badge name and pips
                FString PrimaryBadgeName = TEXT("None");
                int32 PrimaryPips = 0;

                if (BadgeRewardData.PrimaryBadge != nullptr)
                {
                    PrimaryBadgeName = BadgeRewardData.PrimaryBadge->Name.ToString();
                    PrimaryPips = BadgeRewardData.PrimaryBadge->PipsToAward;
                }

                // Build the comprehensive debug string
                FString FullDebugStr = FString::Printf(TEXT("[Badge Pips] Primary:%d, Secondary:%d, Total:%d"), 
                    PrimaryPips, SecondaryPips, PrimaryPips + SecondaryPips);

                // Record the awarded badges to analytics
                TArray<FAnalyticsEventAttribute> AnalyticsAttributes;

                UBHVRAnalytics::AddAttributes(AnalyticsAttributes, TEXT("PrimaryPips_i"), PrimaryPips);
                UBHVRAnalytics::AddAttributes(AnalyticsAttributes, TEXT("SecondaryPips_i"), SecondaryPips);
                UBHVRAnalytics::AddAttributes(AnalyticsAttributes, TEXT("TotalPips_i"), PrimaryPips + SecondaryPips);

                UDBDAnalytics::RecordClientProgress(TEXT("BadgePipsAwarded"), AnalyticsAttributes);

                // Note: The pseudo-C contains AddOnScreenDebugMessage calls
                // but since they are not strictly required for the core functionality, they are omitted here.
            }
        }
    }
}

// starts at line 1146
void ADBDPlayerController::SetYawScale(float yawScale, float adjustTime)
{
    // Reset the internal timer used for scaling yaw input, using the provided adjust time
    this->_yawInputScalingTimer.Reset(adjustTime);
    
    // Store the current scales before modifying them, to interpolate from the correct starting point
    this->_previousConstantYawScale = this->_constantYawScale;
    this->_previousAbsoluteYawScale = this->_absoluteYawScale;
    
    // Disable any active reset process since a new scale is being set
    this->_resettingYawScaling = false;
    
    // Set the new target for the input scaling
    this->_targetYawInputScaling = yawScale;
    
    // If the adjust time is zero or negative, apply the scaling immediately (force update)
    if (adjustTime <= 0.0f)
    {
        // Call UpdateAxisScaling with a DeltaTime of 0.0f and bForce set to true
        this->UpdateAxisScaling(0.0f, true);
    }
}

// starts at line 1177
float ADBDPlayerController::GetOriginalYawScale(bool isUsingGamepad) const
{
    // Ensure we have a valid GameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (IsValid(GameInstance) == false)
    {
        return 0.0f;
    }

    // Try to determine if the currently controlled player is a Slasher
    ADBDPlayer* ControlledPlayer = this->GetControlledPlayer();
    ASlasherPlayer* SlasherPlayer = Cast<ASlasherPlayer>(ControlledPlayer);
    
    bool bIsSlasher = false;
    // The pseudo-C calls IsValidImpl through a vtable call (offset 0x1070 in assembly)
    if (IsValid(SlasherPlayer) == true && SlasherPlayer->IsValidImpl())
    {
        bIsSlasher = true;
    }

    float BaseScale = 0.0f;
    float MinTunable = 0.0f;
    float MaxTunable = 0.0f;
    
    // The DesignTunables object is extracted from the GameInstance (offset 0xF0)
    UDBDDesignTunables* DesignTunables = GameInstance->DesignTunables;

    if (bIsSlasher == true)
    {
        // Slasher logic
        if (isUsingGamepad == true)
        {
            BaseScale = this->_slasherGamepadScale; // Offset 0x8B4
            MinTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_YAW_SCALE_MIN_GAMEPAD);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_YAW_SCALE_MAX_GAMEPAD);
        }
        else
        {
            BaseScale = this->_slasherMouseScale; // Offset 0x8B8
            MinTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_YAW_SCALE_MIN_MOUSE_5);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_YAW_SCALE_MAX_MOUSE);
        }
    }
    else
    {
        // Camper/Survivor logic (fallback)
        if (isUsingGamepad == true)
        {
            BaseScale = this->_camperGamepadScale; // Offset 0x8BC
            MinTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_YAW_SCALE_MIN_GAMEPAD);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_YAW_SCALE_MAX_GAMEPAD);
        }
        else
        {
            BaseScale = this->_camperMouseScale; // Offset 0x8C0
            MinTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_YAW_SCALE_MIN_MOUSE);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_YAW_SCALE_MAX_MOUSE);
        }
    }

    // Clamp the base scale to [0.0, 1.0]
    float ClampedScale = FMath::Clamp(BaseScale, 0.0f, 1.0f);

    // Lerp between the minimum and maximum tunable values using the clamped scale
    // This is mathematically: Min + ClampedScale * (Max - Min)
    return MinTunable + (MaxTunable - MinTunable) * ClampedScale;
}

// starts at line 1206
void ADBDPlayerController::SetPitchScale(float PitchScale, float adjustTime)
{
    // Reset the internal timer used for scaling pitch input with the given adjustment time
    this->_pitchInputScalingTimer.Reset(adjustTime);

    // Save the current scale values before modifying them, to interpolate from the correct starting point
    this->_previousConstantPitchScale = this->_constantPitchScale;
    this->_previousAbsolutePitchScale = this->_absolutePitchScale;

    // We are setting a new target scale, so disable the resetting flag
    this->_resettingPitchScaling = false;

    // Set the target scaling value for the pitch input
    this->_targetPitchInputScaling = PitchScale;

    // If the adjust time is zero or negative, apply the scaling immediately without interpolation
    if (adjustTime <= 0.0f)
    {
        // Call UpdateAxisScaling with 0 DeltaTime and force flag set to true
        this->UpdateAxisScaling(0.0f, true);
    }
}

// starts at line 1237
float ADBDPlayerController::GetOriginalPitchScale(bool isUsingGamepad) const
{
    // Retrieve the GameInstance and ensure it is valid
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (IsValid(GameInstance) == false)
    {
        return 0.0f;
    }

    // Determine if the currently controlled player is a Slasher (Killer)
    ADBDPlayer* ControlledPlayer = this->GetControlledPlayer();
    ASlasherPlayer* SlasherPlayer = Cast<ASlasherPlayer>(ControlledPlayer);
    
    bool bIsSlasher = false;
    if (IsValid(SlasherPlayer) == true && SlasherPlayer->IsValidImpl())
    {
        bIsSlasher = true;
    }

    float BaseScale = 0.0f;
    float MinTunable = 0.0f;
    float MaxTunable = 0.0f;
    
    // Extract DesignTunables from the GameInstance
    UDBDDesignTunables* DesignTunables = GameInstance->DesignTunables;

    if (bIsSlasher == true)
    {
        // Get the Slasher-specific base scale and tunable values
        if (isUsingGamepad == true)
        {
            BaseScale = this->_slasherGamepadScale; // Offset 0x8B4
            MinTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_PITCH_SCALE_MIN_GAMEPAD);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_PITCH_SCALE_MAX_GAMEPAD);
        }
        else
        {
            BaseScale = this->_slasherMouseScale; // Offset 0x8B8
            MinTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_PITCH_SCALE_MIN_MOUSE);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::SLASHER_PITCH_SCALE_MAX_MOUSE);
        }
    }
    else
    {
        // Get the Camper-specific (or fallback) base scale and tunable values
        if (isUsingGamepad == true)
        {
            BaseScale = this->_camperGamepadScale; // Offset 0x8BC
            MinTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_PITCH_SCALE_MIN_GAMEPAD);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_PITCH_SCALE_MAX_GAMEPAD);
        }
        else
        {
            BaseScale = this->_camperMouseScale; // Offset 0x8C0
            MinTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_PITCH_SCALE_MIN_MOUSE);
            MaxTunable = DesignTunables->GetTunableValue(TunableValues::CAMPER_PITCH_SCALE_MAX_MOUSE);
        }
    }

    // Clamp the base scale to ensure it remains between 0.0 and 1.0
    float ClampedScale = FMath::Clamp(BaseScale, 0.0f, 1.0f);

    // Calculate the final interpolated pitch scale value based on the min/max bounds
    // Formula: Min + (Max - Min) * Scale
    return MinTunable + (MaxTunable - MinTunable) * ClampedScale;
}

// starts at line 1267
void ADBDPlayerController::DBDAddPitchInput(float Val, bool constant)
{
    float pitchScale = 0.0f;

    // Determine which scale to use based on the 'constant' flag
    if (constant)
    {
        pitchScale = this->_constantPitchScale;
    }
    else
    {
        pitchScale = this->_absolutePitchScale;
    }

    float finalInput = 0.0f;

    // Check if looking input should be ignored
    if (this->IsLookInputIgnored() == false)
    {
        // Apply the scale to the raw input value
        finalInput = pitchScale * Val;
    }

    // Accumulate the final input into the RotationInput.Pitch component
    this->RotationInput.Pitch += finalInput;
}

// starts at line 1273
void ADBDPlayerController::DBDAddYawInput(float Val, bool constant)
{
    float yawScale = 0.0f;

    // Determine which scale to use based on the 'constant' flag
    if (constant)
    {
        yawScale = this->_constantYawScale;
    }
    else
    {
        yawScale = this->_absoluteYawScale;
    }

    float finalInput = 0.0f;

    // Check if looking input should be ignored
    if (!this->IsLookInputIgnored())
    {
        // Apply the scale to the raw input value
        finalInput = yawScale * Val;
    }

    // Accumulate the final input into the RotationInput.Yaw component
    this->RotationInput.Yaw += finalInput;
}
