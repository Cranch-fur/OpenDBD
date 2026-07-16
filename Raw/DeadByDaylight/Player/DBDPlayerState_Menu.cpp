// starts at line 10
ADBDPlayerState_Menu::ADBDPlayerState_Menu(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    // Initialize default class properties
    this->_dirty = false;
    this->_showPawn = false;
    this->_runningDestroySequence = false;
    this->_spawnedCharacterIndex = -1;
    
    // Setup PrimaryActorTick properties based on the bitmask modification (|= 6u)
    this->PrimaryActorTick.bCanEverTick = true;
    this->PrimaryActorTick.bStartWithTickEnabled = true;
}

// starts at line 16
void ADBDPlayerState_Menu::PostInitializeComponents()
{
    this->PostInitializeComponents();
}

// starts at line 21
void ADBDPlayerState_Menu::BeginPlay()
{
    // Call the parent class BeginPlay
    Super::BeginPlay();

    // Retrieve the persistent data for the player
    FPlayerPersistentData* PlayerPersistentData = this->GetPlayerPersistentData();

    // Retrieve the GameInstance and safely cast it to UDBDGameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if both the persistent data and the game instance are valid
    if (PlayerPersistentData != nullptr && IsValid(GameInstance) == true)
    {
        // Check if the selected camper index is uninitialized (less than 0)
        if (this->SelectedCamperIndex < 0)
        {
            // Generate a random camper index and assign it
            int32_t RandomCamperIndex = GameInstance->GetRandomCamperIndex();
            this->SelectedCamperIndex = RandomCamperIndex;
            PlayerPersistentData->SavedData.SharedData.SelectedCamperIndex = RandomCamperIndex;
        }

        // Check if the selected slasher index is uninitialized (less than 0)
        if (this->SelectedSlasherIndex < 0)
        {
            // Generate a random slasher index and assign it
            int32_t RandomSlasherIndex = GameInstance->GetRandomSlasherIndex();
            this->SelectedSlasherIndex = RandomSlasherIndex;
            PlayerPersistentData->SavedData.SharedData.SelectedSlasherIndex = RandomSlasherIndex;
        }

        // 1 corresponds to the Slasher (Killer) role in EPlayerRole 
        if (this->GameRole == EPlayerRole::VE_Slasher)
        {
            // Check if the character description for the selected slasher is missing
            if (GameInstance->DesignTunables->GetCharacterDescription(this->SelectedSlasherIndex) == nullptr)
            {
                // Fallback to a specific slasher index marker (0x10000000)
                this->SelectedSlasherIndex = 0x10000000;
            }
        }

        // 2 corresponds to the Camper (Survivor) role in EPlayerRole
        if (this->GameRole == EPlayerRole::VE_Camper)
        {
            // Check if the character description for the selected camper is missing
            if (GameInstance->DesignTunables->GetCharacterDescription(this->SelectedCamperIndex) == nullptr)
            {
                // Fallback to the default camper index (0)
                this->SelectedCamperIndex = 0;
            }
        }

        // Mark the state as dirty to trigger necessary updates
        this->_dirty = true;
    }
}

// starts at line 64
void ADBDPlayerState_Menu::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    if (this->_dirty)
    {
        this->_dirty = false;
        this->UpdatePawn();
    }
}

// starts at line 76
void ADBDPlayerState_Menu::Destroyed()
{
    // Retrieve the Level Script Actor for the current world
    ALevelScriptActor* LevelScriptActor = this->GetWorld->GetLevelScriptActor(nullptr);
    
    // Attempt to cast the Level Script Actor to ALobbyLevel
    ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(LevelScriptActor);
    
    // Check if the LobbyLevel is valid and is not in the process of being destroyed
    if (IsValid(LobbyLevel) == true && LobbyLevel->IsActorBeingDestroyed() == false)
    {
        // Get the APlayerStart pointer from the TWeakObjectPtr
        APlayerStart* PlayerStart = this->_playerSpawn.Get();
        
        // Unregister the player start from the lobby level
        LobbyLevel->UnregisterPlayerStart(PlayerStart);
        
        // Reset the weak object pointer variables (ObjectIndex = -1, ObjectSerialNumber = 0)
        this->_playerSpawn.Reset();
    }
    
    // Hide the menu pawn
    this->SetShowMenuPawn(false);
    
    // Call the parent class Destroyed method
    Super::Destroyed();
}

// starts at line 89
void ADBDPlayerState_Menu::UpdatePawn()
{
    // Check if the weak pointer to the pawn is currently valid
    if (this->_pawn.IsValid() == true)
    {
        int32_t TargetCharacterIndex = -1;

        // Determine the target character index based on the player's current role
        if (this->GameRole == EPlayerRole::VE_Slasher)
        {
            TargetCharacterIndex = this->SelectedSlasherIndex;
        }
        else if (this->GameRole == EPlayerRole::VE_Camper)
        {
            TargetCharacterIndex = this->SelectedCamperIndex;
        }

        // Check if the currently spawned character index does not match the target index
        // Or check if there is an active pawn destruction sequence (handle is valid / ID != 0)
        if (this->_spawnedCharacterIndex != TargetCharacterIndex || this->_pawnDestroyHandle.IsValid() == true)
        {
            // Call the virtual function to replace the current pawn
            this->ReplacePawn();
        }

        // Check if the pawn destruction handle is invalid (ID == 0), 
        // meaning the pawn is successfully spawned and not queued for destruction
        if (this->_pawnDestroyHandle.IsValid() == false)
        {
            // Update the customization mesh, passing true for isVfxVisible
            this->TryReplaceCustomizationMesh(true);
            
            // Update the pawn's equipped item
            this->TryReplacePawnItem();
        }
    }
}

// starts at line 112
bool ADBDPlayerState_Menu::Server_SetEquipment_Validate(int32, FName, bool)
{
    return true;
}

// starts at line 118
void ADBDPlayerState_Menu::SetShowMenuPawn(bool show)
{
    // Update the visibility flag for the menu pawn
    this->_showPawn = show;

    // Check if the menu pawn should be shown
    if (show == true)
    {
        // Call the virtual function to replace/spawn the pawn
        this->ReplacePawn();
    }
    else
    {
        // If the pawn is not to be shown, check if it's currently valid 
        // and if a destroy sequence isn't already running
        if (this->_pawn.IsValid() == true && this->_runningDestroySequence == false)
        {
            // Set the flag to indicate that a destruction sequence is now running
            this->_runningDestroySequence = true;

            // Check if there's already an active pawn destruction handle (ID != 0)
            if (this->_pawnDestroyHandle.IsValid() == true)
            {
                // Retrieve the pawn object from the weak pointer
                // Using Cast to ADBDMenuPlayer ensures the type matches what the class is designed to handle
                ADBDMenuPlayer* CurrentPawn = this->_pawn.Get();

                // Check if the cast succeeded before attempting to remove the delegate
                if (CurrentPawn != nullptr)
                {
                    // Remove the previously bound delegate from the pawn's OnDestroyed delegate
                    CurrentPawn->OnDestroyedNonDynamic.Remove(this->_pawnDestroyHandle);
                }
            }

            // Retrieve the pawn object from the weak pointer again
            ADBDMenuPlayer* NewPawn = this->_pawn.Get();

            // Check if the cast succeeded before binding the new delegate
            if (NewPawn != nullptr)
            {
                // Bind the FreeSpawnPointOnPawnDestroyComplete function to the pawn's OnDestroyed event
                // and store the returned FDelegateHandle
                this->_pawnDestroyHandle = NewPawn->OnDestroyedNonDynamic.AddUObject(this, &ADBDPlayerState_Menu::FreeSpawnPointOnPawnDestroyComplete);

                // Start the destruction sequence for the pawn
                NewPawn->BeginDestroySequence();
            }
        }
    }
}

// starts at line 135
void ADBDPlayerState_Menu::ReplacePawn()
{
    // The disassembly shows fetching the GameInstance and calling GetLocalPlayerState,
    // but the result is unused. This is likely a macro or optimized-out check.
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    if (GameInstance != nullptr)
    {
        GameInstance->GetLocalPlayerState();
    }

    // Similarly, it fetches the LevelScriptActor and performs a class check for ALobbyLevel
    // but does not store or branch based on the result.
    ALevelScriptActor* LevelScriptActor = this->GetWorld()->GetLevelScriptActor(nullptr);
    if (LevelScriptActor != nullptr)
    {
        Cast<ALobbyLevel>(LevelScriptActor);
    }

    // Check if the current pawn weak pointer is valid
    if (this->_pawn.IsValid() == true)
    {
        int32_t TargetCharacterIndex = -1;

        // Determine the target character index based on the player's current role
        // 1 corresponds to the Slasher (Killer) role in EPlayerRole
        if (this->GameRole == EPlayerRole::VE_Slasher)
        {
            TargetCharacterIndex = this->SelectedSlasherIndex;
        }
        // 2 corresponds to the Camper (Survivor) role in EPlayerRole
        else if (this->GameRole == EPlayerRole::VE_Camper)
        {
            TargetCharacterIndex = this->SelectedCamperIndex;
        }

        // If the target character index matches the currently spawned one
        if (TargetCharacterIndex == this->_spawnedCharacterIndex)
        {
            ADBDMenuPlayer* CurrentPawn = this->_pawn.Get();
            if (CurrentPawn != nullptr)
            {
                CurrentPawn->RestartInSequence();
            }
        }
        else
        {
            // Set the flag to indicate that a destruction sequence is running
            this->_runningDestroySequence = true;

            ADBDMenuPlayer* CurrentPawn = this->_pawn.Get();
            if (CurrentPawn != nullptr)
            {
                // Check if there's already an active pawn destruction handle (ID != 0)
                if (this->_pawnDestroyHandle.IsValid() == true)
                {
                    // Remove the previously bound delegate from the pawn's OnDestroyed event
                    CurrentPawn->OnDestroyedNonDynamic.Remove(this->_pawnDestroyHandle);
                }

                // Bind the CheckSpawnOnPawnDestroyComplete function to the pawn's OnDestroyed event
                this->_pawnDestroyHandle = CurrentPawn->OnDestroyedNonDynamic.AddUObject(this, &ADBDPlayerState_Menu::CheckSpawnOnPawnDestroyComplete);

                // Start the destruction sequence for the pawn
                CurrentPawn->BeginDestroySequence();
            }
        }
    }
    else
    {
        // If there is no valid pawn, spawn a new one
        this->SpawnNewPawn();
    }
}

// starts at line 161
void ADBDPlayerState_Menu::CheckSpawnOnPawnDestroyComplete()
{
    // Free the spawn point associated with the destroyed pawn
    this->FreeSpawnPointOnPawnDestroyComplete();

    int32_t TargetCharacterIndex = -1;

    // Determine the target character index based on the player's current role
    // 1 corresponds to the Slasher (Killer) role in EPlayerRole
    if (this->GameRole == EPlayerRole::VE_Slasher)
    {
        TargetCharacterIndex = this->SelectedSlasherIndex;
    }
    // 2 corresponds to the Camper (Survivor) role in EPlayerRole
    else if (this->GameRole == EPlayerRole::VE_Camper)
    {
        TargetCharacterIndex = this->SelectedCamperIndex;
    }

    // If a valid character index is selected (not -1), proceed to spawn a new pawn
    if (TargetCharacterIndex != -1)
    {
        this->SpawnNewPawn();
    }
}

// starts at line 171
void ADBDPlayerState_Menu::FreeSpawnPointOnPawnDestroyComplete()
{
    // Check if the current pawn weak pointer is valid
    if (this->_pawn.IsValid() == true)
    {
        // Get the actor pointer and call Destroy, forcing it to skip network checking 
        this->_pawn->Destroy(false, true);
    }

    // Mark the destruction sequence as complete
    this->_runningDestroySequence = false;
    
    // Reset the pawn weak object pointer
    this->_pawn.Reset();
    
    // Invalidate the spawned character index
    this->_spawnedCharacterIndex = -1;
    
    // Reset the delegate handle
    this->_pawnDestroyHandle.Reset();

    // Check if the player spawn point is currently valid
    if (this->_playerSpawn.IsValid() == true)
    {
        // Fetch the world and the current Level Script Actor
        ALevelScriptActor* LevelScriptActor = this->GetWorld()->GetLevelScriptActor(nullptr);

        // Cast to ALobbyLevel and verify it's valid and not queued for destruction
        ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(LevelScriptActor);
        
        // Offset 0x140 bit 4 (0x04) in AActor represents bActorIsBeingDestroyed
        if (IsValid(LobbyLevel) == true && LobbyLevel->IsActorBeingDestroyed() == false)
        {
            // Get the Player Start object
            APlayerStart* PlayerStart = this->_playerSpawn.Get();

            // Unregister the player start from the lobby level
            LobbyLevel->UnregisterPlayerStart(PlayerStart);

            // Reset the player spawn weak object pointer
            this->_playerSpawn.Reset();
        }
    }
}

// starts at line 193
void ADBDPlayerState_Menu::SpawnNewPawn()
{
    // Retrieve the game instance and cast it to UDBDGameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Retrieve the world and the current Level Script Actor, casting it to ALobbyLevel
    ALobbyLevel* LobbyLevel = Cast<ALobbyLevel>(this->GetWorld()->GetLevelScriptActor(nullptr));

    // Verify the validity of GameInstance and LobbyLevel, and ensure LobbyLevel is not being destroyed
    if (IsValid(GameInstance) == false || IsValid(LobbyLevel) == false || LobbyLevel->IsActorBeingDestroyed() == true)
    {
        // Mark the state as dirty and abort the spawn process
        this->_dirty = true;
        return;
    }

    // Validate the selected characters before attempting to spawn
    this->ValidateSelectedCharacters();

    int32_t TargetCharacterIndex = -1;

    // Determine the target character index based on the player's role
    // 1 corresponds to the Slasher (Killer) role in EPlayerRole
    if (this->GameRole == EPlayerRole::VE_Slasher)
    {
        TargetCharacterIndex = this->SelectedSlasherIndex;
    }
    else
    {
        TargetCharacterIndex = this->SelectedCamperIndex;
    }

    // Retrieve the character class synchronously based on the target index
    UClass* MenuCharacterClass = GameInstance->GetMenuCharacterSynchronous(TargetCharacterIndex);

    // Retrieve the local player state
    ADBDPlayerState* LocalPlayerState = GameInstance->GetLocalPlayerState();

    // Check if the local player state is invalid, is being destroyed, or is the current player state
    if (IsValid(LocalPlayerState) == false || LocalPlayerState->IsActorBeingDestroyed() == true || LocalPlayerState == this)
    {
        // Register and fetch the local player start for the given role
        this->_playerSpawn = LobbyLevel->GetAndRegisterLocalPlayerStart(this->GameRole);
    }
    else
    {
        // If the current player spawn weak pointer is not valid, find an unused player start
        if (this->_playerSpawn.IsValid() == false)
        {
            this->_playerSpawn = LobbyLevel->GetAndRegisterUnusedPlayerStart(this->GameRole);
        }
    }

    // Verify if a valid player spawn point was obtained
    if (this->_playerSpawn.IsValid() == true)
    {
        FTransform SpawnTransform;
        APlayerStart* PlayerStart = this->_playerSpawn.Get();

        // Extract the transform from the Player Start's Root Component, if available
        if (PlayerStart->GetRootComponent() != nullptr)
        {
            SpawnTransform = PlayerStart->GetRootComponent()->GetComponentTransform();
        }
        else
        {
            SpawnTransform = FTransform::Identity;
        }

        // Spawn the menu player pawn with deferred initialization
        this->_pawn = this->GetWorld()->SpawnActorDeferred<ADBDMenuPlayer>(
            MenuCharacterClass,
            SpawnTransform,
            this->_playerSpawn.Get(),
            nullptr,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

        // Check if the deferred pawn creation was successful
        if (this->_pawn.IsValid() == true)
        {
            // Perform custom initialization on the spawned pawn
            this->InitSpawnedPawn();

            FTransform FinalTransform = SpawnTransform;

            // Strip Pitch and Roll to ensure the pawn only inherits the Yaw rotation
            if (PlayerStart->GetRootComponent() != nullptr)
            {
                FRotator StartRotation = PlayerStart->GetActorRotation();
                StartRotation.Pitch = 0.0f;
                StartRotation.Roll = 0.0f;
                
                FinalTransform.SetRotation(StartRotation.Quaternion());
                FinalTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
            }
            else
            {
                FinalTransform.SetRotation(FQuat::Identity);
                FinalTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
            }

            // Finalize the actor's spawning process
            UGameplayStatics::FinishSpawningActor(this->_pawn.Get(), FinalTransform);

            // Update the customization mesh (false implies isVfxVisible is disabled)
            this->TryReplaceCustomizationMesh(false);
            
            // Replace or update the item held by the pawn
            this->TryReplacePawnItem();

            // Retrieve the pawn to offset its initial animation
            ADBDMenuPlayer* Pawn = this->_pawn.Get();
            if (Pawn != nullptr && Pawn->GetMesh() != nullptr)
            {
                UAnimInstance* AnimInstance = Pawn->GetMesh()->GetAnimInstance();
                if (AnimInstance != nullptr)
                {
                    // Generate a random time offset (FMath::Rand() * (1.0f / 32767.0f) * 10.0f)
                    float RandomDeltaSeconds = (static_cast<float>(FMath::Rand()) * 0.000030518509f) * 10.0f;
                    
                    // Fast-forward the animation sequence to create visual variance
                    AnimInstance->UpdateAnimation(RandomDeltaSeconds, false);
                }
            }
        }
        else
        {
            // Log a warning message if the pawn could not be spawned
            UE_LOG(LogGameFlow, Warning, TEXT("[ADBDPlayerState_Menu::SpawnNewPawn] Invalid pawn spawned: %s"), *MenuCharacterClass->GetName());
        }

        // Capture the slasher POV tracker offset from the lobby level based on the current player start
        this->_slasherPOVTrackerOffset = LobbyLevel->GetSlasherPOVTrackerOffset(this->_playerSpawn);
    }
    else
    {
        // Log a warning message if no valid player start point could be retrieved
        UE_LOG(LogGameFlow, Warning, TEXT("[ADBDPlayerState_Menu::SpawnNewPawn] Trying to spawn menu pawn on invalid player start."));
    }
}

// starts at line 256
void ADBDPlayerState_Menu::InitSpawnedPawn()
{
    // Check if the current pawn weak pointer is valid
    if (this->_pawn.IsValid() == true)
    {
        FName RoleTag;

        // Determine the role tag and target character index based on the player's role
        // 1 corresponds to the Slasher (Killer) role in EPlayerRole
        if (this->GameRole == EPlayerRole::VE_Slasher)
        {
            RoleTag = LobbyTags::SLASHER_PAWN;
            this->_spawnedCharacterIndex = this->SelectedSlasherIndex;
        }
        else
        {
            // 2 corresponds to the Camper (Survivor) role in EPlayerRole
            // We assume it maps to camper here by default
            RoleTag = LobbyTags::CAMPER_PAWN;
            this->_spawnedCharacterIndex = this->SelectedCamperIndex;
        }

        // Retrieve the pawn from the weak pointer
        ADBDMenuPlayer* Pawn = this->_pawn.Get();

        // Append the correct role tag to the pawn's Tags array
        // Offset 0x1B0 refers to the Tags property in AActor
        Pawn->Tags.Add(RoleTag);

        // Set the pawn to not replicate since this is a menu state
        Pawn->SetReplicates(false);

        // Set the pawn's internal character index
        // Offset 0x814 corresponds to a specific property (e.g., _characterIndex) inside ADBDMenuPlayer
        Pawn->CharacterIndex = this->_spawnedCharacterIndex;

        // Set a boolean flag on the pawn
        // Offset 0x810 corresponds to a specific boolean property inside ADBDMenuPlayer
        Pawn->RoleSelected = true;
    }
}

// starts at line 269
void ADBDPlayerState_Menu::TryReplaceCustomizationMesh(bool isVfxVisible)
{
    // Check if the current pawn weak pointer is valid
    if (this->_pawn.IsValid() == true)
    {
        // Extract the array of customization mesh FNames from PlayerData
        // Create a copy of the array (TArray automatically manages resizing and copying elements)
        TArray<FName> CustomizationMeshCopy = this->PlayerData.CustomizationMesh;

        // Call UpdateCustomizationMesh on the pawn, passing the copied array and the VFX visibility flag
        this->_pawn->UpdateCustomizationMesh(CustomizationMeshCopy, isVfxVisible);
    }
}

// starts at line 278
void ADBDPlayerState_Menu::TryReplacePawnItem()
{
    FName ItemToEquip;

    // Determine the item to equip based on the player's current role
    // 2 corresponds to the Camper (Survivor) role in EPlayerRole
    if (this->GameRole == EPlayerRole::VE_Camper)
    {
        // Offset 0x760 represents the item equipped by the Camper
        ItemToEquip = this->CamperData.EquipedItemId;
    }
    else
    {
        // Offset 0x7A0 represents the item equipped by the Slasher
        ItemToEquip = this->SlasherData.EquipedPowerId;
    }

    // Check if the current pawn weak pointer is valid
    if (this->_pawn.IsValid() == true)
    {
        // Verify if the item currently equipped on the pawn differs from the one we want to equip
        // Offset 0x828 in ADBDMenuPlayer corresponds to a property representing the currently equipped item
        if (this->_pawn->_equippedItemName != ItemToEquip)
        {
            // Equip the new item on the pawn
            this->_pawn->EquipItem(ItemToEquip);
        }
    }
}

// starts at line 288
void ADBDPlayerState_Menu::Server_SetEquipment_Implementation(int32_t slotIndex, FName itemId, bool callOnRep)
{
    // Retrieve the GameInstance and try to cast it to UDBDGameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid
    if (IsValid(GameInstance) == true)
    {
        // 0x3B8 represents the UDBDPersistentData pointer
        UDBDPersistentData* PersistentData = GameInstance->_persistentData;

        // Retrieve the PlayerPersistentData based on the player's unique net ID
        FPlayerPersistentData* PlayerPersistentData = PersistentData->GetPlayerPersistentData(this->UniqueId);

        // Verify that the persistent data for the player is found
        if (PlayerPersistentData != nullptr)
        {
            // Update the correct equipment slot depending on the slotIndex
            switch (slotIndex)
            {
                case 1:
                {
                    // 2 corresponds to the Camper (Survivor) role in EPlayerRole
                    if (this->GameRole == EPlayerRole::VE_Camper)
                    {
                        // Set the equipped item for the camper
                        this->CamperData.EquipedItemId = itemId;
                        
                        // 0x88 represents CurrentLoadout.Item
                        PlayerPersistentData->CurrentLoadout.Item = itemId;
                    }
                    else
                    {
                        // Set the equipped power for the slasher
                        this->SlasherData.EquipedPowerId = itemId;
                        
                        // 0xC8 represents CurrentLoadout.Power
                        PlayerPersistentData->CurrentLoadout.Power = itemId;
                    }
                    break;
                }
                case 2:
                {
                    // 2 corresponds to the Camper (Survivor) role in EPlayerRole
                    if (this->GameRole == EPlayerRole::VE_Camper)
                    {
                        // Update the first add-on slot if the array has at least one element
                        if (this->CamperData.EquipedItemAddonIds.Num() > 0)
                        {
                            this->CamperData.EquipedItemAddonIds[0] = itemId;
                        }
                        // 0x98 corresponds to CurrentLoadout.ItemAddOns.ArrayNum
                        if (PlayerPersistentData->CurrentLoadout.ItemAddOns.Num() > 0)
                        {
                            PlayerPersistentData->CurrentLoadout.ItemAddOns[0] = itemId;
                        }
                    }
                    else
                    {
                        // Update the first power add-on slot if the array has at least one element
                        if (this->SlasherData.EquipedPowerAddonIds.Num() > 0)
                        {
                            this->SlasherData.EquipedPowerAddonIds[0] = itemId;
                        }
                        // 0xD8 corresponds to CurrentLoadout.PowerAddOns.ArrayNum
                        if (PlayerPersistentData->CurrentLoadout.PowerAddOns.Num() > 0)
                        {
                            PlayerPersistentData->CurrentLoadout.PowerAddOns[0] = itemId;
                        }
                    }
                    break;
                }
                case 3:
                {
                    // 2 corresponds to the Camper (Survivor) role in EPlayerRole
                    if (this->GameRole == EPlayerRole::VE_Camper)
                    {
                        // Update the second add-on slot if the array has at least two elements
                        if (this->CamperData.EquipedItemAddonIds.Num() > 1)
                        {
                            this->CamperData.EquipedItemAddonIds[1] = itemId;
                        }
                        // 0x98 corresponds to CurrentLoadout.ItemAddOns.ArrayNum
                        if (PlayerPersistentData->CurrentLoadout.ItemAddOns.Num() > 1)
                        {
                            PlayerPersistentData->CurrentLoadout.ItemAddOns[1] = itemId;
                        }
                    }
                    else
                    {
                        // Update the second power add-on slot if the array has at least two elements
                        if (this->SlasherData.EquipedPowerAddonIds.Num() > 1)
                        {
                            this->SlasherData.EquipedPowerAddonIds[1] = itemId;
                        }
                        // 0xD8 corresponds to CurrentLoadout.PowerAddOns.ArrayNum
                        if (PlayerPersistentData->CurrentLoadout.PowerAddOns.Num() > 1)
                        {
                            PlayerPersistentData->CurrentLoadout.PowerAddOns[1] = itemId;
                        }
                    }
                    break;
                }
                case 6:
                {
                    // Update the equipped favor ID
                    this->PlayerData.EquipedFavorId = itemId;

                    // 2 corresponds to the Camper (Survivor) role in EPlayerRole
                    if (this->GameRole == EPlayerRole::VE_Camper)
                    {
                        // 0xC0 corresponds to CurrentLoadout.CamperFavor
                        PlayerPersistentData->CurrentLoadout.CamperFavor = itemId;
                    }
                    else
                    {
                        // 0x100 corresponds to CurrentLoadout.SlasherFavor
                        PlayerPersistentData->CurrentLoadout.SlasherFavor = itemId;
                    }
                    break;
                }
            }

            // If the flag is true, trigger the replication update logic
            if (callOnRep == true)
            {
                this->OnRep_DisplayData();
            }
        }
    }
}

// starts at line 389
void ADBDPlayerState_Menu::ValidateSelectedCharacters()
{
    // Retrieve the game instance and cast to UDBDGameInstance
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the GameInstance is valid
    if (IsValid(GameInstance) == true)
    {
        // Fetch the player's persistent data using their unique network ID
        FPlayerPersistentData* PlayerPersistentData = GameInstance->_persistentData->GetPlayerPersistentData(this->UniqueId);

        // If the selected camper index is uninitialized (less than 0)
        if (this->SelectedCamperIndex < 0)
        {
            // Assign a random camper index
            int32_t RandomCamperIndex = GameInstance->GetRandomCamperIndex();
            this->SelectedCamperIndex = RandomCamperIndex;

            // Also update the persistent data if it exists
            if (PlayerPersistentData != nullptr)
            {
                PlayerPersistentData->SavedData.SharedData.SelectedCamperIndex = RandomCamperIndex;
            }
        }

        // If the selected slasher index is uninitialized (less than 0)
        if (this->SelectedSlasherIndex < 0)
        {
            // Assign a random slasher index
            int32_t RandomSlasherIndex = GameInstance->GetRandomSlasherIndex();
            this->SelectedSlasherIndex = RandomSlasherIndex;

            // Also update the persistent data if it exists
            if (PlayerPersistentData != nullptr)
            {
                PlayerPersistentData->SavedData.SharedData.SelectedSlasherIndex = RandomSlasherIndex;
            }
        }

        // 1 corresponds to the Slasher (Killer) role in EPlayerRole
        if (this->GameRole == EPlayerRole::VE_Slasher)
        {
            // Check if the character description for the selected slasher is valid
            if (GameInstance->DesignTunables->GetCharacterDescription(this->SelectedSlasherIndex) == nullptr)
            {
                // Fallback to a predefined fallback slasher index (0x10000000)
                this->SelectedSlasherIndex = 0x10000000;
            }
        }

        // 2 corresponds to the Camper (Survivor) role in EPlayerRole
        if (this->GameRole == EPlayerRole::VE_Camper)
        {
            // Check if the character description for the selected camper is valid
            if (GameInstance->DesignTunables->GetCharacterDescription(this->SelectedCamperIndex) == nullptr)
            {
                // Fallback to the default camper index (0)
                this->SelectedCamperIndex = 0;
            }
        }
    }
}

// starts at line 429
void ADBDPlayerState_Menu::OnRep_DisplayData()
{
    // Broadcast the delegate to notify listeners that player data has been updated
    this->_playerDataUpdatedDelegate.Broadcast(this);

    // Mark the player state as dirty to trigger any necessary updates
    this->_dirty = true;
}

// starts at line 437
FVector ADBDPlayerState_Menu::GetUITrackerLocation(EPlayerRole rolePOV) const
{
    // Retrieve the pawn from the weak pointer and attempt to cast to ADBDMenuPlayer
    ADBDMenuPlayer* CurrentPawn = this->_pawn.Get();

    // Check if the pawn is valid and its mesh (Root Component/SkeletalMeshComponent) is initialized
    // Offset 0x3E0 corresponds to a property that provides access to the pawn's mesh/components
    if (CurrentPawn != nullptr && CurrentPawn->GetMesh() != nullptr)
    {
        // Retrieve the socket transform of the "UI_Tracker" socket from the pawn's mesh
        // FName("UI_Tracker") is used to specify the socket
        FTransform SocketTransform = CurrentPawn->GetMesh()->GetSocketTransform("UI_Tracker", ERelativeTransformSpace::RTS_World);
        
        // Extract the location (translation) from the socket transform
        FVector TrackerLocation = SocketTransform.GetLocation();

        // 1 corresponds to the Slasher (Killer) role in EPlayerRole
        if (rolePOV == EPlayerRole::VE_Slasher)
        {
            // Apply the slasher POV tracker offset
            TrackerLocation += this->_slasherPOVTrackerOffset;
        }

        return TrackerLocation;
    }

    // Return a zero vector if the pawn or its mesh is invalid
    return FVector::ZeroVector;
}
