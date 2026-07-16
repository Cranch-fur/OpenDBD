// starts at line 34
UCamperStillnessTrackerComponent::UCamperStillnessTrackerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Reset the trigger boolean flag
    this->_hasTriggered = false;
}

// starts at line 39
void UCamperStillnessTrackerComponent::BeginPlay()
{
    // Call the parent class implementation of BeginPlay
    Super::BeginPlay();

    // Initialize the Exposer component
    this->InitExposer();

    // The extensive string manipulation (wcsstr) and __Internal_AddDynamic calls 
    // are the standard inline expansion of Unreal Engine's AddDynamic macro.
    // The disassembly shows two delegate bindings to a component located at offset 0x3F0 within the tracked player.
    
    // Retrieve the tracked player object from the weak pointer
    ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();

    if (TrackedPlayer != nullptr)
    {
        // Offset 0x3F0 is likely a component within ADBDPlayer, such as a CapsuleComponent or a custom ZoneComponent
        UPrimitiveComponent* TrackedComponent = TrackedPlayer->CapsuleComponent;

        if (TrackedComponent != nullptr)
        {
            // Bind to the OnComponentBeginOverlap delegate (offset 0x4E0 within the component)
            TrackedComponent->OnComponentBeginOverlap.AddDynamic(this, &UCamperStillnessTrackerComponent::OnOverlapEnter);

            // Bind to the OnComponentEndOverlap delegate (offset 0x4F0 within the component)
            TrackedComponent->OnComponentEndOverlap.AddDynamic(this, &UCamperStillnessTrackerComponent::OnOverlapExit);
        }
    }
}

// starts at line 50
void UCamperStillnessTrackerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Check if the weak pointer to the tracked player is still valid
    if (this->_trackedPlayer.IsValid() == true)
    {
        // Get the tracked player object
        ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();

        if (TrackedPlayer != nullptr)
        {
            // Offset 0x3F0 corresponds to the same component bound in BeginPlay
            UPrimitiveComponent* TrackedComponent = TrackedPlayer->CapsuleComponent;

            if (TrackedComponent != nullptr)
            {
                // The disassembly shows standard manual removal of dynamic delegates using RemoveInternal and CompactInvocationList
                // This corresponds to Unreal Engine's RemoveDynamic macro
                
                // Remove the OnOverlapEnter delegate binding (offset 0x4E0)
                TrackedComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UCamperStillnessTrackerComponent::OnOverlapEnter);

                // Remove the OnOverlapExit delegate binding (offset 0x4F0)
                TrackedComponent->OnComponentEndOverlap.RemoveDynamic(this, &UCamperStillnessTrackerComponent::OnOverlapExit);
            }
        }
    }

    // Iterate through the _exposerInstances array and call StartExitSequence on each valid instance
    for (int32 i = 0; i < this->_exposerInstances.Num(); i++)
    {
        ACamperExposerInstance* ExposerInstance = this->_exposerInstances[i];
        if (ExposerInstance != nullptr)
        {
            // Call the function at offset 0x3E0 (based on standard AActor/component layout, though not explicit in VTable here, it's called directly in disassembly)
            ExposerInstance->StartExitSequence();
        }
    }

    // Clear the _exposerInstances array and free its allocated memory
    // The disassembly explicitly sets ArrayNum to 0 and checks ArrayMax before calling ResizeTo(0), which is equivalent to Empty()
    this->_exposerInstances.Empty();

    // Call the parent class implementation of EndPlay
    Super::EndPlay(EndPlayReason);
}

// starts at line 68
void UCamperStillnessTrackerComponent::InitExposer()
{
    // Retrieve the World from the component's private field
    UWorld* World = this->WorldPrivate;

    // If the cached World is null, retrieve it using the uncached method
    if (World == nullptr)
    {
        World = this->GetWorld_Uncached();
    }

    // Ensure the World is valid (incorporates standard Unreal Engine IsValid macro checks, 
    // including checking GUObjectArray flags for Unreachable/PendingKill)
    if (IsValid(World) == true)
    {
        // Retrieve the GameState (offset 0x58 in UWorld) and cast it to ADBDGameState
        ADBDGameState* GameState = Cast<ADBDGameState>(World->GameState);

        // Ensure the GameState is valid (incorporates standard Unreal Engine IsValid macro checks, 
        // including checking Actor's bIsPendingKill flag and GUObjectArray flags)
        if (IsValid(GameState) == true)
        {
            // The disassembly accesses a field at offset 0x5F0 within ADBDGameState
            // Based on standard Dead by Daylight codebase structure, this is highly likely an AAsymmetricGameMode or a similar core manager
            // We will use a generic AActor* representation as the exact type is not provided in structures
            ASlasherPlayer* Slasher = GameState->Slasher;

            // Ensure the Slasher is valid (incorporates standard Unreal Engine IsValid macro checks)
            if (IsValid(Slasher) == true && Slasher->IsValidImpl() == true)
            {
                // If the condition is met, retrieve a TSubclassOf from the Slasher at offset 0x1308
                // and assign it to the component's _exposerInstanceToSpawn field (offset 0x158)
                this->_exposerInstanceToSpawn = Slasher->CamperExposer;
            }
        }
    }
}

// starts at line 99
void UCamperStillnessTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // Store the old stillness timer value before the parent tick updates it
    float OldStillnessTimer = this->_stillnessTimer;

    // Call the parent class (UStillnessTrackerComponent) implementation of TickComponent
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Retrieve the tracked player and check if they are incapacitated
    ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();
    if (TrackedPlayer != nullptr && TrackedPlayer->IsIncapacitated() == true)
    {
        // If incapacitated, disable the component's tick
        this->SetComponentTickEnabled(false);
    }

    // Check if the stillness counter should increase based on current stillness threshold
    if (this->ShouldIncreaseCounter() == true)
    {
        // If the console variable enabling Exposer spawning is true (not 0)
        // Note: The CVar is accessed directly via a global reference, which is standard UE CVar handling
        static IConsoleVariable* CVarSpawnExposerEnabled = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.SpawnExposerEnabled"));
        if (CVarSpawnExposerEnabled != nullptr && CVarSpawnExposerEnabled->GetInt() != 0)
        {
            // Check if it's time to spawn a new Exposer instance
            this->CheckShouldSpawnNewExposer(OldStillnessTimer);
        }
    }
    else
    {
        // The actor is moving (counter is decreasing or reset)
        if (TrackedPlayer != nullptr)
        {
            // The disassembly includes standard IsValid checks and an IsA check for ACamperPlayer
            ACamperPlayer* CamperPlayer = Cast<ACamperPlayer>(TrackedPlayer);
            if (IsValid(CamperPlayer) == true && CamperPlayer->IsValidImpl() == true)
            {
                if (this->IsBeingActedUpon(CamperPlayer) == true)
                {
                    // If the camper is being acted upon (e.g., healed by another), reset the stillness timer immediately
                    this->_stillnessTimer = 0.0f;
                }
            }
        }

        // Check if existing Exposer instances should be destroyed due to movement
        this->CheckShouldDestroyExposer(OldStillnessTimer);
    }

    // Update the intensity/progress of all active Exposer instances
    for (int32 i = 0; i < this->_exposerInstances.Num(); i++)
    {
        ACamperExposerInstance* ExposerInstance = this->_exposerInstances[i];
        if (ExposerInstance != nullptr)
        {
            // Calculate the normalized progress (0.0 to 1.0) based on max spawn time
            float MaxSpawnTime = this->GetMaxExposerSpawnTime();
            float Progress = this->_stillnessTimer / MaxSpawnTime;

            // Set the progress value on the Exposer instance (offset 0x384 in the disassembly)
            ExposerInstance->AggravationLevel = Progress;
        }
    }
}

// starts at line 144
void UCamperStillnessTrackerComponent::SpawnExposer()
{
    // Verify if the assigned exposer class to spawn is valid
    if (this->_exposerInstanceToSpawn.Get() == nullptr)
    {
        // If it is not valid, attempt to initialize the exposer instance class
        this->InitExposer();

        // Check the validity again after the initialization attempt
        if (this->_exposerInstanceToSpawn.Get() == nullptr)
        {
            // Log a warning indicating the failure to find a valid exposer type
            UE_LOG(LogDBDGeneral, Warning, TEXT("Trying to spawn a camper exposer of undefined type. Check Slasher BPs for valid CamperExposers."));
            return;
        }
    }

    // Prepare the spawn parameters for the new actor
    FActorSpawnParameters SpawnInfo;

    // Apply the RF_Transient flag (0x40) to prevent the spawned exposer from being saved
    SpawnInfo.ObjectFlags = SpawnInfo.ObjectFlags | RF_Transient;

    // Check if the component is currently within a valid interior zone
    if (this->_interiorZone != nullptr)
    {
        // Store the current number of exposer instances
        int32 currentInstancesCount = this->_exposerInstances.Num();

        // Verify if there are still available spawn points in the interior zone
        if (currentInstancesCount >= this->_interiorZone->ExposerSpawnPoints.Num() || 
            this->_interiorZone->ExposerSpawnPoints[currentInstancesCount] == nullptr)
        {
            // Log a warning if no valid spawn point is found for the current exposer index
            UE_LOG(LogDBDGeneral, Warning, TEXT("Failed spawning interior exposer on interior zone: %s"), *this->_interiorZone->GetName());
        }
        else
        {
            // Retrieve the designated spawn point for this specific exposer index
            USceneComponent* spawnPoint = this->_interiorZone->ExposerSpawnPoints[currentInstancesCount];

            // Retrieve the current game world
            UWorld* world = this->GetWorld();

            if (world != nullptr)
            {
                // Spawn the exposer instance at the world origin initially
                ACamperExposerInstance* spawnedExposer = world->SpawnActor<ACamperExposerInstance>(
                    this->_exposerInstanceToSpawn.Get(),
                    FVector::ZeroVector,
                    FRotator::ZeroRotator,
                    SpawnInfo
                );

                if (spawnedExposer != nullptr)
                {
                    // Mark the spawned exposer as an interior instance
                    spawnedExposer->SetIsInterior(true);

                    // Attach the exposer's root component to the interior zone's designated spawn point
                    spawnedExposer->GetRootComponent()->AttachToComponent(spawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);

                    // Initiate the starting logic for the newly spawned exposer
                    spawnedExposer->StartSpawnSequence();

                    // Register the spawned instance in the component's array
                    this->_exposerInstances.Add(spawnedExposer);
                }
            }
        }
    }
    else
    {
        // Retrieve the player that is currently being tracked
        ADBDPlayer* trackedPlayer = this->_trackedPlayer.Get();

        if (trackedPlayer != nullptr)
        {
            // Calculate the dynamic spawn location based on the tracked player's collision bounds
            FVector calculatedSpawnLocation = FVector::ZeroVector;

            // Retrieve the capsule component of the tracked player (Offset 0x3F0)
            UCapsuleComponent* capsuleComponent = trackedPlayer->CapsuleComponent;
            if (capsuleComponent != nullptr)
            {
                // Retrieve the ComponentToWorld transform and extract its 3D scale
                // ComponentToWorld is at 0x130, and Scale3D is at offset 0x20 within FTransform (Total offset 0x150)
                FVector scale3D = capsuleComponent->GetComponentTransform().GetScale3D();
                
                // Get the absolute scale values (corresponds to bitwise AND with SignMask_6)
                FVector absScale = scale3D.GetAbs();
                
                // Find the minimum scale component among X, Y, and Z (corresponds to shufps and minps)
                float minScale = absScale.GetMin();

                // Fetch the float value at offset 0x630, which corresponds to the unscaled CapsuleHalfHeight
                float capsuleHalfHeight = capsuleComponent->CapsuleHalfHeight;

                // Calculate the relative spawn location at the base of the capsule
                // The X and Y axes remain 0.0, and the Z axis is inverted (corresponds to xorps with 0x80000000)
                calculatedSpawnLocation = FVector(0.0f, 0.0f, -(minScale * capsuleHalfHeight));
            }

            // Retrieve the current game world
            UWorld* world = this->GetWorld();

            if (world != nullptr)
            {
                // Spawn the exposer instance at the calculated exterior location
                ACamperExposerInstance* spawnedExposer = world->SpawnActor<ACamperExposerInstance>(
                    this->_exposerInstanceToSpawn.Get(),
                    calculatedSpawnLocation,
                    FRotator::ZeroRotator,
                    SpawnInfo
                );

                if (spawnedExposer != nullptr)
                {
                    // Mark the spawned exposer as an exterior instance
                    spawnedExposer->SetIsInterior(false);

                    // Attach the exposer's root component to the tracked player to maintain relative positioning
                    spawnedExposer->GetRootComponent()->AttachToComponent(trackedPlayer->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

                    // Initiate the starting logic for the newly spawned exposer
                    spawnedExposer->StartSpawnSequence();

                    // Register the spawned instance in the component's array
                    this->_exposerInstances.Add(spawnedExposer);
                }
            }
        }
    }

    // Re-acquire the tracked player to send the game event update
    ADBDPlayer* eventTrackedPlayer = this->_trackedPlayer.Get();

    if (eventTrackedPlayer != nullptr)
    {
        // Retrieve the current game world from the tracked player
        UWorld* playerWorld = eventTrackedPlayer->GetWorld();

        if (playerWorld != nullptr)
        {
            // Retrieve the Game Instance and cast it to the specific Dead By Daylight game instance class
            UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(playerWorld->GetGameInstance());

            if (gameInstance != nullptr)
            {
                // Retrieve the game event tracker subsystem from the game instance
                UGameEventTracker* eventTracker = gameInstance->GameEventTracker;

                if (eventTracker != nullptr)
                {
                    // Fire the event notifying the system that an exposer has been added, passing the current count
                    eventTracker->FireGameEvent(EDBDScoreTypes::DBDCamperScore_ExposerAdded, static_cast<float>(this->_exposerInstances.Num()), eventTrackedPlayer, eventTrackedPlayer);
                }
            }
        }
    }
}

// starts at line 200
void UCamperStillnessTrackerComponent::CheckShouldSpawnNewExposer(float oldStillness)
{
    // Retrieve the total number of configured spawn times
    int32 spawnTimesCount = this->ExposerSpawnTimes.Num();

    // Proceed only if there are spawn times defined in the array
    if (spawnTimesCount > 0)
    {
        // Get the current number of active exposer instances
        int32 currentInstancesCount = this->_exposerInstances.Num();

        // Check if we can still spawn more exposers
        if (currentInstancesCount < spawnTimesCount)
        {
            // Iterate through the remaining spawn thresholds
            for (int32 i = currentInstancesCount; i < spawnTimesCount; i++)
            {
                float targetSpawnTime = 0.0f;

                // Determine the active modifier to apply to the spawn time
                float activeModifier = 0.0f;
                if (this->_hasTriggered == true)
                {
                    activeModifier = this->ChickenShitModifier;
                }

                // Calculate the exact time required to trigger this specific exposer
                targetSpawnTime = this->ExposerSpawnTimes[i] - activeModifier;

                // Verify if the previous stillness state was strictly below this threshold
                if (oldStillness < targetSpawnTime)
                {
                    // Check if the current accumulated stillness has reached or surpassed the threshold
                    if (targetSpawnTime <= this->_stillnessTimer)
                    {
                        // Spawn the new exposer instance
                        this->SpawnExposer();

                        // Check if this was the very last exposer threshold in the array
                        if (this->ExposerSpawnTimes.Last() == this->ExposerSpawnTimes[i])
                        {
                            // Mark the component as having fully triggered its exposer sequence
                            this->_hasTriggered = true;

                            // Retrieve the current game world to access the Timer Manager
                            UWorld* world = this->GetWorld();
                            if (world != nullptr)
                            {
                                // Setup the repeating timer for the loud noise event
                                // This replaces the heavily expanded delegate allocation found in the disassembly
                                world->GetTimerManager().SetTimer(
                                    this->_exposerLoudNoiseTimer,
                                    this,
                                    &UCamperStillnessTrackerComponent::TickExposerLoudNoise,
                                    this->LoudNoiseTime,
                                    true,  // bLoop
                                    0.0f   // FirstDelay
                                );
                            }
                        }
                    }
                    
                    // Break out of the loop as we have found and processed the relevant upcoming threshold
                    break;
                }
            }
        }
    }
    else
    {
        // Log a warning if the tracking component has no spawn times configured
        // Replaced the expanded FMsg::Logf_Internal__VA call with the standard Unreal Engine macro
        UE_LOG(LogDBDGeneral, Warning, TEXT("No exposer spawn times loaded."));
    }
}

// starts at line 234
void UCamperStillnessTrackerComponent::CheckShouldDestroyExposer(float oldStillness)
{
    // Verify that there are configured despawn times before proceeding
    if (this->ExposerDespawnTimes.Num() > 0)
    {
        // If the stillness timer has completely reset to zero, but was previously active
        if (this->_stillnessTimer <= 0.0f && oldStillness > 0.0f)
        {
            // Iterate through all currently active exposer instances
            for (int32 i = 0; i < this->_exposerInstances.Num(); i = i + 1)
            {
                ACamperExposerInstance* exposerInstance = this->_exposerInstances[i];
                
                // Ensure the instance pointer is valid before interacting with it
                if (exposerInstance != nullptr)
                {
                    // Trigger the exit sequence for the exposer
                    exposerInstance->StartExitSequence();
                }
            }

            // Empty the array to remove all tracked instances
            this->_exposerInstances.Empty();
        }

        int32 allowedExposerCount = 0;
        float currentDespawnThreshold = 0.0f;

        // Iterate through the despawn times array to find the applicable threshold
        for (int32 i = 0; i < this->ExposerDespawnTimes.Num(); i = i + 1)
        {
            // The first element does not use the modifier, subsequent elements do
            if (i > 0)
            {
                float activeModifier = 0.0f;
                
                // Check if the component has triggered to apply the modifier
                if (this->_hasTriggered == true)
                {
                    activeModifier = this->ChickenShitModifier;
                }
                
                // Calculate the modified threshold for despawning
                currentDespawnThreshold = this->ExposerDespawnTimes[i] - activeModifier;
            }
            else
            {
                currentDespawnThreshold = 0.0f;
            }

            // If the current stillness is below the required threshold, break the loop
            if (this->_stillnessTimer < currentDespawnThreshold)
            {
                break;
            }

            // Increment the allowed number of exposer instances based on the current threshold
            allowedExposerCount = allowedExposerCount + 1;
        }

        // Verify if the previous stillness was above the threshold, but we now have too many instances
        if (oldStillness >= currentDespawnThreshold && this->_exposerInstances.Num() > allowedExposerCount)
        {
            // Continuously remove instances from the end of the array until the count is valid
            while (this->_exposerInstances.Num() > allowedExposerCount)
            {
                // Pop removes the last element from the array and returns it
                ACamperExposerInstance* instanceToDestroy = this->_exposerInstances.Pop();
                
                if (instanceToDestroy != nullptr)
                {
                    // Trigger the exit sequence for the removed exposer
                    instanceToDestroy->StartExitSequence();
                }
            }
        }
    }
    else
    {
        // Log a warning if no despawn times have been loaded into the component
        UE_LOG(LogDBDGeneral, Warning, TEXT("No exposer despawn times loaded."));
    }
}

// starts at line 275
void UCamperStillnessTrackerComponent::RespawnExposersOnInteriorChanged()
{
    // Iterate through all currently active exposer instances and start their exit sequence
    for (int32 i = 0; i < this->_exposerInstances.Num(); ++i)
    {
        ACamperExposerInstance* ExposerInstance = this->_exposerInstances[i];
        if (ExposerInstance != nullptr)
        {
            // Call the virtual function StartExitSequence to properly despawn the exposer
            ExposerInstance->StartExitSequence();
        }
    }

    // Clear the array of active exposers and shrink the allocation
    // The disassembly explicitly sets ArrayNum to 0 and calls ResizeTo(0) if ArrayMax > 0
    this->_exposerInstances.Empty();

    // Re-evaluate and spawn exposers based on the current stillness timer
    int32 TotalSpawnTimes = this->ExposerSpawnTimes.Num();

    if (TotalSpawnTimes > 0)
    {
        for (int32 i = 0; i < TotalSpawnTimes; ++i)
        {
            float TargetSpawnTime = 0.0f;

            // Apply the ChickenShitModifier if the component has triggered before
            if (this->_hasTriggered == true)
            {
                float Modifier = this->ChickenShitModifier;
                TargetSpawnTime = this->ExposerSpawnTimes[i] - Modifier;
            }
            else
            {
                TargetSpawnTime = this->ExposerSpawnTimes[i];
            }

            // Check if the current stillness timer meets the requirement for this exposer level
            if (this->_stillnessTimer >= TargetSpawnTime)
            {
                // Ensure we don't spawn the final level exposer (the "loud noise" trigger stage) 
                // just because of an interior change, unless it was already going to happen.
                // The disassembly checks if the current spawn time equals the last spawn time in the array.
                float LastSpawnTime = this->ExposerSpawnTimes[TotalSpawnTimes - 1];
                float CurrentTargetTime = this->ExposerSpawnTimes[i];

                if (CurrentTargetTime != LastSpawnTime)
                {
                    // Spawn the exposer for this level
                    this->SpawnExposer();
                }
            }

            // Re-evaluate TotalSpawnTimes in case SpawnExposer modified the array (though unlikely, it matches the disassembly flow)
            TotalSpawnTimes = this->ExposerSpawnTimes.Num();
        }
    }
}

// starts at line 326
float UCamperStillnessTrackerComponent::GetMaxExposerSpawnTime() const
{
    float MaxSpawnTime = 0.0f;

    // Check if there are any spawn times configured in the array
    if (this->ExposerSpawnTimes.Num() > 0)
    {
        // Get the last element in the array, which represents the maximum spawn time
        int32 LastIndex = this->ExposerSpawnTimes.Num() - 1;
        MaxSpawnTime = this->ExposerSpawnTimes[LastIndex];
    }

    float Modifier = 0.0f;

    // If the component has already triggered, apply the ChickenShitModifier
    if (this->_hasTriggered == true) // Using 1 as it's an uint8 acting as a boolean
    {
        Modifier = this->ChickenShitModifier;
    }

    // Subtract the modifier from the max spawn time
    float AdjustedSpawnTime = MaxSpawnTime - Modifier;

    // Ensure the result doesn't fall below 0.0f
    float Result = FMath::Max(AdjustedSpawnTime, 0.0f);

    return Result;
}

// starts at line 333
float UCamperStillnessTrackerComponent::GetStillness() const
{
    // Retrieve the tracked player object from the weak pointer
    ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();

    // Check if the tracked player is a valid ACamperPlayer
    // The disassembly includes standard Unreal Engine IsValid macro checks:
    // null check, ClassTreeIndex checks (IsA), and GUObjectArray flags (Unreachable/PendingKill)
    ACamperPlayer* CamperPlayer = Cast<ACamperPlayer>(TrackedPlayer);

    if (IsValid(CamperPlayer) == false || CamperPlayer->IsValidImpl() == false)
    {
        // If the player is invalid, they cannot be still, return max stillness (1.0f represents full movement/activity)
        return 1.0f;
    }

    // Check if the camper is currently being acted upon by another player (e.g., being healed)
    if (this->IsBeingActedUpon(CamperPlayer) == true)
    {
        // If they are being acted upon, they are not considered still
        return 1.0f;
    }

    // If all checks pass, fall back to the parent class implementation to calculate the actual stillness value
    return Super::GetStillness();
}

// starts at line 344
void UCamperStillnessTrackerComponent::EnterInteriorZone(UExposerInteriorZoneComponent* zone)
{
    // The disassembly includes standard Unreal Engine IsValid checks (checking GUObjectArray flags for Unreachable/PendingKill)
    
    // Check if the component already has a valid interior zone assigned
    if (IsValid(this->_interiorZone) == false)
    {
        // If not, check if the newly provided zone is valid
        if (IsValid(zone) == true)
        {
            // Assign the new zone
            this->_interiorZone = zone;
            
            // Trigger a respawn of exposers to adapt to the new interior environment
            this->RespawnExposersOnInteriorChanged();
        }
    }
}

// starts at line 353
void UCamperStillnessTrackerComponent::ExitInteriorZone(UExposerInteriorZoneComponent* zone)
{
    // Check if the current interior zone is valid
    // The disassembly performs standard Unreal Engine IsValid macro checks, including checking GUObjectArray flags for PendingKill
    if (IsValid(this->_interiorZone) == true)
    {
        // Check if the provided zone parameter is valid
        if (IsValid(zone) == true)
        {
            // Verify that the zone we are exiting is the one we are currently tracking
            if (zone == this->_interiorZone)
            {
                // Clear the current interior zone
                this->_interiorZone = nullptr;
                
                // Trigger a respawn of exposers to adapt to exiting the interior environment
                this->RespawnExposersOnInteriorChanged();
            }
        }
    }
}

// starts at line 362
void UCamperStillnessTrackerComponent::OnOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UExposerInteriorZoneComponent* OtherComp)
{
    // The disassembly checks if OtherComp is valid and if it's a UExposerInteriorZoneComponent
    // This is equivalent to standard Unreal Engine IsValid and Cast/IsA checks
    if (IsValid(OtherComp) == true && OtherComp->IsA(UExposerInteriorZoneComponent::StaticClass()) == true)
    {
        // Check if the component we entered is different from the one we are already tracking
        if (OtherComp != this->_interiorZone)
        {
            // The disassembly includes standard IsValid checks on the current _interiorZone
            // We only want to set a new zone if we aren't currently tracking a valid one
            if (IsValid(this->_interiorZone) == false)
            {
                // The disassembly includes a final check to ensure OtherComp is valid (GUObjectArray flag check)
                if (IsValid(OtherComp) == true)
                {
                    // Set the new interior zone
                    this->_interiorZone = OtherComp;
                    
                    // Trigger a respawn of exposers to adapt to the new interior environment
                    this->RespawnExposersOnInteriorChanged();
                }
            }
        }
    }
}

// starts at line 371
void UCamperStillnessTrackerComponent::OnOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UExposerInteriorZoneComponent* OtherComp)
{
    // The disassembly checks if OtherComp is valid and if it's a UExposerInteriorZoneComponent
    // This translates to standard Unreal Engine IsValid and Cast/IsA checks
    if (IsValid(OtherComp) == true && OtherComp->IsA(UExposerInteriorZoneComponent::StaticClass()) == true)
    {
        // Check if the component currently holds a valid interior zone reference
        // The disassembly includes standard GUObjectArray flags checks for Unreachable/PendingKill
        if (IsValid(this->_interiorZone) == true)
        {
            // Verify that the zone being exited is the same one we are currently tracking
            if (OtherComp == this->_interiorZone)
            {
                // Clear the current interior zone reference
                this->_interiorZone = nullptr;
                
                // Trigger a respawn of exposers to adapt to exiting the interior environment
                this->RespawnExposersOnInteriorChanged();
            }
        }
    }
}

// starts at line 380
void UCamperStillnessTrackerComponent::TickExposerLoudNoise()
{
    // Retrieve the tracked player object from the weak pointer
    ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();

    // Ensure the tracked player is valid before proceeding
    if (TrackedPlayer == nullptr)
    {
        return;
    }

    // Retrieve the GameInstance and ensure it's a UDBDGameInstance
    UGameInstance* GameInstance = TrackedPlayer->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    if (IsValid(DBDGameInstance) == true)
    {
        // Get the number of configured spawn times
        int32 TotalSpawnTimes = this->ExposerSpawnTimes.Num();
        float MaxSpawnTime = 0.0f;

        // Determine the maximum spawn time (the last element in the array)
        if (TotalSpawnTimes > 0)
        {
            int32 LastIndex = TotalSpawnTimes - 1;
            
            // Apply the ChickenShitModifier if triggered
            if (this->_hasTriggered == true)
            {
                MaxSpawnTime = this->ExposerSpawnTimes[LastIndex] - this->ChickenShitModifier;
            }
            else
            {
                MaxSpawnTime = this->ExposerSpawnTimes[LastIndex];
            }
        }

        // Check if the stillness timer has dropped below the threshold or if the player is incapacitated
        if (this->_stillnessTimer < MaxSpawnTime || TrackedPlayer->IsIncapacitated() == true)
        {
            // If the conditions to maintain the loud noise are no longer met, clear the timer
            // The disassembly uses the world's TimerManager
            UWorld* World = this->GetWorld();
            if (World != nullptr)
            {
                World->GetTimerManager().ClearTimer(this->_exposerLoudNoiseTimer);
            }
        }
        else
        {
            // The conditions are still met, proceed with firing the loud noise event

            // Get the GameState and Slasher
            UWorld* World = this->GetWorld();
            if (World != nullptr)
            {
                ADBDGameState* GameState = World->GetGameState<ADBDGameState>();

                // Ensure GameState is valid (incorporates standard IsValid checks)
                if (IsValid(GameState) == true)
                {
                    ASlasherPlayer* Slasher = GameState->Slasher;

                    // Ensure Slasher is valid (incorporates standard IsValid checks including IsValidImpl)
                    if (IsValid(Slasher) == true && Slasher->IsValidImpl() == true)
                    {
                        // Check if there is at least one active exposer instance
                        if (this->_exposerInstances.Num() > 0)
                        {
                            // Get the most recently spawned exposer instance (the last one in the array)
                            int32 LastExposerIndex = this->_exposerInstances.Num() - 1;
                            ACamperExposerInstance* ExposerInstance = this->_exposerInstances[LastExposerIndex];

                            // Ensure the exposer instance is valid (incorporates standard IsValid checks)
                            if (IsValid(ExposerInstance) == true)
                            {
                                // Get the location for the sound event.
                                // The disassembly reads a FVector. Based on context, it's likely the actor's location.
                                FVector SoundLocation = FVector::ZeroVector;
                                
                                // In the disassembly, OuterPrivate is accessed at offset 8, and a vector is extracted.
                                // This typically corresponds to getting the root component's location.
                                USceneComponent* RootComp = TrackedPlayer->GetRootComponent();
                                if (RootComp != nullptr)
                                {
                                    SoundLocation = RootComp->GetComponentLocation();
                                }

                                // Fire the sound event
                                DBDGameInstance->FireSoundEvent(
                                    ExposerInstance, // Target actor
                                    SoundLocation,   // Location
                                    true,            // bShouldTrack
                                    25600.0f,        // Audible range
                                    nullptr,         // Instigating player
                                    false            // bIsQuickAction
                                );
                            }
                        }
                    }
                }
            }
        }
    }
}

// starts at line 399
bool UCamperStillnessTrackerComponent::IsBeingActedUpon(ACamperPlayer* camper) const
{
    // Safely cast the player's data component (located at offset 0xD08) to UDBDCamperData
    UDBDCamperData* CamperData = SafeCast<UDBDCamperData>(camper->_playerData);

    if (CamperData != nullptr)
    {
        // Check the current damage state of the camper
        ECamperDamageState DamageState = CamperData->GetDamageState();
        
        // 3 likely corresponds to a state like 'BeingHealed' or 'Incapacitated' based on context
        if (DamageState == ECamperDamageState::VE_Dead)
        {
            return true;
        }

        // Check the current guided action of the camper (located at offset 0x10A in UDBDCamperData)
        // 10 likely corresponds to a specific interaction state (e.g., being picked up)
        uint8 CurrentGuidedAction = CamperData->CurrentGuidedAction;
        if (CurrentGuidedAction == ECamperGuidedAction::VE_BeingHealed)
        {
            return true;
        }

        // Check another specific guided action state
        // 8 likely corresponds to another interaction state (e.g., being carried)
        if (CurrentGuidedAction == ECamperGuidedAction::VE_BeingCarried)
        {
            return true;
        }
    }

    // Check if the camper is currently hooked
    if (camper->IsHooked() == true)
    {
        return true;
    }

    // If none of the conditions are met, the camper is not being acted upon
    return false;
}
