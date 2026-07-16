// starts at line 692 in dbdplayer.h
TArray<ACollectable*> ADBDPlayer::GetPlayerInventory() const
{
    // Return a copy of the player's inventory.
    // The assembly performs a direct TArray copy (ResizeForCopy + memcpy).
    // The 'Inventory' member is located at offset 0xC58 based on the assembly (rcx+0C58h).
    return this->Inventory;
}

// starts at line 1482 in dbdplayer.h
float ADBDPlayer::GetMaximumMovementSpeed() const
{
    // Return the MaxWalkSpeed directly from the CharacterMovement component
    // This property controls the maximum speed for ground movement
    return this->CharacterMovement->MaxWalkSpeed;
}

// starts at line 225
ADBDPlayer::ADBDPlayer(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UDBDCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Initialize constant loading time
    this->kLoadingTime = 2.0f;

    // Movement and input flags
    this->MinFallHeight = 50.0f;
    this->AllowNavigationInput = false;
    this->AllowNavigationBackwardInput = true;
    this->PitchLimitLower = -40.0f;
    this->PitchLimitUpper = 40.0f;
    this->PelvisHeight = 1.0f;
    this->PelvisHeightForPounceOnStandingCamper = 0.5f;

    this->CameraAnchor = nullptr;
    this->CameraBoom = nullptr;

    // Initialize IK Transforms to Identity (Identity Matrix/Transform)
    this->IKLeftHandTransform = FTransform::Identity;
    this->IKRightHandTransform = FTransform::Identity;

    this->CameraResetSpeed = 2.25f;

    // Debug and State flags
    this->_smLaunched = false;
    this->_isMontageMode = false;
    this->DebugInteractionPriorityEnabled = false;
    this->CheatIsInterruptable = true;
    this->DebugAutoInteractEnabled = false;
    this->DebugAutoInteractDuration = 0.0f;
    this->DebugAutoInteractPauseDelay = -1.0f;
    this->DebugAutoItemInteractEnabled = false;
    this->DebugAutoItemInteractPressed = false;
    this->DebugAutoItemInteractDuration = 0.0f;
    this->DebugAutoItemInteractPauseDelay = -1.0f;

    // Initialize Yaw Input Decay with specific bit pattern/float array
    // The ASM copies 0x18 bytes of specific float constants
    this->_emulatedYawInputDecay = 0.2f;
    this->_emulatedYawInputDecayWhenNoInput = 7.0f;
    this->_emulatedYawInputReleaseTime = 0.75f;
    this->_emulatedYawInputDeadZone = 0.075f;
    this->_emulatedYawMinimumInput = 0.1f;
    this->_emulatedYawInputStrength = 0.2f;

    this->AverageSpeedBufferTime = 0.75f;
    this->AlmostCurrentSpeedBufferTime = 0.1f;

    // Initialize Carry Joint Name
    this->CarryJointName = ASlasherPlayer::CAMPER_CARRY_SOCKET;

    this->_pitchOffsetForInteractionPriority = 0.0f;
    this->_cameraResetting = false;

    this->ClearPathTestRadiusMultiplier = 0.9f;
    this->ClearPathTestHeightMultiplier = 0.9f;
    this->LocationClearTestRadiusMultiplier = 0.9f;
    this->LocationClearTestHeightMultiplier = 0.9f;

    this->ClearPathTestStepHeight = 20.0f;
    this->MouseTurnSpeedMultiplier = 1.0f;
    this->MouseLookUpSpeedMultiplier = 1.0f;

    this->_isUsingGamepad = false;

    this->_updateCapsuleSize = false;
    this->_interactingPlayer = nullptr;

    this->_movementSpeedMultiplier = 1.0f;

    this->_emulateGamepadForMouseYaw = false;

    this->_strafeOffsetUpdateAlpha = 0.0f;
    this->kStrafeOffsetUpdateDuration = 0.3f;

    this->_isReadyToPlay = false;
    this->_feetAreOnGround = true;
    this->_invertLookUpAxis = false;
    this->_framesForSlasherAutoAttackOnCamperInteract = 0;

    this->_numMeathooksInZone = 0;
    this->_attackingMaxSpeedPercent = 1.0f;
    this->_isInterrupting = false;

    this->_printServerPositionUpdateEnabled = false;
    this->_inParadise = false;
    this->_debugInteractionInZone = false;
    
    // Initialize Authoritative Movement Flags
    this->_authoritativeMovementFlags._flags = 0;

    this->_forcedAuthoritativeUnmoveable = false;
    this->_windowFreeCam = false;
    this->_reverseTraverse = false;
    this->_runVault = true;
    this->_debugFailAllInteractions = false;
    this->_stamina = 1.0f;

    this->_wasLocallyObserved = false;
    
    this->_isHeadHidden = false;
    this->_isInFirstPerson = false;

    // Mesh Settings
    this->_screenAspectRatio = 16.0f / 9.0f; // 16:9 aspect ratio
    this->AutoPossessAI = EAutoPossessAI::Disabled;

    // Bitwise operation on Mesh->Bitfield
    this->Mesh->bOwnerNoSee = false;

    // Collision Settings
    this->Mesh->SetCollisionObjectType(ECC_Pawn);
    this->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    this->Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    this->CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

    this->GamePadTurnRate = 45.0f;
    this->GamePadLookUpRate = 45.0f;

    // Create Default Subobjects
    this->_interactionHandler = CreateDefaultSubobject<UPlayerInteractionHandler>(TEXT("InteractionHandler"));
    this->_interactionHandler->OwningCharacter = this;

    this->_montagePlayer = CreateDefaultSubobject<UMontagePlayer>(TEXT("MontagePlayer"));

    this->_meatHookZoneDetector = CreateDefaultSubobject<UZoneDetectorComponent>(TEXT("MeatHookZoneDetector"));
    this->_meatHookZoneDetector->ComponentTags.Add(FName("MeatHookZone"));

    this->_perkManager = CreateDefaultSubobject<UPerkManager>(TEXT("PerkManager"));
    
    this->_eventListener = CreateDefaultSubobject<UPollableEventListener>(TEXT("EventListener"));

    this->_characterSnappingComponent = CreateDefaultSubobject<UCharacterSnappingComponent>(TEXT("SnappingComponent"));

    // Item Dropoff setup
    this->_itemDropOffPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ItemDropOffPosition"));
    this->_itemDropOffPosition->SetupAttachment(this->RootComponent);

    this->_authoritativeMovementComponent = CreateDefaultSubobject<UAuthoritativeMovementComponent>(TEXT("AuthoritativeMovement"));

    this->_cameraHandlerComponent = CreateDefaultSubobject<UCameraHandlerComponent>(TEXT("CameraHandlerComponent"));

    // Create Blindness Component (Transient = 1)
    this->BlindingChargeableComponent = CreateDefaultSubobject<UChargeableComponent>(TEXT("BlindnessChargeableComponent"));
    this->BlindingChargeableComponent->ComponentTags.Add(UDBDTags::BLINDNESS);
    this->BlindingChargeableComponent->ChargeableComponentID = UDBDTags::BLINDNESS.ToString();
    this->BlindingChargeableComponent->OnInteractionEmptiedEvent.AddDynamic(this, &ADBDPlayer::OnBlindChargeEmptied);

    // Initialize Shared Pointers for Events
    this->AttackMontageEvent = MakeShareable(new ConsumableEvent<EAttackType>());
    this->MontageEndEvent = MakeShareable(new PollableEvent<bool>(this->_eventListener));
    this->AttackEvent = MakeShareable(new PollableEvent<EAttackType>(this->_eventListener));
    this->InteractionCancelledEvent = MakeShareable(new PollableEvent<bool>(this->_eventListener));
    this->MovementInputEvent = MakeShareable(new PollableEvent<FVector>(this->_eventListener));
    this->BlinkEvent = MakeShareable(new PollableEvent<bool>(this->_eventListener));

    // Reset Loading Timer
    this->_loadingTimer.Reset(this->kLoadingTime);

    // Setup Tick Prerequisites
    if (this->_authoritativeMovementComponent && this->CharacterMovement)
    {
        this->_authoritativeMovementComponent->PrimaryComponentTick.AddPrerequisite(this->CharacterMovement, this->CharacterMovement->PrimaryComponentTick);
    }

    if (this->_characterSnappingComponent && this->CharacterMovement)
    {
        this->CharacterMovement->PrimaryComponentTick.AddPrerequisite(this->_characterSnappingComponent, this->_characterSnappingComponent->PrimaryComponentTick);
    }

    // Debug Interaction Timers
    this->_debugAutoInteractPress = true;
    this->_debugAutoInteractTimer.Reset(this->DebugAutoInteractDuration);

    this->_debugAutoItemInteractPress = true;
    this->_debugAutoItemInteractTimer.Reset(this->DebugAutoItemInteractDuration);

    // Stamina Reset
    this->_staminaRechargeRate = 0.0f;
    this->_staminaRechargeDelayTimer.Reset(0.0f);
    this->_staminaRechargeDelayTimer.Stop();

    this->_secondsSinceLastSave = 0.0f;
    this->_secondsSinceLastFell = 3.0f;

    // Spawn Position Setup
    this->_spawnPosition = FVector(0.0f, 0.0f, 300.0f);

    // Build Keys
    this->BuildInteractionKeys();
}

// starts at line 290
void ADBDPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // 1. Call the parent class implementation first
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 2. Register variables for replication
    // The macro DOREPLIFETIME expands into the static lookup and array insertion logic seen in the ASM.
    // The condition value set in the assembly is 0, which corresponds to COND_None (Always replicate).
    DOREPLIFETIME(ADBDPlayer, _interactable);    
    DOREPLIFETIME(ADBDPlayer, _perks);    
    DOREPLIFETIME(ADBDPlayer, _statusEffects);    
    DOREPLIFETIME(ADBDPlayer, _blindnessRemainingTime);    
    DOREPLIFETIME(ADBDPlayer, _screenAspectRatio);
}

// starts at line 301
void ADBDPlayer::PostInitializeComponents()
{
    // Call parent class implementation
    Super::PostInitializeComponents();

    // Check if we are in a preview world (e.g. editor preview)
    if (this->IsPreviewWorld() == false)
    {
        // Retrieve and cast the game instance to UDBDGameInstance
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (DBDGameInstance != nullptr)
        {
            // Register this player with the game instance
            DBDGameInstance->AddPlayer(this);
        }
    }

    // Check if this actor has authority (Role == ROLE_Authority which is 3)
    if (this->Role == ROLE_Authority)
    {
        // Check if an interactable class is defined
        if (this->Interactable != nullptr)
        {
            // Setup spawn parameters
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            // Spawn the interactable actor
            AInteractable* SpawnedInteractable = this->GetWorld()->SpawnActor<AInteractable>(this->Interactable, SpawnParams);
            this->_interactable = SpawnedInteractable;

            // If spawning was successful
            if (this->_interactable != nullptr)
            {
                // Attach the interactable to this player
                this->_interactable->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

                // Store the initial spawn position derived from the RootComponent
                if (this->RootComponent != nullptr)
                {
                    this->_spawnPosition = this->RootComponent->GetComponentLocation();
                }
            }
        }

        // Set the actor as an autonomous proxy. 
        // In the disassembly, this is called at the end of the Authority block regardless of spawn success.
        this->SetAutonomousProxy(true);
    }

    // Store the original ground friction from the character movement component
    if (this->CharacterMovement != nullptr)
    {
        this->_originalGroundFriction = this->CharacterMovement->GroundFriction;
    }

    // Initialize values based on tunable data.
    // Corresponds to virtual function call at offset 0x1000 found in disassembly.
    this->InitializeTunableValues();

    // Bind Animation Montage delegates if not in preview world
    if (this->IsPreviewWorld() == false)
    {
        if (this->Mesh != nullptr)
        {
            UAnimInstance* AnimInstance = this->Mesh->GetAnimInstance();
            if (AnimInstance != nullptr)
            {
                // Bind to OnMontageBlendingOut (Offset 0x80 in UAnimInstance)
                AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ADBDPlayer::OnCurrentMontageBlendingOut);

                // Bind to OnMontageEnded (Offset 0xA0 in UAnimInstance)
                AnimInstance->OnMontageEnded.AddDynamic(this, &ADBDPlayer::OnCurrentMontageComplete);
            }
        }
    }
}

// starts at line 353
void ADBDPlayer::InitializeTunableValues()
{
    // Retrieve the GameInstance and securely cast it to the custom Dead By Daylight GameInstance class
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the casted GameInstance is invalid and return early if so
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Access the DesignTunables property from the game instance. 
    // The exact property name at offset 0xF0 is unknown as the UDBDGameInstance structure was not provided.
    UDBDDesignTunables* designTunables = DBDGameInstance->DesignTunables;

    // Validate that the design tunables object is valid and not pending kill
    if (IsValid(designTunables) == false)
    {
        return;
    }

    // Retrieve the tunable values and apply them to the player variables and components.
    // The 'false' parameter maps to the XOR'd r8d register ('warnIfRowMissing' boolean in GetTunableValue).
    this->_recentlyBlindedCooldown = designTunables->GetTunableValue(TunableValues::RECENTLY_BLINDED_DURATION, false);
    
    // Assign values to the BlindingChargeableComponent. 
    this->BlindingChargeableComponent->DechargeRate = designTunables->GetTunableValue(TunableValues::BLINDNESS_DECHARGE_RATE, false);
    this->BlindingChargeableComponent->DechargeDelay = designTunables->GetTunableValue(TunableValues::BLINDNESS_DECHARGE_DELAY, false);
}

// starts at line 369
void ADBDPlayer::Destroyed()
{
    // Check if the current world is a preview world (e.g., character selection screen)
    if (this->IsPreviewWorld() == false)
    {
        // Retrieve the generic GameInstance
        UGameInstance* GameInstance = this->GetGameInstance();

        // Cast to the specific UDBDGameInstance
        // The disassembly performs a manual class hierarchy check which is equivalent to Cast<>
        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

        if (DBDGameInstance != nullptr)
        {
            // Remove the player from the game instance logic
            DBDGameInstance->RemovePlayer(this);
        }
    }

    // Call the parent Destroyed implementation
    // Note: Disassembly explicitly calls APawn::Destroyed, bypassing ACharacter::Destroyed if it exists
    Super::Destroyed();

    // Check if the network role is Authority (3 corresponds to ROLE_Authority)
    if (this->Role == ROLE_Authority)
    {
        // Access the _interactable member (Offset 0x0DA0)
        AInteractable* Interactable = this->_interactable;

        if (Interactable != nullptr)
        {
            // Check if the Interactable is valid and not currently being destroyed
            // Bit check [offset 0x140] & 4 corresponds to bActorIsBeingDestroyed flag in AActor
            if (Interactable->IsValidLowLevel() == true && Interactable->bActorIsBeingDestroyed == false)
            {
                // Destroy the interactable actor
                // Arguments: bNetForce = false, bShouldModifyLevel = true
                this->GetWorld()->DestroyActor(Interactable, false, true);
            }
        }
    }

    // Access the StateMachine member (Offset 0x0CE8)
    UStateMachine* StateMachine = this->_SM;

    if (StateMachine != nullptr)
    {
        // Check if the StateMachine object is valid (GUObjectArray check)
        if (StateMachine->IsValidLowLevel() == true)
        {
            // Stop the state machine logic
            StateMachine->Stop();
        }
    }
}

// starts at line 394
UAnimMontage* ADBDPlayer::GetMontage(FAnimationMontageDescriptor AnimMontageID) const
{
    // 1. Check if the Montage ID is valid (Not None)
    // The assembly checks CompositeComparisonValue != 0
    if (AnimMontageID.MontageID.IsNone())
    {
        return nullptr;
    }

    // 2. Validate the Montage Player component (Offset 0xCF8)
    UMontagePlayer* MontagePlayerComp = this->_montagePlayer;

    // The ASM includes a specific IsA check for UMontagePlayer safety
    if (MontagePlayerComp && MontagePlayerComp->IsA(UMontagePlayer::StaticClass()))
    {
        // 3. Inject Contextual Tags
        // The player implements IAnimTagProvider (interface at offset 0x7B0).
        // We call GetAnimTags to populate the descriptor's Tags array with current state (e.g., Injured).
        // This allows the system to differentiate between "Attack" (Base) and "Attack_Injured".
        this->GetAnimTags(AnimMontageID.Tags);

        // 4. Retrieve the Montage
        // Delegate the lookup to the component using the fully populated descriptor.
        // The ASM shows a copy constructor being called here, suggesting pass-by-value or const reference handling.
        return MontagePlayerComp->GetAnimationMontage(AnimMontageID);
    }

    return nullptr;
}

// starts at line 411
float ADBDPlayer::GetMontageLength(FAnimationMontageDescriptor AnimMontageID) const
{
    float Length = 0.0f;

    // 1. Resolve the Montage object
    // Calls the virtual function GetMontage() we reconstructed previously.
    // Pass-by-value or copy logic is observed in the assembly (creating a local descriptor).
    UAnimMontage* Montage = this->GetMontage(AnimMontageID);

    // 2. Validate and Calculate Length
    // Check if the returned pointer is valid (not null, safe UObject).
    if (Montage != nullptr && IsValid(Montage))
    {
        // 3. Get the sequence length
        // Calls the native UAnimMontage function to get the duration in seconds.
        Length = Montage->CalculateSequenceLength();
    }

    return Length;
}

// starts at line 423
FMontagePlaybackDefinition ADBDPlayer::GetMontagePlaybackDefinition(FAnimationMontageDescriptor AnimMontageID, float PlayRate, bool IsFollower)
{
    FMontagePlaybackDefinition Result;

    // 1. Inject Contextual Tags
    // The player implements the IAnimTagProvider interface (Offset 0x7B0).
    // This call populates the input descriptor's Tags array with the current character state.
    // e.g., tags like "IsCrouched", "IsInjured" are added here.
    this->GetAnimTags(AnimMontageID.Tags);

    // 2. Construct the Result
    // The descriptor (now containing tags) is copied into the result struct.
    Result.Descriptor = AnimMontageID;
    
    // 3. Set Playback parameters
    Result.PlayRate = PlayRate;
    Result.Follower = IsFollower; // Likely controls if this montage should be replicated/followed by clients

    return Result;
}

// starts at line 429
UAnimMontage* ADBDPlayer::PlayMontage(FAnimationMontageDescriptor animMontageID, float playRate, bool isFollower)
{
    // Retrieve the MontagePlayer component stored at offset 0xCF8.
    UMontagePlayer* LocalMontagePlayer = this->_montagePlayer;

    // Validate the component.
    // The disassembly checks the pointer and the GUObjectArray flags (IsValid check).
    if (IsValid(LocalMontagePlayer) == true)
    {
        // The function modifies the descriptor before playing, adding context-specific tags.
        // Since 'animMontageID' is passed by value (indicated by the destructor call at the end of the function),
        // we work on a local copy (v18 in disassembly).
        FAnimationMontageDescriptor DescriptorToPlay = animMontageID;

        // Retrieve the IAnimTagProvider interface.
        // The disassembly offsets 'this' by 0x7B0 to access the interface vtable.
        // This implies ADBDPlayer uses multiple inheritance to implement IAnimTagProvider.

        // Call the interface method GetAnimTags to append player-specific tags (e.g., "Injured", "Crouching")
        // to the descriptor's tag list.
        // Disassembly: call qword ptr [rax+10h] (Virtual function at offset 0x10 of the interface).
        this->GetAnimTags(DescriptorToPlay.Tags);

        // Delegate the actual playback to the MontagePlayer component.
        // The complex memcpy/resize logic in the disassembly (v18 -> v19 -> v18) is the internal 
        // implementation of TArray resizing/copying during the struct modification.
        return LocalMontagePlayer->Play(DescriptorToPlay, playRate, isFollower);
    }

    return nullptr;
}

// starts at line 442
void ADBDPlayer::StopMontage()
{
    UMontagePlayer* LocalMontagePlayer = this->_montagePlayer;

    if (IsValid(LocalMontagePlayer) == true)
    {
        LocalMontagePlayer->Stop(-1.0f);
    }
}

// starts at line 456
bool ADBDPlayer::IsPlayingMontage(FAnimationMontageDescriptor animMontageID) const
{
    // Retrieve the UAnimMontage asset corresponding to the provided descriptor ID.
    // Disassembly: Calls virtual function at vtable offset 0xE18.
    // This is likely a custom function like 'GetMontageFromDescriptor' or simply 'GetMontage'.
    UAnimMontage* Montage = this->GetMontage(animMontageID);

    // Call the overloaded IsPlayingMontage function with the resolved montage pointer.
    // Disassembly: Calls ?IsPlayingMontage@ADBDPlayer@@QEBA_NPEAVUAnimMontage@@@Z
    return this->IsPlayingMontage(Montage);
}

// starts at line 461
bool ADBDPlayer::IsPlayingMontage(UAnimMontage* montage) const
{
    // Check if the input montage is null
    if (montage == nullptr)
    {
        return false;
    }

    // Retrieve the SkeletalMeshComponent (ACharacter::Mesh)
    // Disassembly: Accesses offset 0x3E0
    USkeletalMeshComponent* Mesh = this->Mesh;

    // Check if the Mesh exists
    if (Mesh != nullptr)
    {
        // Verify the Mesh object is valid (not pending kill)
        // Disassembly: Checks GUObjectArray flags (0x20000000)
        if (Mesh->IsPendingKill() == false)
        {
            // Retrieve the Animation Instance from the Mesh
            // Disassembly: Calls USkeletalMeshComponent::GetAnimInstance()
            UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

            // Check if the AnimInstance exists
            if (AnimInstance != nullptr)
            {
                // Verify the AnimInstance object is valid
                if (AnimInstance->IsPendingKill() == false)
                {
                    // Check if the specific montage is also valid (not pending kill)
                    if (montage->IsPendingKill() == false)
                    {
                        // Check if the specific montage is currently playing
                        // Disassembly: Calls UAnimInstance::Montage_IsPlaying(montage)
                        return AnimInstance->Montage_IsPlaying(montage);
                    }
                }
            }
        }
    }

    return false;
}

// starts at line 475
bool ADBDPlayer::IsPlayingAnyMontage() const
{
    // Retrieve the SkeletalMeshComponent (ACharacter::Mesh)
    // Disassembly: Accesses offset 0x3E0
    USkeletalMeshComponent* Mesh = this->Mesh;

    // Check if the Mesh exists
    if (Mesh != nullptr)
    {
        // Verify the Mesh object is valid (not pending kill)
        // Disassembly: Checks GUObjectArray flags (0x20000000)
        if (Mesh->IsPendingKill() == false)
        {
            // Retrieve the Animation Instance from the Mesh
            // Disassembly: Calls USkeletalMeshComponent::GetAnimInstance()
            UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

            // Check if the AnimInstance exists
            if (AnimInstance != nullptr)
            {
                // Verify the AnimInstance object is valid
                if (AnimInstance->IsPendingKill() == false)
                {
                    // Check if any montage is currently playing.
                    // Disassembly: Calls Montage_IsPlaying with nullptr (0i64) as the argument.
                    return AnimInstance->Montage_IsPlaying(nullptr);
                }
            }
        }
    }

    return false;
}

// starts at line 487
UMontagePlayer* ADBDPlayer::GetMontagePlayer() const
{
    // Return the cached pointer to the MontagePlayer component
    // Maps to member variable _montagePlayer (Offset 0xCF8 based on previous functions)
    return this->_montagePlayer;
}

// starts at line 492
UAnimInstance* ADBDPlayer::GetAnimInstance()
{
    // Retrieve the SkeletalMeshComponent inherited from ACharacter
    // Offset 0x3E0 in the structure provided earlier
    USkeletalMeshComponent* MeshComponent = this->Mesh;

    // Check if the component pointer is not null
    if (MeshComponent != nullptr)
    {
        // Validate the object against the global object array (Low level check)
        // This corresponds to the GUObjectArray logic in the pseudo code
        if (MeshComponent->IsValidLowLevel() == true)
        {
            // Return the animation instance currently active on the mesh
            return MeshComponent->GetAnimInstance();
        }
    }

    return nullptr;
}

// starts at line 512
void ADBDPlayer::PawnClientRestart()
{
    // Call the parent class implementation to handle standard pawn client restart logic
    Super::PawnClientRestart();

    // Call the custom function to handle additional restart logic specific to ADBDPlayer
    this->OnClientRestart();
}

// starts at line 520
void ADBDPlayer::PossessedBy(AController* InController)
{
    // Call the parent class implementation to handle standard possession logic
    Super::PossessedBy(InController);

    // Call the specific function to handle offerings logic upon spawning/possession on the server (Authority)
    // The pseudo-code shows 'this' passed as an argument, which represents the implicit object instance in C++ member calls
    this->AuthorityHandleOfferingsOnSpawn();
}

// starts at line 526
void ADBDPlayer::BeginPlay()
{
    // Call the parent class implementation of BeginPlay
    Super::BeginPlay();

    // Retrieve the current world context
    UWorld* world = this->GetWorld();
    
    // Check if the world pointer is valid before proceeding
    if (world != nullptr)
    {
        // The disassembly verifies the object validity via GUObjectArray 
        // and casts it to ADBDGameState. This is wrapped safely by Cast<T>().
        ADBDGameState* dbdGameState = Cast<ADBDGameState>(world->GetGameState());
        
        if (dbdGameState != nullptr)
        {
            // The lengthy wcsstr string parsing and __Internal_AddDynamic calls in the disassembly 
            // represent the compiled output of Unreal Engine's AddDynamic macro.
            // Since the ADBDGameState structure is missing from the provided _S block, 
            // the exact delegate property names (at offsets 0x3E0, 0x540, 0x560) are approximated.
            dbdGameState->OnEscapeDoorActivated.AddDynamic(this, &ADBDPlayer::OnEscapeDoorActivated);
            dbdGameState->OnLevelReadyToPlay.AddDynamic(this, &ADBDPlayer::OnLevelReadyToPlay);
            dbdGameState->AuthorityOnAllPlayerLoaded.AddDynamic(this, &ADBDPlayer::OnAllPlayerLoaded);
        }
    }

    // Ensure the global GEngine pointer is valid before accessing it
    if (GEngine != nullptr)
    {
        // Retrieve global engine settings and cast to the specific DBD user settings class
        UDBDGameUserSettings* gameUserSettings = Cast<UDBDGameUserSettings>(GEngine->GetGameUserSettings());
        
        if (gameUserSettings != nullptr)
        {
            // Retrieve the Y-axis inversion setting and apply it to the player's internal variable
            this->_invertLookUpAxis = gameUserSettings->GetInvertY();
        }
    }
}

// starts at line 548
const FPlayerPersistentData* ADBDPlayer::Local_GetPlayerPersistentData() const
{
    // Check if the player is locally controlled.
    // Persistent data is relevant primarily for the local client (the user playing).
    // Disassembly: call qword ptr [rax+630h] (Virtual call to IsLocallyControlled)
    if (this->IsLocallyControlled())
    {
        // Retrieve the Game Instance
        // Disassembly: call ?GetGameInstance@AActor@@QEBAPEAVUGameInstance@@XZ
        UGameInstance* GameInstance = this->GetGameInstance();

        // Check if Game Instance exists
        if (GameInstance != nullptr)
        {
            // Cast to the custom Dead by Daylight Game Instance
            // Disassembly: Checks ClassTreeIndex against UDBDGameInstance::StaticClass
            UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

            // Verify cast success and object validity
            if (DBDGameInstance != nullptr && DBDGameInstance->IsPendingKill() == false)
            {
                // Retrieve a pointer stored at offset 0x3B8 in UDBDGameInstance.
                // This is likely a manager class, e.g., 'PlayerProfileManager' or 'SaveDataManager'.
                // Disassembly: mov rax, [rbx+3B8h]
                UDBDPersistentData* DataManager = DBDGameInstance->_persistentData;

                if (DataManager != nullptr)
                {
                    // Return the FPlayerPersistentData struct located at offset 0x168 within that manager.
                    // Disassembly: add rax, 168h
                    return &DataManager->_localPlayerPersistentData;
                }
            }
        }
    }

    // Return null if not locally controlled or if data is inaccessible
    return nullptr;
}

// starts at line 564
const FPlayerPersistentData* ADBDPlayer::GetPlayerPersistentData() const
{
    // 1. Get the Player State
    // We need the PlayerState to get the UniqueNetId.
    APlayerState* PlayerState = this->GetPlayerState();
    
    // Validate it's a specific ADBDPlayerState (though standard cast checks apply)
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(PlayerState);
    if (!DBDPlayerState)
    {
        return nullptr;
    }

    // 2. Get the Game Instance
    // The Persistent Data Manager is a global system living on the GameInstance.
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    
    if (GameInstance)
    {
        // 3. Access the Persistent Data Manager
        // Located at offset 0x3B8 in UDBDGameInstance (based on assembly [rsi+3B8h])
        UDBDPersistentData* PersistentDataManager = GameInstance->GetPersistentDataManager();

        if (PersistentDataManager)
        {
            // 4. Query Data using UniqueNetId
            // The PlayerState holds the UniqueId (FUniqueNetIdRepl) which acts as the key.
            return PersistentDataManager->GetPlayerPersistentData(DBDPlayerState->UniqueId);
        }
    }

    return nullptr;
}

// starts at line 594
void ADBDPlayer::UpdateCustomizationMesh(const TArray<FName>& customization)
{
    // Retrieve the GameInstance and cast it to the specific DBD Game Instance.
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Validation check: Ensure the GameInstance exists and is valid.
    if (GameInstance == nullptr)
    {
        return;
    }

    // Retrieve the UCustomizedSkeletalMesh component attached to this actor.
    UCustomizedSkeletalMesh* CustomizedSkeletalMesh = Cast<UCustomizedSkeletalMesh>(this->GetComponentByClass(UCustomizedSkeletalMesh::StaticClass()));

    // Validation check: Ensure the component exists and is valid.
    if (CustomizedSkeletalMesh == nullptr)
    {
        return;
    }

    // Check if the GameInstance and Component are not pending kill (Garbage Collection checks).
    // The disassembly explicitly checks InternalIndex against GUObjectArray and flags (0x20000000 = RF_PendingKill/RF_Unreachable).
    if (GameInstance->IsValidLowLevel() == false)
    {
        return;
    }

    if (CustomizedSkeletalMesh->IsValidLowLevel() == false)
    {
        return;
    }

    // Iterate through the array of customization Item IDs.
    for (const FName& CustomizationItemID : customization)
    {
        // Access UDBDDesignTunables from the GameInstance (Offset 0xF0 in disassembly).
        // Calling GetCustomizationItemData to retrieve data struct for the specific Item ID.
        // Second argument 'true' corresponds to 'bWarnIfRowMissing'.
        UDBDDesignTunables* DesignTunables = GameInstance->DesignTunables;
        
        if (DesignTunables != nullptr)
        {
            FCustomizationItemData* ItemData = DesignTunables->GetCustomizationItemData(CustomizationItemID, true);

            // Ensure the item data was found.
            if (ItemData != nullptr)
            {
                // Logic to check if the Skeletal Mesh asset associated with this item is loaded or resolvable.
                // The disassembly performs operations on a TAssetPtr/FStringAssetReference (ItemMesh at offset 0xA0).
                // It checks if the WeakPtr is valid, or attempts to ResolveObject.
                bool bIsAssetValid = false;
                
                // Check if the asset is already loaded in memory.
                if (ItemData->ItemMesh.IsValid())
                {
                    bIsAssetValid = true;
                }
                else
                {
                    // Attempt to resolve the object if it is not currently loaded.
                    if (ItemData->ItemMesh.ToStringReference().ResolveObject() != nullptr)
                    {
                        bIsAssetValid = true;
                    }
                }

                // If the asset is valid/loaded, apply the mesh switch.
                if (bIsAssetValid == true)
                {
                    // Call SwitchMesh on the component.
                    // Arg 1: Category (FName), located at offset 0x178 in FCustomizationItemData.
                    // Arg 2: CustomizationItemID (FName).
                    // Arg 3: bIsVfxVisible (bool), passed as 1 (true).
                    CustomizedSkeletalMesh->SwitchMesh(ItemData->Category, CustomizationItemID, true);
                }
            }
        }
    }
}

// starts at line 611
void ADBDPlayer::UpdateCustomizationMesh()
{
    // Retrieve the PlayerState inherited from APawn and cast it to ADBDPlayerState.
    // The disassembly performs a manual GetPrivateStaticClass and ClassTreeIndex check here.
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    // Validation check: Ensure the PlayerState exists.
    if (DBDPlayerState == nullptr)
    {
        return;
    }

    // Validation check: Ensure the PlayerState is valid for the Object System (Garbage Collection).
    // The disassembly checks InternalIndex against GUObjectArray and flags (0x20000000).
    if (DBDPlayerState->IsValidLowLevel() == false)
    {
        return;
    }

    // Validation check: Ensure the PlayerState actor is not in the process of being destroyed.
    // Checking bit 3 (Value 4) of the bitfield at offset 0x140 (bActorIsBeingDestroyed).
    if (DBDPlayerState->IsActorBeingDestroyed() == true)
    {
        return;
    }

    // Retrieve the customization array from the ADBDPlayerState.
    // The array is located at offset 0x7E8.
    // We pass this array to the overloaded UpdateCustomizationMesh function.
    this->UpdateCustomizationMesh(DBDPlayerState->PlayerData.CustomizationMesh);
}

// starts at line 619
ACollectable* ADBDPlayer::SpawnCollectedItem(FName ItemID)
{
    // Convert FName to string for logging.
    UE_LOG(GameFlow, Verbose, TEXT("Spawn Item: %s"), *ItemID.ToString());

    // Retrieve the Game Instance.
    // Standard engine call: GetGameInstance()
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (GameInstance != nullptr)
    {
        // Access DBDDesignTunables from the GameInstance to retrieve item data.
        // Offset 0xF0 suggests a member pointer to UDBDDesignTunables.
        // GetItemData resolves the FItemData struct for the given ID.
        // Warning if missing is set to true (1).
        FItemData* ItemData = GameInstance->DBDDesignTunables->GetItemData(ItemID, true);

        if (ItemData != nullptr)
        {
            // Resolve the SoftClassPtr (AssetPtr/WeakPtr) for the item blueprint.
            // If the class is already loaded (Get() returns valid), use it.
            // If not, perform a synchronous load using the StreamableManager.
            // Note: The assembly shows logic for FStringAssetReference/FSoftObjectPath resolution and loading.
            UClass* ItemClass = ItemData->ItemBlueprint.Get();
            if (ItemClass == nullptr)
            {
                // Synchronous load if not loaded.
                ItemClass = Cast<UClass>(GameInstance->StreamableManager.SynchronousLoad(ItemData->ItemBlueprint.ToSoftObjectPath()));
            }

            if (ItemClass != nullptr && IsValid(ItemClass))
            {
                // Prepare spawn parameters.
                FActorSpawnParameters SpawnParams;
                // Set ObjectFlags to RF_Transient (0x40) or similar.
                SpawnParams.ObjectFlags |= RF_Transient; 
                SpawnParams.Owner = this;

                // Determine spawn location and rotation.
                // It uses the Mesh component and a socket name stored in 'CollectableAttachPoint' (Offset 0xE58?).
                // Note: The offset 0xC50 might be CollectableAttachPoint name.
                FVector SpawnLocation;
                FRotator SpawnRotation;
                
                if (this->Mesh != nullptr)
                {
                    SpawnLocation = this->Mesh->GetSocketLocation(this->CollectableAttachPoint);
                    SpawnRotation = this->Mesh->GetSocketRotation(this->CollectableAttachPoint);
                }
                else
                {
                    SpawnLocation = FVector::ZeroVector;
                    SpawnRotation = FRotator::ZeroRotator;
                }

                // Spawn the actor.
                return this->GetWorld()->SpawnActor<ACollectable>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
            }
        }
    }

    return nullptr;
}

// starts at line 643
void ADBDPlayer::Local_SpawnCollectedItem(FName itemID)
{
    // 1. Spawn the actual item actor locally based on the Item ID.
    // Disassembly: call ?SpawnCollectedItem@ADBDPlayer@@QEAAPEAVACollectable@@VFName@@@Z
    ACollectable* SpawnedItem = this->SpawnCollectedItem(itemID);

    // 2. Notify the item that it has been collected by this player.
    // Disassembly: call ?Local_SetCollected@ACollectable@@QEAAXPEAVADBDPlayer@@@Z
    if (SpawnedItem != nullptr)
    {
        SpawnedItem->Local_SetCollected(this);
    }

    // 3. Add the item to the Inventory if it isn't already there (AddUnique).
    // The disassembly explicitly loops through the Inventory array to check for existence
    // before appending.
    
    bool bItemFound = false;
    for (int32 i = 0; i < this->Inventory.Num(); i++)
    {
        if (this->Inventory[i] == SpawnedItem)
        {
            bItemFound = true;
            break;
        }
    }

    if (!bItemFound)
    {
        // Add the new item to the inventory array
        this->Inventory.Add(SpawnedItem);
    }

    // 4. Update the player's loadout to reflect the new inventory state.
    // Disassembly: call ?UpdateLoadoutFromInventory@ADBDPlayer@@QEAAXXZ
    this->UpdateLoadoutFromInventory();
}

// starts at line 652
void ADBDPlayer::Authority_SpawnLoadoutPerks()
{
    // Check if the player has network authority (3 corresponds to ROLE_Authority in ENetRole)
    if (this->Role == ROLE_Authority)
    {
        // Retrieve the player state
        ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        if (dbdPlayerState != nullptr)
        {
            // Retrieve the Game Instance
            UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

            if (dbdGameInstance != nullptr)
            {
                // Extract the unique network ID from the player state.
                FUniqueNetIdRepl uniqueNetId = dbdPlayerState->UniqueId;

                // Retrieve the Persistent Data manager from the DBD Game Instance (offset 0x3B8).
                UDBDPersistentData* persistentDataManager = dbdGameInstance->_persistentData;

                if (persistentDataManager != nullptr)
                {
                    // Fetch the loadout/persistent data for this specific player
                    FPlayerPersistentData* playerPersistentData = persistentDataManager->GetPlayerPersistentData(uniqueNetId);

                    if (playerPersistentData != nullptr)
                    {
                        // Check the player's role (offset 0x750 in ADBDPlayerState). 
                        // A value of 2 typically represents the Camper (Survivor) role.
                        if (dbdPlayerState->GameRole == EPlayerRole::VE_Camper)
                        {
                            // Process Camper (Survivor) Perks
                            if (playerPersistentData->CurrentLoadout.CamperPerks.Num() > 0)
                            {
                                for (int i = 0; i < playerPersistentData->CurrentLoadout.CamperPerks.Num(); i++)
                                {
                                    // Ensure we don't read out of bounds of the parallel levels array
                                    if (i >= playerPersistentData->CurrentLoadout.CamperPerkLevels.Num())
                                    {
                                        break;
                                    }

                                    FName perkId = playerPersistentData->CurrentLoadout.CamperPerks[i];
                                    int perkLevel = playerPersistentData->CurrentLoadout.CamperPerkLevels[i];

                                    // VTable offset 0x1078 corresponds to Authority_SpawnPerk
                                    this->Authority_SpawnPerk(perkId, perkLevel);
                                }
                            }
                        }
                        else
                        {
                            // Process Slasher (Killer) Perks
                            if (playerPersistentData->CurrentLoadout.SlasherPerks.Num() > 0)
                            {
                                for (int i = 0; i < playerPersistentData->CurrentLoadout.SlasherPerks.Num(); i++)
                                {
                                    // Ensure we don't read out of bounds of the parallel levels array
                                    if (i >= playerPersistentData->CurrentLoadout.SlasherPerkLevels.Num())
                                    {
                                        break;
                                    }

                                    FName perkId = playerPersistentData->CurrentLoadout.SlasherPerks[i];
                                    int perkLevel = playerPersistentData->CurrentLoadout.SlasherPerkLevels[i];

                                    // VTable offset 0x1078 corresponds to Authority_SpawnPerk
                                    this->Authority_SpawnPerk(perkId, perkLevel);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// starts at line 683
void ADBDPlayer::Authority_SpawnPerk(FName perkID, int perkLevel)
{
    // Check if the player has network authority and if the perkID is valid (not empty or locked)
    if (this->Role == ROLE_Authority && 
        perkID != LoadoutSlotDefaultValue::LOCKED &&
        perkID != LoadoutSlotDefaultValue::EMPTY &&
        perkID != NAME_None)
    {
        // The disassembly checks the GameFlow logging verbosity level before proceeding. 
        // This corresponds to a standard UE_LOG macro call.
        UE_LOG(LogGameFlow, Log, TEXT("Spawn Perk: %s"), *perkID.ToString());

        // Retrieve the Game Instance and cast it to the specific DBD Game Instance class
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (dbdGameInstance != nullptr)
        {
            // Access the DesignTunables component (located at offset 0xF0)
            UDBDDesignTunables* designTunables = dbdGameInstance->DesignTunables;

            if (designTunables != nullptr)
            {
                // Fetch the perk data structure using the given perk ID. The 'true' argument maps to bWarnIfRowMissing.
                FPerkProperties* perkData = designTunables->GetPerkData(perkID, true);

                if (perkData != nullptr)
                {
                    // The disassembly contains heavily inlined code for FStringAssetReference and FWeakObjectPtr resolution.
                    // This is the compiled representation of accessing a TSoftClassPtr (or similar async loadable class reference)
                    // and loading it synchronously via FStreamableManager.
                    UClass* perkClass = perkData->PerkBlueprint.LoadSynchronous();

                    if (perkClass != nullptr)
                    {
                        // Spawn the Perk object using the loaded class, marking it as Transient.
                        // The trailing 'true' refers to bCopyTransientsFromClassDefaults.
                        UPerk* newPerk = NewObject<UPerk>(this, perkClass, NAME_None, RF_Transient, nullptr, true, nullptr);

                        if (newPerk != nullptr)
                        {
                            // Initialize and register the newly spawned perk
                            newPerk->Multicast_InitializePerk(perkID, perkLevel);
                            newPerk->RegisterComponent();

                            // Add the perk to the player's loadout/inventory
                            this->AddPerk(newPerk);
                        }
                    }
                }
            }
        }
    }
}

// starts at line 714
void ADBDPlayer::Server_SpawnPerk_Implementation(FName perkID, int64 perkLevel)
{
  this->Authority_SpawnPerk(perkID, perkLevel);
}

// starts at line 730
void ADBDPlayer::Server_RemovePerks_Implementation()
{
    // Check if the actor has authority (ROLE_Authority == 3)
    if (this->Role == ROLE_Authority)
    {
        // Clear the array of perks (offset 0xC68).
        // The disassembly explicitly sets ArrayNum to 0 and calls ResizeTo(0), which effectively empties the array and frees memory.
        this->_perks.Empty();
    }
}

// starts at line 735
void ADBDPlayer::Authority_RemoveStatusEffectByID(FName statusEffectId, bool bRemoveAllWithID)
{
    // Check if the player has network authority (3 corresponds to ROLE_Authority)
    if (this->Role == ROLE_Authority)
    {
        // Iterate through the status effects array backwards to safely remove elements during iteration
        for (int i = this->_statusEffects.Num() - 1; i >= 0; i--)
        {
            // Retrieve the status effect pointer at the current index
            UStatusEffect* statusEffect = this->_statusEffects[i];

            // Ensure the pointer is valid before dereferencing
            if (statusEffect != nullptr)
            {
                // The disassembly checks a property at offset 0xF0.
                if (statusEffect->ID == statusEffectId)
                {
                    // The memory move and ResizeShrink operations correspond to a standard RemoveAt call
                    this->_statusEffects.RemoveAt(i);

                    // A virtual function at offset 0x308 (776 bytes) is invoked on the status effect.
                    statusEffect->DestroyComponent(false);

                    // If the flag to remove all instances is false, break out of the function after the first removal
                    if (bRemoveAllWithID == false)
                    {
                        return;
                    }
                }
            }
        }
    }
}

// starts at line 761
void ADBDPlayer::Server_RemoveStatusEffectByID_Implementation(FName StatusEffectId, bool bRemoveAllWithID)
{
    // Delegate the removal logic to the authority-specific function.
    // This handles the actual removal of the status effect(s) matching the ID on the server.
    this->Authority_RemoveStatusEffectByID(StatusEffectId, bRemoveAllWithID);
}

// starts at line 777
void ADBDPlayer::Server_RemoveStatusEffects_Implementation()
{
    // Check if the actor has authority (ROLE_Authority == 3)
    if (this->Role == ROLE_Authority)
    {
        // Clear the array of status effects (Offset 0xC88).
        // The disassembly explicitly sets ArrayNum to 0 and calls ResizeTo(0), which effectively empties the array and frees memory.
        this->_statusEffects.Empty();
    }
}

// starts at line 788
void ADBDPlayer::Server_SpawnStatusEffect_Implementation(FName statusEffectID)
{
    this->Authority_SpawnStatusEffect(statusEffectID, this, 0.0f, 0);
}

// starts at line 794
UStatusEffect* ADBDPlayer::Authority_ImposeStatusEffect(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParam, UGameplayModifierContainer* originatingEffect)
{
    // Check if the actor has network authority (3 corresponds to the ROLE_Authority enum value in ENetRole)
    if (this->Role == 3)
    {
        // Delegate to the internal spawn function to create and apply the status effect
        return this->Authority_SpawnStatusEffect(statusEffectID, originatingPlayer, customParam, originatingEffect);
    }

    // If the actor does not have authority (e.g., executing on a client), return a null pointer
    return nullptr;
}

// starts at line 804
UStatusEffect* ADBDPlayer::Authority_SpawnStatusEffect(FName statusEffectId, ADBDPlayer* originatingPlayer, float customParam, UGameplayModifierContainer* originatingEffect)
{
    // Check if the actor has network authority (3 corresponds to ROLE_Authority in ENetRole)
    if (this->Role == ROLE_Authority)
    {
        // The disassembly checks the GameFlow logging verbosity level. 
        // This corresponds to a standard UE_LOG macro call in Unreal Engine C++.
        UE_LOG(LogGameFlow, Log, TEXT("Spawn Status Effect: %s"), *statusEffectId.ToString());

        // Retrieve the Game Instance and safely cast it to the custom DBD Game Instance class
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (dbdGameInstance != nullptr)
        {
            // Access the DesignTunables component (located at offset 0xF0)
            UDBDDesignTunables* designTunables = dbdGameInstance->DesignTunables;

            if (designTunables != nullptr)
            {
                // Fetch the status effect properties structure using the provided ID
                FStatusEffectProperties* statusEffectData = designTunables->GetStatusEffectData(statusEffectId);

                if (statusEffectData != nullptr)
                {
                    // The disassembly contains heavily inlined code for FStringAssetReference resolution.
                    // This is the compiled representation of loading a TSoftClassPtr synchronously.
                    UClass* effectClass = statusEffectData->StatusEffectBlueprint.LoadSynchronous();

                    if (effectClass != nullptr)
                    {
                        // Spawn the Status Effect object using the loaded class, marking it as Transient.
                        // The trailing 'true' maps to the bCopyTransientsFromClassDefaults boolean.
                        UStatusEffect* newStatusEffect = NewObject<UStatusEffect>(this, effectClass, NAME_None, RF_Transient, nullptr, true, nullptr);

                        if (newStatusEffect != nullptr)
                        {
                            // Register the newly spawned component
                            newStatusEffect->RegisterComponent();

                            // Initialize the status effect with the original parameters
                            newStatusEffect->Multicast_InitializeStatusEffect(statusEffectId, originatingPlayer, customParam, originatingEffect);

                            // Add the status effect to the player's internal tracking array
                            this->AddStatusEffect(newStatusEffect);

                            // Return the successfully created and initialized status effect
                            return newStatusEffect;
                        }
                    }
                }
            }
        }
    }

    // Return a null pointer if the player lacks authority or if any creation steps fail
    return nullptr;
}

// starts at line 828
UStatusEffect* ADBDPlayer::Authority_ImposeDynamicStatusEffect(FName statusEffectID, ADBDPlayer* originatingPlayer, float customParam, UGameplayModifierContainer* originatingEffect)
{
    UStatusEffect* newStatusEffect = nullptr;

    // Check if the actor has network authority (3 corresponds to the ROLE_Authority enum value in ENetRole)
    if (this->Role == 3)
    {
        // The disassembly calls AssertIfInConstructor and StaticConstructObject_Internal.
        // This is the compiled representation of Unreal Engine's standard NewObject macro.
        newStatusEffect = NewObject<UStatusEffect>(this);

        // Ensure the object was successfully created before interacting with it
        if (newStatusEffect != nullptr)
        {
            // Register the newly spawned component with the actor
            newStatusEffect->RegisterComponent();

            // Initialize the status effect using the provided parameters
            newStatusEffect->Multicast_InitializeStatusEffect(statusEffectID, originatingPlayer, customParam, originatingEffect);

            // Add the initialized status effect to the player's active effects array
            this->AddStatusEffect(newStatusEffect);
        }
    }

    // Return the newly created status effect, or a nullptr if the player lacks authority
    return newStatusEffect;
}

// starts at line 841
void ADBDPlayer::DispatchOnPostAddonsCreation()
{
    // Define a local array to hold the Item Addons
    // This corresponds to the stack allocation seen in the disassembly (Original, var_10)
    TArray<UItemAddon*> LocalItemAddons;

    // Retrieve the Item Addons via the IPerkOwnerInterface
    // The disassembly adjusts the 'this' pointer by 0x7A0 (offset of IPerkOwnerInterface)
    // and calls the virtual function at vtable offset +0x28.
    // We cast 'this' to the interface to match the assembly behavior.
    IPerkOwnerInterface* PerkOwnerInterface = static_cast<IPerkOwnerInterface*>(this);
    if (PerkOwnerInterface != nullptr)
    {
        LocalItemAddons = PerkOwnerInterface->GetItemAddons();
    }

    // Iterate through the retrieved addons
    // The disassembly performs a loop over the array pointers
    for (int32 i = 0; i < LocalItemAddons.Num(); i++)
    {
        UItemAddon* CurrentAddon = LocalItemAddons[i];

        // Retrieve the current item associated with the player
        ACollectable* CurrentItem = this->GetItem();

        // Call the OnPostItemAddonsCreation function on the addon
        // Note: The pseudo code implies a static call, but UObject functions are usually member calls.
        if (CurrentAddon != nullptr)
        {
            CurrentAddon->OnPostItemAddonsCreation(this, CurrentItem);
        }
    }

    // The disassembly shows FMemory::Free, which is the destructor for the TArray going out of scope.
    // In C++, this is handled automatically at the end of the function/block.

    // Access the Inventory member (Offset 0x0C58)
    // Check if the inventory contains any items (ArrayNum at 0x0C60)
    if (this->Inventory.Num() > 0)
    {
        // Access the first item in the inventory (Index 0)
        ACollectable* FirstItem = this->Inventory[0];

        // Validate the item pointer
        if (FirstItem != nullptr)
        {
            // Perform Low Level Validity checks (GUObjectArray checks seen in ASM)
            // Perform 'IsBeingDestroyed' check (Offset 0x140, bit 4)
            if (FirstItem->IsValidLowLevel() == true && FirstItem->bActorIsBeingDestroyed == false)
            {
                // Call the OnPostItemAddonsCreation function on the Collectable
                FirstItem->OnPostItemAddonsCreation(this);
            }
        }
    }
}

// starts at line 854
void ADBDPlayer::Authority_SpawnLoadoutItemAddons()
{
    // Check if the player has network authority (3 corresponds to ROLE_Authority in ENetRole)
    if (this->Role == 3)
    {
        // Retrieve the player state
        APlayerState* playerState = this->PlayerState;

        // Ensure the player state is valid before proceeding
        if (playerState != nullptr)
        {
            // Safely cast the generic PlayerState to ADBDPlayerState
            ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(playerState);

            if (dbdPlayerState != nullptr)
            {
                // Retrieve the Game Instance
                UGameInstance* gameInstance = this->GetGameInstance();

                if (gameInstance != nullptr)
                {
                    // Cast the generic Game Instance to the custom DBD Game Instance
                    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);

                    if (dbdGameInstance != nullptr)
                    {
                        // Extract the unique network ID from the player state.
                        FUniqueNetIdRepl uniqueNetId = dbdPlayerState->UniqueId;

                        // Retrieve the Persistent Data manager from the DBD Game Instance (offset 0x3B8).
                        UDBDPersistentData* persistentDataManager = dbdGameInstance->_persistentData;

                        if (persistentDataManager != nullptr)
                        {
                            // Fetch the loadout/persistent data for this specific player
                            FPlayerPersistentData* playerPersistentData = persistentDataManager->GetPlayerPersistentData(uniqueNetId);

                            if (playerPersistentData != nullptr)
                            {
                                // Initialize a local array to hold the add-ons we need to spawn.
                                // The disassembly's ResizeForCopy and memcpy translate to this simple assignment.
                                TArray<FName> addonNamesToSpawn;

                                // Check the player's role (2 typically represents the Camper/Survivor role in DbD).
                                if (playerPersistentData->GameRole == EPlayerRole::VE_Camper)
                                {
                                    addonNamesToSpawn = playerPersistentData->CurrentLoadout.ItemAddOns;
                                }
                                else
                                {
                                    addonNamesToSpawn = playerPersistentData->CurrentLoadout.PowerAddOns;
                                }

                                // Update local profile data cache (called for side-effects in the engine)
                                dbdGameInstance->GetLocalCharacterSavedProfileData();

                                // Ensure there are add-ons to spawn
                                if (addonNamesToSpawn.Num() > 0)
                                {
                                    for (int i = 0; i < addonNamesToSpawn.Num(); i++)
                                    {
                                        FName addonName = addonNamesToSpawn[i];

                                        // Call the virtual function to handle the actual spawning of the add-on
                                        this->Authority_SpawnItemAddon(addonName);

                                        // Check if the current match type consumes items/add-ons
                                        if (dbdGameInstance->GetCurrentGametypeAffectsProgression() == true)
                                        {
                                            // Instruct the player state to remove the consumed item from the inventory.
                                            // The false argument maps to the 'updateLoadout' parameter in the disassembly.
                                            dbdPlayerState->Client_RemoveItemFromInventory(addonName, false);
                                        }
                                    }
                                }

                                // Dispatch an event notifying that add-ons have finished spawning
                                this->DispatchOnPostAddonsCreation();
                            }
                        }
                    }
                }
            }
        }
    }
}

// starts at line 887
void ADBDPlayer::Authority_SpawnItemAddon(FName addonID)
{
    // Check if the actor has network authority (3 corresponds to ROLE_Authority in ENetRole)
    // Also verify that the addonID is valid and not set to the default placeholder names.
    if (this->Role == ROLE_Authority && 
        addonID != LoadoutSlotDefaultValue::LOCKED &&
        addonID != LoadoutSlotDefaultValue::EMPTY &&
        addonID != NAME_None)
    {
        // The disassembly checks the GameFlow logging verbosity level before proceeding. 
        // This corresponds to a standard UE_LOG macro call.
        UE_LOG(LogGameFlow, Log, TEXT("Spawn Addon: %s"), *addonID.ToString());

        // Retrieve the Game Instance and safely cast it to the custom DBD Game Instance class
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (dbdGameInstance != nullptr)
        {
            // Access the DesignTunables component (located at offset 0xF0)
            UDBDDesignTunables* designTunables = dbdGameInstance->DesignTunables;

            if (designTunables != nullptr)
            {
                // Fetch the item add-on properties structure using the provided ID
                FItemAddonProperties* itemAddonData = designTunables->GetItemAddonData(addonID);

                if (itemAddonData != nullptr)
                {
                    // The disassembly contains heavily inlined code for FStringAssetReference resolution.
                    // This is the compiled representation of loading a TSoftClassPtr synchronously.
                    UClass* addonClass = itemAddonData->ItemAddonBlueprint.LoadSynchronous();

                    if (addonClass != nullptr)
                    {
                        UObject* outerObject = nullptr;

                        // Check the type of the add-on. A value of 5 typically represents a PowerAddOn,
                        // which attaches directly to the Slasher (Killer) player themselves.
                        if (itemAddonData->Type == EInventoryItemType::PowerAddOn)
                        {
                            outerObject = this;
                        }
                        else
                        {
                            // Regular item add-ons (e.g., Flashlight add-ons) attach to the item held in the inventory
                            if (this->Inventory.Num() > 0)
                            {
                                outerObject = this->Inventory[0];
                            }
                        }

                        // Ensure we have a valid Outer object before attempting to spawn the component
                        if (IsValid(outerObject) == true)
                        {
                            // Spawn the Item Addon object using the loaded class, marking it as Transient.
                            // The trailing 'true' maps to the bCopyTransientsFromClassDefaults boolean.
                            UItemAddon* newItemAddon = NewObject<UItemAddon>(outerObject, addonClass, NAME_None, RF_Transient, nullptr, true, nullptr);

                            if (newItemAddon != nullptr)
                            {
                                // Assign the base parameters manually post-spawn
                                newItemAddon->ID = addonID;
                                newItemAddon->InventoryItemType = itemAddonData->Type;

                                // Register the item add-on onto the player. 
                                // This maps to the vtable call `[r8+0DD0h]` matching `Authority_AddItemAddonToPlayer` from _V.
                                this->Authority_AddItemAddonToPlayer(newItemAddon);

                                // Register and fully initialize the newly spawned component
                                newItemAddon->RegisterComponent();
                                newItemAddon->Multicast_InitializeItemAddon();
                                newItemAddon->Authority_OnCollectablePickedUp(this);
                            }
                        }
                    }
                }
            }
        }
    }
}

// starts at line 924
void ADBDPlayer::Server_SpawnItem_Implementation(FName itemID)
{
    this->SpawnCollectedItem(itemID);
}

// starts at line 934
void ADBDPlayer::Server_SpawnItemAddon_Implementation(FName addonID)
{
    this->Authority_SpawnItemAddon(addonID);
}

// starts at line 1002
void ADBDPlayer::SetupStamina(float rechargeRate, float rechargeDelay)
{
    // Set the rate at which stamina regenerates (Offset 0xEF8).
    this->_staminaRechargeRate = rechargeRate;

    // Access the timer responsible for the delay before recharge begins (Offset 0x10E8).
    // Reset the timer configuration with the new delay value.
    this->_staminaRechargeDelayTimer.Reset(rechargeDelay);

    // Immediately stop the timer.
    // This ensures the delay doesn't start counting down until explicitly triggered later.
    UDBDUtilTimer::Stop(this->_staminaRechargeDelayTimer);
}

// starts at line 1010
float ADBDPlayer::GetStamina() const
{
    // Return the current stamina value.
    return this->_stamina;
}

// starts at line 1015
void ADBDPlayer::ConsumeStamina(float stamina)
{
    // Decrease the current stamina by the specified amount.
    // The disassembly performs a floating-point subtraction and then clamps the result to ensure it doesn't go below zero.
    // movss xmm2, [rcx+10E4h] -> subss xmm2, xmm1 -> maxss xmm2, xmm0 (where xmm0 is 0)
    this->_stamina = FMath::Max(this->_stamina - stamina, 0.0f);

    // Reset the stamina recharge delay timer.
    // The code calculates the address of the timer (this + 0x10E8) and calls the static helper UDBDUtilTimer::Reset.
    UDBDUtilTimer::Reset(this->_staminaRechargeDelayTimer);
}

// starts at line 1065
void ADBDPlayer::Tick(float DeltaSeconds)
{
    // Call the base AActor::Tick first to handle generic update logic.
    Super::Tick(DeltaSeconds);

    // Update camera logic based on new player state/position.
    this->UpdatePlayerCamera();

    // Check if the "Locally Observed" state has changed.
    // This is important for optimizations (e.g., hiding/showing meshes, updating HUD).
    bool bCurrentlyLocallyObserved = this->IsLocallyObserved();
    if (bCurrentlyLocallyObserved != this->_wasLocallyObserved)
    {
        this->_wasLocallyObserved = bCurrentlyLocallyObserved;
        this->OnLocallyObservedChanged(bCurrentlyLocallyObserved);
    }

    // Velocity smoothing logic.
    // This block calculates weighted averages of velocity over time buffers (_averageVelocity and _almostCurrentVelocity).
    // It handles cases where the CharacterMovement might be null or invalid.
    if (this->CharacterMovement != nullptr && IsValid(this->CharacterMovement))
    {
        FVector CurrentVelocity = this->CharacterMovement->Velocity;
        
        // Calculate _averageVelocity
        // Simple exponential smoothing or similar filter: Avg = Avg + (New - Avg) * Alpha
        // Alpha is derived from DeltaSeconds and AverageSpeedBufferTime.
        float AlphaAvg = 0.0f;
        if (this->AverageSpeedBufferTime > 0.0f)
        {
            AlphaAvg = FMath::Min(DeltaSeconds / this->AverageSpeedBufferTime, 1.0f);
        }
        this->_averageVelocity = FMath::Lerp(this->_averageVelocity, CurrentVelocity, AlphaAvg);

        // Calculate _almostCurrentVelocity
        // Similar logic but with a different buffer time (AlmostCurrentSpeedBufferTime).
        float AlphaAlmost = 0.0f;
        if (this->AlmostCurrentSpeedBufferTime > 0.0f)
        {
            AlphaAlmost = FMath::Min(DeltaSeconds / this->AlmostCurrentSpeedBufferTime, 1.0f);
        }
        this->_almostCurrentVelocity = FMath::Lerp(this->_almostCurrentVelocity, CurrentVelocity, AlphaAlmost);
    }

    // Update Stamina.
    // If the recharge delay timer is done, regenerate stamina.
    if (this->_staminaRechargeDelayTimer.IsDone())
    {
        // Regenerate stamina up to a maximum (likely 1.0 or 100.0).
        // Formula: Stamina += Rate * DeltaSeconds
        this->_stamina = FMath::Min(this->_stamina + (this->_staminaRechargeRate * DeltaSeconds), 1.0f);
    }

    // Update various gameplay timers.
    this->_staminaRechargeDelayTimer.Update(DeltaSeconds);
    this->_trapImmunityTimer.Update(DeltaSeconds);
    this->_postChaseTimer.Update(DeltaSeconds);

    // Handle gamepad emulation for mouse yaw input (e.g., for analog stick simulation).
    if (this->IsLocallyControlled())
    {
        if (this->_emulateGamepadForMouseYaw)
        {
            // Update the emulated yaw input value based on decay and deadzones.
            // ... (Complex logic involving _emulatedYawInput, _emulatedYawInputReleaseTimer, etc.) ...
            
            // Apply the emulated input as controller yaw.
            this->AddControllerYawInput(this->_emulatedYawInput * this->GamePadTurnRate * DeltaSeconds);
        }
        else
        {
            this->_emulatedYawInput = 0.0f;
        }
        
        // Handle "Fast Interact" input persistence.
        if (!this->_fastInteractInputPersistence.IsDone())
        {
            if (this->TryInteractionType(EInputInteractionType::VE_FastInteract, this))
            {
                this->_fastInteractInputPersistence.Stop();
            }
            this->_fastInteractInputPersistence.Update(DeltaSeconds);
        }

        // Check for "Item Use" input.
        if (this->_playerData->bUseItemInputPressed)
        {
            // Logic to check if an item interaction is available and valid.
            // If so, attempt to start the interaction.
            // ...
        }
    }

    // Montage logic handling.
    // If in "Montage Mode", ensure the correct animation handling is occurring.
    if (this->_isMontageMode)
    {
        if (this->Mesh != nullptr)
        {
            UAnimInstance* AnimInstance = this->Mesh->GetAnimInstance();
            // If no montage is playing, potentially exit montage mode or handle state change.
            if (AnimInstance && !AnimInstance->Montage_IsPlaying(nullptr))
            {
                this->OnMontageEndedWrapper(); // Virtual function at 0xFD8
            }
        }
    }

    // Match State Logic (Server Only).
    if (this->HasAuthority()) // Role == ROLE_Authority
    {
        // Check if the match has begun.
        if (this->GetWorld()->HasBegunPlay())
        {
            // Check for Loadout Spawning.
            // If the match is InProgress and the loadout hasn't been marked as spawned...
            if (this->GetWorld()->GetGameState()->MatchState == MatchState::InProgress)
            {
                if (!this->_loadingTimer.IsDone())
                {
                    this->_loadingTimer.Update(DeltaSeconds);
                }

                if (this->_loadingTimer.IsDone())
                {
                    this->Server_PlayerReady();
                }

                if (!this->_playerData->bLoadoutSpawned && this->Local_IsLoadoutItemLoaded())
                {
                    UE_LOG(GameFlow, Log, TEXT("SetLoadoutSpawned."));
                    this->_playerData->bLoadoutSpawned = true;
                    this->_playerData->Server_LoadoutSpawned();
                }
            }
        }
    }

    // Update State Machine (if valid and not yet launched).
    if (!this->_smLaunched && this->_SM != nullptr)
    {
        // ... validity checks ...
        // Start the state machine.
        this->StartStateMachine(this->IsLocallyControlled());
    }

    // Calculate Frame Displacement as Speed.
    // Used for animation or physics calculations requiring frame-by-frame velocity.
    if (this->RootComponent != nullptr)
    {
        FVector CurrentPos = this->RootComponent->GetComponentLocation();
        // Calculate displacement
        // ...
        this->_previousPosition = CurrentPos; // Update for next frame
        
        // Save valid location (likely for anti-cheat or stuck detection).
        this->SaveLastValidLocation(DeltaSeconds);
    }

    // Clamp Pitch Rotation.
    float ClampedPitch = this->ClampPitch();
    // Potentially apply this clamped pitch to the controller or pawn.
    // ...

    // Update Strafe Camera.
    this->UpdateStrafeCamera(DeltaSeconds);

    // Crouch Logic.
    // If crouch button is pressed but we aren't crouching yet, try to crouch.
    if (this->_crouchPressed && !this->bIsCrouched)
    {
        this->Crouch(); // Virtual function 0xD70
    }

    // Flashlight Blindness Logic.
    this->_recentlyBlindedTimer.Update(DeltaSeconds);

    if (this->HasAuthority())
    {
        this->Authority_UpdateBlinded(DeltaSeconds);

        // Process blinding effectors (players attempting to blind this character).
        // ... (Iterate through _blindingEffectors map, update charges, remove invalid ones) ...
    }

    // Test Spawn Loadout CVar check.
    static IConsoleVariable* CVarTestSpawnLoadout = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.TestSpawnLoadout"));
    if (CVarTestSpawnLoadout->GetInt() != 0 && this->HasAuthority())
    {
        this->TestSpawnLoadout(); // Virtual function 0xF00
    }
}

// starts at line 1334
void ADBDPlayer::AuthorityHandleLoadoutOnSpawn()
{
    // Retrieve the Game Instance and safely cast it to the custom UDBDGameInstance class
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the game instance is valid and if the player has network authority 
    // (the value 3 corresponds to the ROLE_Authority enum value in ENetRole)
    if (dbdGameInstance != nullptr && this->Role == ROLE_Authority)
    {
        // Retrieve the standard Unreal Engine Player State
        APlayerState* playerState = this->PlayerState;

        // The disassembly does not explicitly check if playerState is null before dereferencing it 
        // to copy the UniqueId, but adding a check is standard safe C++ practice.
        if (playerState != nullptr)
        {
            // Access the UniqueId from the PlayerState.
            // The exact layout of APlayerState is missing from the provided structures, so it is marked.
            FUniqueNetIdRepl uniqueNetId = playerState->UniqueId;

            // Retrieve the Persistent Data manager from the DBD Game Instance (offset 0x3B8).
            UDBDPersistentData* persistentDataManager = dbdGameInstance->_persistentData;

            if (persistentDataManager != nullptr)
            {
                // Fetch the persistent data for this specific player using their Unique ID
                FPlayerPersistentData* playerPersistentData = persistentDataManager->GetPlayerPersistentData(uniqueNetId);

                if (playerPersistentData != nullptr)
                {
                    // The GamePresetData is stored at offset 0x90 inside the UDBDPersistentData object.
                    const FGamePresetData& gamePresetData = persistentDataManager._gamePersistentData.GamePresetData;
                    
                    // Update the character's loadout and data from the game preset
                    playerPersistentData->Local_UpdateCharacterDataFromGamePreset(gamePresetData);
                }
            }
        }
    }
}

// starts at line 1347
void ADBDPlayer::DBD_ToggleDebugInteractionInZone()
{
    // Toggle the debug interaction in zone flag stored at offset 0x1082
    // Comparison checks if the value is 0 (false) and sets it to 1 (true), otherwise 0
    this->_debugInteractionInZone = (this->_debugInteractionInZone == false);
}

// starts at line 1352
void ADBDPlayer::DBD_DebugStartInteraction()
{
    // Check if the player is locally controlled by looking up the VTable
    // Assembly: call qword ptr [rax+630h] -> IsLocallyControlled()
    if (this->IsLocallyControlled() == true)
    {
        // Attempt to perform an interaction with a specific type (1)
        // 'this' is passed as the requester
        this->TryInteractionType(EInputInteractionType::VE_Interact, this);

        // Access the PlayerData member (Offset 0x0D08)
        // Modify the boolean flag at offset 0xE9 inside UDBDPlayerData
        // Based on pseudo-code, this member is identified as _interactionInputPressed
        if (this->_playerData != nullptr)
        {
            this->_playerData->_interactionInputPressed = true;
        }

        // Send the interaction input to the server via RPC
        // The argument passed is 'true' (1)
        this->Server_SendInteractionInput(true);
    }
}

// starts at line 1360
void ADBDPlayer::UpdateRTPCValues()
{
    // Only update if the player is locally observed (e.g., the local player or spectated target).
    // Accessing a custom function or property designated as 'IsLocallyObserved'.
    if (this->IsLocallyObserved())
    {
        FRotator ControlRotation = FRotator::ZeroRotator;

        // Determine how to get the control rotation based on controller status and network role.
        // Role 3 corresponds to ROLE_Authority.
        if (this->IsLocallyControlled() || this->Role == ROLE_Authority)
        {
            ControlRotation = this->GetControlRotation();
        }
        else
        {
            // If not locally controlled (e.g., simulated proxy), retrieve rotation from custom player data.
            // Accessing _playerData member at offset 0xD08 and its _controlRotation member.
            if (this->_playerData != nullptr)
            {
                // Accessing _controlRotation from UDBDPlayerData
                ControlRotation = this->_playerData._controlRotation; 
            }
        }

        // Normalize the rotation values to be within the range [0, 360) or [-180, 180).
        // The disassembly performs vector math (SIMD) to normalize the Pitch, Yaw, and Roll.
        // This effectively mirrors FRotator::Normalize() logic but manually implemented in assembly.
        ControlRotation.Normalize();

        // Calculate the value for the Audio RTPC based on the Yaw.
        // The algorithm in disassembly is: floor((Yaw * 2.0 - 0.5) / 2) -> essentially Yaw as integer.
        // It treats the angle as a value for the audio engine.
        float ListenerAngleValue = (float)((int)((ControlRotation.Yaw * 2.0f) - 0.5f) >> 1);

        // Get the Audio Device and set the RTPC value.
        FAkAudioDevice* AudioDevice = FAkAudioDevice::Get();
        if (AudioDevice != nullptr)
        {
            // "AudioRTPC_Listener_Angle_Absolute"
            AudioDevice->SetRTPCValue(TEXT("AudioRTPC_Listener_Angle_Absolute"), ListenerAngleValue, 0, nullptr);
        }

        // Call the helper function to update proximity logic for audio (Border/Escape tiles).
        this->UpdateExteriorProximity();
    }
}

// starts at line 1374
void ADBDPlayer::UpdateExteriorProximity()
{
    // Retrieve the current world from the player
    UWorld* World = this->GetWorld();
    if (World == nullptr)
    {
        return;
    }

    // Initialize player location and rotation to defaults
    FVector PlayerLocation = FVector::ZeroVector;
    FRotator PlayerRotation = FRotator::ZeroRotator;

    if (this->RootComponent != nullptr)
    {
        PlayerLocation = this->RootComponent->GetComponentLocation();
        PlayerRotation = this->RootComponent->GetComponentRotation();
    }

    // Initialize closest points to max float values
    FVector ClosestPoint = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
    FVector SecondClosestPoint = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
    
    // Get BorderTile class
    UClass* BorderTileClass = ABorderTile::StaticClass();
    if (BorderTileClass != nullptr)
    {
        // Iterate over all ABorderTile instances in the world
        for (TActorIterator<AActor> It(World, BorderTileClass); It; ++It)
        {
            AActor* Tile = *It;
            FBox Bounds = Tile->GetComponentsBoundingBox(false);

            // Clamp player location to the tile bounds to find the closest point on the bounding box
            FVector PointOnBox;
            PointOnBox.X = FMath::Clamp(PlayerLocation.X, Bounds.Min.X, Bounds.Max.X);
            PointOnBox.Y = FMath::Clamp(PlayerLocation.Y, Bounds.Min.Y, Bounds.Max.Y);
            PointOnBox.Z = FMath::Clamp(PlayerLocation.Z, Bounds.Min.Z, Bounds.Max.Z);

            // Calculate squared distances
            float DistToNewSq = FVector::DistSquared(PlayerLocation, PointOnBox);
            float DistToClosestSq = FVector::DistSquared(PlayerLocation, ClosestPoint);

            // Original logic only updates the second closest point when a new closest point is found
            if (DistToNewSq <= DistToClosestSq)
            {
                SecondClosestPoint = ClosestPoint;
                ClosestPoint = PointOnBox;
            }
        }
    }

    // Get EscapeTile class
    UClass* EscapeTileClass = AEscapeTile::StaticClass();
    if (EscapeTileClass != nullptr)
    {
        // Iterate over all AEscapeTile instances in the world
        for (TActorIterator<AActor> It(World, EscapeTileClass); It; ++It)
        {
            AActor* Tile = *It;
            FBox Bounds = Tile->GetComponentsBoundingBox(false);

            // Clamp player location to the tile bounds to find the closest point on the bounding box
            FVector PointOnBox;
            PointOnBox.X = FMath::Clamp(PlayerLocation.X, Bounds.Min.X, Bounds.Max.X);
            PointOnBox.Y = FMath::Clamp(PlayerLocation.Y, Bounds.Min.Y, Bounds.Max.Y);
            PointOnBox.Z = FMath::Clamp(PlayerLocation.Z, Bounds.Min.Z, Bounds.Max.Z);

            // Calculate squared distances
            float DistToNewSq = FVector::DistSquared(PlayerLocation, PointOnBox);
            float DistToClosestSq = FVector::DistSquared(PlayerLocation, ClosestPoint);

            // Original logic only updates the second closest point when a new closest point is found
            if (DistToNewSq <= DistToClosestSq)
            {
                SecondClosestPoint = ClosestPoint;
                ClosestPoint = PointOnBox;
            }
        }
    }

    // Update Wwise Audio RTPC values
    FAkAudioDevice* AudioDevice = FAkAudioDevice::Get();
    if (AudioDevice != nullptr)
    {
        // ---------------------------------------------------------
        // Calculate and set RTPC values for the Closest Point
        // ---------------------------------------------------------
        
        // Using absolute point instead of direction to perfectly match original assembly instructions
        FRotator TargetRot1 = FRotationMatrix::MakeFromXZ(ClosestPoint, FVector::UpVector).Rotator();
        FRotator DeltaRot1 = (TargetRot1 - PlayerRotation).GetNormalized();
        float Angle1 = DeltaRot1.Yaw;
        
        float Distance1 = FVector::Dist(PlayerLocation, ClosestPoint);
        
        AudioDevice->SetRTPCValue(TEXT("AudioRTPC_LimitPoint_Angle_01"), Angle1, 0, this);
        AudioDevice->SetRTPCValue(TEXT("AudioRTPC_LimitPoint_Distance_01"), Distance1, 0, this);

        // ---------------------------------------------------------
        // Calculate and set RTPC values for the Second Closest Point
        // ---------------------------------------------------------
        
        // Using absolute point instead of direction to perfectly match original assembly instructions
        FRotator TargetRot2 = FRotationMatrix::MakeFromXZ(SecondClosestPoint, FVector::UpVector).Rotator();
        FRotator DeltaRot2 = (TargetRot2 - PlayerRotation).GetNormalized();
        float Angle2 = DeltaRot2.Yaw;
        
        float Distance2 = FVector::Dist(PlayerLocation, SecondClosestPoint);
        
        AudioDevice->SetRTPCValue(TEXT("AudioRTPC_LimitPoint_Angle_02"), Angle2, 0, this);
        AudioDevice->SetRTPCValue(TEXT("AudioRTPC_LimitPoint_Distance_02"), Distance2, 0, this);
    }
}

// starts at line 1441
FRotator ADBDPlayer::GetRepControlRotation() const
{
    // 1. Check if we have the "True" Control Rotation
    // IsLocallyControlled(): Returns true if this is our own character.
    // HasAuthority(): Returns true if we are the Server (Role == ROLE_Authority).
    if (this->IsLocallyControlled() || this->HasAuthority())
    {
        // Use the standard APawn function which queries the Controller.
        return this->GetControlRotation();
    }

    // 2. Fallback to Replicated Data for Remote Clients
    // If we are a client looking at another player, their Controller doesn't exist on our machine.
    // We must read the replicated rotation from the PlayerData component.
    // The assembly accesses offset 0xF8 inside the _playerData component (Offset 0xD08).
    if (this->_playerData)
    {
        // Returns the rotation value synced from the server
        return this->_playerData->GetControlRotation();
    }

    return FRotator::ZeroRotator;
}

// starts at line 1465
FVector ADBDPlayer::GetCameraAnchorLocation() const
{
    // Retrieve the custom CameraAnchor component (Offset 0x920)
    USceneComponent* Anchor = this->CameraAnchor;

    // Check if the anchor component exists
    if (Anchor != nullptr)
    {
        // Validate the object (GUObjectArray check in disassembly)
        // Checks if the object is reachable and part of the global object array
        if (Anchor->IsValidLowLevel() == true)
        {
            // Return the world location of the anchor
            // The disassembly reads offset 0x140 (ComponentToWorld.Translation)
            return Anchor->GetComponentLocation();
        }
    }

    // Fallback: If no valid camera anchor is found, try the RootComponent
    // Offset 0x160 corresponds to the standard RootComponent in AActor
    if (this->GetRootComponent() != nullptr)
    {
        return this->GetRootComponent()->GetComponentLocation();
    }

    // Final Fallback: Return (0,0,0) if no components are valid
    return FVector::ZeroVector;
}

// starts at line 1480
void ADBDPlayer::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
    // Bind movement axes to virtual functions (likely wrappers for AddMovementInput)
    // "MoveForward" -> Virtual Function 4064 (e.g., MoveForward)
    InputComponent->BindAxis("MoveForward", this, &ADBDPlayer::OnMoveForwardInput);
    
    // "MoveRight" -> Virtual Function 4072 (e.g., MoveRight)
    InputComponent->BindAxis("MoveRight", this, &ADBDPlayer::OnMoveRightInput);
    
    // "MoveUp" -> Explicitly named function
    InputComponent->BindAxis("MoveUp", this, &ADBDPlayer::OnMoveUpInput);

    // Bind rotation axes (Mouse/Stick input)
    // "Turn" -> Virtual Function 3408 (Likely APawn::AddControllerYawInput)
    InputComponent->BindAxis("Turn", this, &APawn::Turn);
    
    // "TurnConstant" -> Virtual Function 3392 (Likely for Gamepad rate-based turning)
    InputComponent->BindAxis("TurnConstant", this, &ADBDPlayer::TurnAtRate);

    // "LookUp" -> Virtual Function 3416 (Likely APawn::AddControllerPitchInput)
    InputComponent->BindAxis("LookUp", this, &ADBDPlayer::LookUp);
    
    // "LookUpConstant" -> Virtual Function 3400 (Likely for Gamepad rate-based looking)
    InputComponent->BindAxis("LookUpConstant", this, &ADBDPlayer::LookUpAtRate);

    // Bind Crouch actions
    InputComponent->BindAction("Crouch", IE_Pressed, this, &ADBDPlayer::CrouchInputPressed);
    InputComponent->BindAction("Crouch", IE_Released, this, &ADBDPlayer::CrouchInputReleased);

    // Bind logic for detecting input method switches (Keyboard vs Gamepad)
    InputComponent->BindAxis("SwitchToGamepadControlsFromAxis", this, &ADBDPlayer::SwitchToGamepadControlsFromAxis);
    InputComponent->BindAction("SwitchToGamepadControls", IE_Pressed, this, &ADBDPlayer::SwitchToGamepadControls);
    InputComponent->BindAction("SwitchToKeyboardControls", IE_Pressed, this, &ADBDPlayer::SwitchToKeyboardControls);

    // Bind utility actions
    InputComponent->BindAction("ToggleInvertY", IE_Pressed, this, &ADBDPlayer::ToggleInvertLookUpAxis);
}

// starts at line 1505
void ADBDPlayer::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
    // Check if the multicast delegate has any bound functions to invoke.
    // The disassembly directly accesses the InvocationList.ArrayNum, which translates to IsBound() in UE C++.
    if (this->OnMovementInput.IsBound() == true)
    {
        // Notify the server about the movement input
        this->Server_OnMovementInput(this);
    }

    // Call the parent class implementation to handle standard pawn movement
    APawn::AddMovementInput(WorldDirection, ScaleValue, bForce);

    // Verify if the player possesses a valid controller
    if (this->Controller != nullptr)
    {
        // The disassembly performs internal Unreal Engine type-casting and safety checks 
        // (verifying it is not pending kill via GUObjectArray and checking the ClassTreeIndex).
        // This is perfectly represented by a standard UE Cast.
        ADBDPlayerController* PlayerController = Cast<ADBDPlayerController>(this->Controller);
        
        if (PlayerController != nullptr)
        {
            // Modifies a boolean value at offset 0x960 inside ADBDPlayerController.
            PlayerController->_hasMoveInputThisFrame = true;
        }
    }
}

// starts at line 1528
void ADBDPlayer::Server_OnMovementInput_Implementation(ADBDPlayer* Player)
{
    // Broadcast the OnMovementInput dynamic multicast delegate.
    // The disassembly marshals the Player argument and calls ProcessMulticastDelegate on the member at offset 0x7C8.
    this->OnMovementInput.Broadcast(Player);
}

// starts at line 1533
void ADBDPlayer::OnMoveForwardInput(float Val)
{
    // Local variable to handle the input value, potentially modified by logic
    float LocalVal = Val;

    // Check if the player is currently allowed to input navigation commands
    if (this->AllowNavigationInput != false)
    {
        // Check if backward navigation is specifically disallowed
        if (this->AllowNavigationBackwardInput == false)
        {
            // If backward input is disabled, clamp negative values to 0.0
            LocalVal = FMath::Max(Val, 0.0f);
        }

        // Call a virtual function at offset 0xD30. 
        // Based on context, this likely handles the actual movement logic (e.g., AddMovementInput).
        this->MoveForward(LocalVal);
    }

    // Check if the absolute value of the input is greater than a small tolerance (Deadzone check)
    // 0.0001f represents the constant 38d1b717 (approx 1.0e-4) found in disassembly
    if (FMath::Abs(LocalVal) > 0.0001f)
    {
        // Calculate the world space movement vector based on the Actor's Forward Vector
        FVector MovementVector = FVector::ForwardVector * LocalVal;

        // Access the movement input event handler. 
        // This is a TSharedPtr<PollableEvent<FVector>>, accessing the internal object.
        if (this->MovementInputEvent.IsValid())
        {
            // Add the calculated movement vector to the event buffer's array.
            // The disassembly explicitly performs a TArray::Add / ResizeGrow operation here.
            // _eventBuffer is inferred from the Pseudo Code and TArray structure offsets (0x8 Data, 0x10 Num).
            this->MovementInputEvent->_eventBuffer.Add(MovementVector);
        }
    }
}

// starts at line 1550
void ADBDPlayer::MoveForward(float Val)
{
    // Ensure we have a controller and non-zero input
    // Disassembly: cmp qword ptr [rcx+3B8h], 0; ucomiss xmm6, xmm0
    if (this->Controller != nullptr && Val != 0.0f)
    {
        FRotator RotationToUse;

        // Check movement mode (Ground or Falling)
        // Disassembly: Call IsMovingOnGround, Call IsFalling
        if (this->CharacterMovement->IsMovingOnGround() || 
            this->CharacterMovement->IsFalling())
        {
            // When walking/falling, move relative to the character's facing direction.
            // Disassembly: Access RootComponent->ComponentToWorld.Rotation
            RotationToUse = this->GetActorRotation();
        }
        else
        {
            // For other modes (e.g. maybe Flying/Swimming/Debug), move relative to where the camera is looking.
            // Disassembly: Call Controller->GetControlRotation
            RotationToUse = this->Controller->GetControlRotation();
        }

        // Convert the rotation into a forward direction vector.
        // Disassembly: FRotationMatrix::FRotationMatrix(RotationToUse)
        // Then extracts the X-axis (Forward vector) from the matrix columns.
        // Disassembly: movss xmm2, dword ptr [rax] (Matrix.M[0][0])
        // Disassembly: movss xmm1, dword ptr [rax+8] (Matrix.M[0][1] ? No, Matrix is column major usually, 
        // effectively getting GetUnitAxis(EAxis::X))
        FVector Direction = FRotationMatrix(RotationToUse).GetScaledAxis(EAxis::X);

        // Apply movement speed multiplier (possibly a debug or perk modifier).
        // Disassembly: mulss xmm1, xmm0 (where xmm0 is _movementSpeedMultiplier offset 0xE40)
        Direction *= this->_movementSpeedMultiplier;

        // Add the movement input to the pawn.
        // Disassembly: Call AddMovementInput
        this->AddMovementInput(Direction, Val);
    }
}

// starts at line 1562
void ADBDPlayer::OnMoveRightInput(float Val)
{
    // Check if the player is currently allowed to input navigation commands
    if (this->AllowNavigationInput != false)
    {
        // Call a virtual function at offset 0xD38.
        // Based on the context of 'OnMoveRightInput', this likely handles the actual right/strafe movement logic.
        this->MoveRight(Val);
    }

    // Check if the absolute value of the input is greater than a small tolerance (Deadzone check)
    // 0.0001f represents the constant 38d1b717 (approx 1.0e-4) found in disassembly
    if (FMath::Abs(Val) > 0.0001f)
    {
        // Calculate the world space movement vector based on the Actor's Right Vector
        FVector MovementVector = FVector::RightVector * Val;

        // Access the movement input event handler.
        // This is a TSharedPtr<PollableEvent<FVector>>, accessing the internal object.
        if (this->MovementInputEvent.IsValid())
        {
            // Add the calculated movement vector to the event buffer's array.
            // The disassembly explicitly performs a TArray::Add / ResizeGrow operation here.
            this->MovementInputEvent->_eventBuffer.Add(MovementVector);
        }
    }
}

// starts at line 1575
void ADBDPlayer::MoveRight(float Val)
{
    // Ensure non-zero input
    // Disassembly: ucomiss xmm6, xmm0 (Val != 0.0)
    if (Val != 0.0f)
    {
        // Retrieve the Root Component (Capsule)
        // Disassembly: mov rbx, [rcx+160h]
        USceneComponent* RootComp = this->RootComponent;

        if (RootComp != nullptr)
        {
            // Get the Actor's Rotation (from WorldRotationCache/ComponentToWorld)
            // Note: Unlike MoveForward, there is no branch here to check IsMovingOnGround().
            // Strafing appears to always be relative to the Character's facing direction.
            FRotator ActorRotation = RootComp->GetComponentRotation();

            // Create a Rotation Matrix from the actor's rotation
            // Disassembly: FRotationMatrix::FRotationMatrix(...)
            FRotationMatrix Matrix(ActorRotation);

            // Extract the Y-Axis (Right Vector).
            // Disassembly: Accesses offset +0x10 (Column 1 in UE4 Matrix)
            FVector RightVector = Matrix.GetScaledAxis(EAxis::Y);

            // Apply the custom movement speed multiplier.
            // Disassembly: mulss xmm1, dword ptr [rdi+0E40h] (_movementSpeedMultiplier)
            RightVector *= this->_movementSpeedMultiplier;

            // Apply the movement input.
            // Disassembly: Calls AddMovementInput with the calculated vector.
            // The input 'Val' (Scale) is likely passed as the second argument (r8/xmm1 setup).
            this->AddMovementInput(RightVector, Val);
        }
    }
}

// starts at line 1585
void ADBDPlayer::OnMoveUpInput(float Val)
{
    // Check if the input value is not exactly zero
    if (Val != 0.0f)
    {
        // Call the standard Unreal Engine AddMovementInput function
        // Passes the global UpVector as the direction and Val as the scale
        // Corresponds to the virtual function call at offset 0x6A8 found in disassembly
        this->AddMovementInput(FVector::UpVector, Val, false);
    }

    // Check if the absolute value of the input is greater than a small tolerance (Deadzone check)
    // 0.0001f represents the constant 38d1b717 (approx 1.0e-4) found in disassembly
    if (FMath::Abs(Val) > 0.0001f)
    {
        // Calculate the world space movement vector based on the global Up Vector
        FVector MovementVector = FVector::UpVector * Val;

        // Access the movement input event handler.
        // This is a TSharedPtr<PollableEvent<FVector>>, accessing the internal object.
        if (this->MovementInputEvent.IsValid())
        {
            // Add the calculated movement vector to the event buffer's array.
            // The disassembly explicitly performs a TArray::Add / ResizeGrow operation here.
            this->MovementInputEvent->_eventBuffer.Add(MovementVector);
        }
    }
}

// starts at line 1598
void ADBDPlayer::TurnAtRate(float Val)
{
    // Retrieve the curve asset defining the gamepad yaw response.
    // Offset 0xB60 corresponds to 'GamepadYawCurve'.
    UCurveFloat* Curve = this->GamepadYawCurve;

    // Validate the curve object exists and is valid.
    if (Curve != nullptr && IsValid(Curve))
    {
        // Determine the sign of the input (-1.0, 0.0, or 1.0).
        // This allows the curve to be defined only for positive values (0 to 1) 
        // while applying symmetrically to both left and right turns.
        float Sign = 0.0f;
        if (Val > 0.0f)
        {
            Sign = 1.0f;
        }
        else if (Val < 0.0f)
        {
            Sign = -1.0f;
        }

        // Evaluate the curve using the absolute value of the input.
        // The curve maps the raw stick input (X-axis) to the desired turn rate (Y-axis).
        float EvaluatedRate = Curve->FloatCurve.Eval(FMath::Abs(Val));

        // Apply the direction (sign) back to the evaluated magnitude.
        // Store the result in _yawRateInput (Offset 0x10B8).
        this->_yawRateInput = EvaluatedRate * Sign;
    }
    else
    {
        // If no curve is provided, pass the raw input value directly.
        this->_yawRateInput = Val;
    }
}

// starts at line 1611
void ADBDPlayer::LookUpAtRate(float Val)
{
    // 1. Apply Response Curve (Gamepad Sensitivity Curve)
    // Disassembly: Accesses member at offset 0xB68 (GamepadPitchCurve)
    UCurveFloat* PitchCurve = this->GamepadPitchCurve;
    
    if (PitchCurve != nullptr && PitchCurve->IsPendingKill() == false)
    {
        // Calculate the sign of the input (-1.0, 0.0, or 1.0)
        float Sign = (Val >= 0.0f) ? 1.0f : -1.0f;
        if (FMath::IsNearlyZero(Val)) Sign = 0.0f;

        // Evaluate the curve using the absolute value of the input, then re-apply the sign.
        // This allows for non-linear sensitivity (e.g., fine control at low tilt, fast turn at max tilt).
        // Disassembly: call ?Eval@FRichCurve@@QEBAMMM@Z
        float AbsVal = FMath::Abs(Val);
        Val = PitchCurve->GetFloatValue(AbsVal) * Sign;
    }

    // 2. Handle Axis Inversion
    // Disassembly: cmp byte ptr [rbx+0FE4h], 0
    float InvertMult = this->_invertLookUpAxis ? 1.0f : -1.0f;

    // 3. Calculate Delta Rotation
    // Formula: Input * BaseRate * DeltaTime * Inversion
    // Disassembly: mulss xmm7, dword ptr [rbx+0C30h] (GamePadLookUpRate)
    // Disassembly: mulss xmm7, dword ptr [rax+8CCh] (DeltaTimeSeconds)
    float DeltaPitch = Val * this->GamePadLookUpRate * this->GetWorld()->GetDeltaSeconds() * InvertMult;

    // 4. Apply to Player Controller
    if (!FMath::IsNearlyZero(DeltaPitch))
    {
        AController* Ctrl = this->Controller;
        if (Ctrl != nullptr && Ctrl->IsLocalController())
        {
            // Cast to custom player controller
            // Disassembly: call ??$SafeCast@VADBDPlayerController@@...
            ADBDPlayerController* DBDCtrl = Cast<ADBDPlayerController>(Ctrl);
            
            if (DBDCtrl != nullptr)
            {
                // Check if input is currently ignored
                if (!DBDCtrl->IsLookInputIgnored())
                {
                    // Apply a constant scaling factor from the controller
                    // Disassembly: movss xmm6, dword ptr [rax+9DCh] (_constantPitchScale)
                    float FinalPitch = DeltaPitch * DBDCtrl->_constantPitchScale;

                    // Directly accumulate the rotation input into the controller's struct
                    // Disassembly: addss xmm6, dword ptr [rbx+570h]; movss dword ptr [rbx+570h], xmm6
                    DBDCtrl->RotationInput.Pitch += FinalPitch;
                }
            }
        }
    }
}

// starts at line 1621
void ADBDPlayer::Turn(float Val)
{
    // Check if the camera is currently resetting.
    // Offset 0xC05 likely corresponds to the boolean flag '_cameraResetting'.
    // If resetting, ignore turn input.
    if (!this->_cameraResetting)
    {
        // Check if mouse yaw should be emulated as gamepad input.
        // Offset 0xE70 corresponds to '_emulateGamepadForMouseYaw'.
        if (this->_emulateGamepadForMouseYaw)
        {
            // Calculate absolute value of input.
            float AbsVal = FMath::Abs(Val);
            
            // Check if input exceeds a minimum threshold scaled by delta time.
            // This prevents tiny mouse jitters from registering as stick input.
            float Threshold = this->_emulatedYawMinimumInput * this->GetWorld()->DeltaTimeSeconds;
            
            if (AbsVal > Threshold)
            {
                // Reset the timer that decays the emulated input back to zero.
                this->_emulatedYawInputReleaseTimer.Reset(this->_emulatedYawInputReleaseTime);

                // Accumulate the input into the emulated yaw value.
                // Formula: NewInput = (Val * MouseTurnSpeedMultiplier * Strength) + CurrentEmulatedInput
                float NewInput = (Val * this->MouseTurnSpeedMultiplier * this->_emulatedYawInputStrength) + this->_emulatedYawInput;
                
                // Clamp and normalize the result to stay within valid stick ranges (-1.0 to 1.0).
                float Sign = 0.0f;
                if (NewInput > 0.0f)
                {
                    Sign = 1.0f;
                }
                else if (NewInput < 0.0f)
                {
                    Sign = -1.0f;
                }

                this->_emulatedYawInput = FMath::Min(FMath::Abs(NewInput), 1.0f) * Sign;
            }
        }
        else
        {
            // Standard mouse turn behavior.
            // Directly add yaw input scaled by the turn speed multiplier.
            // The second parameter '0' indicates this is NOT a 'constant' (rate-based) input, but an immediate value.
            this->DBDAddControllerYawInput(Val * this->MouseTurnSpeedMultiplier, false);
        }
    }
}

// starts at line 1651
void ADBDPlayer::LookUp(float Val)
{
    // Check if the camera is currently resetting (e.g., center camera action).
    // If so, ignore mouse look input.
    // Disassembly: cmp byte ptr [rcx+0C05h], 0; jnz ...
    if (this->_cameraResetting)
    {
        return;
    }

    // Handle Axis Inversion.
    // Logic: If _invertLookUpAxis is FALSE (0), we Negate the input.
    //        If _invertLookUpAxis is TRUE (1), we keep the input as is.
    // Disassembly: cmp byte ptr [rcx+0FE4h], 0; jnz ... xorps xmm1, ... (Negate)
    if (!this->_invertLookUpAxis)
    {
        Val = -Val;
    }

    // Apply the mouse look speed multiplier (Sensitivity).
    // Disassembly: mulss xmm1, dword ptr [rcx+0C38h]
    float ScaledVal = Val * this->MouseLookUpSpeedMultiplier;

    // Apply the pitch input to the controller.
    // Disassembly: jmp ?DBDAddControllerPitchInput@ADBDPlayer@@QEAAXM_N@Z
    this->DBDAddControllerPitchInput(ScaledVal, false);
}

// starts at line 1660
void ADBDPlayer::StopAllMovement()
{
    // Access the CharacterMovement component (Offset 0x3E8).
    // Note: The assembly does not perform a null check, assuming the component is valid.
    
    // Immediately halt physical movement.
    // Calls virtual function at offset 0x388 (StopMovementImmediately).
    this->CharacterMovement->StopMovementImmediately();

    // Consume the pending input vector.
    // This ensures that if the player was holding a direction key, that input is cleared 
    // and doesn't immediately re-trigger movement in the next tick.
    // Calls virtual function at offset 0x4E0 (ConsumeInputVector).
    this->CharacterMovement->ConsumeInputVector();

    // Explicitly zero out the velocity vector to ensure total stillness.
    // Offsets 0xFC and 0x104 correspond to the X/Y and Z components of Velocity.
    this->CharacterMovement->Velocity = FVector::ZeroVector;
}

// starts at line 1672
void ADBDPlayer::ItemUsePressed()
{
    // Delegate to the internal implementation
    this->DoItemUsePressed();
}

// starts at line 1694
void ADBDPlayer::ItemUseReleased()
{
    // Delegate to the internal implementation
    this->DoItemUseReleased();
}

// starts at line 1708
void ADBDPlayer::DoItemUseReleased()
{
    // The disassembly explicitly calls IsLocallyControlled (vtable offset 0x630)
    // However, unlike DoItemUsePressed, there is no conditional jump based on the result in the provided assembly.
    // It proceeds to execute the rest of the function regardless of the return value.
    this->IsLocallyControlled();

    // Send the item use input release to the server via RPC
    // The argument passed is 'false' (0) (xor edx, edx)
    this->Server_SendItemUseInput(false);

    // Access the PlayerData member (Offset 0x0D08)
    // Set the boolean flag at offset 0xEA inside UDBDPlayerData to false (0)
    this->_playerData->_useItemInputPressed = false;
}

// starts at line 1715
void ADBDPlayer::DoItemUsePressed()
{
    // Attempt to start an interaction of type 5
    // Type 5 corresponds to 'UseItem' in the interaction enum (inferred from context/name)
    this->TryInteractionType(EInputInteractionType::VE_ItemUse, this);

    // Access the PlayerData member (Offset 0x0D08)
    // Set the boolean flag at offset 0xEA inside UDBDPlayerData
    // Based on pseudo-code, this member is identified as _useItemInputPressed
    if (this->_playerData != nullptr)
    {
        this->_playerData->_useItemInputPressed = true;
    }

    // Send the item use input to the server via RPC
    // The argument passed is 'true' (1)
    this->Server_SendItemUseInput(true);
}

// starts at line 1722
void ADBDPlayer::ItemDropPressed()
{
    // Attempt to perform the interaction locally.
    // Note: The decompiler output 'VE_Disconnected' is likely a misinterpretation of the Enum value.
    // Based on the function name and previous context, this is EInputInteractionType::VE_ItemDrop (0x06).
    this->TryInteractionType(EInputInteractionType::VE_ItemDrop, this);

    // Update the player data to reflect that the interaction input is currently pressed.
    // Note: VE_ItemDrop (6) shares the same input flag (_interactionInputPressed) as VE_Interact (1)
    // in the IsInteractionInputPressed function.
    if (this->_playerData != nullptr)
    {
        this->_playerData->_interactionInputPressed = true;
    }

    // Send the input event to the server via RPC.
    this->Server_SendItemDropInput(true);
}

// starts at line 1729
void ADBDPlayer::ItemDropReleased()
{
    // Notify the server that the item drop input has been released (bPressed = false)
    this->Server_SendItemDropInput(false);

    // Clear the local interaction input flag in PlayerData
    if (this->_playerData != nullptr)
    {
        this->_playerData->_interactionInputPressed = false;
    }
}

// starts at line 1768
ADBDPlayerCameraManager* ADBDPlayer::GetCameraManager()
{
    // Retrieve the custom Player Controller using the internal helper
    ADBDPlayerController* Controller = this->GetPlayerController();

    // Check if the controller is valid
    if (Controller != nullptr)
    {
        // Retrieve the generic PlayerCameraManager from the controller
        // Offset 0x418 corresponds to the PlayerCameraManager member in APlayerController
        APlayerCameraManager* CameraManager = Controller->PlayerCameraManager;

        // Validate the camera manager instance
        if (CameraManager != nullptr)
        {
            // Perform a safe cast to ADBDPlayerCameraManager
            // The disassembly explicitly checks the class hierarchy (GetPrivateStaticClass + ClassTreeIndex comparison)
            // which is the internal implementation of the Cast<> template.
            return Cast<ADBDPlayerCameraManager>(CameraManager);
        }
    }

    return nullptr;
}

// starts at line 1784
ADBDPlayerController* ADBDPlayer::GetPlayerController() const
{
    // 1. Retrieve the generic Controller (Offset 0x3B8 in APawn)
    // The assembly accesses [rcx+3B8h] directly.

    // 2. Perform a Safe Cast
    // The assembly calls GetPrivateStaticClass for ADBDPlayerController and checks 
    // the ClassTreeIndex. This is the compiled logic for Cast<T>().
    return Cast<ADBDPlayerController>(this->GetController());
}

// starts at line 1807
void ADBDPlayer::Server_CheatDispatchOnPostItemAddonsCreation_Implementation()
{
    this->DispatchOnPostAddonsCreation();
}

// starts at line 1817
void ADBDPlayer::Server_SendItemUseInput_Implementation(bool Pressed)
{
    // Update the player data to reflect the current state of the item use input.
    // The disassembly writes the boolean value to offset 0xEA of the _playerData object.
    if (this->_playerData != nullptr)
    {
        this->_playerData->_useItemInputPressed = Pressed;
    }

    // Process the actual item usage logic based on the input state.
    this->DoUseItem(Pressed);
}

// starts at line 1823
void ADBDPlayer::Server_SendItemUse_Implementation(bool pressed)
{
    // Forward the replication request to the internal item usage handler.
    this->DoUseItem(pressed);
}

// starts at line 1833
void ADBDPlayer::DoUseItem(bool use)
{
    // Check if the inventory contains any items
    if (this->Inventory.Num() > 0)
    {
        // Access the first item in the inventory (Index 0)
        ACollectable* FirstItem = this->Inventory[0];

        if (FirstItem != nullptr)
        {
            // Check if the item is currently equipped
            if (FirstItem->IsEquipped() == true)
            {
                // Logic branch: Are we trying to Use (Press) or Release?
                // If 'use' is true, we also check if the item is capable of being used (CanUse)
                if (use == true && FirstItem->CanUse() == true)
                {
                    // Check a specific boolean flag at offset 0x3A5 in ACollectable
                    bool bIsAlreadyActive = FirstItem->HasUseInteraction;

                    if (bIsAlreadyActive == false)
                    {
                        // Call the virtual function at VTable offset +0x6A0
                        // Context suggests this is 'OnUse'
                        FirstItem->OnUse();
                    }
                }
                else
                {
                    // Use is false (Release) OR CanUse returned false
                    // Call the virtual function at VTable offset +0x6A8
                    // Context suggests this is 'OnUseReleased'
                    FirstItem->OnUseReleased();
                }
            }
        }
    }
}

// starts at line 1852
ACollectable* ADBDPlayer::GetItem() const
{
    // Check if the inventory array contains any items.
    // In UE4, TArray::Num() returns the ArrayNum member (Offset 0xC60 in the ASM).
    if (this->Inventory.Num() > 0)
    {
        // Return the first item in the inventory (Index 0).
        // The ASM dereferences the data pointer (Offset 0xC58) directly to get the first element.
        return this->Inventory[0];
    }

    return nullptr;
}

// starts at line 1857
FName ADBDPlayer::GetItemID() const
{
    // Check if the inventory contains any items (Offset 0xC60)
    if (this->Inventory.Num() > 0)
    {
        // Retrieve the first item from the inventory (Offset 0xC58)
        ACollectable* FirstItem = this->Inventory[0];

        // Validate the item pointer:
        // 1. Check if not null
        // 2. Check internal object validity (GUObjectArray)
        // 3. Check "Pending Kill" flag (Offset 0x140, bit 0x4)
        // IsValid() encapsulates all these checks.
        if (IsValid(FirstItem))
        {
            // Return the ItemID of the first item (Offset 0x3B0)
            return FirstItem->ItemID;
        }
    }

    // Return a default "Empty" name constant if no valid item is found
    return LoadoutSlotDefaultValue::EMPTY;
}

// starts at line 1867
bool ADBDPlayer::IsUsingAimItem() const
{
    // Check if the Inventory array has at least one item
    // Disassembly: cmp dword ptr [rcx+0C60h], 0 (Accesses TArray::Num at offset 0xC60)
    if (this->Inventory.Num() > 0)
    {
        // Retrieve the first item in the inventory
        // Disassembly: mov rax, [rcx+0C58h]; mov rdx, [rax] (Accesses Data pointer at 0xC58)
        ACollectable* CurrentItem = this->Inventory[0];

        // Ensure the item exists
        if (CurrentItem != nullptr)
        {
            // Verify the item is valid (Standard UE4 IsPendingKill/IsActorBeingDestroyed checks)
            // Disassembly: Checks GUObjectArray flags (0x20000000) and Actor flags (offset 0x140)
            if (CurrentItem->IsPendingKill() == false && CurrentItem->IsActorBeingDestroyed() == false)
            {
                // Check two specific boolean flags on the item.
                // Given the function name, these likely correspond to:
                // 1. Is this an item type that *can* be aimed? (e.g., Flashlight/Map vs Medkit)
                // 2. Is the "use" input currently active/is the state "aiming"?
                
                // Disassembly: cmp byte ptr [rdx+3A1h], 0
                bool bIsAimableItem = CurrentItem->AimOnUse;
                
                // Disassembly: cmp byte ptr [rdx+3A9h], 0
                bool bIsCurrentlyAiming = CurrentItem->IsInUse;

                if (bIsAimableItem && bIsCurrentlyAiming)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// starts at line 1883
void ADBDPlayer::Server_SendItemDropInput_Implementation(bool Pressed)
{
    // Update the player data with the current input state.
    // The disassembly writes to offset 0xE9 in UDBDPlayerData, which corresponds to the interaction input field in previous contexts.
    if (this->_playerData != nullptr)
    {
        this->_playerData->_interactionInputPressed = Pressed;
    }

    // Check if the inventory contains any items
    if (this->Inventory.Num() > 0)
    {
        // Retrieve the first item from the inventory
        ACollectable* Item = this->Inventory[0];

        // Ensure the item is valid
        if (Item != nullptr)
        {
            // Check if the item is currently equipped
            if (Item->IsEquipped() != false)
            {
                // Multicast the drop confirmation state (e.g., handles UI or charging visualization)
                this->Multicast_ConfirmItemDrop(Pressed);

                // Verify the inventory state again to ensure the item still exists
                if (this->Inventory.Num() > 0)
                {
                    // Re-acquire the item at index 0
                    Item = this->Inventory[0];
                    if (Item != nullptr)
                    {
                        // If the input was released (button up), execute the drop or action completion logic
                        if (Pressed == false)
                        {
                            // Call a virtual function at offset 0x6A8 on the item.
                            // Based on context, this is likely the handler for the actual drop action or release event.
                            Item->UseReleased();
                        }
                    }
                }
            }
        }
    }
}

// starts at line 1906
void ADBDPlayer::Server_PlayerReady_Implementation()
{
  this->_playerData->_gameLevelLoaded = true;
}

// starts at line 1926
void ADBDPlayer::UpdateLoadoutFromInventory()
{
    // Check if the pawn is controlled by a controller.
    if (this->IsControlled() == false)
    {
        return;
    }

    // Check if the player is ready to play (offset 0xF98).
    if (this->_isReadyToPlay == false)
    {
        return;
    }

    // Retrieve the GameInstance and cast it to UDBDGameInstance.
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Validation check: Ensure GameInstance exists and is valid (not pending kill).
    if (GameInstance == nullptr || GameInstance->IsValidLowLevel() == false)
    {
        return;
    }

    // Retrieve the PlayerState.
    APlayerState* PlayerState = this->PlayerState;

    // Validation check: Ensure PlayerState exists, is valid, and is not being destroyed.
    if (PlayerState == nullptr || 
        PlayerState->IsValidLowLevel() == false || 
        PlayerState->IsActorBeingDestroyed() == true)
    {
        return;
    }

    // Prepare the UniqueNetId for data retrieval.
    // The disassembly constructs FUniqueNetIdRepl on the stack using the PlayerState's UniqueId.
    FUniqueNetIdRepl UniqueIdRepl = PlayerState->UniqueId;

    // Retrieve the persistent data for this player.
    // Accessing _persistentData from GameInstance (offset inferred from context).
    FPlayerPersistentData* PlayerData = GameInstance->_persistentData->GetPlayerPersistentData(UniqueIdRepl);

    if (PlayerData == nullptr)
    {
        return;
    }

    bool bItemUpdateSuccessful = false;

    // Check if the inventory has items.
    if (this->Inventory.Num() > 0)
    {
        // Get the first item from the inventory.
        ACollectable* InventoryItem = this->Inventory[0];

        // Validate the item: check for null, pending kill, and being destroyed.
        if (InventoryItem != nullptr && 
            InventoryItem->IsValidLowLevel() == true && 
            InventoryItem->IsActorBeingDestroyed() == false)
        {
            // --- ITEM IS VALID LOGIC ---

            // Update the Item ID/Value in the current loadout.
            // Accessing offset 0x3B0 of ACollectable (likely the ItemID or composite value).
            PlayerData->CurrentLoadout.Item = InventoryItem->ItemID;

            // Clear existing addons in the loadout (Resize to 0).
            PlayerData->CurrentLoadout.ItemAddOns.Empty();

            // Retrieve the list of addons currently on the collectable item.
            TArray<UItemAddon*> ItemAddons = InventoryItem->GetItemAddons();

            // Iterate through the 2 addon slots (Hardcoded loop limit in disassembly).
            for (int32 AddonIndex = 0; AddonIndex < 2; ++AddonIndex)
            {
                UItemAddon* CurrentAddon = nullptr;
                
                // Safely retrieve the addon if the index is within bounds.
                if (AddonIndex < ItemAddons.Num())
                {
                    CurrentAddon = ItemAddons[AddonIndex];
                }

                // Check if the retrieved addon is valid.
                if (CurrentAddon == nullptr || CurrentAddon->IsValidLowLevel() == false)
                {
                    // --- INVALID ADDON LOGIC ---
                    
                    // Log an analytics event indicating an invalid addon reference.
                    FString AnalyticsMessage = TEXT("Inventory_InvalidAddonRef");
                    TArray<FAnalyticsEventAttribute> AnalyticsAttributes; // Empty attributes
                    
                    UBHVRAnalytics::RecordEvent(AnalyticsMessage, AnalyticsAttributes);

                    // If invalid, use the EMPTY slot value.
                    // Accessing static LoadoutSlotDefaultValue::EMPTY (offset 0x1E in one of the branches).
                    PlayerData->CurrentLoadout.ItemAddOns.Add(LoadoutSlotDefaultValue::EMPTY);
                }
                else
                {
                    // --- VALID ADDON LOGIC ---
                    
                    // Add the addon's ID/Value to the loadout.
                    // Accessing offset 0xF0 on UItemAddon (likely the ID).
                    PlayerData->CurrentLoadout.ItemAddOns.Add(CurrentAddon->ID);
                }
            }

            // Mark that we successfully processed the item, skipping the failure block.
            bItemUpdateSuccessful = true;
        }
    }

    // If we failed to process the item (Inventory empty or Item invalid).
    if (bItemUpdateSuccessful == false)
    {
        // --- INVALID ITEM LOGIC ---

        FString LogMessage;
        
        // Determine the specific error message based on why it failed.
        if (this->Inventory.Num() > 0 && this->Inventory[0] == nullptr)
        {
             LogMessage = TEXT("[ADBDPlayer::UpdateLoadoutFromInventory] Inventory contains a null pointer");
        }
        else
        {
             LogMessage = TEXT("[ADBDPlayer::UpdateLoadoutFromInventory] Inventory contains a pointer to an invalid ACollectable object");
        }

        // Log the error to analytics.
        FString AnalyticsEventName = TEXT("Inventory_InvalidAddonRef");
        TArray<FAnalyticsEventAttribute> EmptyAttributes;
        
        UBHVRAnalytics::RecordEvent(AnalyticsEventName, EmptyAttributes);
        UDBDAnalytics::Log(AnalyticsEventName, LogMessage, TEXT("D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\DBDPlayer.cpp"), 1956);

        // Clear the item from the loadout.
        PlayerData->CurrentLoadout.ClearItem();
    }

    // --- SAVE PROFILE LOGIC ---

    // Check if the persistent data we modified belongs to the local player.
    if (PlayerData == GameInstance->GetLocalPlayerPersistentData())
    {
        // Retrieve the local saved profile.
        FCharacterSavedProfileData* SavedProfile = GameInstance->GetLocalCharacterSavedProfileData();

        if (SavedProfile != nullptr)
        {
            // Update the saved profile's loadout with the persistent data's current loadout.
            // Using the assignment operator for FPlayerLoadoutData.
            SavedProfile->CharacterLoadoutData = PlayerData->CurrentLoadout;
        }
    }
}

// starts at line 1979
void ADBDPlayer::Server_SetCustomization_Implementation(const TArray<FName>& customizationParts)
{
    this->Multicast_SetCustomization(customizationParts);
}

// starts at line 1989
void ADBDPlayer::Multicast_SetCustomization_Implementation(const TArray<FName>& customizationParts)
{
    // Check if PlayerState exists.
    if (this->PlayerState != nullptr)
    {
        // Attempt to cast the generic PlayerState to the specific ADBDPlayerState.
        // The disassembly explicitly checks the class tree index against ADBDPlayerState's static class.
        ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        // Ensure the cast was successful.
        if (dbdPlayerState != nullptr)
        {
            // Perform validity checks:
            // 1. Check against Global Object Array flags (RF_Unreachable).
            // 2. Check bActorIsBeingDestroyed (Offset 0x140, bit 4).
            // In standard UE4 C++, IsValid() or !IsPendingKill() handles these.
            if (!dbdPlayerState->IsPendingKill())
            {
                // Assign the received customization parts to the member located at offset 0x7E8 in ADBDPlayerState.
                // The disassembly uses the TArray assignment operator.
                // We access the member via the pointer, assuming the structure of ADBDPlayerState matches the offset.
                dbdPlayerState->PlayerData.CustomizationMesh = customizationParts;

                // Call the function to update the character's mesh based on the new customization.
                this->UpdateCustomizationMesh();
            }
        }
    }
}

// starts at line 2004
FVector ADBDPlayer::GetMovementSpeed() const
{
    // Retrieve the current velocity vector from the actor
    FVector Velocity = this->GetVelocity();

    // Zero out the Z (vertical) component to get the planar movement vector
    Velocity.Z = 0.0f;

    return Velocity;
}

// starts at line 2021
FName ADBDPlayer::GetCollectableAttachPoint() const
{
    // Return the socket name used for attaching collectables
    // This maps directly to the member variable CollectableAttachPoint
    return this->CollectableAttachPoint;
}

// starts at line 2031
bool ADBDPlayer::IsLocallyObserved() const
{
    // Check if the character is locally controlled by the client
    // Disassembly: Calls virtual function IsLocallyControlled at offset 0x630
    if (this->IsLocallyControlled() == true)
    {
        return true;
    }

    // Retrieve the GameInstance
    UGameInstance* GameInstance = this->GetGameInstance();

    if (GameInstance == nullptr)
    {
        return false;
    }

    // Cast to the custom UDBDGameInstance class
    // Disassembly: Checks ClassTreeIndex against UDBDGameInstance::StaticClass()
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    if (DBDGameInstance == nullptr || DBDGameInstance->IsPendingKill() == true)
    {
        return false;
    }

    // Retrieve the local PlayerController from the GameInstance
    // Disassembly: Calls UDBDGameInstance::GetLocalPlayerController()
    APlayerController* LocalController = DBDGameInstance->GetLocalPlayerController();

    if (LocalController == nullptr)
    {
        return false;
    }

    // Cast to the custom ADBDPlayerController class
    // Disassembly: Checks ClassTreeIndex against ADBDPlayerController::StaticClass()
    ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(LocalController);

    // Verify controller validity (cast success, pending kill, being destroyed)
    if (DBDController == nullptr || DBDController->IsPendingKill() || DBDController->IsActorBeingDestroyed())
    {
        return false;
    }

    // Check if the local controller is currently in a spectating state
    // Disassembly: Checks byte at offset 0x970 (likely _isSpectating or similar flag)
    if (DBDController->_spectating == false)
    {
        return false;
    }

    // Retrieve the player currently being spectated by the controller
    // Disassembly: Calls ADBDPlayerController::GetSpectatedPlayer()
    ADBDPlayer* SpectatedPlayer = DBDController->GetSpectatedPlayer();

    // Check if the spectated player matches 'this' instance
    if (SpectatedPlayer == this)
    {
        return true;
    }

    return false;
}

// starts at line 2052
bool ADBDPlayer::TryInteractionType(EInputInteractionType interactionInputType, ADBDPlayer* requester)
{
    UInteractionDefinition* CurrentInteraction = nullptr;

    // 1. Identify Current Interaction State
    // Check 'IsInteracting' flag (Offset 0xC04).
    if (this->IsInteracting)
    {
        // Retrieve the current interaction from the handler (Offset 0xCF0).
        if (this->_interactionHandler != nullptr)
        {
            CurrentInteraction = this->_interactionHandler->_currentInteraction;
        }
    }

    // 2. Conflict Check: Interacting Player
    // Check if another player is currently interacting with us (Offset 0xE18).
    ADBDPlayer* InteractingPlayer = this->_interactingPlayer;
    if (InteractingPlayer != nullptr)
    {
        // If that player is valid and active (Offset 0x1070 virtual call), we cannot start a new interaction.
        // This prevents logic breaks where two players try to interact with the same object/player simultaneously.
        if (IsValid(InteractingPlayer) && InteractingPlayer->IsValidImpl())
        {
            return false;
        }
    }

    // 3. Interrupt/Override Logic
    // If we are currently executing an interaction, we check if the new input can override it.
    if (CurrentInteraction != nullptr && IsValid(CurrentInteraction))
    {
        // Retrieve the potential interaction for the *new* input type.
        UInteractionDefinition* NewInteraction = this->_interactionHandler->GetAvailableInteraction(interactionInputType);

        if (NewInteraction != nullptr && IsValid(NewInteraction))
        {
            // Ask the new interaction if it is allowed to cancel/override the current one.
            if (NewInteraction->CanOverrideInteraction(CurrentInteraction))
            {
                // Force the new interaction to start (force = true).
                return this->TryInteraction(NewInteraction, requester, true);
            }
        }

        // If we are busy and cannot override, the attempt fails.
        return false;
    }

    // 4. Default Logic (Not Interacting)
    // If we are idle, delegate finding and starting the interaction to the handler.
    return this->_interactionHandler->TryFirstAvailableInteractionOfType(interactionInputType, requester);
}

// starts at line 2078
bool ADBDPlayer::TryInteraction(UInteractionDefinition* interaction, ADBDPlayer* requester, bool force)
{
    // 1. Validate the interaction object.
    // Standard validity check (checks NULL, Pending Kill, and Reachability).
    if (!IsValid(interaction))
    {
        return false;
    }

    // 2. Conflict Check: Is this player currently being interacted with by someone else?
    // Access the '_interactingPlayer' member at offset 0xE18.
    // If another player is interacting with us (and they are valid), we generally cannot start a new interaction.
    ADBDPlayer* InteractingPlayer = this->_interactingPlayer;
    if (InteractingPlayer != nullptr)
    {
        // Perform deep validity checks (GUObjectArray flags, PendingKill).
        // The assembly calls a virtual function at offset 0x1070 (likely a specific 'IsAliveAndActive' or 'IsValidImpl' check).
        if (IsValid(InteractingPlayer) && InteractingPlayer->IsValidImpl())
        {
            return false;
        }
    }

    // 3. State Check: Are we free to interact?
    // Offset 0xC04 corresponds to the 'IsInteracting' boolean flag.
    // We can proceed if we are NOT currently interacting, OR if the 'force' flag is true (allowing interruption).
    if (!this->IsInteracting || force)
    {
        // Delegate the logic to the PlayerInteractionHandler component (Offset 0xCF0).
        if (this->_interactionHandler != nullptr)
        {
            // Extract the Input Type from the interaction definition (Offset 0x2A0).
            EInputInteractionType InputType = interaction->InteractionInputType;

            // Forward the call.
            // Note: The last parameter 'force' maps to the 'allowSwap' or 'force' param in the handler.
            return this->_interactionHandler->TryInteraction(interaction, InputType, requester, force);
        }
    }

    return false;
}

// starts at line 2097
void ADBDPlayer::InteractionInputPressed()
{
    // Attempt to perform the interaction locally based on the input type.
    // Passing 'this' as the initiator.
    this->TryInteractionType(EInputInteractionType::VE_Interact, this);

    // Access the PlayerData component to record the input state.
    if (this->_playerData != nullptr)
    {
        // Set the boolean flag indicating the interaction button is pressed.
        this->_playerData->_interactionInputPressed = true;
    }

    // Send the input event to the server for replication.
    this->Server_SendInteractionInput(true);
}

// starts at line 2104
void ADBDPlayer::InteractionInputReleased()
{
    // Access the PlayerData component to update the input state
    if (this->_playerData != nullptr)
    {
        // Clear the boolean flag indicating the interaction button is pressed
        this->_playerData->_interactionInputPressed = false;
    }

    // Send the input release event to the server for replication
    this->Server_SendInteractionInput(false);
}

// starts at line 2110
void ADBDPlayer::FastInteractionInputPressed()
{
    // Attempt to perform an interaction of type 18 (Fast Interact)
    // 0x12 = 18
    bool bInteractionStarted = this->TryInteractionType(EInputInteractionType::VE_FastInteract, this);

    if (bInteractionStarted == true)
    {
        // If the interaction started successfully, stop the input persistence timer
        // UDBDUtilTimer::Stop is a static helper function
        UDBDUtilTimer::Stop(this->_fastInteractInputPersistence);
    }
    else
    {
        // If interaction failed, check if we are NOT currently interacting
        // Offset 0xC04 corresponds to IsInteracting
        if (this->IsInteracting == false)
        {
            // Retrieve the tunable value for fast interact input persistence time
            // "FAST_INTERACT_INPUT_PERSISTENCE_TIME"
            static const FName FAST_INTERACT_INPUT_PERSISTENCE_TIME = FName("FastInteractInputPersistenceTime");
            float PersistenceTime = this->GetNonPlayerSpecificTunableValue(FAST_INTERACT_INPUT_PERSISTENCE_TIME);

            // Reset the persistence timer with the retrieved time
            this->_fastInteractInputPersistence.Reset(PersistenceTime);
        }
    }

    // Update the PlayerData state
    if (this->_playerData != nullptr)
    {
        // Set _fastInteractionInputPressed (Offset 0xEB in UDBDPlayerData) to true
        this->_playerData->_fastInteractionInputPressed = true;
    }

    // Send the input to the server via RPC
    this->Server_SendFastInteractionInput(true);
}

// starts at line 2128
void ADBDPlayer::FastInteractionInputReleased()
{
    // Access the PlayerData member (Offset 0x0D08)
    // The disassembly loads the pointer and immediately writes to offset 0xEB
    if (this->_playerData != nullptr)
    {
        // Set _fastInteractionInputPressed to false (0)
        this->_playerData->_fastInteractionInputPressed = false;
    }

    // Send the fast interaction input release to the server via RPC
    // The argument passed is 'false' (0) (xor edx, edx)
    this->Server_SendFastInteractionInput(false);
}

// starts at line 2135
void ADBDPlayer::CrouchInputPressed()
{
    // Set the boolean flag at offset 0xE11 to indicate the input is pressed.
    this->_crouchPressed = true;

    // Call the virtual function at offset 0xD70. 
    // The Pseudo C identifies this as 'DBDCrouch', which likely triggers the actual crouch mechanics.
    this->DBDCrouch();

    // Check if the player is currently using an item that requires aiming (e.g., Flashlight).
    if (this->IsUsingAimItem() == true)
    {
        // If they are aiming, cancel the usage (send 'false' to server).
        // Disassembly: xor edx, edx (0) -> jmp Server_SendItemUse
        this->Server_SendItemUse(false);
    }
}

// starts at line 2146
void ADBDPlayer::CrouchInputReleased()
{
    // Set the boolean flag at offset 0xE11 to indicate the input is released (false).
    this->_crouchPressed = false;

    // Call the virtual function at offset 0xD78.
    this->DBDUnCrouch();
}

// starts at line 2152
bool ADBDPlayer::IsDashing() const
{
    // Retrieve the interface pointer for the Perk Manager Owner.
    // The class implements IPerkManagerOwnerInterface at offset 0x7A8.
    const IPerkManagerOwnerInterface* PerkOwnerInterface = static_cast<const IPerkManagerOwnerInterface*>(this);

    // Get the PerkManager instance from the interface.
    UPerkManager* PerkManager = PerkOwnerInterface->GetPerkManager();

    // Check if the PerkManager is valid.
    // Disassembly: Checks if pointer is null before proceeding.
    if (PerkManager != nullptr)
    {
        // Verify the object is valid (not pending kill).
        // Disassembly: Checks GUObjectArray flags (0x20000000).
        if (PerkManager->IsPendingKill() == false)
        {
            // Prepare an array to hold the gameplay modifiers.
            TArray<UGameplayModifierContainer*> CombinedEffects;

            // Retrieve all effects from the PerkManager.
            // Disassembly passes '4' as the filter argument (EGameplayModifierSource).
            PerkManager->GetAllEffects(CombinedEffects, EGameplayModifierSource::VE_All);

            // Iterate through the retrieved effects.
            for (int32_t i = 0; i < CombinedEffects.Num(); i++)
            {
                UGameplayModifierContainer* Effect = CombinedEffects[i];

                // Ensure the effect object is valid.
                if (Effect != nullptr)
                {
                    // Check if the effect object is not pending kill.
                    if (Effect->IsPendingKill() == false)
                    {
                        // Check if the effect has the 'Dash' flag.
                        // Disassembly uses 0x49 (73 decimal). Pseudo-code refers to this as VE_Dash.
                        if (Effect->HasFlag(EGameplayModifierFlag::VE_Dash) == true)
                        {
                            // Check if the effect is currently applicable.
                            // Disassembly: Accesses boolean at offset 0x158 (m_bIsApplicable).
                            if (Effect->m_bIsApplicable != false)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

// starts at line 2157
void ADBDPlayer::Server_SendFastInteractionInput_Implementation(bool pressed)
{
  this->_playerData->_fastInteractionInputPressed = pressed;
}

// starts at line 2167
void ADBDPlayer::Server_SendInteractionInput_Implementation(bool pressed)
{
    // Check if the interaction input is being pressed
    if (pressed != false)
    {
        // Access the Interaction Handler component
        if (this->_interactionHandler != nullptr)
        {
            // Check if there is a requested interaction currently active.
            // Corresponds to the check at offset 0x1B8 in UPlayerInteractionHandler found in disassembly.
            if (this->_interactionHandler->_requestedInteraction != nullptr)
            {
                // Execute the interaction press delegate.
                // Corresponds to the TBaseDelegate::ExecuteIfBound call at offset 0x110 in UPlayerInteractionHandler.
                this->_interactionHandler->InteractionPressDelegate.ExecuteIfBound();
            }
        }
    }

    // Update the player data with the current input state.
    if (this->_playerData != nullptr)
    {
        // Set the boolean flag for interaction input.
        // Corresponds to offset 0xE9 in UDBDPlayerData found in disassembly.
        this->_playerData->_interactionInputPressed = pressed;
    }
}

// starts at line 2181
void ADBDPlayer::SecondaryActionInputPressed()
{
    // Check if the player currently has an active skill check displayed
    if (this->HasActiveSkillCheck() == true)
    {
        // If a skill check is active, retrieve the SkillCheck instance from the InteractionHandler
        // and forward the input event to it.
        // The member _skillCheck is located at offset 0x240 in UPlayerInteractionHandler based on disassembly.
        this->_interactionHandler->_skillCheck->OnSkillCheckInput();
    }
    else
    {
        // If no skill check is active, notify the server that the secondary action input was pressed via RPC
        this->Server_SendSecondaryActionPressed();
    }
}

// starts at line 2194
bool ADBDPlayer::HasInInventory(TSubclassOf<ACollectable> item)
{
    // Use FindByPredicate to search the Inventory array.
    // The lambda captures the requested item class and checks each inventory item against it.
    // FindByPredicate returns a pointer to the element in the array (ACollectable**) if found, or nullptr.
    ACollectable** FoundItemPtr = this->Inventory.FindByPredicate([item](ACollectable* InItem)
    {
        // Ensure the pointer in the inventory is valid
        if (InItem != nullptr)
        {
            // Check if the item in the inventory is an instance of the specified class.
            // The pseudo-code logic involving ClassTreeIndex comparisons is the internal implementation of IsA.
            if (InItem->IsA(item) == true)
            {
                return true;
            }
        }
        return false;
    });

    // If the result is not nullptr, the item was found in the inventory.
    if (FoundItemPtr != nullptr)
    {
        return true;
    }

    return false;
}

// starts at line 2202
void ADBDPlayer::UseInventoryItem()
{
    // Check if the Inventory array contains any items
    if (this->Inventory.Num() > 0)
    {
        // Retrieve the first item from the inventory (Active Item)
        ACollectable* Item = this->Inventory[0];

        // Ensure the item pointer is valid
        if (Item != nullptr)
        {
            // Verify the object is valid and not pending kill (checks RF_PendingKill or similar internal flags seen in ASM)
            if (Item->IsValidLowLevel() == true)
            {
                // Retrieve the Perk Manager via the Interface (IPerkManagerOwnerInterface)
                // Interface pointer is at offset 0x7A8, function index 0x10 in vtable
                UPerkManager* PerkManager = this->GetPerkManager();

                // Prepare an array to hold gameplay modifiers/effects
                TArray<UGameplayModifierContainer*> CombinedEffects;

                PerkManager->GetAllEffects(CombinedEffects, EGameplayModifierSource::VE_All);

                bool bDoNotLoseItem = false;

                // Iterate through all effects to check if the item consumption should be prevented
                for (int32 i = 0; i < CombinedEffects.Num(); i++)
                {
                    UGameplayModifierContainer* Effect = CombinedEffects[i];

                    if (Effect != nullptr)
                    {
                        // Check specific flag (0x14 = 20) which corresponds to VE_DoNotLoseItemOnUse
                        // Also check if the modifier is currently applicable
                        if (Effect->HasFlag(EGameplayModifierFlag::VE_DoNotLoseItemOnUse) == true && Effect->m_bIsApplicable == true)
                        {
                            bDoNotLoseItem = true;
                            break; 
                        }
                    }
                }

                // If no effect prevents item loss, consume the item
                if (bDoNotLoseItem == false)
                {
                    // Remove the item from the inventory (Index 0)
                    // This creates the memmove and ResizeShrink logic seen in disassembly (0x1A5, 0x1B1)
                    this->Inventory.RemoveAt(0);

                    // Call a virtual function on the Item instance at offset 0x6A0.
                    // This function likely handles the specific "On Used" logic for the collectable.
                    Item->Use(); 
                }
                else
                {
                    // The item is preserved, but specific addons might still be consumed.
                    
                    // Retrieve all addons attached to the item
                    TArray<UItemAddon*> ItemAddons = Item->GetItemAddons();

                    // Filter the addons using a predicate (lambda) to determine which ones to remove.
                    TArray<UItemAddon*> AddonsToRemove = ItemAddons.FilterByPredicate([&](UItemAddon* Addon) {
                        // Check if the addon has the flag 0x14 (20).
                        // 0x14 corresponds to the 2nd argument (EDX register) in the assembly (mov dl, 0x14).
                        // Note: The decompiler labels this VE_DoNotLoseItemOnUse, but strictly following 
                        // the provided logic, we filter for items that HAVE this flag.
                        if (ItemAddon->HasFlag(EGameplayModifierFlag::VE_DoNotLoseItemOnUse) == true)
                        {
                            return true;
                        }
                        return false;
                    });

                    // Iterate through the filtered list and remove the addons from the item
                    for (int32 i = 0; i < AddonsToRemove.Num(); i++)
                    {
                        UItemAddon* Addon = AddonsToRemove[i];
                        if (Addon != nullptr)
                        {
                            Item->Authority_RemoveItemAddon(Addon);
                        }
                    }
                }
            }
        }
    }
}

// starts at line 2227
void ADBDPlayer::AddToInventory(ACollectable* item)
{
    // Ensure the passed item pointer is not null
    if (item != nullptr)
    {
        // The disassembly checks the GUObjectArray flags (0x20000000) and the actor's 
        // internal destruction bitfield to ensure the object is fully valid and not pending kill. 
        // This is perfectly represented by Unreal Engine's standard IsValid() check.
        if (IsValid(item) == true)
        {
            // Verify if the executing player possesses network authority.
            // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
            if (this->Role == 3)
            {
                // Trigger the multicast RPC to replicate the inventory addition to all connected clients.
                // Note: The RPC wrapper itself is auto-generated by the Unreal Header Tool.
                this->Multicast_AddToInventory(item);
            }
        }
    }
}

// starts at line 2235
void ADBDPlayer::Multicast_AddToInventory_Implementation(ACollectable* item)
{
    // Check if the item pointer is valid.
    if (item != nullptr)
    {
        // Perform low-level validity checks (IsPendingKill / IsActorBeingDestroyed).
        // Disassembly: Checks GUObjectArray flags (0x20000000) and Actor flags (offset 0x140).
        if (!item->IsPendingKill() && !item->IsActorBeingDestroyed())
        {
            // Add the item to the Inventory array only if it isn't already there.
            // Disassembly: Calls TArray::AddUniqueImpl on offset 0xC58 (Inventory).
            this->Inventory.AddUnique(item);

            // Update the local loadout to reflect the new inventory state.
            // Disassembly: Call UpdateLoadoutFromInventory
            this->UpdateLoadoutFromInventory();
        }
    }
}

// starts at line 2244
void ADBDPlayer::AddPerk(UPerk* perk)
{
    // Make sure the pointer is valid before attempting to access its UObject data.
    if (perk != nullptr)
    {
        // The disassembly checks the internal GUObjectArray to see if the object 
        // has the 0x20000000 flag (pending kill/unreachable). 
        // In Unreal Engine C++, IsValid() safely wraps this exact check.
        if (IsValid(perk) == true)
        {
            // The disassembly shows manual array bounds checking, calling ResizeGrow if needed,
            // and inserting the pointer into the array memory at the new index.
            // This is exactly what the TArray::Add() macro compiles down to.
            this->_perks.Add(perk);
        }
    }
}

// starts at line 2252
void ADBDPlayer::AddStatusEffect(UStatusEffect* statusEffect)
{
    // Ensure the provided status effect pointer is not null before proceeding
    if (statusEffect != nullptr)
    {
        // The disassembly queries the GUObjectArray to check for the 0x20000000 flag,
        // which verifies the object is not pending garbage collection. 
        // In standard UE4 C++, this is wrapped by the IsValid() macro.
        if (IsValid(statusEffect) == true)
        {
            // The memory manipulation and ResizeGrow calls seen in the disassembly 
            // represent the compiled instructions for standard TArray addition.
            this->_statusEffects.Add(statusEffect);
        }
    }
}

// starts at line 2260
void ADBDPlayer::RemoveStatusEffect(UStatusEffect* StatusEffect)
{
    // Check if the provided StatusEffect pointer is not null
    if (StatusEffect != nullptr)
    {
        // Perform standard validity check (PendingKill flags via Global Object Array)
        if (IsValid(StatusEffect))
        {
            // Remove the status effect from the internal array.
            // RemoveSwap is used for efficiency as order likely does not matter.
            // Matches disassembly call to TArray::RemoveSwap at offset 0xC88.
            this->_statusEffects.RemoveSwap(StatusEffect);

            // Destroy the component.
            // Corresponds to the virtual function call at offset 0x308 (UActorComponent::DestroyComponent).
            // The disassembly passes 0 (false) for the bPromoteChildren parameter.
            StatusEffect->DestroyComponent(false);
        }
    }
}

// starts at line 2269
bool ADBDPlayer::HasStatusEffect(FName statusEffectID) const
{
    // Iterate through all status effects in the array
    // Disassembly: Accesses _statusEffects at offset 0xC88
    for (int32_t i = 0; i < this->_statusEffects.Num(); ++i)
    {
        // Retrieve the status effect instance
        UStatusEffect* StatusEffect = this->_statusEffects[i];

        // Ensure the pointer is valid before dereferencing
        if (StatusEffect != nullptr)
        {
            // Check if the effect's ID matches the requested ID
            // Disassembly: Compares qword at [rcx+0F0h] (StatusEffect + 0xF0) with rdx (statusEffectID)
            if (StatusEffect->ID == statusEffectID)
            {
                return true;
            }
        }
    }

    return false;
}

// starts at line 2274
void ADBDPlayer::RemoveFromInventory(ACollectable* Item)
{
    // Check if the item pointer is valid
    if (Item != nullptr)
    {
        // Perform validity checks:
        // 1. Check global object array flags (represented by IsValid in UE4).
        // 2. Explicitly check the bActorIsBeingDestroyed flag (offset 0x140, bit 4).
        if (IsValid(Item) && Item->bActorIsBeingDestroyed == false)
        {
            // Check if this actor has network authority (ROLE_Authority == 3)
            if (this->Role == ROLE_Authority)
            {
                // Call the multicast function to remove the item on all clients
                this->Multicast_RemoveFromInventory(Item);
            }
        }
    }
}

// starts at line 2283
void ADBDPlayer::Multicast_RemoveFromInventory_Implementation(ACollectable* item)
{
    // Ensure the pointer is valid before accessing it.
    if (item != nullptr)
    {
        // Validate the object against the Global Object Array (checking for unreachable flags)
        // and check the Actor's state (specifically bActorIsBeingDestroyed at offset 0x140 bit 4).
        // In standard UE4, IsValid() handles pending kill checks, but the disassembly is explicit.
        if (IsValid(item) == true && item->bActorIsBeingDestroyed == false)
        {
            // Check if the item is currently marked as 'In Use' (based on Pseudo Code accessing offset 0x3A9)
            // and if the inventory currently has items.
            // Note: 'IsInUse' is likely a property of ACollectable not fully defined in the provided structure.
            if (item->IsInUse == true && this->Inventory.Num() > 0)
            {
                // Get the first item in the inventory (the currently held item).
                ACollectable* currentItem = this->Inventory[0];

                if (currentItem != nullptr)
                {
                    // Check if the current item is equipped.
                    if (currentItem->IsEquipped() == true)
                    {
                        // Execute a virtual function at offset 0x6A8 on the current item.
                        // This likely handles un-equipping or cancelling the action associated with the item.
                        currentItem->UseReleased();
                    }
                }
            }

            // Remove the item from the Inventory array.
            // The disassembly iterates through the array, finds the item, and uses memmove to shift the remaining memory.
            // This is the standard implementation of TArray::RemoveSingle.
            this->Inventory.RemoveSingle(item);

            // Update the player's loadout to reflect the changes in the inventory.
            this->UpdateLoadoutFromInventory();
        }
    }
}

// starts at line 2297
void ADBDPlayer::ClearAndDestroyInventory()
{
    // Check if the current actor has network authority.
    // The disassembly compares the Role byte at offset 0x110 with 3 (ROLE_Authority).
    // In UE4, HasAuthority() is the standard helper for (Role == ROLE_Authority).
    if (this->Role == ROLE_Authority)
    {
        // If we have authority, call the multicast function to replicate the action to all clients.
        this->Multicast_ClearAndDestroyInventory();
    }
}

// starts at line 2306
void ADBDPlayer::Multicast_ClearAndDestroyInventory_Implementation()
{
    // Iterate through all items in the Inventory
    // Disassembly: Standard TArray iteration loop
    for (int32 i = 0; i < this->Inventory.Num(); i++)
    {
        // Retrieve the item (ACollectable*)
        AActor* Item = this->Inventory[i];

        if (Item != nullptr)
        {
            // Destroy the actor to remove it from the world.
            // params: (bNetForce = false, bShouldModifyLevel = true)
            // Disassembly: call ?Destroy@AActor@@QEAA_N_N0@Z
            Item->Destroy(false, true);
        }
    }

    // Clear the inventory array and free memory
    // Disassembly: Sets ArrayNum to 0 and calls ResizeTo(0)
    this->Inventory.Empty();

    // Update the player's loadout to reflect the empty inventory
    // Disassembly: call ?UpdateLoadoutFromInventory@ADBDPlayer@@QEAAXXZ
    this->UpdateLoadoutFromInventory();
}

// starts at line 2318
void ADBDPlayer::Collect(ACollectable* item)
{
    // Ensure the item to be collected is valid.
    if (!IsValid(item))
    {
        return;
    }

    // This action must only occur on the server (Authority).
    if (this->Role == ROLE_Authority)
    {
        // Check if the player already has items in their inventory.
        // Offset 0xC60 corresponds to the TArray Num for the Inventory.
        if (this->Inventory.Num() > 0)
        {
            // Determine if we need to drop the currently held item to make room.
            // Logic: 
            // 1. If the new item is NOT a Bear Trap, we must drop the current item (swapping).
            // 2. If the new item IS a Bear Trap, we only drop if our hands are already full of traps 
            //    (some killers can carry multiple traps).
            bool bShouldDrop = true;
            
            if (item->ItemID == BEARTRAP_ID)  // "Item_Slasher_Beartrap"
            {
                // If it's a trap, check if we have space.
                if (this->ArePlayersHandsFullOfBearTraps() == false)
                {
                    bShouldDrop = false;
                }
            }

            if (bShouldDrop)
            {
                // Prepare the location/rotation for the dropped item.
                // It appears the dropped item is placed at the 'Last Valid Ground Anchor Position' of the item being picked up.
                // Offsets 0x3F8 (X/Y) and 0x400 (Z) on the item correspond to this vector.
                FVector DropLocation = item->_lastValidGroundAnchorPosition;
                
                // Rotation is explicitly zeroed out in the assembly.
                FRotator DropRotation = FRotator::ZeroRotator;

                // Identify the current item to drop (the first item in the inventory).
                ACollectable* ItemToDrop = nullptr;
                if (this->Inventory.Num() > 0)
                {
                    ItemToDrop = this->Inventory[0];
                }

                // Call the Drop function.
                // The boolean arguments (onDeath, adjustToGround) are passed as false (sil = 0).
                this->Drop(ItemToDrop, DropLocation, DropRotation, false, false);
            }
        }

        // Notify the item that it has been collected by this player.
        item->Authority_Collected(this);

        // Add the new item to the player's inventory.
        this->AddToInventory(item);

        // Handle visibility/storage state.
        // If the collected item is NOT the primary item (index 0) after adding it,
        // it implies it was added to a secondary slot or stack, so we "Store" (hide) it.
        ACollectable* PrimaryItem = nullptr;
        if (this->Inventory.Num() > 0)
        {
            PrimaryItem = this->Inventory[0];
        }

        if (item != PrimaryItem)
        {
            item->Store();
        }
    }
}

// starts at line 2340
void ADBDPlayer::Drop(ACollectable* item, const FVector& location, const FRotator& rotation, bool onDeath, bool adjustToGround)
{
    // Check if the item pointer is not null
    if (item != nullptr)
    {
        // Perform validity checks on the item object (GUObjectArray lookup in disassembly)
        // Check if the actor is not currently being destroyed (Offset 0x140, bit 4 in AActor)
        if (item->IsValidLowLevel() == true && item->bActorIsBeingDestroyed == false)
        {
            // Check if this player actor has network authority (Role == 3 corresponds to ROLE_Authority)
            if (this->Role == ROLE_Authority)
            {
                // Attempt to consume add-on drop protection (e.g. offerings that prevent item loss)
                // If the function returns true, the protection was consumed, and we stop the drop process.
                if (item->Authority_TryConsumeAddonDropProtection(onDeath) == false)
                {
                    // Call the authoritative function to handle the physical dropping of the item
                    // location and rotation determine where the item spawns
                    // adjustToGround likely performs a line trace to snap the item to the floor
                    item->Authority_Dropped(location, rotation, this, adjustToGround);

                    // Remove the item reference from the player's internal inventory array
                    this->RemoveFromInventory(item);
                }
            }
        }
    }
}

// starts at line 2349
void ADBDPlayer::StartDrop(ACollectable* item)
{
    // Check if the item provided is valid.
    if (item != nullptr)
    {
        // Check if this actor has network authority (Role == ROLE_Authority which is 3).
        // The assembly checks the byte at offset 0x110.
        if (this->HasAuthority())
        {
            // Call the authority-specific start drop function on the item.
            item->Authority_StartDrop();
        }
    }
}

// starts at line 2357
void ADBDPlayer::TryDropCurrentItemOnDeath(const FVector& Location, const FRotator& Rotation)
{
    // Check if this actor has network authority (Role == ROLE_Authority/3).
    // Offset 0x110 is the Role property.
    if (this->HasAuthority())
    {
        // Check if the Inventory is not empty (Offset 0xC60 is ArrayNum).
        if (this->Inventory.Num() > 0)
        {
            // Retrieve the first item (Offset 0xC58 is Data pointer).
            ACollectable* CurrentItem = this->Inventory[0];

            // Validate the item (not null and not pending kill).
            // Assembly checks flags at offset 0x140 against 0x4 (RF_PendingKill).
            if (IsValid(CurrentItem))
            {
                // Server Side: Execute the drop immediately.
                // The assembly passes 'true' for both boolean arguments on the stack.
                // onDeath = true
                // adjustToGround = true
                this->Drop(CurrentItem, Location, Rotation, true, true);
            }
        }
    }
    else // Client Logic
    {
        // Client still checks if it locally thinks it has an item to predict/validate state.
        if (this->Inventory.Num() > 0)
        {
            ACollectable* CurrentItem = this->Inventory[0];

            if (IsValid(CurrentItem))
            {
                // Client Side: Request the server to drop the item via RPC.
                // Note that Server_Drop takes fewer arguments than Drop (likely assumes adjustToGround logic on server).
                // onDeath = true
                this->Server_Drop(CurrentItem, Location, Rotation, true);
            }
        }
    }
}

// starts at line 2375
void ADBDPlayer::Server_Drop_Implementation(ACollectable* Item, const FVector& Location, const FRotator& Rotation, bool OnDeath)
{
    // Check if the inventory contains any items
    if (this->Inventory.Num() > 0)
    {
        // Retrieve the first item from the inventory.
        // Note: The function ignores the 'Item' parameter passed to the function and enforces using the item at index 0.
        // This is likely a security measure to ensure the client drops the item the server believes they are holding.
        ACollectable* ItemToDrop = this->Inventory[0];

        // Check if the retrieved item is not null
        if (ItemToDrop != nullptr)
        {
            // Perform validity checks:
            // 1. IsValid handles the global object array checks (PendingKill).
            // 2. Explicitly check the bActorIsBeingDestroyed flag (offset 0x140, bit 4).
            if (IsValid(ItemToDrop) && ItemToDrop->bActorIsBeingDestroyed == false)
            {
                // Call the Drop function to perform the drop logic.
                // Arguments: Item, Location, Rotation, OnDeath, bAdjustToGround (hardcoded to true/1 in disassembly).
                this->Drop(ItemToDrop, Location, Rotation, OnDeath, true);
            }
        }
    }
}

// starts at line 2393
void ADBDPlayer::Server_SendSecondaryActionPressed_Implementation()
{
    // Access the interaction handler component
    if (this->_interactionHandler != nullptr)
    {
        // Check if there is currently a requested interaction active.
        // Corresponds to the check: if ( v1->_requestedInteraction )
        if (this->_interactionHandler->_requestedInteraction != nullptr)
        {
            // Execute the delegate associated with the secondary action input.
            // Corresponds to: TBaseDelegate::ExecuteIfBound(&v1->SecondaryActionInteractionPressDelegate)
            this->_interactionHandler->SecondaryActionInteractionPressDelegate.ExecuteIfBound();
        }
    }
}

// starts at line 2406
void ADBDPlayer::ClientCheatGhost()
{
    // Ensure the character movement component is valid before attempting to access its properties
    if (this->CharacterMovement != nullptr)
    {
        // The disassembly checks bit 0x400 within a bitfield at offset 0x3AC.
        // This evaluates to true if the player is currently NOT in Ghost mode.
        bool bShouldGhost = (this->CharacterMovement->bCheatFlying == 0);

        // Forward the request to the server to apply the ghost state.
        // Calling a Server RPC directly invokes the Unreal Header Tool (UHT) auto-generated thunk, 
        // which handles the network packaging and dispatch.
        this->Server_Ghost(bShouldGhost); /* UNREAL AUTO GENERATED FUNCTION */
    }
}

// starts at line 2415
void ADBDPlayer::Server_Ghost_Implementation(bool ghost)
{
    // Call a virtual function at offset 0x858.
    // The disassembly preserves the 'ghost' argument across this call and the pseudo-code casts it to a void function taking only 'this', 
    // implying this function does not take the boolean argument.
    this->Multicast_Ghost_Implementation(ghost);

    // Call the multicast function to broadcast the ghost state to clients.
    this->Multicast_Ghost(ghost);
}

// starts at line 2425
void ADBDPlayer::Multicast_Ghost_Implementation(bool bGhost)
{
    // 1. Toggle Collision
    // If Ghosting is ON, Collision is OFF.
    // Disassembly: call ?SetActorEnableCollision@AActor@@QEAAX_N@Z
    this->SetActorEnableCollision(!bGhost);

    // 2. Update Movement Component
    // Disassembly: mov r8, [rbx+3E8h] (CharacterMovement)
    if (this->CharacterMovement != nullptr)
    {
        // Update internal flags (likely bCheatFlying or similar at bit 10)
        // Disassembly: and dword ptr [r8+3ACh], 0FFFFFBFFh (Clear bit 10)
        // Disassembly: or [r8+3ACh], eax (Set bit 10 based on bGhost)
        this->CharacterMovement->bCheatFlying = bGhost;

        // Determine the new movement mode
        // 5 = MOVE_Flying (Standard UE4 Flying mode)
        // 1 = MOVE_Walking (Standard UE4 Walking mode)
        EMovementMode NewMode = bGhost ? MOVE_Flying : MOVE_Walking;

        // Apply the movement mode
        // Disassembly: call ?SetMovementMode@UCharacterMovementComponent@@QEAAXW4EMovementMode@@E@Z
        this->CharacterMovement->SetMovementMode(NewMode, 0);
    }
}

// starts at line 2439
bool ADBDPlayer::TryFireScoreEvent(EDBDScoreTypes scoreType, float percentToAward, bool redirectToGameEventSystem)
{
    // Retrieve and cast the PlayerState to the game-specific ADBDPlayerState.
    // Offset 0x3A0 is the standard offset for APawn::PlayerState.
    // The assembly includes logic to verify the class type (GetPrivateStaticClass).
    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

    // Check for Network Authority.
    // Offset 0x110 corresponds to the AActor::Role property.
    // Value 3 represents ROLE_Authority. Score events are typically server-authoritative.
    if (this->HasAuthority())
    {
        // Validate the PlayerState pointer.
        // The assembly performs deep checks against GUObjectArray (flags 0x20000000) and Actor flags (0x140 bit 4),
        // which corresponds to the standard IsValid() check (ensuring not garbage collected or pending kill).
        if (DBDPlayerState != nullptr && IsValid(DBDPlayerState))
        {
            // Delegate the logic to the PlayerState.
            DBDPlayerState->FireScoreEvent(scoreType, percentToAward, redirectToGameEventSystem);
            return true;
        }
    }

    return false;
}

// starts at line 2450
bool ADBDPlayer::Authority_TryForceEndOngoingScoreEvent(EDBDScoreTypes scoreType)
{
    // Verify that the executing player has network authority. 
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ROLE_Authority)
    {
        // Retrieve the current player state and safely cast it to the custom ADBDPlayerState class
        ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        // Ensure the cast was successful and the object pointer is not null
        if (dbdPlayerState != nullptr)
        {
            // The disassembly explicitly checks the GUObjectArray flags and the AActor destruction bitfield.
            // This is perfectly represented by Unreal Engine's standard IsValid() macro.
            if (IsValid(dbdPlayerState) == true)
            {
                // Delegate the score event cancellation to the player state.
                // Because ADBDPlayerState is not defined in the provided structures, it is marked.
                dbdPlayerState->Authority_ForceEndOngoingScoreEvent(scoreType);
                
                // Return true to indicate the operation was successfully executed
                return true;
            }
        }
    }

    // Return false if the player lacks authority, or if the player state is missing/invalid
    return false;
}

// starts at line 2461
bool ADBDPlayer::Authority_TryForceCancelOngoingScoreEvent(EDBDScoreTypes scoreType)
{
    // Verify that the executing player has network authority. 
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ENetRole::ROLE_Authority)
    {
        // Retrieve the current player state and safely cast it to the custom ADBDPlayerState class
        ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        // Ensure the cast was successful and the object pointer is not null
        if (dbdPlayerState != nullptr)
        {
            // The disassembly explicitly checks the GUObjectArray flags and the AActor destruction bitfield.
            // This is perfectly represented by Unreal Engine's standard IsValid() macro.
            if (IsValid(dbdPlayerState) == true)
            {
                // The disassembly reveals a call to TArray::RemoveAll with a lambda predicate.
                // It accesses an array located at offset 0x938 inside the ADBDPlayerState.
                dbdPlayerState->_ongoingScoreEvents.RemoveAll([scoreType](const FOngoingScoreData& scoreData)
                {
                    return scoreData.ScoreType == scoreType;
                });
                
                // Return true to indicate the operation was successfully executed
                return true;
            }
        }
    }

    // Return false if the player lacks authority, or if the player state is missing/invalid
    return false;
}

// starts at line 2472
UPlayerInteractionHandler* ADBDPlayer::GetPlayerInteractionHandler() const
{
    // Return the Interaction Handler component.
    // This component likely manages valid interactions (repair, heal, sabotage) available to the player.
    return this->_interactionHandler;
}

// starts at line 2477
UPerkManager* ADBDPlayer::GetPerkManager() const
{
    // Return the PerkManager component.
    // Based on the assembly [rcx+558h], this member is located at offset 0x558.
    return this->_perkManager;
}

// starts at line 2482
bool ADBDPlayer::HasAnimMontageEnded() const
{
    // Access the PollableEvent via the TSharedPtr.
    // The disassembly loads the pointer from offset 0x808 (this->MontageEndEvent).
    // It implies the TSharedPtr is expected to be valid (no nullptr check in assembly).
    
    // We check if the number of events in the PollableEvent is greater than 0.
    // Using '->' operator on TSharedPtr accesses the underlying PollableEvent<bool> object.
    // We assume the PollableEvent class exposes a method to get the event count, typically Num().
    if (this->MontageEndEvent->_events.Num() > 0)
    {
        return true;
    }

    return false;
}

// starts at line 2487
bool ADBDPlayer::HasInteractionCancelledEvent() const
{
    // Access the PollableEvent via the TSharedPtr and check the event count.
    // We assume the PollableEvent class exposes a Num() function returning the count of its internal '_events' array.
    if (this->InteractionCancelledEvent->Num() > 0)
    {
        return true;
    }

    return false;
}

// starts at line 2492
EAttackType ADBDPlayer::GetAttackMontageEvent() const
{
    // Access the raw pointer to ConsumableEvent stored inside the TSharedPtr at offset 0x00
    // Offset 0x828 in ADBDPlayer is the TSharedPtr instance.
    ConsumableEvent<EAttackType>* EventObject = this->AttackMontageEvent.Get();

    // Check if the EventObject pointer is valid
    if (EventObject != nullptr)
    {
        // Check if the event buffer (TArray at offset 0x00 of ConsumableEvent) has elements
        // TArray offset 0x08 is ArrayNum
        if (EventObject->_eventBuffer.Num() > 0)
        {
            // Retrieve the first event from the buffer
            // The assembly copies the data to a local stack array and reads the first byte.
            // Functionally, this extracts the first element (index 0).
            EAttackType Result = EventObject->_eventBuffer[0];

            // Clear the event buffer and free its memory
            // The assembly explicitly sets ArrayNum to 0 and calls ResizeTo(0) if ArrayMax > 0,
            // which is the internal implementation of TArray::Empty()
            EventObject->_eventBuffer.Empty();

            return Result;
        }
    }

    // Return default value (0) if no event is found
    return EAttackType::VE_None;
}

// starts at line 2503
EInteractionAnimation ADBDPlayer::GetCurrentInteractionAnimation() const
{
    // Return the cached interaction animation enum
    // The pseudo code casts a byte member to int64, implying it's likely an enum stored as a uint8
    // Maps to member _interactionAnimation
    return this->_interactionAnimation;
}

// starts at line 2508
UInteractionDefinition* ADBDPlayer::GetCurrentInteraction() const
{
    // Access the InteractionHandler (Offset 0x0CF0)
    // The UPlayerInteractionHandler class manages available and active interactions
    UPlayerInteractionHandler* Handler = this->_interactionHandler;

    // Check if the handler pointer is valid
    if (Handler != nullptr)
    {
        // Validate the handler object (GUObjectArray check in disassembly)
        if (Handler->IsValidLowLevel() == true)
        {
            // Return the currently active interaction stored in the handler
            // Offset 0x1C8 corresponds to the _currentInteraction member in UPlayerInteractionHandler
            return Handler->_currentInteraction;
        }
    }

    return nullptr;
}

// starts at line 2517
void ADBDPlayer::SetCurrentInteractionAnimation(EInteractionAnimation Anim)
{
    // Update the member variable storing the current interaction animation state.
    this->_interactionAnimation = Anim;
}

// starts at line 2522
void ADBDPlayer::OnCurrentMontageComplete(UAnimMontage* MontageAsset, bool interrupted)
{
    // Execute the delegate located at offset 0x930 with the provided montage and interruption state.
    this->OnMontageEndDelegate.ExecuteIfBound(MontageAsset, interrupted);

    // Access the shared pointer for the MontageEndEvent at offset 0x808.
    // The disassembly suggests a TSharedPtr or similar wrapper where the actual event object is accessed.
    if (this->MontageEndEvent.IsValid() == true)
    {
        // Get the internal event object (PollableEvent<bool>).
        PollableEvent<bool>* endEvent = this->MontageEndEvent.Get();

        if (endEvent != nullptr)
        {
            // Trigger or "Add" a true (1) result to the event buffer.
            // The disassembly manually performs a TArray::Add(true) logic:
            // 1. Increment ArrayNum (offset 0x10 inside the event object).
            // 2. Check if ResizeGrow is needed.
            // 3. Set the value at the new index to true.
            endEvent->Send(true);
        }
    }
}

// starts at line 2528
void ADBDPlayer::OnCurrentMontageBlendingOut(UAnimMontage* MontageAsset, bool interrupted)
{
    // Execute the delegate located at offset 0x970 with the provided arguments.
    // The disassembly shows a direct jump to ExecuteIfBound after offsetting the 'this' pointer.
    this->OnMontageBlendingOutDelegate.ExecuteIfBound(MontageAsset, interrupted);
}

// starts at line 2533
void ADBDPlayer::DebugPrint(const FString& InString)
{
    // The disassembly checks if the "Interaction" log category verbosity is >= 6.
    // In Unreal Engine's ELogVerbosity enum, level 6 corresponds to VeryVerbose.
    // The conditional check, string array extraction, and FMsg::Logf_Internal__VA 
    // call are all completely encapsulated by the standard UE_LOG macro.
    
    // Note: 'LogInteraction' is assumed as the standard UE4 macro naming convention 
    // for the 'Interaction' category seen in the disassembly.
    UE_LOG(LogInteraction, VeryVerbose, TEXT("___ %s ____ "), *InString);
}

// starts at line 2538
bool ADBDPlayer::IsStrafing() const
{
    // Retrieve the generic Character Movement Component
    // Disassembly: Accesses offset 0x3E8 (ACharacter::CharacterMovement)
    UCharacterMovementComponent* MovementComponent = this->CharacterMovement;

    // Check if the component exists
    if (MovementComponent != nullptr)
    {
        // Cast to the custom Dead by Daylight movement component
        // Disassembly: Checks ClassTreeIndex against UDBDCharacterMovementComponent::StaticClass
        UDBDCharacterMovementComponent* DBDMovement = Cast<UDBDCharacterMovementComponent>(MovementComponent);

        // Check if the cast was successful
        if (DBDMovement != nullptr)
        {
            // Check a boolean flag at offset 0x758 in the movement component.
            // This flag likely indicates the movement logic considers itself in "Strafe Mode".
            // Disassembly: cmp byte ptr [rbx+758h], 0
            bool bAllowNavigation = DBDMovement->_allowNavigation;

            if (bAllowNavigation)
            {
                // Verify that the player is actually holding/using an item that enables strafing.
                // Disassembly: call ?IsUsingStrafeItem@ADBDPlayer@@QEBA_NXZ
                if (this->IsUsingStrafeItem())
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// starts at line 2543
bool ADBDPlayer::IsUsingStrafeItem() const
{
    // Check if the Inventory is not empty
    // Disassembly: Checks ArrayNum at offset 0xC60
    if (this->Inventory.Num() > 0)
    {
        // Retrieve the first item (Active Item)
        // Disassembly: Accesses Data at offset 0xC58
        ACollectable* CurrentItem = this->Inventory[0];

        // Ensure the item exists
        if (CurrentItem != nullptr)
        {
            // Verify item validity (Pending Kill / Actor Being Destroyed checks)
            if (CurrentItem->IsPendingKill() == false && CurrentItem->IsActorBeingDestroyed() == false)
            {
                // Check specific item flags.
                
                // Offset 0x3A0: Likely '_canStrafe' or '_enableStrafing'.
                // Note: This differs from IsUsingAimItem which checked offset 0x3A1.
                // This suggests 0x3A0 is specific to items that change movement mode (Flashlight),
                // while 0x3A1 might be for items that just aim (Maps/Keys).
                bool bCanStrafe = CurrentItem->StrafeOnUse;

                // Offset 0x3A9: Likely '_isAiming' or '_isInUse'.
                // This is the same offset checked in IsUsingAimItem.
                bool bIsUsing = CurrentItem->IsInUse;

                // If the item supports strafing and is currently being used/aimed
                if (bCanStrafe && bIsUsing)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// starts at line 2553
void ADBDPlayer::UpdateStrafeCamera(float DeltaSeconds)
{
    // Retrieve the SpringArmComponent (CameraBoom) stored at offset 0x928.
    USpringArmComponent* CameraBoom = this->CameraBoom;

    // Validation check: Ensure the component exists.
    if (CameraBoom == nullptr)
    {
        return;
    }

    // Validation check: Ensure the component is valid (not pending kill / garbage collected).
    // The disassembly checks the internal object flags against the global object array.
    if (CameraBoom->IsValidLowLevel() == false)
    {
        return;
    }

    // Check the current update alpha state (Offset 0xF38).
    // If it is less than or equal to 0, the interpolation is complete.
    if (this->_strafeOffsetUpdateAlpha <= 0.0f)
    {
        // Snap the camera offset directly to the target.
        // _targetStrafeOffset is at offset 0xF20.
        CameraBoom->SocketOffset = this->_targetStrafeOffset;
    }
    else
    {
        // Calculate the interpolation progress (0.0 to 1.0).
        // kStrafeOffsetUpdateDuration is a constant at offset 0xF3C.
        // The alpha counts down, so we invert it to get progress.
        float CurrentFraction = this->_strafeOffsetUpdateAlpha / this->kStrafeOffsetUpdateDuration;
        float Progress = 1.0f - CurrentFraction;

        // Calculate the eased alpha using an Ease-In-Out Quadratic function.
        // The disassembly performs manual math calling powf, consistent with FMath::InterpEaseInOut with Exp=2.
        float EasedAlpha = 0.0f;
        float ScaledProgress = Progress * 2.0f;

        if (ScaledProgress < 1.0f)
        {
            // Ease In part: 0.5 * (2x)^2
            EasedAlpha = 0.5f * powf(ScaledProgress, 2.0f);
        }
        else
        {
            // Ease Out part
            float InvProgress = 1.0f - (ScaledProgress - 1.0f);
            EasedAlpha = 0.5f * ((1.0f - powf(InvProgress, 2.0f)) + 1.0f);
        }

        // Interpolate the SocketOffset between Start and Target using the Eased Alpha.
        // _startStrafeOffset is at offset 0xF2C.
        // _targetStrafeOffset is at offset 0xF20.
        FVector NewSocketOffset;
        NewSocketOffset.X = FMath::Lerp(this->_startStrafeOffset.X, this->_targetStrafeOffset.X, EasedAlpha);
        NewSocketOffset.Y = FMath::Lerp(this->_startStrafeOffset.Y, this->_targetStrafeOffset.Y, EasedAlpha);
        NewSocketOffset.Z = FMath::Lerp(this->_startStrafeOffset.Z, this->_targetStrafeOffset.Z, EasedAlpha);

        // Apply the new offset to the camera boom.
        CameraBoom->SocketOffset = NewSocketOffset;

        // Decrement the alpha timer by DeltaSeconds and clamp at 0.0f.
        this->_strafeOffsetUpdateAlpha = FMath::Max(this->_strafeOffsetUpdateAlpha - DeltaSeconds, 0.0f);
    }
}

// starts at line 2574
bool ADBDPlayer::CheckTransitionInteraction()
{
    // Retrieve the interaction handler from offset 0xCF0.
    UPlayerInteractionHandler* InteractionHandler = this->_interactionHandler;

    // Check if the handler has flagged an interaction request (offset 0x1B0).
    // Disassembly: cmp byte ptr [rdx+1B0h], 0
    if (InteractionHandler->_hasInteractionRequest)
    {
        // Retrieve the specific interaction definition being requested (offset 0x1B8).
        UInteractionDefinition* RequestedInteraction = InteractionHandler->_requestedInteraction;

        // Verify the interaction object is valid (not null and not PendingKill).
        if (IsValid(RequestedInteraction))
        {
            // Delegate the check to the interaction definition itself.
            // Arguments passed:
            // - RCX (Member context): RequestedInteraction
            // - RDX (Arg1): this (The ADBDPlayer)
            // - R8  (Arg2): InteractionHandler->_requestedInteractionType (offset 0x1C0)
            // - R9  (Arg3): true (constant 1)
            return RequestedInteraction->IsInteractionPossible(this, InteractionHandler->_requestedInteractionType, true);
        }
    }

    // If no request exists or the interaction object is invalid, return false.
    return false;
}

// starts at line 2592
bool ADBDPlayer::CheckTransitionInterrupting() const
{
    // Retrieve the interaction handler from offset 0xCF0.
    UPlayerInteractionHandler* InteractionHandler = this->_interactionHandler;

    // Check if the handler has flagged an interruption request.
    // The disassembly checks the byte at offset 0x2B8 of the InteractionHandler.
    // Disassembly: cmp byte ptr [rax+2B8h], 0
    if (InteractionHandler->_hasInterruptionRequest)
    {
        // Retrieve the interruption definition object associated with the request.
        // The disassembly loads the pointer from offset 0x2C8.
        UInterruptionDefinition* RequestedInterruption = InteractionHandler->_requestedInterruptionDefinition;

        // Validate that the requested interruption definition is valid (not nullptr and not marked PendingKill).
        // The disassembly performs the explicit GUObjectArray flag check here.
        if (IsValid(RequestedInterruption))
        {
            return true;
        }
    }

    // If there is no request or the definition is invalid, return false.
    return false;
}

// starts at line 2601
bool ADBDPlayer::CheckTransitionAttack(EAttackType attackType) const
{
    // Access the raw pointer from the TSharedPtr located at offset 0x818.
    // TSharedPtr layout typically holds the Object pointer as its first member.
    PollableEvent<EAttackType>* EventPtr = this->AttackEvent.Get();

    // The disassembly does not explicitly check if EventPtr is nullptr here, 
    // assuming it is valid or relying on [rcx+818h] being valid.
    
    // Check the 'Num' of the '_events' TArray.
    // '_events' starts at offset 0x18 of PollableEvent.
    // TArray's 'ArrayNum' is at offset 0x08 relative to the array start.
    // Total offset: 0x18 + 0x08 = 0x20.
    // Disassembly: cmp dword ptr [rax+20h], 0
    if (EventPtr->_events.Num() <= 0)
    {
        return false;
    }

    // Access the Data pointer of the '_events' TArray.
    // TArray's 'Data' pointer is at offset 0x00 relative to the array start.
    // Total offset: 0x18 + 0x00 = 0x18.
    // Disassembly: mov rax, [rax+18h] (Load Data pointer) -> cmp [rax], dl (Compare first element)
    if (EventPtr->_events[0] == attackType)
    {
        return true;
    }

    return false;
}

// starts at line 2612
bool ADBDPlayer::CheckTransitionBlink() const
{
    // Access the raw pointer from the TSharedPtr<PollableEvent<bool>> located at offset 0x858.
    PollableEvent<bool>* EventPtr = this->BlinkEvent.Get();

    // Check if the event list is empty.
    // _events is at offset 0x18 of PollableEvent.
    // TArray.Num is at offset 0x8 relative to TArray start -> 0x20 total.
    // Disassembly: cmp dword ptr [rax+20h], 0
    if (EventPtr->_events.Num() <= 0)
    {
        return false;
    }

    // Return the first element of the event list.
    // TArray.Data is at offset 0x0 relative to TArray start -> 0x18 total.
    // Disassembly: mov rax, [rax+18h] -> movzx eax, byte ptr [rax]
    return EventPtr->_events[0];
}

// starts at line 2623
void ADBDPlayer::SetPawnAuthoritativeMovement(bool enabled)
{
    // Log the change.
    // dbd::ToTCHAR is a helper converting bool to "True"/"False".
    UE_LOG(AuthoritativeMovement, Verbose, TEXT("%s AuthoritativeMovement set to %s"), 
        *this->GetName(), 
        enabled ? TEXT("True") : TEXT("False"));

    // Call the internal flag setter.
    // The assembly passes 0 for the flag index (xor edx, edx), which corresponds to EAuthoritativeMovementFlag::INTERACTION.
    this->SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag::INTERACTION, enabled);
}

// starts at line 2630
void ADBDPlayer::SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag FlagIndex, bool Enabled)
{
    // Calculate the bitmask corresponding to the provided flag index
    int32 FlagMask = 1 << (int32)FlagIndex;

    // Update the internal bitfield flags
    if (Enabled)
    {
        this->_authoritativeMovementFlags._flags |= FlagMask;
    }
    else
    {
        this->_authoritativeMovementFlags._flags &= ~FlagMask;
    }

    // Debug logging loop: Iterates through all flags (0 to 5) to log the current state.
    // Only runs if the 'AuthoritativeMovement' log category is set to Verbose or higher.
    if (UE_LOG_ACTIVE(AuthoritativeMovement, Verbose))
    {
        for (int32 i = 0; i < 5; i++)
        {
            bool bIsSet = (this->_authoritativeMovementFlags._flags & (1 << i)) != 0;
            
            // Log format: "[PlayerName] FlagName set to True/False"
            // Uses Enum::ToString for the flag name and a helper (dbd::ToTCHAR) for the boolean.
            UE_LOG(AuthoritativeMovement, Verbose, TEXT("[%s] %s set to %s"), 
                *this->GetName(), 
                *EnumToString((EAuthoritativeMovementFlag)i), 
                bIsSet ? TEXT("True") : TEXT("False"));
        }
    }

    // Determine if any authoritative movement flags are currently set
    bool bAnyFlagSet = (this->_authoritativeMovementFlags._flags != 0);

    // Update the AuthoritativeMovementComponent's enabled state
    if (this->_authoritativeMovementComponent != nullptr)
    {
        this->_authoritativeMovementComponent->SetEnabled(bAnyFlagSet);
    }

    // Determine the new collision response.
    // Calls a virtual function at offset 0xE20 (likely GetAuthoritativeMovementCollisionResponse or similar).
    ECollisionResponse NewResponse = this->GetPawnCollisionChannel();

    // Apply the new collision response to the CapsuleComponent for specific channels.
    if (this->CapsuleComponent != nullptr)
    {
        // ECC_Pawn (Channel 2)
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);
        
        // ECC_GameTraceChannel3 (Channel 16) - Often used for project-specific traces (e.g., Interaction).
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, NewResponse);
    }
}

// starts at line 2653
void ADBDPlayer::OnInteractionZoneOverlap(UInteractionDefinition* interactionDef, bool overlapping)
{
    // Check if the player is locally controlled.
    // The disassembly calls a virtual function at offset 0x630, which is standard for IsLocallyControlled in UE4.
    if (this->IsLocallyControlled() == true)
    {
        // Retrieve the interaction handler component at offset 0x0CF0.
        UPlayerInteractionHandler* interactionHandler = this->_interactionHandler;

        if (interactionHandler != nullptr)
        {
            // If the player entered the zone, add the interaction to the available list.
            if (overlapping == true)
            {
                interactionHandler->AddInteraction(interactionDef);
            }
            // If the player left the zone, remove it from the list.
            else
            {
                interactionHandler->RemoveInteraction(interactionDef);
            }
        }
    }

    // Execute the delegate located at offset 0x9B0.
    // This allows other systems (like UI or Quests) to respond to the overlap event.
    // The disassembly tail-calls ExecuteIfBound after cleaning up the stack.
    this->OnInteractionZoneOverlapDelegate.ExecuteIfBound(interactionDef, overlapping);
}

// starts at line 2675
void ADBDPlayer::SetGuidingPlayer(ADBDPlayer* GuidingPlayer)
{
    // Update the guiding player reference (Offset 0xE08)
    this->_guidingPlayer = GuidingPlayer;

    // Logging logic: Only execute if the 'Interaction' log category is active at Verbose level (5).
    if (UE_LOG_ACTIVE(Interaction, Verbose))
    {
        FString GuidingPlayerName = TEXT("Null");

        // Validate the provided player to determine the name string
        // 1. Check if pointer is not null
        // 2. Standard IsValid checks (Global object array index)
        // 3. Check if actor is being destroyed (Offset 0x140 bit 4)
        if (GuidingPlayer != nullptr && IsValid(GuidingPlayer) && !GuidingPlayer->bActorIsBeingDestroyed)
        {
            // Call a virtual function at offset 0x1070.
            if (GuidingPlayer->IsValidImpl())
            {
                GuidingPlayerName = GuidingPlayer->GetName();
            }
        }

        // Log the operation
        UE_LOG(Interaction, Verbose, TEXT("[%s] SetGuidingPlayer [%s]"), *this->GetName(), *GuidingPlayerName);
    }
}

// starts at line 2681
ADBDPlayer* ADBDPlayer::GetGuidingPlayer() const
{
    // Return the cached pointer to the player currently guiding this player
    // Maps to member variable _guidingPlayer
    return this->_guidingPlayer;
}

// starts at line 2686
float ADBDPlayer::GetPercentMovementSpeed() const
{
    // 1. Retrieve the maximum possible speed for the current movement mode
    float MaxSpeed = this->GetMaximumMovementSpeed();

    // 2. Check for "Paradise" State
    // Likely a debug, lobby, or tutorial state where movement is always considered "full speed"
    if (this->_inParadise)
    {
        return 1.0f;
    }

    // 3. Calculate Speed Ratio
    if (MaxSpeed > 0.0f)
    {
        // Get the current velocity vector
        FVector Velocity = this->GetVelocity();

        // Calculate the 2D magnitude (Speed on the ground plane), ignoring vertical movement (Z).
        // The ASM manually computes sqrt(X*X + Y*Y).
        float CurrentSpeed2D = Velocity.Size2D();

        // Return the ratio
        return CurrentSpeed2D / MaxSpeed;
    }

    return 0.0f;
}

// starts at line 2702
bool ADBDPlayer::GetIsBeyondCrouchWalkSpeed() const
{
    // Check if the current movement speed percentage is greater than 20%
    // 0.2f likely represents the normalized speed threshold for crouch walking
    return this->GetPercentMovementSpeed() > 0.2f;
}

// starts at line 2707
bool ADBDPlayer::ShouldFall() const
{
    // Retrieve the collision shape (Capsule) from the component to perform the sweep.
    FCollisionShape CollisionShape = this->CapsuleComponent->GetCollisionShape();

    // Calculate the start and end points for the downward trace.
    // Start is the current Actor location.
    FVector Start = this->GetActorLocation();

    // Calculate the End vector by moving downwards (negative UpVector) by the minimum fall height.
    // MinFallHeight is located at offset 0x7DC.
    // Logic: End = Start - (UpVector * MinFallHeight)
    FVector End = Start - (this->GetActorUpVector() * this->MinFallHeight);

    // Setup collision query parameters to ignore the player itself.
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    // The assembly sets bFindInitialOverlaps to true and IgnoreMask to 0x100.
    Params.bFindInitialOverlaps = true;

    FHitResult OutHit;

    // Perform a sweep test against the world to check for ground.
    // Channel 0x1F (31) corresponds to ECC_GameTraceChannel18.
    // Uses the actor's current rotation (GetActorQuat).
    bool bHit = this->GetWorld()->SweepSingleByChannel(
        OutHit,
        Start,
        End,
        this->GetActorQuat(),
        ECollisionChannel::ECC_GameTraceChannel18,
        CollisionShape,
        Params,
        FCollisionResponseParams::DefaultResponseParam
    );

    // Logic inversion:
    // If bHit is true, ground was detected -> ShouldFall is false.
    // If bHit is false, no ground detected -> ShouldFall is true.
    return !bHit;
}

// starts at line 2742
FRotator ADBDPlayer::GetPlayerDirection() const
{
    // Access the RootComponent (Offset 0x160 in this build's AActor layout)
    USceneComponent* RootComp = this->GetRootComponent();

    // The assembly performs the logic of RootComp->GetComponentRotation() manually/inlined:
    // 1. Compare ComponentToWorld.Rotation (Quat) with the internal CachedQuat.
    // 2. If different, convert Quat to Rotator and update the Cache.
    // 3. Return the CachedRotator.
    
    // In standard engine terms, this is simply:
    return this->GetActorRotation();
}

// starts at line 2746
void ADBDPlayer::SetPlayerDirection(FRotator rotation)
{
    // Apply the specified rotation to the Actor.
    this->SetActorRotation(rotation, ETeleportType::None);

    // Check if the Pawn is configured to use the Controller's Yaw rotation.
    // Offset 0x388 bit 2 corresponds to 'bUseControllerRotationYaw' in standard APawn.
    // If enabled, the Pawn's rotation is normally overwritten by the Controller's rotation every frame.
    // To make this manual rotation persist, we must also update the Controller's rotation.
    
    if (this->bUseControllerRotationYaw)
    {
        ADBDPlayerController* PlayerController = this->GetPlayerController();

        // Perform standard validity checks (IsValid and not pending kill).
        if (PlayerController != nullptr && IsValid(PlayerController) && !PlayerController->bActorIsBeingDestroyed)
        {
            // Get current control rotation to preserve Pitch and Roll.
            FRotator ControlRotation = PlayerController->GetControlRotation();

            // Sync the Controller's Yaw with the new Actor Yaw.
            ControlRotation.Yaw = rotation.Yaw;

            // Apply the new rotation to the Controller.
            PlayerController->SetControlRotation(ControlRotation);
        }
    }
}

// starts at line 2760
void ADBDPlayer::UpdatePlayerCamera()
{
    // Check if the player is locally controlled.
    // If not, clear the cached camera and return.
    if (this->IsLocallyControlled() == false)
    {
        this->_cachedPlayerCamera.Reset();
        return;
    }

    // Retrieve the custom Player Controller.
    ADBDPlayerController* PlayerController = this->GetPlayerController();

    // Determine the viewport size to calculate aspect ratio.
    int32 SizeX = 0;
    int32 SizeY = 0;
    
    // Note: Disassembly implies direct call without null check on PC, assuming validity for locally controlled pawns.
    if (PlayerController != nullptr)
    {
        PlayerController->GetViewportSize(SizeX, SizeY);
    }

    // Calculate Aspect Ratio and update server if it has changed.
    if (SizeY != 0)
    {
        float CurrentAspectRatio = (float)SizeX / (float)SizeY;

        // Accessing _screenAspectRatio at offset 0x1290.
        if (CurrentAspectRatio != this->_screenAspectRatio)
        {
            this->Server_UpdateScreenAspectRatio(CurrentAspectRatio);
        }
    }

    // Retrieve all UCameraComponents attached to this actor.
    // Disassembly explicitly uses TInlineAllocator<24> for optimization.
    TArray<UCameraComponent*, TInlineAllocator<24>> CameraComponents;
    this->GetComponents(CameraComponents);

    UCameraComponent* TargetCamera = nullptr;

    // Iterate through components to find the specific one intended for use.
    for (UCameraComponent* Component : CameraComponents)
    {
        // The disassembly performs a bitwise check on a member at offset 0xA0 with mask 0x40000.
        // In the context of ActorComponents, this typically corresponds to checking the 'bIsActive' flag 
        // or a specific component state flag.
        if (Component->bIsActive == true)
        {
            TargetCamera = Component;
            break;
        }
    }

    // Update the cached weak object pointer with the found camera.
    if (TargetCamera != nullptr)
    {
        this->_cachedPlayerCamera = TargetCamera;
    }
    else
    {
        // If no matching camera was found, invalidate the cache.
        this->_cachedPlayerCamera.Reset();
    }
}

// starts at line 2785
UObject* ADBDPlayer::GetPlayerCamera() const
{
    // Resolve the weak pointer to the cached camera object.
    // _cachedPlayerCamera is defined as TWeakObjectPtr<UObject> (or FWeakObjectPtr).
    // This ensures the function doesn't return a dangling pointer if the camera has been destroyed.
    return this->_cachedPlayerCamera.Get();
}

// starts at line 2790
void ADBDPlayer::Server_UpdateScreenAspectRatio_Implementation(const float value)
{
    this->_screenAspectRatio = value;
}

// starts at line 2800
float ADBDPlayer::GetCameraAspectRatio() const
{
    // Return the cached screen aspect ratio member variable
    // This variable is likely updated during tick or camera setup
    return this->_screenAspectRatio;
}

// starts at line 2805
void ADBDPlayer::SetMontageMode(bool montageMode)
{
    this->_isMontageMode = montageMode;
}

// starts at line 2820
bool ADBDPlayer::CanAuthoritativePush()
{
    // Evaluate the lower 4 bits (using the 0xF mask) of the authoritative movement bitfield.
    // If any of these bits are set, the player is permitted to perform an authoritative push.
    return (this->_authoritativeMovementFlags._flags & 0x0F) != 0;
}

// starts at line 2827
bool ADBDPlayer::CanBeAuthoritativePushed()
{
    // Return true only if all of the following conditions are met:
    // 1. The authoritative movement flags bitfield is empty (0).
    // 2. The pawn's collision channel is currently set to block (ECR_Block).
    // 3. The player is not actively performing an interaction.
    // 4. The player is not currently snapping to a specific location/interaction.
    // 5. The player is not actively interrupting another player or action.    
    return (this->_authoritativeMovementFlags._flags == 0)
        && (this->GetPawnCollisionChannel() == ECR_Block)
        && (this->IsInteracting == false)
        && (this->IsSnapping() == false)
        && (this->_isInterrupting == false);
}

// starts at line 2832
void ADBDPlayer::ClampPitch()
{
    // Check if the pawn is controlled by a local player.
    if (this->IsLocallyControlled())
    {
        AController* CurrentController = this->Controller;

        // Ensure the controller is valid before accessing it.
        if (CurrentController != nullptr)
        {
            // Get the current control rotation.
            // The disassembly passes a stack address (rdx) to the virtual function at 0x5B8 (GetControlRotation).
            FRotator Rotation = CurrentController->GetControlRotation();

            // Clamp the Pitch component of the rotation.
            // In UE4, FRotator.Pitch is typically the first float member.
            // We use FMath::ClampAngle with values loaded from offsets 0x7E8 (Min) and 0x7EC (Max).
            Rotation.Pitch = FMath::ClampAngle(Rotation.Pitch, this->PitchLimitLower, this->PitchLimitUpper);

            // Update the control rotation with the clamped value.
            // The disassembly passes the modified stack struct to the virtual function at 0x5C0 (SetControlRotation).
            CurrentController->SetControlRotation(Rotation);
        }
    }
}

// starts at line 2849
FAnimData* ADBDPlayer::GetAnimData()
{
    // Return the pointer to the _animData member variable (Offset 0x0FA9)
    return &this->_animData;
}

// starts at line 2854
void ADBDPlayer::Server_TeleportTo_Implementation(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck)
{
    // The code performs a jump to a virtual function at offset 0x450 (vtable + 0x450).
    // Given the parameters (DestLocation, DestRotation, bIsATest, bNoCheck), this signature matches 
    // the standard AActor::TeleportTo virtual function.
    // The function forwards the arguments directly to TeleportTo and ignores the boolean return value.
    this->TeleportTo(DestLocation, DestRotation, bIsATest, bNoCheck);
}

// starts at line 2864
void ADBDPlayer::SetCapsuleSize(float NewRadius, float NewHalfHeight, bool bUpdateOverlaps)
{
    // Clamp the half-height to ensure it's not smaller than the radius
    float ClampedHalfHeight = FMath::Max(NewHalfHeight, NewRadius);

    // Get the current capsule half-height
    float CurrentHalfHeight = this->CapsuleComponent->GetUnscaledCapsuleHalfHeight();

    // Access the default object to calculate the base eye height offset
    ADBDPlayer* DefaultObject = this->GetClass()->GetDefaultObject<ADBDPlayer>();
    float DefaultCapsuleHalfHeight = DefaultObject->CapsuleComponent->GetUnscaledCapsuleHalfHeight();
    float HeightDelta = DefaultCapsuleHalfHeight - ClampedHalfHeight;

    // Recalculate base eye height
    this->RecalculateBaseEyeHeight();

    // Adjust mesh relative location based on capsule size change
    if (this->Mesh != nullptr)
    {
        // Calculate new Z offset for the mesh
        float DefaultMeshZ = DefaultObject->Mesh->RelativeLocation.Z;
        this->Mesh->RelativeLocation.Z = HeightDelta + DefaultMeshZ;
        
        // Update the base translation offset
        this->BaseTranslationOffset.Z = this->Mesh->RelativeLocation.Z;
    }

    // Set the new capsule size
    this->CapsuleComponent->SetCapsuleSize(NewRadius, ClampedHalfHeight, bUpdateOverlaps);

    // Adjust actor location to keep the bottom of the capsule at the same relative position
    // This prevents the character from sinking into the floor when shrinking or popping up when growing
    if (this->RootComponent != nullptr)
    {
        FVector NewLocation = this->GetActorLocation();
        // Adjust Z by the difference in half-heights (Old - New)
        // Note: The logic effectively maintains the bottom position:
        // Bottom = Center - HalfHeight. If HalfHeight decreases, Center must decrease to keep Bottom constant.
        // Wait, the disassembly logic is: NewLocation.Z = CurrentZ - (CurrentHalfHeight - ClampedHalfHeight)
        // If getting smaller (Current > Clamped), Z decreases (moves down).
        NewLocation.Z -= (CurrentHalfHeight - ClampedHalfHeight);
        
        this->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);
    }
}

// starts at line 2951
void ADBDPlayer::ResetCapsuleSize()
{
    // Check if the flag allowing capsule size updates is set (offset 0xE10)
    if (this->_updateCapsuleSize != false)
    {
        // Retrieve the Class Default Object (CDO) to access the default properties for this class.
        // This corresponds to accessing the class (offset 0x10) and then the default object (offset 0xF8).
        ADBDPlayer* DefaultObject = this->GetClass()->GetDefaultObject<ADBDPlayer>();

        if (DefaultObject != nullptr)
        {
            // Access the CapsuleComponent of the default object (Offset 0x3F0).
            UCapsuleComponent* DefaultCapsule = DefaultObject->CapsuleComponent;

            if (DefaultCapsule != nullptr)
            {
                // Read the default dimensions from the CDO's capsule.
                // Offsets 0x630 and 0x634 correspond to the internal float values for HalfHeight and Radius.
                float DefaultHalfHeight = DefaultCapsule->GetUnscaledCapsuleHalfHeight();
                float DefaultRadius = DefaultCapsule->GetUnscaledCapsuleRadius();

                // Call the custom SetCapsuleSize function to revert to defaults.
                // Passes 'true' for bUpdateOverlaps (R9B = 1 in disassembly).
                this->SetCapsuleSize(DefaultRadius, DefaultHalfHeight, true);
            }
        }
    }
}

// starts at line 2960
FVector ADBDPlayer::GetVelocity() const
{
    // Pass the call to the parent class implementation.
    // In Unreal Engine, 'Super' is a typedef for the parent class (APawn or ACharacter).
    return Super::GetVelocity();
}

// starts at line 2980
FVector ADBDPlayer::GetFeetPosition() const
{
    // 1. Get the current actor location (typically the geometric center of the capsule)
    FVector ActorLocation = FVector::ZeroVector;
    if (this->GetRootComponent() != nullptr)
    {
        ActorLocation = this->GetRootComponent()->GetComponentLocation();
    }

    // 2. Retrieve the Capsule Component (Offset 0x3F0)
    UCapsuleComponent* Capsule = this->CapsuleComponent;

    if (Capsule != nullptr)
    {
        // Get the component's world scale (Offset 0x150)
        FVector Scale3D = Capsule->GetComponentScale();

        // Calculate absolute scale values to handle negative scaling
        float AbsScaleX = FMath::Abs(Scale3D.X);
        float AbsScaleY = FMath::Abs(Scale3D.Y);
        float AbsScaleZ = FMath::Abs(Scale3D.Z);

        // Determine the minimum scaling factor across all axes
        // The assembly logic explicitly selects the smallest dimension
        float MinScale = FMath::Min(AbsScaleX, FMath::Min(AbsScaleY, AbsScaleZ));

        // Calculate the scaled half-height (Offset 0x630 corresponds to CapsuleHalfHeight)
        float ScaledHalfHeight = Capsule->CapsuleHalfHeight * MinScale;

        // 3. Calculate feet position by moving down from the center
        // Formula: Location - (UpVector * ScaledHalfHeight)
        return ActorLocation - (FVector::UpVector * ScaledHalfHeight);
    }

    // Return center location if capsule is missing
    return ActorLocation;
}

// starts at line 2985
FVector ADBDPlayer::GetFeetPositionAtTargetLocation(const FVector& location) const
{
    // Retrieve the Capsule Component (Offset 0x3F0)
    UCapsuleComponent* Capsule = this->CapsuleComponent;

    // Default to the input location if the capsule is missing (though the ASM assumes it exists based on offsets)
    if (Capsule != nullptr)
    {
        // Get the component's world scale (Offset 0x150)
        FVector Scale3D = Capsule->GetComponentScale();

        // Calculate absolute scale values (handle negative scaling)
        float AbsScaleX = FMath::Abs(Scale3D.X);
        float AbsScaleY = FMath::Abs(Scale3D.Y);
        float AbsScaleZ = FMath::Abs(Scale3D.Z);

        // Determine the minimum scaling factor across all axes
        // Uses minps intrinsics to find the smallest element
        float MinScale = FMath::Min(AbsScaleX, FMath::Min(AbsScaleY, AbsScaleZ));

        // Calculate the scaled half-height (Offset 0x630 is CapsuleHalfHeight)
        float ScaledHalfHeight = Capsule->CapsuleHalfHeight * MinScale;

        // Calculate feet position relative to the provided target location
        // Formula: TargetLocation - (UpVector * ScaledHalfHeight)
        return location - (FVector::UpVector * ScaledHalfHeight);
    }

    return location;
}

// starts at line 2990
FVector ADBDPlayer::GetMeshFeetPosition() const
{
    // 1. Get the world location of the Skeletal Mesh (Offset 0x3E0)
    // In ACharacter, the 'Mesh' member is located at this offset.
    // The assembly reads offset 0x140 from the component (ComponentToWorld.Translation).
    FVector Result = this->GetMesh()->GetComponentLocation();

    // 2. Calculate the "Feet" position based on the Capsule
    // Calls the helper function we reconstructed earlier: GetFeetPosition()
    FVector CapsuleFeetPos = this->GetFeetPosition();

    // 3. Override the Mesh's Z height with the Capsule's Feet Z height
    // This ensures the returned vector aligns with the mesh's horizontal displacement 
    // (e.g., during animations) but remains grounded at the collider's floor level.
    Result.Z = CapsuleFeetPos.Z;

    return Result;
}

// starts at line 2997
void ADBDPlayer::ToggleInvertLookUpAxis()
{
    this->_invertLookUpAxis = !this->_invertLookUpAxis;
}

// starts at line 3003
void ADBDPlayer::SetInvertLookUpAxis(bool invert)
{
    this->_invertLookUpAxis = invert;
}

// starts at line 3041
UInterruptionDefinition* ADBDPlayer::FindPossibleInterruption(const ADBDPlayer* interruptor) const
{
    // Access the InteractionHandler (Offset 0x0CF0)
    if (this->_interactionHandler != nullptr)
    {
        // Retrieve the current interaction from the handler (Offset 0x1C8)
        // Stored in a temporary variable for initial validation
        UInteractionDefinition* CurrentInteraction = this->_interactionHandler->_currentInteraction;

        // Check if the interaction instance is not null
        if (CurrentInteraction != nullptr && CurrentInteraction->IsValidLowLevel() == true)
        {
            // Call a method on the interaction.
            // Note: The disassembly explicitly calls 'AOnlineBeaconClient::GetNetConnection',
            // but the context and pseudo-code indicate this is 'UInteractionDefinition::GetInteractor'.
            // This suggests a VTable offset collision or shared interface in the engine build.
            // We use the pseudo-code interpretation as it aligns with the 'UInteractionDefinition' type.
            UInteractor* Interactor = CurrentInteraction->GetInteractor();

            // Check if the Interactor is valid (not null and valid low level)
            if (Interactor != nullptr && Interactor->IsValidLowLevel() == true)
            {
                // Forward the call to the interaction's definition of FindPossibleInterruption.
                // Passes the interruptor (argument) and the current player (this) as the interruptee.
                return CurrentInteraction->FindPossibleInterruption(interruptor, this);
            }
        }
    }

    return nullptr;
}

// starts at line 3051
bool ADBDPlayer::RequestInterruption(ADBDPlayer* OtherParty)
{
    // Delegate the interruption request to the player's interaction handler component.
    // The pseudo-code maps the call UPlayerInteractionHandler::TryInterruption(this->_interactionHandler, otherParty) 
    // to a member function call on the _interactionHandler instance.
    return this->_interactionHandler->TryInterruption(OtherParty);
}

// starts at line 3056
void ADBDPlayer::SetInParadise(bool inParadise)
{
    this->_inParadise = inParadise;
}

// starts at line 3061
void ADBDPlayer::IsIncapacitated()
{
    return this->_inParadise;
}

// starts at line 3066
void ADBDPlayer::Server_SetEnableCapsuleDynamicResize_Implementation(bool enabled)
{
    // If dynamic resizing is being disabled, immediately reset the capsule to its default size.
    if (enabled == false)
    {
        this->ResetCapsuleSize();
    }

    // Update the state flag (offset 0xE10 in the disassembly)
    this->_updateCapsuleSize = enabled;

    // Propagate the change to all clients via a multicast RPC
    this->Multicast_SetEnableCapsuleDynamicResize(enabled);
}

// starts at line 3077
void ADBDPlayer::Server_SetSlasherAutoAttackOnCamperInteractFrameDelay_Implementation(int frames)
{
    this->_framesForSlasherAutoAttackOnCamperInteract = frames;
}

// starts at line 3088
void ADBDPlayer::Multicast_SetEnableCapsuleDynamicResize_Implementation(bool enabled)
{
    // Check if dynamic resizing is being enabled or disabled.
    if (enabled == true)
    {
        // Set the flag at offset 0xE10 to true to allow dynamic updates.
        this->_updateCapsuleSize = true;
    }
    else
    {
        // If disabling, first reset the capsule size to its default configuration.
        this->ResetCapsuleSize();

        // Set the flag at offset 0xE10 to false to stop dynamic updates.
        this->_updateCapsuleSize = false;
    }
}

// starts at line 3107
void ADBDPlayer::AttachInteractor(UInteractor* interactor)
{
    // Assign the provided interactor to the interaction handler's weak pointer.
    // The C++ assignment operator directly compiles into FWeakObjectPtr::operator=.
    this->_interactionHandler->_attachedInteractor = interactor;
}

// starts at line 3112
void ADBDPlayer::DetachInteractor()
{
    // Access the InteractionHandler member (Offset 0x0CF0)
    // Note: The disassembly accesses this pointer directly without a null check, 
    // but for functional stability, a check is implied or recommended.
    if (this->_interactionHandler != nullptr)
    {
        // Set the _attachedInteractor (Offset 0x2B0 in UPlayerInteractionHandler) to nullptr.
        // The disassembly manually constructs a null WeakObjectPtr on the stack and copies its 
        // InternalIndex and ObjectSerialNumber to the member variable, effectively clearing it.
        this->_interactionHandler->_attachedInteractor = nullptr;
    }
}

// starts at line 3117
bool ADBDPlayer::IsValidImpl() const
{
    // 1. Check Interaction Handler
    // Disassembly: Accesses offset 0xCF0
    UPlayerInteractionHandler* InteractionHandler = this->_interactionHandler;
    if (InteractionHandler == nullptr || InteractionHandler->IsPendingKill())
    {
        return false;
    }

    // 2. Check Player Data
    // Disassembly: Accesses offset 0xD08
    UDBDPlayerData* PlayerData = this->_playerData;
    if (PlayerData == nullptr || PlayerData->IsPendingKill())
    {
        return false;
    }

    // 3. Check Movement Component
    // Disassembly: Calls virtual GetMovementComponent (vtable offset 0x5A8)
    UPawnMovementComponent* MovementComponent = this->GetMovementComponent();
    if (MovementComponent == nullptr || MovementComponent->IsPendingKill())
    {
        return false;
    }

    // 4. Check Skeletal Mesh
    // Disassembly: Accesses offset 0x3E0
    USkeletalMeshComponent* Mesh = this->Mesh;
    if (Mesh == nullptr || Mesh->IsPendingKill())
    {
        return false;
    }

    // 5. Check Anim Instance
    // Disassembly: Calls USkeletalMeshComponent::GetAnimInstance
    UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
    if (AnimInstance == nullptr || AnimInstance->IsPendingKill())
    {
        return false;
    }

    // All checks passed
    return true;
}

// starts at line 3126
bool ADBDPlayer::CanCrouch()
{
    // Call the virtual function located at VTable offset 0x1028.
    // Based on previous contexts and the pseudo-code, this is the 'CanCrouchGameSpecific' check.
    // This handles logic like "Are we dashing?" or "Are we in an interaction that forbids crouching?".
    if (this->CanCrouchGameSpecific() == false)
    {
        return false;
    }

    // Call the parent class implementation.
    // The disassembly explicitly calls ACharacter::CanCrouch.
    // This handles standard engine checks (e.g., not swimming, not flying, RootComponent valid).
    if (Super::CanCrouch() == false)
    {
        return false;
    }

    // If both checks pass, the player is allowed to crouch.
    return true;
}

// starts at line 3131
bool ADBDPlayer::CanCrouchGameSpecific()
{
    if (this->IsInteracting != false)
    {
        UPlayerInteractionHandler* InteractionHandler = this->_interactionHandler;
        if (IsValid(InteractionHandler) == false)
        {
            return false;
        }

        UInteractionDefinition* CurrentInteraction = InteractionHandler->_currentInteraction;
        if (IsValid(CurrentInteraction) == false)
        {
            return false;
        }

        if (CurrentInteraction->CanCrouch == false)
        {
            return false;
        }
    }

    if (this->IsDashing() != false)
    {
        return false;
    }

    // If we are not dashing, and either not interacting OR interacting with a crouch-compatible interaction, return true.
    return true;
}

// starts at line 3136
void ADBDPlayer::DBDCrouch()
{
    // Check the 'bIsCrouched' bit-flag at offset 0x48C.
    // The disassembly tests the first bit. If it is already set (1), the player is already crouched, 
    // so we jump to the end (loc_140374E4F) and do nothing.
    if (!this->bIsCrouched)
    {
        // Check if the game-specific logic allows crouching.
        // Calls virtual function at offset 0x1028 (CanCrouchGameSpecific).
        if (this->CanCrouchGameSpecific())
        {
            // Check if the parent Character class logic allows crouching.
            // Calls ACharacter::CanCrouch.
            if (Super::CanCrouch())
            {
                // Execute the actual crouch logic.
                // Calls the virtual function at offset 0x798, which corresponds to ACharacter::Crouch(bool bClientSimulation).
                // The disassembly passes 0 (false) for the bClientSimulation argument.
                this->Crouch(false);
            }
        }
    }
}

// starts at line 3144
void ADBDPlayer::DBDUnCrouch()
{
    // Check the 'bIsCrouched' bit-flag at offset 0x48C (Bit 1).
    // If the bit is 0 (jz), the player is not currently crouched, so we do nothing and return.
    if (this->bIsCrouched)
    {
        // Execute the standard UnCrouch logic.
        // The disassembly jumps to the virtual function at offset 0x7A0.
        // In ACharacter, UnCrouch(bool bClientSimulation) usually follows Crouch(bool bClientSimulation) in the VTable.
        // The argument passed in EDX is 0 (false).
        this->UnCrouch(false);
    }
}

// starts at line 3152
bool ADBDPlayer::IsCrouching() const
{
    // Check the bIsCrouched flag in the Character movement state
    // Disassembly: Accesses the first bit (& 1) of the bitfield at offset 0x48C
    if (this->bIsCrouched != false)
    {
        return true;
    }

    return false;
}

// starts at line 3162
bool ADBDPlayer::IsUnintentionallyIncapacitated() const
{
    // Call the virtual function IsIncapacitated()
    return this->IsIncapacitated();
}

// starts at line 3167
void ADBDPlayer::SwitchToGamepadControlsFromAxis(float Val)
{
    // Only switch if the input value is positive (indicating active usage)
    // and if the player is not already using gamepad controls (Offset 0xD20).
    if (Val > 0.0f && !this->_isUsingGamepad)
    {
        // Update the state to indicate a Gamepad is being used.
        this->_isUsingGamepad = true;

        // Broadcast the delegate to notify other systems of the input change.
        // Offset 0xD30 corresponds to the OnSwitchedInputControlsEvent delegate.
        // The parameter 'true' indicates switching TO gamepad.
        this->_onSwitchedInputControlsEvent.Broadcast(true);
    }
}

// starts at line 3175
void ADBDPlayer::SwitchToGamepadControls()
{
    // Check if the input method is currently set to something other than Gamepad (Offset 0xD20).
    if (!this->_isUsingGamepad)
    {
        // Update the state to indicate a Gamepad is being used.
        this->_isUsingGamepad = true;

        // Broadcast the delegate to notify other systems (e.g., UI prompts) of the input change.
        // Offset 0xD30 corresponds to the OnSwitchedInputControlsEvent multicast delegate.
        // The parameter 'true' indicates switching TO gamepad.
        this->_onSwitchedInputControlsEvent.Broadcast(true);
    }
}

// starts at line 3184
void ADBDPlayer::SwitchToKeyboardControls()
{
    // Check if the input method is currently set to Gamepad (Offset 0xD20).
    // If it is true, we need to switch it off.
    if (this->_isUsingGamepad)
    {
        // Update the state to indicate Gamepad is NOT being used (i.e., Keyboard/Mouse).
        this->_isUsingGamepad = false;

        // Broadcast the delegate to notify other systems of the input change.
        // Offset 0xD30 corresponds to the OnSwitchedInputControlsEvent delegate.
        // The parameter 'false' indicates switching AWAY from gamepad (to keyboard).
        this->_onSwitchedInputControlsEvent.Broadcast(false);
    }
}

// starts at line 3193
void ADBDPlayer::SetGroundFriction(float groundFriction)
{
    // Access the CharacterMovement component (Offset 0x3E8)
    if (this->CharacterMovement != nullptr)
    {
        // Update the ground friction property on the movement component.
        this->CharacterMovement->GroundFriction = groundFriction;
    }
}

// starts at line 3201
void ADBDPlayer::ResetGroundFriction()
{
    // Check if the CharacterMovement component is valid
    if (this->CharacterMovement != nullptr)
    {
        // Restore the GroundFriction to the value stored in _originalGroundFriction
        this->CharacterMovement->GroundFriction = this->_originalGroundFriction;
    }
}

// starts at line 3206
bool ADBDPlayer::GetEnableServerPositionUpdates() const
{
    // Return the boolean flag. 
    // The disassembly ((&this->AActor + 0x7C) & 0x20) indicates this is a bitfield member.
    // It is likely the 6th bit in a pack of booleans at offset 0x7C (relative to AActor base).
    return this->bReplicateMovement;
}

// starts at line 3211
void ADBDPlayer::SetEnableServerPositionUpdates(bool Enable)
{
    // Access the CharacterMovement component (Offset 0x3E8)
    if (this->CharacterMovement != nullptr)
    {
        // Check if the replication state is actually changing.
        // The disassembly checks bit 0x20 at offset 0x7C (bReplicateMovement).
        if (this->IsReplicatingMovement() != Enable)
        {
            // Clear saved moves to prevent rubber-banding/correction spikes when toggling modes.
            this->CharacterMovement->ClearSavedMoves();
            
            // Reset the mesh's relative position to default to ensure alignment.
            this->ResetMeshRelativePosition();
        }

        // Logic splits based on whether this actor has authority (Server) or not (Client).
        if (this->Role == ROLE_Authority)
        {
            // Server-side: Update the replication flag.
            this->SetReplicateMovement(Enable);

            // If this player is not locally controlled (i.e., it represents a remote client connection)
            if (this->IsLocallyControlled() == false)
            {
                if (this->Mesh != nullptr && IsValid(this->Mesh))
                {
                    // Update a flag on the SkeletalMeshComponent.
                    // The disassembly sets bit 0 at offset 0x910. 
                    // This often relates to pausing anims or optimization when server updates are disabled.
                    this->Mesh->bAutonomousTickPose = Enable;
                }
            }
        }
        else
        {
            // Client-side: Manually toggle the replication flag.
            // Note: On clients, SetReplicateMovement usually doesn't replicate back to server, 
            // but here it controls whether the client *accepts* updates.
            this->SetReplicateMovement(Enable);

            // If not locally controlled (i.e., this is a Simulated Proxy for another player)
            if (this->IsLocallyControlled() == false)
            {
                // Switch the Role to change how movement is handled.
                // If Enabled (True): Set to ROLE_SimulatedProxy (1) - Standard replication.
                // If Disabled (False): Set to ROLE_AutonomousProxy (2) - Client predicts movement locally.
                this->Role = Enable ? ROLE_SimulatedProxy : ROLE_AutonomousProxy;

                // Update CharacterMovement flag to ignore errors.
                // If we disable server updates (Enable = false), we must ignore correction checks.
                // This corresponds to bit 0x20 at offset 0x224 (bIgnoreClientMovementErrorChecksAndCorrection).
                this->CharacterMovement->bIgnoreClientMovementErrorChecksAndCorrection = !Enable;
            }
        }
    }
}

// starts at line 3258
void ADBDPlayer::SetIgnoreActorCollision(AActor* ActorToIgnore, bool Ignore)
{
    // Ensure the actor to ignore is valid and not pending destruction.
    // The assembly performs standard IsValid() checks (Global Array bounds, PendingKill flag, etc.).
    if (ActorToIgnore != nullptr && IsValid(ActorToIgnore) && !ActorToIgnore->bActorIsBeingDestroyed)
    {
        if (Ignore)
        {
            // Add the actor to the collision ignore list.
            // Calls virtual function at offset 0x6D8.
            this->MoveIgnoreActorAdd(ActorToIgnore);

            UE_LOG(AuthoritativeMovement, Log, TEXT(">>> MoveIgnoreActorAdd %s."), *ActorToIgnore->GetName());
        }
        else
        {
            // Remove the actor from the collision ignore list.
            // Calls virtual function at offset 0x6E0.
            this->MoveIgnoreActorRemove(ActorToIgnore);

            UE_LOG(AuthoritativeMovement, Log, TEXT("<<< MoveIgnoreActorRemove %s."), *ActorToIgnore->GetName());
        }
    }
}

// starts at line 3278
bool ADBDPlayer::IsInteractionInputPressed(EInputInteractionType interactionType) const
{
    // Switch based on the requested interaction type
    // Disassembly: Uses a jump table based on the interactionType enum value
    switch (interactionType)
    {
        case EInputInteractionType::VE_Interact:      // 1
        case EInputInteractionType::VE_ItemInteract:  // 3
        case EInputInteractionType::VE_ItemDrop:      // 6
        {
            // Check the standard interaction input flag in PlayerData
            // Disassembly: Accesses _playerData (0xD08) -> _interactionInputPressed (0xE9)
            if (this->_playerData != nullptr)
            {
                return this->_playerData->_interactionInputPressed;
            }
            break;
        }

        case EInputInteractionType::VE_AttackInteract: // 2
        {
            // Check the attack input flag
            // Disassembly: Accesses _playerData (0xD08) -> _attackInputPressed (0xEE)
            if (this->_playerData != nullptr)
            {
                return this->_playerData->_attackInputPressed;
            }
            break;
        }

        case EInputInteractionType::VE_ItemUse: // 5
        {
            // Check the item use input flag
            // Disassembly: Accesses _playerData (0xD08) -> _useItemInputPressed (0xEA)
            if (this->_playerData != nullptr)
            {
                return this->_playerData->_useItemInputPressed;
            }
            break;
        }

        case EInputInteractionType::VE_InteractMash: // 7
        {
            // Check the interaction button masher
            // Disassembly: Accesses _interactInputMasher (0xD18) and calls IsMashing()
            if (this->_interactInputMasher != nullptr)
            {
                return this->_interactInputMasher->IsMashing();
            }
            break;
        }

        case EInputInteractionType::VE_LeftRightMash: // 8
        {
            // Check the wiggle/struggle axis masher
            // Disassembly: Accesses _wiggleAxisFlickMasher (0xD10) and calls IsMashing()
            if (this->_wiggleAxisFlickMasher != nullptr)
            {
                return this->_wiggleAxisFlickMasher->IsMashing();
            }
            break;
        }

        case EInputInteractionType::VE_FastInteract: // 18 (0x12)
        {
            // Check the fast interaction input flag (e.g., vaulting while running)
            // Disassembly: Accesses _playerData (0xD08) -> _fastInteractionInputPressed (0xEB)
            if (this->_playerData != nullptr)
            {
                return this->_playerData->_fastInteractionInputPressed;
            }
            break;
        }

        case EInputInteractionType::VE_Action: // 30 (0x1E)
        {
            // Check the generic action input flag
            // Disassembly: Accesses _playerData (0xD08) -> _actionInputPressed (0xF0)
            if (this->_playerData != nullptr)
            {
                return this->_playerData->_actionInputPressed;
            }
            break;
        }

        default:
        {
            // Return false for any unhandled interaction types
            return false;
        }
    }

    return false;
}

// starts at line 3304
void ADBDPlayer::TriggerAnimNotify(EAnimNotifyType animNotifyType)
{
    // The function handles specific animation notification types by broadcasting corresponding delegates.
    // Based on the assembly, it specifically looks for Enum values 1 and 2.

    // Case 1: Pickup (Mapped from Offset 0x8E0 usage in pseudocode)
    if (animNotifyType == EAnimNotifyType::VE_Pickup) 
    {
        // Check if the multicast delegate has any bound functions (InvocationList.Num() > 0).
        // Offset 0x8E8 represents the ArrayNum of the delegate's invocation list.
        if (this->OnAnimNotify_Pickup.IsBound())
        {
            // Broadcast the event, passing 'this' (the player) as the parameter.
            this->OnAnimNotify_Pickup.Broadcast(this);
        }
    }
    // Case 2: Release (Mapped from Offset 0x8F0 usage in pseudocode)
    else if (animNotifyType == EAnimNotifyType::VE_Release)
    {
        // Offset 0x8F8 represents the ArrayNum of this delegate.
        if (this->OnAnimNotify_Release.IsBound())
        {
            this->OnAnimNotify_Release.Broadcast(this);
        }
    }
}

// starts at line 3327
bool ADBDPlayer::HasItem() const
{
    // Check if the number of items in the Inventory array is greater than 0
    // Disassembly: Accesses ArrayNum of the Inventory TArray at offset 0xC58
    if (this->Inventory.Num() > 0)
    {
        return true;
    }

    return false;
}

// starts at line 3332
int32 ADBDPlayer::GetItemCount() const
{
    // Return the number of items currently in the inventory array
    // Corresponds to TArray::Num()
    return this->Inventory.Num();
}

// starts at line 3337
int32 ADBDPlayer::GetItemCountByID(FName itemID) const
{
    int32 Count = 0;

    // Iterate through the Inventory array
    // The loop logic in assembly iterates over the data pointer (Offset 0xC58) 
    // based on the ArrayNum (Offset 0xC60).
    for (ACollectable* Item : this->Inventory)
    {
        // 1. Check if the pointer is not null
        // 2. Check internal validity (GUObjectArray lookup)
        // 3. Check "Pending Kill" flag (Offset 0x140 bit 4)
        // In UE4 C++, IsValid() encapsulates these checks.
        if (IsValid(Item))
        {
            // Compare the item's ID (Offset 0x3B0) with the argument
            if (Item->ItemID == itemID)
            {
                Count++;
            }
        }
    }

    return Count;
}

// starts at line 3350
bool ADBDPlayer::CanCollectExtraItem(ACollectable* item) const
{
    // Check if the provided item matches the specific ID for a Bear Trap.
    if (item->ItemID == BEARTRAP_ID)  // "Item_Slasher_Beartrap"
    {
        // If the item is a Bear Trap, check if the player's inventory (hands) is already full of them.
        // We invoke the internal check ArePlayersHandsFullOfBearTraps.
        if (this->ArePlayersHandsFullOfBearTraps() == false)
        {
            // If the hands are not full, we are allowed to collect this extra item.
            return true;
        }
    }

    // If the item is not a Bear Trap, or if the player's hands are already full of traps, return false.
    return false;
}

// starts at line 3362
bool ADBDPlayer::HasEquippedItem() const
{
    // Check if the Inventory array has at least one element
    // Disassembly: Checks if ArrayNum > 0
    if (this->Inventory.Num() > 0)
    {
        // Retrieve the first collectable item from the inventory
        // Disassembly: Dereferences the data pointer to get the first element
        ACollectable* FirstItem = this->Inventory[0];

        // Check if the item instance exists
        if (FirstItem != nullptr)
        {
            // Check if the collectable is marked as equipped
            if (FirstItem->IsEquipped() == true)
            {
                return true;
            }
        }
    }

    return false;
}

// starts at line 3368
void ADBDPlayer::EquipItem()
{
    // Check if the player has network authority (Role == 3 corresponds to ROLE_Authority)
    if (this->Role == ROLE_Authority)
    {
        // Check if the inventory contains any items
        // The disassembly checks offset 0xC60 (ArrayNum) against 0
        if (this->Inventory.Num() > 0)
        {
            // Access the first item in the inventory (Index 0)
            // The disassembly loads the array data pointer from 0xC58 and dereferences the first element
            ACollectable* FirstItem = this->Inventory[0];

            // Validate the item pointer
            if (FirstItem != nullptr)
            {
                // Call the Equip function on the Collectable item
                FirstItem->Equip();
            }
        }
    }
}

// starts at line 3376
void ADBDPlayer::StoreItem()
{
    // Ensure that only the server (Authority) executes this logic.
    // Offset 0x110 is the Role property. 3 corresponds to ROLE_Authority.
    if (this->HasAuthority())
    {
        // Check if the Inventory array has any elements.
        // Offset 0xC60 corresponds to TArray::ArrayNum.
        if (this->Inventory.Num() > 0)
        {
            // Retrieve the first item from the inventory.
            // Offset 0xC58 is TArray::AllocatorInstance.Data.
            // The assembly dereferences the data pointer to get the first element (Index 0).
            ACollectable* ItemToStore = this->Inventory[0];

            // Verify the item pointer is valid before calling methods on it.
            if (ItemToStore != nullptr)
            {
                // Call the Store function on the collectable item.
                ItemToStore->Store();
            }
        }
    }
}

// starts at line 3384
void ADBDPlayer::SetAllowNavigation(bool Enable)
{
    // Access the standard CharacterMovement component (Offset 0x3E8)
    if (this->CharacterMovement != nullptr)
    {
        // Cast the movement component to the specific UDBDCharacterMovementComponent class.
        // The assembly manually checks the class hierarchy (ClassTreeIndex comparison).
        UDBDCharacterMovementComponent* DBDMovement = Cast<UDBDCharacterMovementComponent>(this->CharacterMovement);

        // Ensure the cast was successful before accessing the member
        if (DBDMovement != nullptr)
        {
            // Set the navigation allowance flag.
            // Writes the boolean value to offset 0x758 in UDBDCharacterMovementComponent.
            DBDMovement->bAllowNavigation = Enable;
        }
    }
}

// starts at line 3392
bool ADBDPlayer::IsAllowedNavigation() const
{
    // Retrieve the standard CharacterMovement component
    // Disassembly: Accesses offset 0x3E8 (inherited from ACharacter)
    UCharacterMovementComponent* MovementComponent = this->CharacterMovement;

    // Check if the component exists
    if (MovementComponent != nullptr)
    {
        // Cast to the custom UDBDCharacterMovementComponent class
        // Disassembly: Uses GetPrivateStaticClass and checks ClassTreeIndex logic
        UDBDCharacterMovementComponent* DBDMovement = Cast<UDBDCharacterMovementComponent>(MovementComponent);

        // If the cast is successful
        if (DBDMovement != nullptr)
        {
            // Return the boolean flag located at offset 0x758
            return DBDMovement->_allowNavigation;
        }
    }

    return false;
}

// starts at line 3401
float ADBDPlayer::GetMaxSpeed() const
{
    // 1. Retrieve the CharacterMovement component
    // In ACharacter, this is stored in the CharacterMovement member variable.
    UCharacterMovementComponent* MovementComponent = this->GetCharacterMovement();

    // 2. Validate component existence
    if (!MovementComponent)
    {
        return 0.0f;
    }

    // 3. Perform a safe type cast to UDBDCharacterMovementComponent
    // The assembly manually checks the ClassTreeIndex to verify the class inheritance.
    // This is equivalent to Cast<UDBDCharacterMovementComponent>(...)
    UDBDCharacterMovementComponent* DBDMovement = Cast<UDBDCharacterMovementComponent>(MovementComponent);

    // 4. If the cast succeeds, return the MaxSpeed
    if (DBDMovement)
    {
        return DBDMovement->GetMaxSpeed();
    }

    // Return 0.0f if the component is missing or incorrect type (based on the snippet's fall-through logic)
    return 0.0f;
}

// starts at line 3410
bool ADBDPlayer::HasMoveInput() const
{
    // Retrieve the Controller
    // Disassembly: Accesses offset 0x3B8 (inherited from APawn)
    AController* Controller = this->Controller;

    // Check if Controller exists
    if (Controller != nullptr)
    {
        // Verify Controller is valid (not pending kill) and not being destroyed
        // Disassembly: Checks GUObjectArray flags and bActorIsBeingDestroyed
        if (Controller->IsPendingKill() == false && Controller->bActorIsBeingDestroyed == false)
        {
            // Cast to ADBDPlayerController
            // Disassembly: Uses GetPrivateStaticClass and checks ClassTreeIndex
            ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(Controller);

            // If the cast is successful
            if (DBDController != nullptr)
            {
                // Access the boolean at offset 0x960 in ADBDPlayerController
                return DBDController->_hasMoveInputThisFrame; 
            }
        }
    }

    return false;
}

// starts at line 3419
FString ADBDPlayer::GetCharacterAudioSurfaceName() const
{
    // The function implementation delegates directly to getting the left foot's audio surface.
    // This implies the character's general audio surface is defined by where their left foot is standing.
    return this->GetLeftFootAudioSurfaceName();
}

// starts at line 3424
void ADBDPlayer::SetCharacterAudioSurfaceName(FString Name)
{
    // Update the character's audio surface name.
    // The disassembly shows a deep copy of the string data (TArray operations) to the member at offset 0xFF0.
    // The explicit memory management (ResizeForCopy, memcpy, Free) corresponds to the FString assignment logic.
    this->CharacterAudioSurfaceName = Name;
}

// starts at line 3429
FString ADBDPlayer::GetLeftFootAudioSurfaceName() const
{
    // Return the cached surface name string.
    // The assembly code performs a deep copy of the FString's internal TArray<TCHAR> data.
    // Offsets 0xFF0 (Data) and 0xFF8 (ArrayNum) correspond to the member variable _leftFootSurface.
    return this->_leftFootSurface;
}

// starts at line 3434
void ADBDPlayer::SetLeftFootAudioSurfaceName(FString name)
{
    // Assign the provided string to the left foot surface member (Offset 0xFF0).
    // The disassembly calls the assignment operator (TArray::operator=) to copy the data.
    // The memory management code at the end represents the destructor of the 'name' parameter (passed by value) cleaning up its allocation.
    this->_leftFootSurface = name;
}

// starts at line 3439
FString ADBDPlayer::GetRightFootAudioSurfaceName() const
{
    // Return the cached surface name string.
    // Based on the assembly access at [rcx+1000h] (Data) and [rcx+1008h] (Count),
    // this maps to an FString member variable.
    return this->_rightFootSurface;
}

// starts at line 3444
void ADBDPlayer::SetRightFootAudioSurfaceName(FString name)
{
    // Assign the provided string to the right foot surface member (Offset 0x1000).
    // The disassembly calls the assignment operator (TArray::operator=) to copy the data.
    // The memory management code at the end represents the destructor of the 'name' parameter (passed by value) cleaning up its allocation.
    this->_rightFootSurface = name;
}

// starts at line 3476
void ADBDPlayer::OnPickupStart(ADBDPlayer* Target)
{
    // Check if the provided target pointer is not null
    if (Target != nullptr)
    {
        // Perform validity checks:
        // 1. IsValid(Target) handles the check against the Global Object Array (GUObjectArray) for PendingKill flags found in disassembly.
        // 2. Explicitly check the bActorIsBeingDestroyed flag (offset 0x140, bit 4).
        if (IsValid(Target) && Target->bActorIsBeingDestroyed == false)
        {
            // Call the virtual function identified as IsValidImpl in the pseudo-code (offset 0x1070).
            // This performs additional specific validity logic for the interaction.
            if (Target->IsValidImpl() != false)
            {
                // Call the virtual function identified as OnPickedupStart in the pseudo-code (offset 0x0E78).
                // Notifies the target that the pickup interaction has started, passing the current player (this) as the instigator.
                Target->OnPickedupStart(this);
            }
        }
    }
}

// starts at line 3485
void ADBDPlayer::OnPickupEnd(ADBDPlayer* Target)
{
    // Check if the provided target pointer is not null
    if (Target != nullptr)
    {
        // The disassembly checks the global object array for validity (PendingKill flags) 
        // and checks the bActorIsBeingDestroyed flag (0x140 bit 4).
        // IsValid(Target) in UE4 covers the low-level object array checks.
        if (IsValid(Target))
        {
            // Explicitly check if the actor is in the process of being destroyed
            if (Target->bActorIsBeingDestroyed == false)
            {
                // Call a virtual function at offset 0x1070 on the Target.
                // The disassembly checks the boolean result of this function before proceeding.
                // This is likely a specific validity or state check implementation (e.g., IsValidImpl).
                if (Target->IsValidImpl() != false)
                {
                    // Call a virtual function at offset 0x0E80 on the Target.
                    // Passes the current instance (this) as the argument.
                    // This likely corresponds to 'OnPickedupEnd' mentioned in the Pseudo Code.
                    Target->OnPickedupEnd(this);
                }
            }
        }
    }
}

// starts at line 3503
void ADBDPlayer::Server_FailInterruption_Implementation(ADBDPlayer *interruptee)
{
    this->AuthorityFailInterruption(interruptee);
}

// starts at line 3552
bool ADBDPlayer::IsInMeathookZone() const
{
    // Retrieve the MeatHook Zone Detector component
    // Disassembly: Accesses offset 0xFA0
    UZoneDetectorComponent* ZoneDetector = this->_meatHookZoneDetector;

    // Check if the component exists
    if (ZoneDetector != nullptr)
    {
        // Verify the component is valid (not pending kill)
        // Disassembly: Checks GUObjectArray flags (0x20000000)
        if (ZoneDetector->IsPendingKill() == false)
        {
            // Check if the number of detected zones is greater than 0.
            // Disassembly: cmp dword ptr [rdx+0F0h], 0 (Accesses TArray::Num at offset 0xF0)
            if (ZoneDetector->_zones.Num() > 0)
            {
                return true;
            }
        }
    }

    return false;
}

// starts at line 3562
void ADBDPlayer::Client_SendCancelInteraction_Implementation()
{
    // The disassembly checks the 'Interaction' log category verbosity level (5 corresponds to Log).
    // This is the compiled representation of a standard UE_LOG macro.
    UE_LOG(LogInteraction, Log, TEXT("Client_SendCancelInteraction_Implementation"));

    // Retrieve the raw pointer to the PollableEvent from the TSharedPtr wrapper.
    PollableEvent<bool>* cancelledEvent = this->InteractionCancelledEvent.Get();

    // The original assembly boldly dereferences the pointer without checking for null.
    // However, it is standard safe C++ practice to verify the pointer is valid first.
    if (cancelledEvent != nullptr)
    {
        // The disassembly manually increments the ArrayNum, checks it against ArrayMax, 
        // calls ResizeGrow, and writes a 1 (true) to the new memory address.
        // This maps perfectly to adding a boolean 'true' to an internal TArray.
        cancelledEvent->Send(true);
    }
}

// starts at line 3573
AInteractable* ADBDPlayer::GetInteractable() const
{
    // Return the interactable actor currently associated with the player
    // Maps to member variable _interactable
    return this->_interactable;
}

// starts at line 3581
FVector ADBDPlayer::GetBoneRelativeLocation(FName targetBone, FName relativeBone) const
{
    // Retrieve the SkeletalMeshComponent (Offset 0x3E0)
    USkeletalMeshComponent* MeshComponent = this->GetMesh();

    // Check if the mesh pointer is not null
    if (MeshComponent != nullptr)
    {
        // Validate the mesh object (GUObjectArray check)
        if (MeshComponent->IsValidLowLevel() == true)
        {
            // Get the world location of the target bone
            // Space 0 corresponds to EBoneSpaces::WorldSpace
            FVector TargetLocation = MeshComponent->GetBoneLocation(targetBone, EBoneSpaces::WorldSpace);

            // Get the world location of the relative (parent/reference) bone
            FVector RelativeBaseLocation = MeshComponent->GetBoneLocation(relativeBone, EBoneSpaces::WorldSpace);

            // Calculate the relative vector by subtracting the base location from the target location
            return TargetLocation - RelativeBaseLocation;
        }
    }

    // Return ZeroVector if mesh is invalid
    return FVector::ZeroVector;
}

// starts at line 3595
FVector ADBDPlayer::GetBoneLocation(FName name, EBoneSpaces::Type space) const
{
    // Retrieve the SkeletalMeshComponent (Inherited from ACharacter, offset 0x3E0)
    USkeletalMeshComponent* MeshComponent = this->GetMesh();

    // Check if the mesh pointer is not null
    if (MeshComponent != nullptr)
    {
        // Validate the mesh object (GUObjectArray check in disassembly)
        // Checks internal flags to ensure the object is not unreachable or pending kill
        if (MeshComponent->IsValidLowLevel() == true)
        {
            // Call the USkinnedMeshComponent function to get the socket/bone location
            return MeshComponent->GetBoneLocation(name, space);
        }
    }

    // Return FVector::ZeroVector if the mesh is invalid or missing
    return FVector::ZeroVector;
}

// starts at line 3605
void ADBDPlayer::SetWorldCollisionResponse(ECollisionResponse response)
{
    this->CapsuleComponentSetCollisionResponseToChannel(ECC_WorldStatic, response);
    this->CapsuleComponentSetCollisionResponseToChannel(ECC_WorldDynamic, response);
}

// starts at line 3611
void ADBDPlayer::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
    // Call the parent class implementation of DisplayDebug
    ACharacter::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

    // Retrieve the World pointer
    UWorld* World = this->GetWorld();

    // Check if the World is valid
    if (World != nullptr)
    {
        // Iterate through all Actors in the world using TActorIterator
        // The disassembly initializes the iterator with AActor::StaticClass() (/Script/Engine)
        for (TActorIterator<AActor> It(World); It; ++It)
        {
            AActor* CurrentActor = *It;

            // Attempt to cast the actor to ADBDPlayer
            // The disassembly manually checks the class hierarchy against ADBDPlayer::StaticClass()
            ADBDPlayer* OtherPlayer = Cast<ADBDPlayer>(CurrentActor);

            // Check if the cast was successful and ensure we are not processing 'this' player
            if (OtherPlayer != nullptr && OtherPlayer != this)
            {
                // Define a TArray with an inline allocator of 24 to hold the components
                // This matches TArray<USkeletalMeshComponent*, TInlineAllocator<24>>
                TArray<USkeletalMeshComponent*, TInlineAllocator<24>> Components;

                // Get all SkeletalMeshComponents from the other player
                OtherPlayer->GetComponents(Components, false);

                // Iterate through the retrieved components
                for (int32 Index = 0; Index < Components.Num(); Index++)
                {
                    USkeletalMeshComponent* MeshComponent = Components[Index];

                    if (MeshComponent != nullptr)
                    {
                        // Draw debug bones for the mesh
                        // The boolean argument '1' (true) corresponds to bSimpleBones
                        MeshComponent->DebugDrawBones(Canvas, true);
                    }
                }
            }
        }
    }
}

// starts at line 3639
bool ADBDPlayer::HasActiveSkillCheck() const
{
    // Retrieve the Interaction Handler from the player
    UPlayerInteractionHandler* InteractionHandler = this->_interactionHandler;

    // Check if InteractionHandler instance exists
    if (InteractionHandler != nullptr)
    {
        // Verify the object is valid and not marked for garbage collection (Pending Kill)
        // Disassembly: Checks flags in GUObjectArray (0x20000000 mask)
        if (InteractionHandler->IsPendingKill() == false)
        {
            // Access the SkillCheck component from the InteractionHandler
            // Offset 0x240 (derived from Pseudo Code and Context)
            USkillCheck* SkillCheck = InteractionHandler->_skillCheck;

            // Check if SkillCheck instance exists
            if (SkillCheck != nullptr)
            {
                // Verify the object is valid and not marked for garbage collection
                if (SkillCheck->IsPendingKill() == false)
                {
                    // Check the boolean flag '_activated' at offset 0x2C4
                    if (SkillCheck->_activated != false)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// starts at line 3644
void ADBDPlayer::DeactivateSkillChecks()
{
    // Retrieve the pointer to the InteractionHandler (Offset 0x0CF0)
    UPlayerInteractionHandler* InteractionHandler = this->_interactionHandler;

    // Check if the InteractionHandler instance is not null
    if (InteractionHandler != nullptr)
    {
        // Check if the InteractionHandler is valid (not marked as PendingKill or Unreachable in GUObjectArray)
        // This corresponds to the bitwise checks on GUObjectArray in the disassembly
        if (InteractionHandler->IsValidLowLevel() == true)
        {
            // Access the SkillCheck instance from the InteractionHandler (Offset 0x240)
            // Assumed member name based on Pseudo Code
            USkillCheck* SkillCheck = InteractionHandler->_skillCheck;

            // Check if the SkillCheck instance is not null
            if (SkillCheck != nullptr)
            {
                // Validate the SkillCheck object against the global object array
                if (SkillCheck->IsValidLowLevel() == true)
                {
                    // Check the boolean flag '_activated' (Offset 0x2C4) to see if a skill check is active
                    if (SkillCheck->_activated == true)
                    {
                        // Call the function to deactivate the skill check
                        // The float argument 0.25f corresponds to 0x3e800000 in the disassembly
                        SkillCheck->DeactivateSkillCheck(0.25f);
                    }
                }
            }
        }
    }
}

// starts at line 3664
void ADBDPlayer::AddDetectionZone(EDetectionZone detectionZoneID, UPrimitiveComponent* zone)
{
    // Add the provided detection zone component and its corresponding ID to the detection zone map
    // We use Emplace here to match the underlying TSet::Emplace operation seen in the disassembly
    this->_detectionZoneMap.Emplace(detectionZoneID, zone);
}

// starts at line 3669
UPrimitiveComponent* ADBDPlayer::GetDetectionZone(EDetectionZone detectionZoneID) const
{
    // Look up the requested detection zone in the internal map
    // The map is likely defined as TMap<EDetectionZone, UPrimitiveComponent*> _detectionZoneMap;
    // The disassembly contains the inlined logic of TMap::FindRef:
    // 1. Check if map is empty
    // 2. Hash the key (detectionZoneID)
    // 3. Traverse the linked list in the bucket
    // 4. Return the value if found, or nullptr
    
    return this->_detectionZoneMap.FindRef(detectionZoneID);
}

// starts at line 3679
void ADBDPlayer::SetAllDetectionZonesEnabled(bool Enabled)
{
    // Iterate through all detection zone indices.
    // The loop runs from 0 to 12 (0xD), covering all defined detection zones in the EDetectionZone enum.
    for (int32 i = 0; i < 13; i++)
    {
        // Retrieve the primitive component corresponding to the current detection zone ID.
        UPrimitiveComponent* DetectionZone = this->GetDetectionZone((EDetectionZone)i);

        // Verify the component exists and is valid (not pending kill).
        if (DetectionZone != nullptr && IsValid(DetectionZone))
        {
            // The disassembly explicitly checks a bitfield (Offset 0x2A8, bit 1) and only updates if necessary.
            // This bit corresponds to 'bGenerateOverlapEvents'.
            if (DetectionZone->bGenerateOverlapEvents != Enabled)
            {
                // Set the flag and update the overlaps.
                // The disassembly performs the bitwise toggle and calls UpdateOverlaps(nullptr, true, nullptr).
                // This is the implementation of SetGenerateOverlapEvents.
                DetectionZone->SetGenerateOverlapEvents(Enabled);
            }
        }
    }
}

// starts at line 3695
void ADBDPlayer::SetDetectionZoneEnabled(EDetectionZone DetectionZoneID, bool Enabled)
{
    // Retrieve the primitive component associated with the specific detection zone ID.
    UPrimitiveComponent* Zone = this->GetDetectionZone(DetectionZoneID);

    // Verify the component exists and is not pending kill (IsValid check).
    if (Zone != nullptr && IsValid(Zone))
    {
        // Check if the 'Generate Overlap Events' flag matches the desired state.
        // The disassembly checks bit 1 at offset 0x2A8, which corresponds to bGenerateOverlapEvents in UPrimitiveComponent.
        if (Zone->bGenerateOverlapEvents != Enabled)
        {
            // Set the flag and trigger an overlap update.
            // The disassembly manually toggles the bit and calls the virtual function UpdateOverlaps (offset 0x3E0).
            // This logic is encapsulated in the standard engine function SetGenerateOverlapEvents.
            Zone->SetGenerateOverlapEvents(Enabled);
        }
    }
}

// starts at line 3726
bool ADBDPlayer::HasLineOfSightToTarget(ADBDPlayer* target) const
{
    // 1. Validity Checks
    // Ensure the target is not null
    if (target == nullptr)
    {
        return false;
    }

    // Ensure the target object is not pending garbage collection
    // Disassembly: Checks GUObjectArray flags (0x20000000)
    if (target->IsPendingKill() == true)
    {
        return false;
    }

    // Disassembly: Checks a specific flag on the target (offset 0x140 bit 4)
    // This is likely 'bActorIsBeingDestroyed' or similar internal actor state
    if (target->IsActorBeingDestroyed() == true)
    {
        return false;
    }

    // 2. Setup Trace Locations
    // Get the start location from the camera bone of the current player
    // Disassembly: Uses FName "joint_Cam_01" and EBoneSpaces::WorldSpace (0)
    FVector StartLocation = this->Mesh->GetBoneLocation(FName("joint_Cam_01"), EBoneSpaces::WorldSpace);

    // Get the end location from the torso bone of the target player
    // Disassembly: Uses FName "joint_TorsoC_01" and EBoneSpaces::WorldSpace (0)
    FVector EndLocation = target->Mesh->GetBoneLocation(FName("joint_TorsoC_01"), EBoneSpaces::WorldSpace);

    // 3. Setup Collision Parameters
    // Disassembly: Sets up FCollisionObjectQueryParams with mask 0x10000003
    // 0x3 = ECC_WorldStatic | ECC_WorldDynamic
    // 0x10000000 = Bit 28 (Likely a custom GameTraceChannel)
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.ObjectTypesToQuery = 0x10000003; 

    FCollisionQueryParams QueryParams;
    QueryParams.bFindInitialOverlaps = true;
    QueryParams.IgnoreMask = 0x100; // Custom ignore mask used by the game

    // 4. Perform Line Trace
    // Retrieve the World pointer
    UWorld* World = this->GetWorld();
    
    // Array to hold hit results
    TArray<FHitResult> OutHits;

    // Perform a multi-line trace against specific object types
    bool bHit = World->LineTraceMultiByObjectType(OutHits, StartLocation, EndLocation, ObjectQueryParams, QueryParams);

    // Default result is true (has line of sight) unless a blocker is found
    bool bHasLineOfSight = true;

    // 5. Analyze Hits
    if (bHit == true)
    {
        // Iterate through all hit results
        for (const FHitResult& Hit : OutHits)
        {
            // Retrieve the component that was hit
            // Disassembly uses FWeakObjectPtr::Get() logic on the Hit Result's Component
            UPrimitiveComponent* HitComponent = Hit.Component.Get();

            // Check if this component acts as an attack blocker
            // Disassembly calls internal function: ADBDPlayer::FilterAttackBlockers
            if (this->FilterAttackBlockers(target, HitComponent) == true)
            {
                // If a valid blocker is found, line of sight is obstructed
                bHasLineOfSight = false;
                
                // Note: The disassembly logic iterates through all hits even after finding a blocker 
                // (it sets the flag to false but continues the loop).
            }
        }
    }

    return bHasLineOfSight;
}

// starts at line 3760
ADBDPlayer* ADBDPlayer::FindClosestSlashablePlayerInDetectionZone(EDetectionZone detectionZoneID)
{
    // Retrieve the primitive component corresponding to the requested Detection Zone ID
    // The disassembly calls the internal helper GetDetectionZone
    UPrimitiveComponent* DetectionComponent = this->GetDetectionZone(detectionZoneID);

    // Find the closest slashable player within the shape of the retrieved component
    // The disassembly uses a tail call (JMP) to forward the result directly
    return this->FindClosestSlashablePlayerInShape(DetectionComponent);
}

// starts at line 3766
ADBDPlayer* ADBDPlayer::FindClosestSlashablePlayerInShape(UPrimitiveComponent* shape) const
{
    ADBDPlayer* ClosestPlayer = nullptr;

    // Check if the shape component is valid
    if (shape != nullptr)
    {
        // Check if the shape component is a valid object (not pending kill)
        if (shape->IsValidLowLevel() == true)
        {
            float MinDistanceSquared = FLT_MAX; // Initialize with FLT_MAX

            // Prepare arrays to hold overlapping actors and components
            TArray<AActor*> OverlappingActors;
            TArray<UPrimitiveComponent*> OverlappingComponents;

            // Get overlapping actors of type ADBDPlayer
            // Note: The disassembly shows getting actors first, then components.
            // However, the core logic iterates through COMPONENTS.
            // The actors array might be for a different strategy not fully used or optimized out in C++.
            // We focus on the component iteration loop which drives the logic.
            // shape->GetOverlappingActors(OverlappingActors, ADBDPlayer::StaticClass());

            // Get overlapping components
            shape->GetOverlappingComponents(OverlappingComponents);

            // Iterate through all overlapping components
            for (int32 i = 0; i < OverlappingComponents.Num(); i++)
            {
                UPrimitiveComponent* OverlappedComponent = OverlappingComponents[i];

                // Check if the component has the "Slashable" tag
                // This tag likely marks hitboxes that can receive damage
                if (OverlappedComponent->ComponentHasTag(UDBDTags::SLASHABLE) == true)
                {
                    // Get the owner of the component
                    AActor* OwnerActor = OverlappedComponent->GetOwner();

                    // Check if the owner is an ADBDPlayer
                    // Manual cast check logic in ASM
                    ADBDPlayer* TargetPlayer = Cast<ADBDPlayer>(OwnerActor);

                    if (TargetPlayer != nullptr)
                    {
                        // Validate target player state
                        // 1. Must be valid low level
                        // 2. Must not be being destroyed
                        // 3. Must be valid impl
                        // 4. Must not be 'this' (self)
                        // 5. Must be slashable (CanSlash check)
                        // 6. Must have line of sight
                        if (TargetPlayer->IsValidLowLevel() == true &&
                            TargetPlayer->bActorIsBeingDestroyed == false &&
                            TargetPlayer->IsValidImpl() &&
                            TargetPlayer != this &&
                            this->CanSlash(TargetPlayer) == true &&
                            this->HasLineOfSightToTarget(TargetPlayer) == true)
                        {
                            // Calculate squared distance to the target
                            // We use squared distance for performance (avoids sqrt)
                            float DistanceSquared = 0.0f;

                            FVector MyLocation = FVector::ZeroVector;
                            if (this->GetRootComponent() != nullptr)
                            {
                                MyLocation = this->GetRootComponent()->GetComponentLocation();
                            }

                            FVector TargetLocation = FVector::ZeroVector;
                            if (TargetPlayer->GetRootComponent() != nullptr)
                            {
                                TargetLocation = TargetPlayer->GetRootComponent()->GetComponentLocation();
                            }

                            DistanceSquared = FVector::DistSquared(MyLocation, TargetLocation);

                            // Check if this player is closer than the current closest
                            // OR if no closest player has been found yet
                            bool bIsCloser = false;

                            // Special check: Is the current closest player hooked?
                            // If the *current best* candidate (ClosestPlayer) is hooked, we might prefer an unhooked one?
                            // Logic in ASM:
                            // if (ClosestPlayer == nullptr) -> simple case
                            // else -> check if ClosestPlayer is Hooked.
                            // The logic seems to prioritize ANY valid player if the current best is invalid/hooked,
                            // or distance if both are valid.
                            bool bCurrentClosestIsHooked = false;
                            if (ClosestPlayer != nullptr)
                            {
                                // Check if the *currently selected* closest player is hooked
                                // We cast to ACamperPlayer to check IsHooked status
                                ACamperPlayer* CamperClosest = Cast<ACamperPlayer>(ClosestPlayer);
                                if (CamperClosest != nullptr && CamperClosest->IsHooked() == true)
                                {
                                    bCurrentClosestIsHooked = true;
                                }
                            }

                            // If we don't have a candidate yet, OR the distance is smaller
                            // (The 'bCurrentClosestIsHooked' logic in ASM is a bit complex,
                            // it seems to force update if the previous best was bad, effectively reseting comparison)
                            // Simplified standard distance check:
                            if (ClosestPlayer == nullptr || bCurrentClosestIsHooked || DistanceSquared < MinDistanceSquared)
                            {
                                ClosestPlayer = TargetPlayer;
                                MinDistanceSquared = DistanceSquared;
                            }
                        }
                    }
                }
            }
        }
    }

    return ClosestPlayer;
}

// starts at line 3816
bool ADBDPlayer::FilterAttackBlockers(const ADBDPlayer* target, UPrimitiveComponent* blocker) const
{
    // Check if the blocker component is not null
    if (blocker != nullptr)
    {
        // Validate the blocker object (matches GUObjectArray checks in disassembly)
        if (blocker->IsValidLowLevel() == true)
        {
            // Check the collision response to the Pawn channel (ECC_Pawn = 2)
            // The function at VTable offset 0x440 is GetCollisionResponseToChannel
            // We check if it returns ECR_Block (2)
            if (blocker->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block)
            {
                // Call the internal validation function IsBlockerValid
                // This is a direct member call (jmp in disassembly), not a virtual call
                return this->IsBlockerValid(target, blocker);
            }
        }
    }

    return false;
}

// starts at line 3830
bool ADBDPlayer::IsBlockerValid(const ADBDPlayer* target, UPrimitiveComponent* blocker) const
{
    // Default result is true (valid blocker)
    bool bIsValid = true;

    // 1. Validate Target
    if (target == nullptr)
    {
        return true; 
    }

    // Check if target is pending kill or being destroyed
    // Disassembly: Checks GUObjectArray flags (0x20000000) and offset 0x140 bit 4
    if (target->IsPendingKill() == false && target->IsActorBeingDestroyed() == false)
    {
        // Check custom validity implementation
        if (target->IsValidImpl() == true)
        {
            // 2. Retrieve Interaction Handler
            UPlayerInteractionHandler* InteractionHandler = target->_interactionHandler;

            if (InteractionHandler != nullptr && InteractionHandler->IsPendingKill() == false)
            {
                // 3. Get Current Interaction
                UInteractionDefinition* CurrentInteraction = InteractionHandler->_currentInteraction;

                if (CurrentInteraction != nullptr && CurrentInteraction->IsPendingKill() == false)
                {
                    // Check if the interaction specifically blocks attacks
                    if (CurrentInteraction->OwnerBlocksAttack == false)
                    {
                        // 4. Validate Blocker Owner
                        // Retrieve the actor that owns the blocker component
                        AActor* BlockerOwner = blocker->GetOwner();

                        if (BlockerOwner != nullptr && 
                            BlockerOwner->IsPendingKill() == false && 
                            BlockerOwner->IsActorBeingDestroyed() == false)
                        {
                            // 5. Check Owner's Components for Interaction Match
                            // Iterate through components owned by the blocker's owner
                            // Disassembly uses TConstSetBitIterator to traverse OwnedComponents
                            for (UActorComponent* Component : BlockerOwner->OwnedComponents)
                            {
                                // Check if the component is the current interaction
                                if (Component == CurrentInteraction)
                                {
                                    // If found, this blocker is associated with the current interaction
                                    // Set result to false (invalid blocker in this context?)
                                    bIsValid = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return bIsValid;
}

// starts at line 3858
bool ADBDPlayer::CanInteract()
{
    // If the player is already in an interacting state, they cannot initiate a new interaction.
    if (this->IsInteracting)
    {
        return false;
    }

    // Retrieve the reference to the other player currently involved in the interaction.
    ADBDPlayer* InteractingPlayer = this->_interactingPlayer;

    // Check if the InteractingPlayer is valid (not nullptr and not marked for garbage collection/PendingKill).
    if (IsValid(InteractingPlayer))
    {
        // This function appears to determine if the other player is in a state that validates the interaction 
        // If this check returns true, it implies the interaction link is still active/valid, returning false for CanInteract.
        if (InteractingPlayer->IsValidImpl())
        {
            return false;
        }
    }

    // If we are not interacting, or the interacting player is invalid/inactive, we are free to interact.
    return true;
}

// starts at line 3864
bool ADBDPlayer::HasGameEnded() const
{
    // Retrieve the World pointer
    UWorld* World = this->GetWorld();

    // Check if World is valid
    if (World == nullptr)
    {
        return false;
    }

    // Check if the World is pending kill (garbage collection)
    // Disassembly: Checks flags in GUObjectArray (0x20000000)
    if (World->IsPendingKill() != false)
    {
        return false;
    }

    // Retrieve the GameState from the World
    // Note: The assembly retrieves the world again here, but we use the cached pointer
    AGameState* GameState = World->GameState;

    // Check if GameState is valid
    if (GameState == nullptr)
    {
        return false;
    }

    // Cast to the specific custom GameState class (ADBDGameState)
    // Disassembly: Performs ClassTreeIndex comparison against ADBDGameState::StaticClass()
    ADBDGameState* DBDGameState = Cast<ADBDGameState>(GameState);

    if (DBDGameState == nullptr)
    {
        return false;
    }

    // Check if the GameState is pending kill
    if (DBDGameState->IsPendingKill() != false)
    {
        return false;
    }

    // Check if the GameState actor is being destroyed
    // Disassembly: test byte ptr [rdi+140h], 4 (bActorIsBeingDestroyed is the 3rd bit)
    if (DBDGameState->bActorIsBeingDestroyed != false)
    {
        return false;
    }

    // Return the boolean flag at offset 0x611 indicating if the game has ended
    // Disassembly: movzx eax, byte ptr [rbx+611h]
    return DBDGameState->IsGameEnded;
}

// starts at line 3873
bool ADBDPlayer::CanAttack()
{
    // Execute the Blueprint implementation of the check first.
    if (this->CanAttack_BP() == false)
    {
        return false;
    }

    // This function performs the native internal checks for the attack capability.
    if (this->CanAttack_Internal() == false)
    {
        return false;
    }
    return true;
}

// starts at line 3917
void ADBDPlayer::Authority_OnSlashed(ADBDPlayer* attacker)
{
    // Check if the executing player has network authority.
    // The value 3 maps to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ENetRole::ROLE_Authority)
    {
        // Check if the player is currently engaged in an interaction
        if (this->IsInteracting == true)
        {
            // Ensure the interaction handler is valid before dereferencing
            if (this->_interactionHandler != nullptr)
            {
                // Retrieve the current interaction. 
                // UPlayerInteractionHandler and UInteractionDefinition schemas are not in the provided structures.
                UInteractionDefinition* currentInteraction = this->_interactionHandler->_currentInteraction;

                // The disassembly checks GUObjectArray flags to ensure the object is valid and not pending kill.
                if (IsValid(currentInteraction) == true)
                {
                    // Check if the interaction is configured to be canceled upon taking damage
                    if (currentInteraction->CancelOnHit == true)
                    {
                        // Fire the Client RPC to notify the client to cancel the action.
                        // Network RPCs generate thunk functions via the Unreal Header Tool.
                        this->Client_SendCancelInteraction(); /* UNREAL AUTO GENERATED FUNCTION */
                    }
                }
            }
        }

        // Next, check if there is a player "guiding" this player (e.g., carrying them or healing them)
        ADBDPlayer* guidingPlayer = this->_guidingPlayer;

        // Perform standard Unreal validation to ensure the guiding player exists and isn't being destroyed
        if (IsValid(guidingPlayer) == true)
        {
            // Call the virtual validation function located at VTable offset 0x1070
            if (guidingPlayer->IsValidImpl() == true)
            {
                UPlayerInteractionHandler* guideInteractionHandler = guidingPlayer->_interactionHandler;

                // Validate the guiding player's interaction handler
                if (IsValid(guideInteractionHandler) == true)
                {
                    UInteractionDefinition* guideCurrentInteraction = guideInteractionHandler->_currentInteraction;

                    // Validate the guiding player's specific interaction object
                    if (IsValid(guideCurrentInteraction) == true)
                    {
                        // Fetch the active interaction via its getter method
                        UInteractionDefinition* activeInteraction = guidingPlayer->GetCurrentInteraction();
                        
                        if (activeInteraction != nullptr)
                        {
                            // Check if the guiding player's interaction should be canceled on hit
                            if (activeInteraction->CancelOnHit == true)
                            {
                                // Fire the Client RPC for the guiding player to cancel their interaction
                                guidingPlayer->Client_SendCancelInteraction(); /* UNREAL AUTO GENERATED FUNCTION */
                            }
                        }
                    }
                }
            }
        }
    }
}

// starts at line 3950
void ADBDPlayer::AppendPerksTo(TArray<UPerk*>& OutPerks) const
{
    // The disassembly receives the 'this' pointer as an IPerkOwnerInterface*.
    // Since IPerkOwnerInterface is located at offset 0x7A0 inside ADBDPlayer,
    // the assembly instruction [this + 0x4C8] effectively accesses memory at 0x7A0 + 0x4C8 = 0x0C68.
    // 0x0C68 is the exact offset of the '_perks' TArray inside the ADBDPlayer structure.
    
    // Iterate over the player's perks array
    for (int i = 0; i < this->_perks.Num(); i++)
    {
        UPerk* currentPerk = this->_perks[i];

        // The disassembly checks if the pointer is null and queries GUObjectArray 
        // to ensure the object is not pending destruction (flag 0x20000000). 
        // This is perfectly represented by Unreal Engine's standard IsValid() check.
        if (IsValid(currentPerk) == true)
        {
            // The memory manipulation, bounds checking, and ResizeGrow call 
            // correspond directly to the standard TArray::Add() method.
            OutPerks.Add(currentPerk);
        }
    }
}

// starts at line 3962
void ADBDPlayer::AppendOfferingsTo(TArray<FOffering>& OutOfferings) const
{
    // Iterate through all the offerings currently held by the player
    for (int i = 0; i < this->_offerings.Num(); i++)
    {
        // Add a copy of the current offering to the output array.
        // The disassembly shows bounds checking, ResizeGrow, and a call to the 
        // FOfferingProperties copy constructor. This is the compiled form of TArray::Add().
        OutOfferings.Add(this->_offerings[i]);
    }
}

// starts at line 3974
void ADBDPlayer::AppendStatusEffectsTo(TArray<UStatusEffect*>& OutStatusEffects) const
{
    // Iterate over the player's internal status effects array
    for (int i = 0; i < this->_statusEffects.Num(); i++)
    {
        UStatusEffect* currentEffect = this->_statusEffects[i];

        // The disassembly explicitly checks if the pointer is null and queries GUObjectArray 
        // to ensure the object is not pending destruction (flag 0x20000000). 
        // This maps perfectly to Unreal Engine's standard IsValid() macro.
        if (IsValid(currentEffect) == true)
        {
            // The memory manipulation, bounds checking, and ResizeGrow call 
            // correspond directly to the standard TArray::Add() method.
            OutStatusEffects.Add(currentEffect);
        }
    }
}

// starts at line 3992
TArray<UItemModifier*> ADBDPlayer::GetItemModifiers() const
{
    // Note: Function is called through interface IPerkOwnerInterface (offset 0x7A0).
    // Compiler automatically adjusts offsets.
    // Offset 0x4B8 relative to interface points to Inventory (0xC58 relative to class base offset).
    TArray<UItemModifier*> Result;

    // 1. Check if inventory has items (Inventory.Num() > 0)
    if (this->Inventory.Num() > 0)
    {
        // 2. Get the first item (Inventory[0])
        ACollectable* Item = this->Inventory[0];

        // 3. Check if item is valid (!= nullptr, GUObjectArray, !PendingKill)
        if (IsValid(Item))
        {
            // 4. Call GetItemModifiers of the item
            Result = Item->GetItemModifiers();
        }
    }

    // Return array copy (memcpy is assembly)
    return Result;
}

// starts at line 4003
void ADBDPlayer::AuthorityTriggerSprint()
{
    // Check if the executing player possesses network authority.
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ENetRole::ROLE_Authority)
    {
        // Trigger the multicast RPC to execute the sprint logic on all connected clients.
        // In Unreal Engine, calling the base function invokes the auto-generated UHT thunk 
        // which handles the network serialization and dispatch.
        this->Multicast_TriggerSprint(); /* UNREAL AUTO GENERATED FUNCTION */
    }
}

// starts at line 4013
void ADBDPlayer::AuthorityTriggerSprintWithParams(float duration, float cooldown, UCurveFloat* curve, FName effector)
{
    // Check if the executing player possesses network authority.
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ENetRole::ROLE_Authority)
    {
        // Trigger the multicast RPC to execute the parameterized sprint logic on all connected clients.
        // Notice that the 'cooldown' parameter is intentionally omitted from the multicast call, 
        // implying that cooldown logic is managed strictly on the server or locally by the effector, 
        // rather than being broadcasted to all observing clients.
        this->Multicast_TriggerSprintWithParams(duration, curve, effector); /* UNREAL AUTO GENERATED FUNCTION */
    }
}

// starts at line 4027
void ADBDPlayer::Multicast_TriggerSprintWithParams_Implementation(float duration, UCurveFloat* curve, FName effector)
{
    // Retrieve the movement component and cast it to the custom DeadByDaylight movement component.
    // The disassembly calls the virtual GetMovementComponent (offset 0x5A8) and checks the class type.
    UDBDCharacterMovementComponent* dbdMoveComp = Cast<UDBDCharacterMovementComponent>(this->GetMovementComponent());

    // Retrieve the GameInstance.
    // The disassembly accesses the World via virtual function 0x108, then accesses OwningGameInstance at offset 0x120.
    UDBDGameInstance* dbdGameInstance = nullptr;
    if (this->GetWorld() != nullptr)
    {
        dbdGameInstance = Cast<UDBDGameInstance>(this->GetWorld()->GetGameInstance());
    }

    // Perform validation checks.
    // The disassembly manually checks against the GlobalObjectArray and RF_Unreachable flags. 
    // IsValid() is the functional equivalent in UE4.
    if (dbdMoveComp != nullptr && IsValid(dbdMoveComp) == true &&
        dbdGameInstance != nullptr && IsValid(dbdGameInstance) == true)
    {
        UPerkManager* perkManager = this->GetPerkManager();

        // Check if the perk manager is valid (though the disassembly jumps straight to using it if the previous checks passed).
        if (perkManager != nullptr)
        {
            // Retrieve the perk modifier value.
            // Arg2 (dl) = 0x46 (70) -> Likely EGameplayModifierType::VE_HideBloodTrail or similar movement related modifier.
            // Arg3 (r8b) = 4 -> Likely EGameplayModifierSource::VE_All or VE_Perk.
            float modifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifySprintDuration, EGameplayModifierSource::VE_All);

            // Calculate the final duration: (Modifier + 1.0f) * BaseDuration.
            float finalDuration = (modifier + 1.0f) * duration;

            // Apply the calculated duration to the Sprint Timer in the movement component.
            // Offset 0x738 in UDBDCharacterMovementComponent corresponds to a FDBDTimer.
            dbdMoveComp->_sprintSpeedBoostDurationTimer.Reset(finalDuration);

            // Set the Sprint Curve (Offset 0x708).
            dbdMoveComp->OnSprintMaxSpeedCurve = curve;

            // Set the Sprint Effector Name (Offset 0x710).
            dbdMoveComp->SprintEffector = effector;
        }
    }
}

// starts at line 4042
void ADBDPlayer::Multicast_TriggerSprint_Implementation()
{
    // Retrieve the movement component and cast it to the custom DeadByDaylight movement component.
    // The disassembly calls the virtual GetMovementComponent (offset 0x5A8).
    UDBDCharacterMovementComponent* dbdMoveComp = Cast<UDBDCharacterMovementComponent>(this->GetMovementComponent());

    // Retrieve the GameInstance to access global tunables.
    UDBDGameInstance* dbdGameInstance = nullptr;
    if (this->GetWorld() != nullptr)
    {
        dbdGameInstance = Cast<UDBDGameInstance>(this->GetWorld()->GetGameInstance());
    }

    // Perform validation checks.
    if (dbdMoveComp != nullptr && IsValid(dbdMoveComp) == true &&
        dbdGameInstance != nullptr && IsValid(dbdGameInstance) == true)
    {
        UPerkManager* perkManager = this->GetPerkManager();

        if (perkManager != nullptr)
        {
            // Retrieve the perk modifier for the sprint.
            // Arg2 (0x46) = 70. The pseudo-code identifies this as VE_HideBloodTrail, but in this context, 
            // it likely represents a Sprint Duration modifier.
            // Arg3 (4) = Likely VE_All or VE_Perk sources.
            float modifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifySprintDuration, EGameplayModifierSource::VE_All);

            // Retrieve the base duration from the global tunables.
            // The Disassembly accesses the DBDDesignTunables from the GameInstance (offset 0xF0).
            UDBDDesignTunables* tunables = dbdGameInstance->GetDesignTunables();
            
            // Retrieve the specific tunable value for "CAMPER_SPRINT_DURATION".
            // The third argument (1) is 'warnIfRowMissing'.
            float baseDuration = tunables->GetTunableValue(TunableValues::CAMPER_SPRINT_DURATION, true);

            // Calculate the final duration: (Modifier + 1.0f) * BaseDuration.
            float finalDuration = (modifier + 1.0f) * baseDuration;

            // Apply the calculated duration to the Sprint Timer in the movement component (Offset 0x738).
            dbdMoveComp->_sprintSpeedBoostDurationTimer.Reset(finalDuration);

            // Set the Sprint Effector Name (Offset 0x710).
            // The disassembly loads a string constant (byte_14297952F). 
            // Without the string table, we assume a default name like "Sprint" or "Burst".
            dbdMoveComp->SprintEffector = FName(NAME_None);
        }
    }
}

// starts at line 4133
bool ADBDPlayer::HasClearPathToTarget(FVector targetPosition) const
{
    // Initialize StartPosition to zero vector
    FVector StartPosition = FVector::ZeroVector;

    // Check if the RootComponent is valid before accessing its location
    if (this->RootComponent != nullptr)
    {
        // Get the world translation of the root component
        // Disassembly: Accesses ComponentToWorld.Translation at offset 0x140
        StartPosition = this->RootComponent->GetComponentLocation();
    }

    // Initialize the parameters struct for the utility check
    // The constructor is called at 0x1403820B6
    UDBDUtilities::FCapsuleParams CapsuleParams;

    // Populate parameters from ADBDPlayer member variables
    // Offsets: 0xC28 (StepHeight), 0xC0C (HeightMultiplier), 0xC08 (RadiusMultiplier)
    CapsuleParams.StepHeight = this->ClearPathTestStepHeight;
    CapsuleParams.HeightMultiplier = this->ClearPathTestHeightMultiplier;
    CapsuleParams.RadiusMultiplier = this->ClearPathTestRadiusMultiplier;

    // Set the player context
    CapsuleParams.Player = this;

    // Create a local FHitResult variable to satisfy the utility function's signature
    // Disassembly: memset at 0x140382050 clears this struct and sets Time to 1.0f (0x14038205C)
    FHitResult OutHitResult;

    // Retrieve the World context
    UWorld* World = this->GetWorld();

    // Call the static utility function
    // StartPosition and targetPosition are passed by value as per the mangled name
    return UDBDUtilities::HasClearPathToTarget(World, CapsuleParams, StartPosition, targetPosition, OutHitResult);
}

// starts at line 4139
bool ADBDPlayer::HasClearPathToTargetWithIgnore(FVector targetPosition, const TArray<AActor*>& ignoreActors) const
{
    // Create a local TSet to hold the ignored actors.
    // The utility function's parameters struct likely uses a TSet for efficient lookups.
    TSet<AActor*> IgnoredActorsSet;

    // Iterate through the input array and add each actor to the set.
    // Disassembly: Loop from 0x140382410 to 0x14038243B calling Emplace.
    for (AActor* Actor : ignoreActors)
    {
        IgnoredActorsSet.Add(Actor);
    }

    // Determine the starting position from the RootComponent.
    FVector StartPosition = FVector::ZeroVector;
    if (this->RootComponent != nullptr)
    {
        StartPosition = this->RootComponent->GetComponentLocation();
    }

    // Initialize the parameters struct for the utility check.
    UDBDUtilities::FCapsuleParams CapsuleParams;

    // Populate parameters from ADBDPlayer member variables.
    CapsuleParams.StepHeight = this->ClearPathTestStepHeight;
    CapsuleParams.HeightMultiplier = this->ClearPathTestHeightMultiplier;
    CapsuleParams.RadiusMultiplier = this->ClearPathTestRadiusMultiplier;

    // Append the locally created set of ignored actors to the params' internal set.
    CapsuleParams.IgnoreActors.Append(IgnoredActorsSet);

    // Set the player context.
    CapsuleParams.Player = this;

    // Create a local FHitResult (required by the utility signature).
    FHitResult OutHitResult;

    // Retrieve the World context.
    UWorld* World = this->GetWorld();

    // Call the static utility function.
    return UDBDUtilities::HasClearPathToTarget(World, CapsuleParams, StartPosition, targetPosition, OutHitResult);
}

// starts at line 4150
bool ADBDPlayer::HasClearPathToTarget(FVector targetPosition, const TSet<AActor*>* ignoreActors) const
{
    // Initialize StartPosition to ZeroVector.
    // This acts as a fallback if the RootComponent is invalid.
    FVector StartPosition = FVector::ZeroVector;

    // Retrieve the RootComponent to determine the starting position.
    // Disassembly: Accesses offset 0x160.
    USceneComponent* RootComp = this->RootComponent;

    // Check if the RootComponent is valid (not nullptr).
    if (RootComp != nullptr)
    {
        // Get the world location (Translation) of the RootComponent.
        // Disassembly: Accesses ComponentToWorld at offset 0x140.
        StartPosition = RootComp->GetComponentLocation();
    }

    // Create a local FHitResult variable.
    // Disassembly: Initializes 0x88 bytes to 0 and sets Time to 1.0f.
    FHitResult OutHitResult;

    // Call the detailed HasClearPathToTarget overload.
    // Passes the calculated StartPosition, the provided targetPosition, the ignore list, and the local HitResult.
    return this->HasClearPathToTarget(StartPosition, targetPosition, ignoreActors, OutHitResult);
}

// starts at line 4156
bool ADBDPlayer::HasClearPathToTarget(FVector targetPosition, const TSet<AActor*>* ignoreActors, FHitResult* outHitResult) const
{
    // Initialize the start position to zero
    FVector StartPosition = FVector::ZeroVector;

    // Retrieve the RootComponent
    // Disassembly: Accesses offset 0x160
    USceneComponent* RootComp = this->RootComponent;

    // Check if RootComponent is valid
    if (RootComp != nullptr)
    {
        // Get the world location (Translation) from the component's transform
        // Disassembly: Accesses ComponentToWorld at offset 0x140
        StartPosition = RootComp->GetComponentLocation();
    }

    // Call the detailed overload of HasClearPathToTarget
    // Note: The 'outHitResult' pointer passed to this function is dereferenced 
    // to match the reference signature (&) expected by the called function (implied by previous context/mangling).
    return this->HasClearPathToTarget(StartPosition, targetPosition, ignoreActors, *outHitResult);
}

// starts at line 4161
bool ADBDPlayer::HasClearPathToTarget(FVector StartPosition, FVector EndPosition, const TSet<AActor*>* IgnoreActors, FHitResult* OutHitResult) const
{
    // Initialize the parameters struct for the utility check
    // The constructor is called at 0x140381D47
    UDBDUtilities::FCapsuleParams CapsuleParams;

    // Populate parameters from ADBDPlayer member variables
    // Offsets: 0xC28 (StepHeight), 0xC0C (HeightMultiplier), 0xC08 (RadiusMultiplier)
    CapsuleParams.StepHeight = this->ClearPathTestStepHeight;
    CapsuleParams.HeightMultiplier = this->ClearPathTestHeightMultiplier;
    CapsuleParams.RadiusMultiplier = this->ClearPathTestRadiusMultiplier;

    // If an IgnoreActors set was provided, append it to the params' internal ignore list
    if (IgnoreActors != nullptr)
    {
        CapsuleParams.IgnoreActors.Append(*IgnoreActors);
    }

    // Set the player context
    CapsuleParams.Player = this;

    // Retrieve the World context
    UWorld* World = this->GetWorld();

    // Call the static utility function
    // Note: StartPosition and EndPosition are passed by value in the utility signature
    return UDBDUtilities::HasClearPathToTarget(World, CapsuleParams, StartPosition, EndPosition, *OutHitResult);
}

// starts at line 4176
bool ADBDPlayer::HasClearPathToTarget(FVector StartPosition, FVector EndPosition, const TSet<AActor*>* IgnoreActors, FHitResult* OutHitResult, const TArray<FDBDResponseChannel>* AdditionalCollisionResponses) const
{
    // Initialize the parameters struct for the utility check
    UDBDUtilities::FCapsuleParams CapsuleParams;

    // Populate parameters from ADBDPlayer member variables
    // Offsets: 0xC28 (StepHeight), 0xC0C (HeightMultiplier), 0xC08 (RadiusMultiplier)
    CapsuleParams.StepHeight = this->ClearPathTestStepHeight;
    CapsuleParams.HeightMultiplier = this->ClearPathTestHeightMultiplier;
    CapsuleParams.RadiusMultiplier = this->ClearPathTestRadiusMultiplier;

    // If an IgnoreActors set was provided, append it to the params' internal ignore list
    if (IgnoreActors != nullptr)
    {
        CapsuleParams.IgnoreActors.Append(*IgnoreActors);
    }

    // Check if the pointer to AdditionalCollisionResponses is valid
    if (AdditionalCollisionResponses != nullptr)
    {
        // Check if there are any elements in the provided array
        if (AdditionalCollisionResponses->Num() > 0)
        {
            // Append the additional collision responses to the CapsuleParams
            // Disassembly performs a ResizeTo followed by a memcpy, which is the internal implementation of TArray::Append
            CapsuleParams.AdditionalCollisionResponses.Append(*AdditionalCollisionResponses);
        }
    }

    // Set the player context
    CapsuleParams.Player = this;

    // Retrieve the World context
    UWorld* World = this->GetWorld();

    // Call the static utility function
    // Note: StartPosition and EndPosition are passed to the utility
    return UDBDUtilities::HasClearPathToTarget(World, CapsuleParams, StartPosition, EndPosition, *OutHitResult);
}

// starts at line 4193
bool ADBDPlayer::IsLocationClear(FVector targetPosition) const
{
    // Initialize a HitResult structure
    // Disassembly: memset to 0, sets Time to 1.0f
    FHitResult HitResult(ForceInit);
    HitResult.Time = 1.0f;

    // Initialize the parameter struct for the capsule check
    // Disassembly: Calls UDBDUtilities::FCapsuleParams constructor
    UDBDUtilities::FCapsuleParams CapsuleParams;

    // Populate the parameters using the player's specific multipliers
    // Disassembly: Accesses offsets 0xC10 (Radius) and 0xC14 (Height)
    CapsuleParams.RadiusMultiplier = this->LocationClearTestRadiusMultiplier;
    CapsuleParams.HeightMultiplier = this->LocationClearTestHeightMultiplier;
    
    // Set the reference to the player actor
    // Note: Disassembly passes 'this' (rdi) to the struct member
    CapsuleParams.Player = this;

    // Retrieve the World pointer
    UWorld* World = this->GetWorld();

    // Perform the static utility check
    // Disassembly: Calls ?IsLocationClear@UDBDUtilities@@SA_N...
    bool bIsClear = UDBDUtilities::IsLocationClear(World, CapsuleParams, targetPosition, HitResult);

    // Note: The extensive cleanup code (FMemory::Free, ResizeTo, etc.) seen in the disassembly 
    // corresponds to the destructor of the local 'CapsuleParams' struct (handling its internal TArrays/TSets) 
    // executing as the function scope ends. In C++, this is implicit.

    return bIsClear;
}

// starts at line 4199
bool ADBDPlayer::IsLocationClear(FVector targetPosition, const TSet<AActor*>* ignoreActors, const TArray<FDBDResponseChannel>& additionalCollisionResponses) const
{
    // Initialize a local FHitResult structure
    // Disassembly: memset to 0, sets Time to 1.0f (indicates no hit initially)
    FHitResult HitResult(ForceInit);
    HitResult.Time = 1.0f;

    // Call the overloaded internal function that performs the actual check
    // Pass the local HitResult to capture any collision data (unused here)
    return this->IsLocationClear(targetPosition, ignoreActors, additionalCollisionResponses, HitResult);
}

// starts at line 4204
bool ADBDPlayer::IsLocationClear(FVector targetPosition, const TSet<AActor*>* ignoreActors, const TArray<FDBDResponseChannel>& additionalCollisionResponses, FHitResult& outHitResult) const
{
    // Initialize the Capsule Parameters struct used by the utility function
    UDBDUtilities::FCapsuleParams CapsuleParams;

    // Set multipliers from member variables
    // Disassembly: Accesses offset 0xC10 and 0xC14
    CapsuleParams.RadiusMultiplier = this->LocationClearTestRadiusMultiplier;
    CapsuleParams.HeightMultiplier = this->LocationClearTestHeightMultiplier;
    
    // Set the player reference
    CapsuleParams.Player = this;

    // If an IgnoreActors set was provided, append it to the params
    // Disassembly: Checks for null before calling Append
    if (ignoreActors != nullptr)
    {
        CapsuleParams.IgnoreActors.Append(*ignoreActors);
    }

    // If there are additional collision responses, append them to the params
    // Disassembly: Checks ArrayNum > 0, then Resizes and Memcpys (TArray::Append logic)
    if (additionalCollisionResponses.Num() > 0)
    {
        CapsuleParams.AdditionalCollisionResponses.Append(additionalCollisionResponses);
    }

    // Retrieve the World
    UWorld* World = this->GetWorld();

    // Perform the location check using the static utility function
    // Disassembly: Calls UDBDUtilities::IsLocationClear
    return UDBDUtilities::IsLocationClear(World, CapsuleParams, targetPosition, outHitResult);
}

// starts at line 4220
FVector ADBDPlayer::FindFurthestClearLocationBetweenPoints(FVector startPosition, FVector targetPosition) const
{
    // Initialize FHitResult
    // The disassembly memsets this to 0 and sets Time to 1.0f (standard constructor behavior)
    FHitResult OutHitResult;

    // Initialize FCapsuleParams structure defined in UDBDUtilities
    // This structure likely contains collision parameters used for the trace
    UDBDUtilities::FCapsuleParams Params;

    // Populate the parameters with values from this player's configuration
    // Offsets 0xC28, 0xC0C, 0xC08 correspond to these members
    Params.StepHeight = this->ClearPathTestStepHeight;
    Params.HeightMultiplier = this->ClearPathTestHeightMultiplier;
    Params.RadiusMultiplier = this->ClearPathTestRadiusMultiplier;
    Params.Player = this;

    // Retrieve the World pointer (Virtual function call at VTable offset 0x108)
    UWorld* World = this->GetWorld();

    // Call the static utility function to perform the calculation
    // This function returns the calculated FVector result
    return UDBDUtilities::FindFurthestClearLocationBetweenPoints(World, Params, startPosition, targetPosition, OutHitResult);
}

// starts at line 4226
FVector ADBDPlayer::FindFurthestClearLocationBetweenPoints(FVector startPosition, FVector targetPosition, const TSet<AActor*>* ignoreActors, const TArray<FDBDResponseChannel>* additionalCollisionResponses) const
{
    // Initialize FHitResult
    // The disassembly memsets this to 0 and sets Time to 1.0f
    FHitResult OutHitResult;

    // Call the other overloaded version of FindFurthestClearLocationBetweenPoints
    // This overload takes FHitResult as a reference parameter, which is likely where the actual logic resides or delegates further
    // Note: The disassembly shows it calling a function with the same name but different signature (likely internal or protected helper)
    return this->FindFurthestClearLocationBetweenPoints(startPosition, targetPosition, ignoreActors, additionalCollisionResponses, OutHitResult);
}

// starts at line 4233
FVector ADBDPlayer::FindFurthestClearLocationBetweenPoints(FVector startPosition, FVector targetPosition, const TSet<AActor*>* ignoreActors, const TArray<FDBDResponseChannel>* additionalCollisionResponses, FHitResult& outHitResult) const
{
    // Initialize the HitResult to default (no hit)
    // The disassembly memsets outHitResult to 0 and sets Time to 1.0f
    outHitResult = FHitResult();

    // Prepare collision parameters using UDBDUtilities::FCapsuleParams
    UDBDUtilities::FCapsuleParams Params;

    // Copy player settings to parameters
    Params.StepHeight = this->ClearPathTestStepHeight;
    Params.HeightMultiplier = this->ClearPathTestHeightMultiplier;
    Params.RadiusMultiplier = this->ClearPathTestRadiusMultiplier;
    Params.Player = this;

    // Handle 'ignoreActors'
    // If the input set is provided, merge it into the params.IgnoreActors set
    if (ignoreActors != nullptr)
    {
        Params.IgnoreActors.Append(*ignoreActors);
    }

    // Handle 'additionalCollisionResponses'
    // The disassembly manually appends elements from the input array to the params array
    if (additionalCollisionResponses != nullptr && additionalCollisionResponses->Num() > 0)
    {
        Params.AdditionalCollisionResponses.Append(*additionalCollisionResponses);
    }

    // Retrieve the world
    UWorld* World = this->GetWorld();

    // Call the utility function to perform the sweep/trace
    // Returns the calculated clear location
    return UDBDUtilities::FindFurthestClearLocationBetweenPoints(World, Params, startPosition, targetPosition, outHitResult);
}

// starts at line 4249
float ADBDPlayer::GetClearPathTestCapsuleRadius() const
{
    // Retrieve the Character's Capsule Component (Offset 0x3F0)
    UCapsuleComponent* Capsule = this->CapsuleComponent;

    // Validate the component pointer and object state (GUObjectArray check)
    if (Capsule != nullptr && Capsule->IsValidLowLevel() == true)
    {
        // Retrieve the component's scale from its world transform (Offset 0x150)
        FVector Scale3D = Capsule->GetComponentScale();

        // Calculate the absolute scale values to handle negative scaling (SignMask)
        float AbsScaleX = FMath::Abs(Scale3D.X);
        float AbsScaleY = FMath::Abs(Scale3D.Y);
        float AbsScaleZ = FMath::Abs(Scale3D.Z);

        // Determine the minimum scaling factor across all axes
        // The assembly uses vector intrinsics (minps) to find the smallest element
        float MinScale = FMath::Min(AbsScaleX, FMath::Min(AbsScaleY, AbsScaleZ));

        // Calculate the base scaled radius (Offset 0x634 is CapsuleRadius)
        float ScaledRadius = MinScale * Capsule->CapsuleRadius;

        // Apply the specific test multiplier defined on the player (Offset 0xC08)
        return ScaledRadius * this->ClearPathTestRadiusMultiplier;
    }

    // Return 0.0f if the capsule is invalid
    return 0.0f;
}

// starts at line 4257
bool ADBDPlayer::IsUsingGamepad() const
{
    // Return the boolean flag indicating gamepad usage
    // Disassembly: return this->_isUsingGamepad;
    return this->_isUsingGamepad;
}

// starts at line 4262
float ADBDPlayer::GetSprintDurationTimerPercentage() const
{
    // 1. Retrieve the generic Movement Component
    // Calls the virtual function GetMovementComponent() (VTable offset 0x5A8).
    UPawnMovementComponent* MoveComp = this->GetMovementComponent();

    // 2. Safe Cast to UDBDCharacterMovementComponent
    // The assembly performs the ClassTreeIndex check to ensure the component is the correct type.
    UDBDCharacterMovementComponent* DBDMoveComp = Cast<UDBDCharacterMovementComponent>(MoveComp);

    if (DBDMoveComp)
    {
        // 3. Retrieve the Timer Percentage
        // Accesses a specific FDBDTimer member at offset 0x738 in the movement component.
        // Let's assume the member is named 'SprintDurationTimer'.
        return DBDMoveComp->SprintDurationTimer.GetPercentTimeLeft();
    }

    return 0.0f;
}

// starts at line 4272
bool ADBDPlayer::IsSprinting() const
{
    // Retrieve the Movement Component (APawn::GetMovementComponent)
    // Disassembly: call qword ptr [rax+5A8h]
    UPawnMovementComponent* MovementComponent = this->GetMovementComponent();

    // Check if component exists
    if (MovementComponent != nullptr)
    {
        // Cast to the specific custom movement component class
        // Disassembly: Checks ClassTreeIndex against UDBDCharacterMovementComponent::StaticClass
        UDBDCharacterMovementComponent* DBDMovement = Cast<UDBDCharacterMovementComponent>(MovementComponent);

        // Verify the cast succeeded and the object is valid
        if (DBDMovement != nullptr && DBDMovement->IsPendingKill() == false)
        {
            // Access the timer struct at offset 0x738
            // Disassembly: lea rcx, [rbx+738h] -> FDBDTimer
            // We check if the timer is *not* done.
            // Disassembly: test al, al; setz al (Returns true if IsDone() is false)
            
            // Assuming the member is named something like 'SprintTimer' or 'RunTimer'
            return !DBDMovement->_sprintSpeedBoostDurationTimer.IsDone();
        }
    }

    return false;
}

// starts at line 4282
bool ADBDPlayer::CanSprint() const
{
    // Retrieve the generic movement component and cast it to the specific UDBDCharacterMovementComponent.
    // The pseudo-code explicitly checks the ClassTreeIndex, which is the internal logic for a Safe Cast in UE4.
    UDBDCharacterMovementComponent* DBDMovementComponent = Cast<UDBDCharacterMovementComponent>(this->GetMovementComponent());

    if (IsValid(DBDMovementComponent) == false)
    {
        return false;
    }

    // Call the CanSprint function on the custom movement component.
    if (DBDMovementComponent->CanSprint() == false)
    {
        return false;
    }

    // Check if the player is currently exhausted.
    // If IsExhausted returns true, we cannot sprint, so we return false.
    if (this->IsExhausted() != false)
    {
        return false;
    }

    // If the movement component allows it and the player is not exhausted, return true.
    return true;
}

// starts at line 4292
bool ADBDPlayer::IsExhausted() const
{
    // Retrieve the interface pointer for the Perk Manager Owner
    // Disassembly: Address calculation at offset 0x7A8
    const IPerkManagerOwnerInterface* PerkOwnerInterface = static_cast<const IPerkManagerOwnerInterface*>(this);

    // Get the PerkManager instance via the interface
    UPerkManager* PerkManager = PerkOwnerInterface->GetPerkManager();

    // Check if the PerkManager is valid
    if (PerkManager != nullptr)
    {
        // Verify the object is not pending garbage collection
        // Disassembly: Checks GUObjectArray flags (0x20000000)
        if (PerkManager->IsPendingKill() == false)
        {
            // Prepare an array to hold the gameplay modifiers
            TArray<UGameplayModifierContainer*> CombinedEffects;

            // Retrieve all effects from the PerkManager
            // Disassembly passes '4' as the filter argument (EGameplayModifierSource)
            PerkManager->GetAllEffects(CombinedEffects, EGameplayModifierSource::VE_All);

            // Iterate through the retrieved effects
            for (int32_t i = 0; i < CombinedEffects.Num(); i++)
            {
                UGameplayModifierContainer* Effect = CombinedEffects[i];

                // Ensure the effect object is valid and not pending kill
                if (Effect != nullptr && Effect->IsPendingKill() == false)
                {
                    // Check if the effect has the 'Exhausted' flag
                    // Disassembly: mov dl, 36h (54 decimal)
                    if (Effect->HasFlag(EGameplayModifierFlag::VE_IsExhausted) == true)
                    {
                        // Check if the effect is currently applicable
                        // Disassembly: Accesses boolean at offset 0x158
                        if (Effect->m_bIsApplicable != false)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

// starts at line 4297
FName ADBDPlayer::GetSprintEffector() const
{
    // 1. Retrieve the generic Movement Component
    // Virtual call to GetMovementComponent() (Offset 0x5A8 in VTable)
    UPawnMovementComponent* MoveComp = this->GetMovementComponent();

    // 2. Safe Cast to UDBDCharacterMovementComponent
    // The assembly verifies the class inheritance hierarchy.
    UDBDCharacterMovementComponent* DBDMoveComp = Cast<UDBDCharacterMovementComponent>(MoveComp);

    if (DBDMoveComp)
    {
        // 3. Return the Effector Name
        // Retrieves the FName stored at offset 0x710 in the movement component.
        return DBDMoveComp->SprintEffector;
    }

    // 4. Fallback
    // If component is missing or incorrect type, return "None".
    return NAME_None; 
}

// starts at8 line 430
void ADBDPlayer::SetFeetOnGround(bool feetOnGround)
{
    this->_feetAreOnGround = feetOnGround;
}

// starts at line 4313
bool ADBDPlayer::GetFeetOnGround() const
{
    // Return the cached boolean state indicating if feet are grounded.
    return this->_feetAreOnGround;
}

// starts at line 4318
float ADBDPlayer::GetNonPlayerSpecificTunableValue(FName Key) const
{
    // 1. Retrieve the custom Game Instance
    // The assembly performs a Cast<UDBDGameInstance> check via ClassTreeIndex
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (GameInstance)
    {
        // 2. Access the Design Tunables container
        // Located at offset 0xF0 in the GameInstance class
        UDBDDesignTunables* Tunables = GameInstance->GetDesignTunables();

        // 3. Validate the Tunables object
        // Checks against NULL and internal object flags (PendingKill, etc.)
        if (IsValid(Tunables))
        {
            // 4. Retrieve the value
            // Calls GetTunableValue with the raw Key.
            // The boolean argument (warnIfRowMissing) is set to false (xor r8d, r8d in ASM).
            return Tunables->GetTunableValue(Key, false);
        }
    }

    // Return 0.0f if GameInstance or Tunables are missing
    return 0.0f;
}

// starts at line 4333
float ADBDPlayer::GetTunableValue(FName Key, float DefaultValue, bool WarnIfRowMissing) const
{
    float OutValue = 0.0f;

    // Tier 1: Character-Specific Tunables
    // Check the DataTable unique to this character instance (e.g., "Trapper_Tunables").
    // The member 'TunableDB' is an FDataTableProxy at offset 0xE78.
    UDataTable* CharacterTable = this->TunableDB.Get();
    if (UDBDDesignTunables::GetTunableValue(CharacterTable, Key, OutValue))
    {
        return OutValue;
    }

    // Tier 2: Global Tunables
    // If not found, check the general DataTable shared by all characters.
    // The member 'GlobalTunableDB' is an FDataTableProxy at offset 0xEA8.
    UDataTable* GlobalTable = this->GlobalTunableDB.Get();
    if (UDBDDesignTunables::GetTunableValue(GlobalTable, Key, OutValue))
    {
        return OutValue;
    }

    // Tier 3: Master Tunables (Fallback)
    // If both DataTables fail, defer to GetMasterTunableValue.
    // This function likely handles complex key mapping (e.g., "Speed" -> "Trapper_Speed") 
    // or queries the GameInstance.
    return this->GetMasterTunableValue(Key, DefaultValue, WarnIfRowMissing);
}

// starts at line 4357
float ADBDPlayer::GetMasterTunableValue(FName key, float defaultValue, bool warnIfRowMissing) const
{
    // 1. Get the Game Instance
    UGameInstance* GameInstance = this->GetGameInstance();
    if (!GameInstance)
    {
        return defaultValue;
    }

    // 2. Validate that it's the correct Game Instance type (UDBDGameInstance)
    // The ASM checks inheritance via ClassTreeIndex
    if (!GameInstance->IsA(UDBDGameInstance::StaticClass()))
    {
        return defaultValue;
    }

    // 3. Access the UDBDDesignTunables object
    // It is located at offset 0xF0 in the GameInstance (v6[1]... vftable logic in ASM implies a member object)
    // In the reconstructed context, let's assume a getter or direct member access.
    auto* DesignTunables = Cast<UDBDGameInstance>(GameInstance)->GetDesignTunables(); // or member access

    // 4. Construct the Lookup Key
    FString LookupKeyString;

    // Check if the player has a valid Character Name (Offset 0xA70)
    // If the name is valid (Length > 1), prepend it to the key.
    if (this->CharacterName.Len() > 0)
    {
        // Format: "CharacterName_Key"
        LookupKeyString = this->CharacterName + TEXT("_") + key.ToString();
    }
    else
    {
        // Just use the key as is
        LookupKeyString = key.ToString();
    }

    // 5. Query the Tunables System
    // Convert the constructed string back to an FName for lookup
    FName FinalKey(*LookupKeyString);

    return DesignTunables->GetTunableValue(FinalKey, defaultValue, warnIfRowMissing);
}

// starts at line 4369
bool ADBDPlayer::CanInteractWith(const UInteractionDefinition* interaction)
{
    // Access the CharacterMovement component.
    return !this->GetCharacterMovement()->IsFalling();
}

// starts at line 4374
void ADBDPlayer::NotifyEquipedItemUpdated()
{
    // Execute the delegate located at offset 0xAF0.
    // The disassembly performs a tail call (JMP) to ExecuteIfBound after adjusting the 'this' pointer to the delegate member.
    this->OnEquippedItemUpdated.ExecuteIfBound();
}

// starts at line 4379
ADBDPlayerState* ADBDPlayer::GetDBDPlayerState() const
{
    // Perform a safe cast to the game-specific player state class
    // "SafeCast" in the pseudo-code typically maps to the UE4 Cast<T> template
    return Cast<ADBDPlayerState>(this->GetPlayerState());
}

// starts at line 4384
void ADBDPlayer::OnLevelReadyToPlay()
{
    // Check if the current role is Authority (Server).
    // offset 0x0110 in _S corresponds to AActor::Role.
    if (this->Role == ROLE_Authority)
    {
        // Server-side: Spawn perks based on the player's selected loadout.
        this->Authority_SpawnLoadoutPerks();

        // Server-side: Spawn item add-ons (like toolboxes or medkit modifiers).
        this->Authority_SpawnLoadoutItemAddons();
    }

    // Attempt to notify the PlayerState that this pawn is now active in the match.
    if (this->PlayerState != nullptr)
    {
        // Cast the generic PlayerState to the game-specific ADBDPlayerState.
        // The disassembly performs a class tree check against the DeadByDaylight script class.
        ADBDPlayerState* dbdPlayerState = Cast<ADBDPlayerState>(this->PlayerState);

        if (dbdPlayerState != nullptr)
        {
            // Validate that the PlayerState object is still alive and not pending destruction.
            // (Checks InternalIndex and GUObjectArray flags for RF_Unreachable).
            if (IsValid(dbdPlayerState) == true && dbdPlayerState->bActorIsBeingDestroyed == false)
            {
                // Notify the state that the pawn is becoming active.
                // This typically triggers UI updates and start-of-match logic for that specific player.
                dbdPlayerState->OnPlayerPawnBecomingActive();
            }
        }
    }

    // Set the local flag at offset 0x0F98 to indicate readiness.
    this->_isReadyToPlay = true;
}

// starts at line 4401
void ADBDPlayer::OnAllPlayerLoaded()
{
    // Check if the current role is Authority (Server).
    // offset 0x0110 in _S corresponds to AActor::Role.
    // 3 represents ROLE_Authority in the ENetRole enum.
    if (this->Role == ROLE_Authority)
    {
        // Retrieve the persistent data associated with this player.
        // This appears to be a custom getter function specific to this game.
        const FPlayerPersistentData* persistentData = this->GetPlayerPersistentData();

        // Ensure the returned data pointer is valid.
        if (persistentData != nullptr)
        {
            // Check if the customization array has elements.
            // In the disassembly (0x198), this compares the TArray::ArrayNum property to 0.
            // Access path derived from Pseudo Code: SavedData.SharedData.SelectedCharacterCustomization
            if (persistentData->SavedData.SharedData.SelectedCharacterCustomization.Num() != 0)
            {
                // Call the Multicast function to apply the customization.
                // We pass the array found at offset 0x190 (from disassembly lea instruction).
                this->Multicast_SetCustomization(persistentData->SavedData.SharedData.SelectedCharacterCustomization);
            }
        }
    }
}

// starts at line 4409
void ADBDPlayer::DBD_TogglePrintServerPositionUpdateEnabled()
{
    // Toggle the value of _printServerPositionUpdateEnabled at offset 0x1080
    // If it is currently false (0), set it to true (1), otherwise set it to false (0)
    this->_printServerPositionUpdateEnabled = (this->_printServerPositionUpdateEnabled == false);
}

// starts at line 4414
bool ADBDPlayer::GetIsInteracting() const
{
    // Return the boolean state indicating if the player is currently interacting
    // Maps to the member variable IsInteracting (or bIsInteracting in standard UE4 naming)
    return this->IsInteracting;
}

// starts at line 4419
bool ADBDPlayer::IsSnapping() const
{
    // Retrieve the Character Snapping Component
    // Disassembly: Accesses member at offset 0xE58
    UCharacterSnappingComponent* SnappingComponent = this->_characterSnappingComponent;

    // Check if the component pointer is not null
    if (SnappingComponent != nullptr)
    {
        // Verify the component object is valid (not marked for garbage collection/pending kill)
        // Disassembly: Standard UE4 GUObjectArray flag check (0x20000000)
        if (SnappingComponent->IsPendingKill() == false)
        {
            // Delegate the check to the component's IsSnapping method
            // Disassembly: Tail call (jmp) to UCharacterSnappingComponent::IsSnapping
            return SnappingComponent->IsSnapping();
        }
    }

    // If component is missing or invalid, we are not snapping
    return false;
}

// starts at line 4429
bool ADBDPlayer::IsSnappingPosition() const
{
    // Retrieve the Character Snapping Component
    // Disassembly: Accesses member at offset 0xE58
    UCharacterSnappingComponent* SnappingComponent = this->_characterSnappingComponent;

    // Check if the component exists
    if (SnappingComponent != nullptr)
    {
        // Verify the component object is valid (not pending kill)
        // Disassembly: Standard UE4 GUObjectArray flag check (0x20000000)
        if (SnappingComponent->IsPendingKill() == false)
        {
            // Return the boolean flag indicating if position snapping is active.
            // Disassembly: movzx eax, byte ptr [rdx+115h]
            return SnappingComponent->_snapPosition;
        }
    }

    // If the component is missing or invalid, assume false
    return false;
}

// starts at line 4439
bool ADBDPlayer::IsSnappingRotation() const
{
    // Retrieve the Character Snapping Component
    // Disassembly: Accesses member at offset 0xE58
    UCharacterSnappingComponent* SnappingComponent = this->_characterSnappingComponent;

    // Check if the component exists
    if (SnappingComponent != nullptr)
    {
        // Verify the component object is valid (not pending kill)
        // Disassembly: Standard UE4 GUObjectArray flag check (0x20000000)
        if (SnappingComponent->IsPendingKill() == false)
        {
            // Return the boolean flag indicating if rotation snapping is active.
            // Disassembly: movzx eax, byte ptr [rdx+114h]
            // Note: This is adjacent to _snapPosition (0x115) seen in the previous function.
            return SnappingComponent->_snapRotation;
        }
    }

    // If the component is missing or invalid, assume false
    return false;
}

// starts at line 4449
void ADBDPlayer::SetIsInteracting(bool value)
{
    this->IsInteracting = value;
}

// starts at line 4459
void ADBDPlayer::SetIsSwappingInteraction(bool value)
{
    this->_isSwappingInteraction = value;
}

// starts at line 4464
void ADBDPlayer::OnFinishedPlaying()
{
    // Retrieve the interaction handler component at offset 0x0CF0.
    UPlayerInteractionHandler* interactionHandler = this->_interactionHandler;

    // Verify the interaction handler is valid and not pending destruction.
    // The disassembly performs a manual check against GUObjectArray flags (RF_Unreachable).
    if (interactionHandler != nullptr && IsValid(interactionHandler) == true)
    {
        // Disable the ability to scan for new interactions.
        // offset 0x30C in UPlayerInteractionHandler corresponds to _scanForInteractions.
        interactionHandler->_scanForInteractions = false;

        // Retrieve the standard Unreal Engine InputComponent at offset 0x0118.
        UInputComponent* inputComp = this->InputComponent;

        // Verify the input component is valid and not pending destruction.
        if (inputComp != nullptr && IsValid(inputComp) == true)
        {
            // Clear all action bindings associated with this player's input.
            // This prevents the player from triggering any further actions (jump, interact, etc.)
            inputComp->ClearActionBindings();
        }
    }
}

// starts at line 4488
bool ADBDPlayer::GetIsInterrupting() const
{
    // Return the cached boolean state indicating if the player is currently performing an interruption
    // Maps to the member variable _isInterrupting
    return this->_isInterrupting;
}

// starts at line 4492
void ADBDPlayer::SetIsInterrupting(bool isInterrupting)
{
    this->_isInterrupting = isInterrupting;
}

// starts at line 4498
bool ADBDPlayer::ArePlayersHandsFullOfBearTraps() const
{
    // Retrieve the Perk Manager via the IPerkManagerOwnerInterface.
    // The interface is located at offset 0x7A8 in the ADBDPlayer structure.
    UPerkManager* perkManager = this->GetPerkManager();

    // Fetch the modifier for extra bear trap capacity from the perk manager.
    // The disassembly loads 0x75 (117 in decimal) for the modifier type, which 
    // the pseudo-code strangely maps to a combined enum. It also uses 4 for the filter.
    // Since the exact enum EGameplayModifierType definitions are not provided, we mark them.
    float extraTrapModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyMaximumBearTrapCapacity, EGameplayModifierSource::VE_All);

    // The assembly performs the following math:
    // 1. mulss xmm0, 2.0f
    // 2. addss xmm0, 0.5f
    // 3. cvtss2si ebx, xmm1 (truncating to integer)
    // 4. sar ebx, 1 (divide by 2)
    // This is mathematically equivalent to: FMath::RoundToInt(extraTrapModifier)
    int32 extraTrapCapacity = FMath::RoundToInt(extraTrapModifier);

    // The base capacity for bear traps is 1. The total capacity is base + extra.
    int32 maxBearTrapCapacity = extraTrapCapacity + 1;

    // Check if the current amount of bear traps in the inventory meets or exceeds the maximum capacity.
    return this->GetItemCountByID(BEARTRAP_ID) >= maxBearTrapCapacity;
}

// starts at line 4503
USceneComponent* ADBDPlayer::GetItemDropOffTransform() const
{
    // Return the scene component that defines the drop-off location/transform
    // Maps to member variable _itemDropOffPosition
    return this->_itemDropOffPosition;
}

// starts at line 4507
bool ADBDPlayer::HasClearPathToItemDropOffPosition() const
{
    // Retrieve the SceneComponent used for the item drop-off position.
    // Disassembly: Accesses the pointer at offset 0xE68.
    USceneComponent* DropOffComponent = this->_itemDropOffPosition;

    // Retrieve the world location (Translation) from the component's transform.
    // Disassembly: Accesses the ComponentToWorld struct (offset 0x140) to extract the translation vector.
    // In UE4, GetComponentLocation() returns the translation component of the ComponentToWorld transform.
    FVector DropOffLocation = DropOffComponent->GetComponentLocation();

    // Check if there is a clear path to the target location.
    return this->HasClearPathToTarget(DropOffLocation);
}

// starts at line 4512
void ADBDPlayer::DBD_ToggleForceAuthoritativeMovement()
{
    // Check the specific bit in the authoritative movement flags (Offset 0x10B0)
    // The disassembly reads the value, shifts right by 2, inverts it, and masks with 1.
    // This checks if the 3rd bit (value 4) is currently 0 (false).
    // If the bit is 0, we want to send true (enable). If it is 1, we send false (disable).
    bool bShouldEnable = (this->_authoritativeMovementFlags._flags & (1 << 2)) == 0;

    // Call the server RPC to set the new state
    this->Server_SetForceAuthoritativeMovement(bShouldEnable);
}

// starts at line 4517
void ADBDPlayer::Server_SetForceAuthoritativeMovement_Implementation(bool value)
{
    this->Multicast_SetForceAuthoritativeMovement(value);
}

// starts at line 4527
void ADBDPlayer::Multicast_SetForceAuthoritativeMovement_Implementation(bool value)
{
    // Set the authoritative movement flag specifically for the 'CHEAT' category.
    // 'CHEAT' is likely a constant or enum value used to identify the source of the flag.
    this->SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag::CHEAT, value);
}

// starts at line 4543
void ADBDPlayer::UpdateCharacterCollisions()
{
    // Retrieve the collision response from the virtual table at offset 0xE20.
    // This appears to be a custom virtual function designated as 'GetPawnCollisionChannel' in the pseudo code.
    ECollisionResponse NewResponse = this->GetPawnCollisionChannel();

    // Set the collision response for the standard Pawn channel (Index 2).
    // Accessing the CapsuleComponent inherited from ACharacter.
    this->CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);

    // Set the collision response for the custom trace channel (Index 16).
    // In Unreal Engine, index 16 typically corresponds to ECC_GameTraceChannel3.
    this->CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, NewResponse);
}

// starts at line 4550
void ADBDPlayer::Server_SetEthereal_Implementation(bool enabled)
{
    this->Multicast_SetEthereal(enabled);
}

// starts at line 4559
void ADBDPlayer::Multicast_SetEthereal_Implementation(bool enabled)
{
    // Call the internal function to apply the Ethereal state change.
    this->SetEthereal(enabled);
}

// starts at line 4568
void ADBDPlayer::SetEthereal(bool Enabled)
{
    // Determine the new collision response based on the 'Enabled' flag.
    // If Ethereal is enabled (True), we want to IGNORE collisions (Response = 1).
    // If Ethereal is disabled (False), we want to BLOCK collisions (Response = 2).
    // Logic from assembly: (Enabled ? ECR_Ignore : ECR_Block)
    ECollisionResponse NewResponse = Enabled ? ECR_Ignore : ECR_Block;

    // Access the CapsuleComponent (Offset 0x3F0)
    if (this->CapsuleComponent != nullptr)
    {
        // Update collision responses for specific channels to allow the player to pass through objects.
        
        // ECC_Pawn (Channel 2)
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);

        // ECC_GameTraceChannel3 (Channel 16) - Typically used for Interaction traces
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, NewResponse);

        // ECC_WorldStatic (Channel 0)
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, NewResponse);

        // ECC_WorldDynamic (Channel 1)
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, NewResponse);

        // ECC_GameTraceChannel1 (Channel 14)
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, NewResponse);

        // ECC_GameTraceChannel15 (Channel 28)
        this->CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel15, NewResponse);
    }
}

// starts at line 4578
void ADBDPlayer::Server_SetWindowFreeCam_Implementation(bool enabled)
{
    this->Multicast_SetWindowFreeCam(enabled);
}

// starts at line 4587
void ADBDPlayer::Multicast_SetWindowFreeCam_Implementation(bool enabled)
{
    // Update the member variable located at offset 0x10DD.
    this->_windowFreeCam = enabled;
}

// starts at line 4596
void ADBDPlayer::Server_SetReverseTraverseEnabled_Implementation(bool enabled)
{
    this->Multicast_SetReverseTraverseEnabled(enabled);
}

// starts at line 4605
void ADBDPlayer::Multicast_SetReverseTraverseEnabled_Implementation(bool enabled)
{
    // Update the member variable located at offset 0x10DE.
    this->_reverseTraverse = enabled;
}

// starts at line 4615
void ADBDPlayer::Server_SetRunVaultEnabled_Implementation(bool enabled)
{
    this->Multicast_SetRunVaultEnabled(enabled);
}

// starts at line 4624
void ADBDPlayer::Multicast_SetRunVaultEnabled_Implementation(bool enabled)
{
    // Update the member variable located at offset 0x10DF.
    this->_runVault = enabled;
}

// starts at line 4633
void ADBDPlayer::Server_SetAuthoritativeMovement_Implementation(bool enabled)
{
    this->Multicast_SetAuthoritativeMovement(enabled);
}

// starts at line 4642
void ADBDPlayer::Multicast_SetAuthoritativeMovement_Implementation(bool enabled)
{
    // Delegate the logic to the internal handler function.
    this->SetPawnAuthoritativeMovement(enabled);
}

// starts at line 4656
bool ADBDPlayer::LineTraceSingleForFootIK(const FVector& Start, const FVector& End, bool bTraceComplex, FHitResult& OutHit)
{
    // 1. Setup Collision Query Parameters
    // Initialize the query params struct
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = bTraceComplex;
    
    // Explicitly ignore the player character so the trace doesn't hit the player's own mesh/capsule.
    // Disassembly: FCollisionQueryParams::AddIgnoredActor(&Params, this);
    QueryParams.AddIgnoredActor(this);

    // Retrieve the Capsule Component
    // Disassembly: Accesses offset 0x3F0 (ACharacter::CapsuleComponent)
    UCapsuleComponent* Capsule = this->CapsuleComponent;

    if (Capsule != nullptr)
    {
        // Copy any actors that the capsule is currently ignoring into the trace's ignore list.
        // This ensures foot placement doesn't try to stand on actors the player is phasing through.
        // Disassembly: FCollisionQueryParams::AddIgnoredActors(&Params, &this->CapsuleComponent->MoveIgnoreActors);
        QueryParams.AddIgnoredActors(Capsule->MoveIgnoreActors);
    }

    // 2. Setup Collision Response and Channel
    ECollisionChannel TraceChannel = ECC_WorldStatic; // Default fallback
    FCollisionResponseContainer ResponseContainer;
    
    // Default to blocking everything initially
    ResponseContainer.SetAllChannels(ECR_Block);

    if (Capsule != nullptr)
    {
        // Use the Capsule's own Object Type as the channel for the trace.
        // This effectively asks: "If a [Pawn] moved along this line, what would it hit?"
        // Disassembly: TraceChannel = this->CapsuleComponent->GetCollisionObjectType(...);
        TraceChannel = Capsule->GetCollisionObjectType();

        // Copy the Capsule's specific response settings.
        // This ensures the trace ignores volume/channels that the player ignores (e.g., Visibility, Camera).
        // Disassembly: v13 = this->CapsuleComponent->GetCollisionResponseToChannels(...);
        ResponseContainer = Capsule->GetCollisionResponseToChannels();
    }

    // Wrap the container in the params struct expected by the World trace function
    FCollisionResponseParams ResponseParams(ResponseContainer);

    // 3. Perform the Trace
    UWorld* World = this->GetWorld();
    if (World != nullptr)
    {
        // Execute the single line trace
        // Disassembly: UWorld::LineTraceSingleByChannel(v16, outHit, start, end, TraceChannel, &Params, ...);
        return World->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, QueryParams, ResponseParams);
    }

    return false;
}

// starts at line 4676
void ADBDPlayer::SetEmulateGamepadForMouseYaw(bool Emulate)
{
    // Check if the player is locally controlled before applying input changes.
    // Calls the virtual function IsLocallyControlled() (Offset 0x630 in vtable).
    if (this->IsLocallyControlled() != false)
    {
        // Only perform updates if the value is actually changing.
        // Corresponds to the check: if (this->_emulateGamepadForMouseYaw != emulate)
        if (this->_emulateGamepadForMouseYaw != Emulate)
        {
            // Update the state flag (Offset 0xE70).
            this->_emulateGamepadForMouseYaw = Emulate;

            // Reset the emulated yaw input accumulator to 0.
            // Corresponds to: this->_emulatedYawInput = 0.0; (Offset 0x10B4)
            this->_emulatedYawInput = 0.0f;
        }
    }
}

// starts at line 4688
void ADBDPlayer::SetYawScale(float yawScale, float adjustTime)
{
    // Check if the player is locally controlled (Offset 0x630).
    if (this->IsLocallyControlled())
    {
        // Retrieve the specialized player controller.
        ADBDPlayerController* PlayerController = this->GetPlayerController();

        // Forward the yaw scale setting to the controller.
        // This affects mouse/stick sensitivity for horizontal rotation.
        if (PlayerController != nullptr)
        {
            PlayerController->SetYawScale(yawScale, adjustTime);
        }
    }
}

// starts at line 4695
void ADBDPlayer::ResetYawScale(float AdjustTime)
{
    // Check if the player is locally controlled. 
    // Only local players handle input scaling logic directly to ensure responsiveness.
    if (this->IsLocallyControlled() != false)
    {
        // Retrieve the specific player controller class for this player.
        // The disassembly calls a specific member function 'GetPlayerController'.
        ADBDPlayerController* PlayerController = this->GetPlayerController();

        // Reset the timer responsible for yaw input scaling.
        // Corresponds to FDBDTimer::Reset call in disassembly on the _yawInputScalingTimer member.
        PlayerController->_yawInputScalingTimer.Reset(AdjustTime);

        // Store the current scale values as 'previous' values for interpolation/transition logic.
        // This ensures smooth transitions when scaling changes.
        PlayerController->_previousConstantYawScale = PlayerController->_constantYawScale;
        PlayerController->_previousAbsoluteYawScale = PlayerController->_absoluteYawScale;

        // Set the flag indicating that the yaw scaling is currently being reset.
        PlayerController->_resettingYawScaling = true;

        // If the adjustment time is immediate (less than or equal to 0), force the update immediately.
        if (AdjustTime <= 0.0f)
        {
            // Update the axis scaling immediately.
            // Arguments: DeltaTime (0.0f), bForce (true).
            PlayerController->UpdateAxisScaling(0.0f, true);
        }
    }
}

// starts at line 4703
void ADBDPlayer::SetPitchScale(float pitchScale, float adjustTime)
{
    // Check if the player is locally controlled.
    // Calls virtual function IsLocallyControlled() at offset 0x630.
    if (this->IsLocallyControlled())
    {
        // Retrieve the player controller.
        ADBDPlayerController* PlayerController = this->GetPlayerController();

        // Forward the pitch scale adjustment to the controller.
        // Note: The assembly implies GetPlayerController returns a valid pointer (or the called function handles null),
        // but standard UE4 practice usually checks for nullptr. The disassembly jumps straight to the call.
        if (PlayerController != nullptr)
        {
            PlayerController->SetPitchScale(pitchScale, adjustTime);
        }
    }
}

// starts at line 4710
void ADBDPlayer::ResetPitchScale(float AdjustTime)
{
    // Check if the player is locally controlled. 
    // Only local players handle input scaling logic directly.
    if (this->IsLocallyControlled() != false)
    {
        // Retrieve the specific player controller class for this player.
        // The disassembly calls a specific member function 'GetPlayerController'.
        ADBDPlayerController* PlayerController = this->GetPlayerController();

        // Reset the timer responsible for pitch input scaling.
        // Corresponds to FDBDTimer::Reset call in disassembly.
        PlayerController->_pitchInputScalingTimer.Reset(AdjustTime);

        // Store the current scale values as 'previous' values for interpolation/transition logic.
        PlayerController->_previousConstantPitchScale = PlayerController->_constantPitchScale;
        PlayerController->_previousAbsolutePitchScale = PlayerController->_absolutePitchScale;

        // Set the flag indicating that the pitch scaling is in a resetting state.
        PlayerController->_resettingPitchScaling = true;

        // If the adjustment time is immediate (less than or equal to 0), force the update immediately.
        if (AdjustTime <= 0.0f)
        {
            // Update the axis scaling immediately.
            // Arguments: DeltaTime (0.0f), bForce (true).
            PlayerController->UpdateAxisScaling(0.0f, true);
        }
    }
}

// starts at line 4719
bool ADBDPlayer::IsAuthoritativeMoveable() const
{
    // Check if the 'forced authoritative unmoveable' flag is set.
    // Disassembly: Accesses the boolean at offset 0x10DC.
    if (this->_forcedAuthoritativeUnmoveable != false)
    {
        return false;
    }

    // Check if any flags are set in the authoritative movement bitfield.
    // Disassembly: Accesses the '_flags' member of the FBitfield struct at offset 0x10B0.
    if (this->_authoritativeMovementFlags._flags != 0)
    {
        return false;
    }

    return true;
}

// starts at line 4724
void ADBDPlayer::Client_TryInteractionType_Implementation(EInputInteractionType interactionType, ADBDPlayer* requester)
{
    // Forward the replication request to the internal interaction type handler.
    this->TryInteractionType(interactionType, requester);
}

// starts at line 4734
void ADBDPlayer::Local_RequestBlink(const FTransform& Destination)
{
    // Ensure this logic only runs on the client controlling this character.
    // Disassembly: call IsLocallyControlled
    if (this->IsLocallyControlled())
    {
        // 1. Queue the Blink Event
        // It retrieves a "PollableEvent" object stored at offset 0x858 and adds a 'true' event.
        // This is likely part of an input buffering system.
        // Disassembly: Accesses [rdi+858h], calls ResizeGrow on TArray inside it.
        if (this->BlinkEvent != nullptr)
        {
            this->BlinkEvent->Add(true);
        }

        // 2. Extract Location and Rotation from the Transform
        // The disassembly shows heavy use of vector shuffling (shufps, unpcklps) 
        // to decompose the FTransform into FVector and FRotator.
        
        // Convert Quaternion rotation to Euler (Rotator)
        // Disassembly: call FQuat::Rotator
        FRotator TargetRotation = Destination.GetRotation().Rotator();
        
        // Extract Translation
        FVector TargetLocation = Destination.GetTranslation();

        // 3. Broadcast the Destination
        // This likely sends an RPC to the server or notifies local gameplay systems.
        // Disassembly: call ?Broadcast_BlinkDestination@ADBDPlayer@@IEAAXUFVector@@UFRotator@@@Z
        this->Broadcast_BlinkDestination(TargetLocation, TargetRotation);

        // 4. Update Local State Members
        // Store the calculated position and rotation in member variables (offsets 0x1108 and 0x1114)
        // This is likely used for visual interpolation (the "blink" travel visual) on the client 
        // while waiting for server confirmation.
        this->_blinkDestinationPosition = TargetLocation;
        this->_blinkDestinationRotation = TargetRotation;
    }
}

// starts at line 4745
void ADBDPlayer::Broadcast_BlinkDestination_Implementation(FVector position, FRotator rotation)
{
    // Check if the executing player possesses network authority.
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ROLE_Authority)
    {
        // Broadcast the blink destination and rotation to all connected clients.
        // Calling the base Multicast function triggers the Unreal Header Tool (UHT) 
        // auto-generated thunk to handle the network replication.
        this->Broadcast_Multicast_BlinkDestination(position, rotation); /* UNREAL AUTO GENERATED FUNCTION */
    }
}

// starts at line 4758
void ADBDPlayer::Broadcast_Multicast_BlinkDestination_Implementation(FVector position, FRotator rotation)
{
    // Check if the current client/instance is NOT the local controller for this player character.
    // The disassembly shows a virtual function call at offset [rax+630h], which corresponds 
    // to IsLocallyControlled() on APawn in Unreal Engine.
    if (this->IsLocallyControlled() == false)
    {
        // Copy the replicated position and rotation vectors into the local member variables.
        // The compiler unrolls these assignments into distinct register transfers, 
        // but high-level C++ elegantly maps this to direct struct assignment.
        this->_blinkDestinationPosition = position;
        this->_blinkDestinationRotation = rotation;
    }
}

// starts at line 4777
void ADBDPlayer::SetReverseMontage(bool reverse)
{
    // Retrieve the currently active animation montage.
    UAnimMontage* CurrentMontage = this->GetCurrentMontage();

    // Check if a montage is currently playing and is valid (standard object check).
    if (CurrentMontage != nullptr && IsValid(CurrentMontage))
    {
        // Get the current playback rate of the montage.
        float CurrentRate = CurrentMontage->RateScale;

        // Logic to determine if we need to flip the sign of the playback rate.
        bool bShouldFlip = false;

        if (reverse)
        {
            // If requesting reverse (negative rate), and current rate is positive (>= 0), flip it.
            if (CurrentRate >= 0.0f)
            {
                bShouldFlip = true;
            }
        }
        else
        {
            // If requesting forward (positive rate), and current rate is negative (<= 0), flip it.
            if (CurrentRate <= 0.0f)
            {
                bShouldFlip = true;
            }
        }

        // Apply the change if necessary.
        if (bShouldFlip)
        {
            // Invert the sign of the playback rate.
            // The assembly uses XOR with 0x80000000 (sign bit flip).
            CurrentMontage->RateScale = -CurrentRate;
        }
    }
}

// starts at line 4792
FInteractionPlayerProperties* ADBDPlayer::GetInteractionPlayerProperties() const
{
    // Access the InteractionHandler component
    if (this->_interactionHandler != nullptr)
    {
        // Return the address of the player properties struct stored within the handler
        // Maps to member variable _currentInteractionPlayerProperties in UPlayerInteractionHandler
        return &this->_interactionHandler->_currentInteractionPlayerProperties;
    }

    return nullptr;
}

// starts at line 4797
void ADBDPlayer::Server_SetDebugFailAllInteractions_Implementation(bool enabled)
{
    this->_debugFailAllInteractions = enabled;
}

// starts at line 4806
bool ADBDPlayer::IsInTerrorRadius() const
{
    // Retrieve the World
    UWorld* World = this->GetWorld();
    if (World == nullptr)
    {
        return false;
    }

    // Retrieve the GameState
    AGameState* GameState = World->GameState;
    if (GameState == nullptr)
    {
        return false;
    }

    // Cast to the custom ADBDGameState
    // Disassembly: Checks ClassTreeIndex against ADBDGameState::StaticClass()
    ADBDGameState* DBDGameState = Cast<ADBDGameState>(GameState);

    // Verify Cast, PendingKill check, and ActorBeingDestroyed check
    if (DBDGameState == nullptr || DBDGameState->IsPendingKill() || DBDGameState->IsActorBeingDestroyed())
    {
        return false;
    }

    // Retrieve the Terror Radius Source (The Killer) from the GameState
    // Disassembly: Accesses member at offset 0x5F0 in ADBDGameState
    ASlasherPlayer* Killer = DBDGameState->Slasher;

    // Validate the Killer actor
    if (Killer == nullptr)
    {
        return false;
    }

    // Check if Killer is valid (IsPendingKill, IsActorBeingDestroyed)
    if (Killer->IsPendingKill() || Killer->IsActorBeingDestroyed())
    {
        return false;
    }

    // Perform a custom validity check on the Killer (Virtual function call)
    // Disassembly: call qword ptr [rdx+1070h]
    if (Killer->IsValidImpl() == false)
    {
        return false;
    }

    // Retrieve the current Terror Radius value from the Killer
    // Disassembly: Accesses float at offset 0x1488 in the Killer object
    float TerrorRadius = Killer->_terrorRadius;

    // Calculate the distance between this player and the Killer
    // Disassembly: Calls AActor::GetDistanceTo
    float DistanceToKiller = this->GetDistanceTo(Killer);

    // Return true if the distance is less than the radius (Player is inside)
    return TerrorRadius > DistanceToKiller;
}

// starts at line 4816
bool ADBDPlayer::K2_SetActorLocationByBottomCapsule(FVector NewLocation, bool bSweep, FHitResult& SweepHitResult)
{
    // Determine the pointer to pass for the HitResult.
    // If bSweep is true, pass the address of the output struct.
    // If bSweep is false, pass nullptr (0).
    // Disassembly: xor r9d, r9d; test r8b, r8b; cmovnz r9, rax
    FHitResult* HitResultPtr = bSweep ? &SweepHitResult : nullptr;

    // Call the native implementation
    // Disassembly: jmp ?SetActorLocationByCapsuleBottom@ADBDPlayer@@QEAA_NAEBUFVector@@_NPEAUFHitResult@@@Z
    return this->SetActorLocationByCapsuleBottom(NewLocation, bSweep, HitResultPtr);
}

// starts at line 4821
bool ADBDPlayer::SetActorLocationByCapsuleBottom(const FVector& NewLocation, bool bSweep, FHitResult* OutSweepHitResult)
{
    // Access the CapsuleComponent (Offset 0x3F0)
    UCapsuleComponent* Capsule = this->CapsuleComponent;

    // Retrieve the component's 3D scale from its world transform (Offset 0x150).
    FVector Scale3D = Capsule->GetComponentTransform().GetScale3D();

    // The disassembly applies a sign mask (ABS) and finds the minimum of the X, Y, and Z components.
    // minps logic corresponds to finding the smallest scalar among the three axes.
    float MinAbsScale = FMath::Min3(FMath::Abs(Scale3D.X), FMath::Abs(Scale3D.Y), FMath::Abs(Scale3D.Z));

    // Calculate the effective scaled half-height using this minimum scale.
    // Offset 0x630 is UnscaledHalfHeight.
    float ScaledOffset = Capsule->GetUnscaledCapsuleHalfHeight() * MinAbsScale;

    // The input 'NewLocation' represents the bottom of the capsule.
    // To find the center (Actor Location), we move up by the scaled half-height.
    // NewLocation + (FVector::UpVector * ScaledOffset)
    FVector NewActorLocation = NewLocation + (FVector::UpVector * ScaledOffset);

    // Call SetActorLocation with the calculated center position.
    // Passes ETeleportType::None (last argument in pseudocode is None/0).
    return this->SetActorLocation(NewActorLocation, bSweep, OutSweepHitResult, ETeleportType::None);
}

// starts at line 4826
FVector ADBDPlayer::GetActorLocationFromFeetLocation(const FVector& FeetLocation) const
{
    // Access the Character's CapsuleComponent (Offset 0x3F0)
    UCapsuleComponent* Capsule = this->CapsuleComponent;

    // Retrieve the 3D scale from the component's world transform
    // The disassembly reads offset 0x150, which points to the Scale3D vector within the ComponentToWorld FTransform
    FVector Scale3D = Capsule->GetComponentScale();

    // Calculate the absolute values of the scale components (SignMask in ASM)
    float AbsScaleX = FMath::Abs(Scale3D.X);
    float AbsScaleY = FMath::Abs(Scale3D.Y);
    float AbsScaleZ = FMath::Abs(Scale3D.Z);

    // Find the minimum absolute scale factor across all axes
    // The disassembly uses minps instructions to compare all components
    float MinScale = FMath::Min(AbsScaleX, FMath::Min(AbsScaleY, AbsScaleZ));

    // Calculate the scaled half-height of the capsule
    // Offset 0x630 corresponds to CapsuleHalfHeight in this version of the engine
    float ScaledHalfHeight = MinScale * Capsule->CapsuleHalfHeight;

    // Calculate the center actor location
    // We start at the FeetLocation and move up along the UpVector (0,0,1) by the scaled half-height
    FVector Result = FeetLocation + (FVector::UpVector * ScaledHalfHeight);

    return Result;
}

// starts at line 4832
UDBDGameInstance* ADBDPlayer::GetDBDGameInstance() const
{
    // Perform a safe cast to the specific UDBDGameInstance class
    // The manual class tree index comparison in the pseudo-code is the compiled implementation 
    // of the Unreal Engine Cast<T> template function.
    return Cast<UDBDGameInstance>(this->GetGameInstance());
}

// starts at line 4853
void ADBDPlayer::BuildInteractionKeys()
{
    // The loop runs from 0 to 31, which corresponds to the number of elements 
    // in the EInputInteractionType enum.
    for (int32 interactionTypeIndex = 0; interactionTypeIndex < 31; ++interactionTypeIndex)
    {
        FName finalKeyName;
        EInputInteractionType interactionType = static_cast<EInputInteractionType>(interactionTypeIndex);

        // Attempt to retrieve the custom GameInstance. 
        // The disassembly checks ClassTreeIndex against UDBDGameInstance, indicating a safe Cast.
        UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        // Perform validity checks on the GameInstance, PlayerController, and PlayerState.
        // The deep GUObjectArray checks map directly to Unreal Engine's standard IsValid() macro.
        if (IsValid(gameInstance) == true)
        {
            ADBDPlayerController* playerController = this->GetPlayerController();
            
            if (IsValid(playerController) == true)
            {
                ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);

                if (IsValid(playerState) == true)
                {
                    // Retrieve the mapped action name from the KeyDisplayInfo subsystem.
                    UDBDKeyDisplayInfo* keyDisplayInfo = gameInstance->KeyDisplayInfo;
                    
                    if (keyDisplayInfo != nullptr)
                    {
                        FName mappedActionName = keyDisplayInfo->GetActionForInteractionType(
                            playerState->GameRole,
                            interactionType,
                            this->_isUsingGamepad,
                            false // ignoreInteractMash
                        );

                        // Retrieve the specific hardware key mapped to this action name
                        FKey inputKey = keyDisplayInfo->GetFirstKeyForAction(
                            playerController->PlayerInput,
                            mappedActionName,
                            this->_isUsingGamepad
                        );

                        // If a valid key mapping was found, extract its FName representation
                        if (inputKey.GetFName().IsNone() == false)
                        {
                            finalKeyName = inputKey.GetFName();
                        }
                    }
                }
            }
        }

        // If all validations failed or no specific hardware key was found, fall back to 
        // mapping the key name directly to the Enum string representation.
        // The disassembly calls Enum::ToString, safely extracts the TCHAR* string, 
        // and creates a new FName from it.
        if (finalKeyName.IsNone() == true)
        {
            FString enumString = Enum::ToString(interactionType);
            
            // Construct the FName using standard Unreal string dereferencing
            if (enumString.IsEmpty() == false)
            {
                finalKeyName = FName(*enumString);
            }
            else
            {
                // The disassembly references an empty string literal placeholder ('in_pszStateGroup')
                finalKeyName = FName(TEXT(""));
            }
        }

        // Store the resolved FName into the class array at the current enum index.
        // The disassembly directly accesses the FName block at offset 0x1198.
        this->InteractionKeys[interactionTypeIndex] = finalKeyName;
    }
}

// starts at line 4865
float ADBDPlayer::GetBlinkTime() const
{
    // Default return value in case of failure (0.01f)
    float Result = 0.01f;

    // Retrieve the GameInstance via the World
    UWorld* World = this->GetWorld();
    if (World != nullptr)
    {
        // Cast to the specific game instance class (UDBDGameInstance)
        // The disassembly performs a GetPrivateStaticClass check here
        UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(World->GetGameInstance());

        if (GameInstance != nullptr)
        {
            // Retrieve the Design Tunables (likely stored in or accessible via GameInstance)
            // The disassembly accesses a pointer offset from the GameInstance
            UDBDDesignTunables* Tunables = GameInstance->DesignTunables;

            if (Tunables != nullptr)
            {
                // 1. Get the Base Reappearance Time
                // Key: SLASHER_BLINK_REAPPEARANCE_TIME
                float BaseTime = Tunables->GetTunableValue(TunableValues::SLASHER_BLINK_REAPPEARANCE_TIME);

                // 2. Calculate Perk Modifier
                // Get the PerkManager via the interface
                float PerkModifier = 0.0f;
                UPerkManager* PerkMgr = this->GetPerkManager();
                if (PerkMgr != nullptr)
                {
                    PerkModifier = PerkMgr->GetPerkModifier(EGameplayModifierType::VE_ModifyNurseReappearanceTimeMultiplicative, EGameplayModifierSource::VE_All);
                }

                // Apply the modifier: Time * (1.0 + Modifier)
                float ModifiedTime = BaseTime * (1.0f + PerkModifier);

                // 3. Calculate Distance Scaler
                // Formula: (1.0 / BaseRange) * CurrentBlinkDistance
                // Key: SLASHER_BLINK_BASE_RANGE
                float BaseRange = Tunables->GetTunableValue(TunableValues::SLASHER_BLINK_BASE_RANGE);
                
                // Avoid division by zero if necessary, though ASM simply divides
                float InverseRange = 1.0f / BaseRange;
                
                // Offset 0xB50 corresponds to CurrentBlinkDistance
                float DistanceScaler = InverseRange * this->CurrentBlinkDistance;

                // Combine to get calculated time
                float CalculatedTime = ModifiedTime * DistanceScaler;

                // 4. Clamp to Minimum Time
                // Key: SLASHER_BLINK_REAPPEARANCE_TIME_MINIMUM
                float MinTime = Tunables->GetTunableValue(TunableValues::SLASHER_BLINK_REAPPEARANCE_TIME_MINIMUM);

                // Return the maximum of CalculatedTime and MinTime
                if (CalculatedTime >= MinTime)
                {
                    Result = CalculatedTime;
                }
                else
                {
                    Result = MinTime;
                }
            }
        }
    }

    return Result;
}

// starts at line 4879
void ADBDPlayer::DBDLog(FString logText)
{
    // The disassembly checks if the "Interaction" log category verbosity is >= 5.
    // In Unreal Engine's ELogVerbosity enum, level 5 corresponds to Log.
    // The string extraction (using *logText) and the call to FMsg::Logf_Internal__VA 
    // are standard expansions of the UE_LOG macro.
    UE_LOG(LogInteraction, Log, TEXT("%s"), *logText);
}

// starts at line 4884
UActorComponent* ADBDPlayer::GetFirstComponentByClass(TSubclassOf<UActorComponent> ComponentClass) const
{
    // Delegates directly to the AActor implementation to find the first component of the given class
    // This maps to the standard UE4 function AActor::GetComponentByClass
    return this->GetComponentByClass(ComponentClass);
}

// starts at line 4889
bool ADBDPlayer::HasDamageImmunity() const
{
    // Access the interface IPerkManagerOwnerInterface. 
    // ADBDPlayer inherits from this interface at offset 0x7A8.
    const IPerkManagerOwnerInterface* PerkOwnerInterface = static_cast<const IPerkManagerOwnerInterface*>(this);

    // Call GetPerkManager via the interface (Virtual Function Index 2).
    UPerkManager* PerkManager = PerkOwnerInterface->GetPerkManager();

    // Check if the PerkManager instance exists.
    if (PerkManager != nullptr)
    {
        // Create a local array to hold the retrieved gameplay modifiers.
        TArray<UGameplayModifierContainer*> CombinedEffects;

        // Retrieve all effects from the PerkManager.
        // Argument 4 corresponds to a specific EGameplayModifierSource (likely StatusEffects or All).
        PerkManager->GetAllEffects(CombinedEffects, EGameplayModifierSource::VE_All);

        // Iterate through the collected effects.
        for (int32_t i = 0; i < CombinedEffects.Num(); i++)
        {
            UGameplayModifierContainer* Effect = CombinedEffects[i];

            // Ensure the effect object is valid.
            if (Effect != nullptr)
            {
                // Verify the object is not marked for garbage collection (Pending Kill).
                // Disassembly: Checks GUObjectArray flags (0x20000000).
                if (Effect->IsPendingKill() == false)
                {
                    // Check if the effect has the specific flag for Damage/Trap immunity.
                    // 0x48 (72) corresponds to VE_ImmuneToDamageAndTraps in EGameplayModifierFlag.
                    if (Effect->HasFlag(EGameplayModifierFlag::VE_ImmuneToDamageAndTraps) == true)
                    {
                        // Check if the effect is currently applicable.
                        // Disassembly: Accesses boolean at offset 0x158 (m_bIsApplicable).
                        if (Effect->m_bIsApplicable != false)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

// starts at line 4894
void ADBDPlayer::TriggerTrapImmunity(float duration)
{
    // Access the trap immunity timer member variable (Offset 0x1138).
    // Compare the requested duration against the time remaining on the current timer.
    // The logic ensures we don't shorten an existing immunity duration, only extend it.
    if (duration > UDBDUtilTimer::GetTimeLeft(this->_trapImmunityTimer))
    {
        // Reset the timer to the new, longer duration.
        this->_trapImmunityTimer.Reset(duration);
    }
}

// starts at line 4902
float ADBDPlayer::GetRemainingTrapImmunityDuration() const
{
    // Retrieve the remaining time from the trap immunity timer.
    // The cast to 'physx::PxVehicleWheelData' in the decompiled code is a type inference error;
    // the underlying type is likely a custom struct like FDBDTimer.
    return UDBDUtilTimer::GetTimeLeft(this->_trapImmunityTimer);
}

// starts at line 4907
bool ADBDPlayer::HasTrapImmunity() const
{
    // Check if the trap immunity timer is currently active.
    // IsDone() returns false if the timer is still running.
    // Disassembly: Accesses _trapImmunityTimer at offset 0x1138.
    if (this->_trapImmunityTimer.IsDone() == false)
    {
        return true;
    }

    // If the timer has expired, check if the player has general damage immunity.
    // Disassembly calls the member function HasDamageImmunity.
    if (this->HasDamageImmunity() != false)
    {
        return true;
    }

    return false;
}

// starts at line 4913
void ADBDPlayer::MoveIgnoreActorAdd(AActor* ActorToIgnore)
{
    // The disassembly explicitly iterates the array at offset 0xED8 first.
    // This is likely 'TArray<AActor*> _ignoreActors'.
    
    // Check if the actor is NOT already in our local list.
    // Disassembly: Loops through [rcx+0ED8h], if found (jz loc_140389CB3), return immediately.
    if (!this->IgnoreActors.Contains(ActorToIgnore))
    {
        // 1. Add to the local custom list
        // Disassembly: call TArray::AddUniqueImpl
        this->IgnoreActors.AddUnique(ActorToIgnore);

        // 2. Call the base class implementation to update actual physics/collision settings.
        // Disassembly: call ?MoveIgnoreActorAdd@APawn@@UEAAXPEAVAActor@@@Z
        Super::MoveIgnoreActorAdd(ActorToIgnore);
    }
    
    // If it was already in the local list, we skip calling Super to avoid redundant processing.
}

// starts at line 4923
void ADBDPlayer::MoveIgnoreActorRemove(AActor* ActorToIgnore)
{
    // 1. Check if the actor is in our local 'IgnoreActors' list (Offset 0xED8).
    // The disassembly explicitly iterates the array first. If the actor isn't found,
    // the function returns immediately (does nothing).
    if (this->IgnoreActors.Contains(ActorToIgnore))
    {
        // 2. Remove the actor from the local list.
        // Disassembly: Calls TArray::RemoveAll (via lambda predicate)
        this->IgnoreActors.Remove(ActorToIgnore); 

        // 3. Check with the Authoritative Movement Component.
        // Even if we locally want to stop ignoring the actor, the authoritative component 
        // (likely handling server-validated movement logic) might still need it ignored.
        // Disassembly: Accesses offset 0x10A8 (_authoritativeMovementComponent)
        bool bAuthStillIgnoring = false;
        
        if (this->_authoritativeMovementComponent != nullptr)
        {
            // Disassembly: call IsIgnoringActor
            bAuthStillIgnoring = this->_authoritativeMovementComponent->IsIgnoringActor(ActorToIgnore);
        }

        // 4. Only call the base class implementation if the Auth component is NOT ignoring it.
        // Disassembly: test al, al; jnz ... (Skip Super call if IsIgnoringActor returns true)
        if (!bAuthStillIgnoring)
        {
            // Disassembly: call ?MoveIgnoreActorRemove@APawn@@UEAAXPEAVAActor@@@Z
            Super::MoveIgnoreActorRemove(ActorToIgnore);
        }
    }
}

// starts at line 4935
void ADBDPlayer::AuthoritativeMovementIgnoreActorAdd(AActor* ActorToIgnore)
{
  this->MoveIgnoreActorAdd(ActorToIgnore);
}

// starts at line 4940
void ADBDPlayer::AuthoritativeMovementIgnoreActorRemove(AActor* ActorToIgnore)
{
    // Iterate through the internal IgnoreActors array to see if the actor is present.
    // The explicit loop in the disassembly translates perfectly to TArray::Contains().
    // We explicitly check for 'false' to adhere to constraints avoiding the '!' operator.
    if (this->IgnoreActors.Contains(ActorToIgnore) == false)
    {
        // If the actor is not in our custom ignore list, fall back and remove it 
        // using the standard base class movement ignore functionality.
        this->MoveIgnoreActorRemove(ActorToIgnore);
    }
}

// starts at line 4966
void ADBDPlayer::ResetMeshRelativePosition()
{
    // Check if the Mesh component is valid before accessing it
    if (this->Mesh != nullptr)
    {
        // Retrieve the base translation offset stored in the character
        // This will be the new relative location for the mesh
        FVector TargetLocation = this->BaseTranslationOffset;

        // Calculate the quaternion from the current relative rotation.
        // The disassembly explicitly checks and updates the internal RelativeRotationCache here.
        // It normalizes the rotation and ensures the cached quaternion is up to date before using it.
        // We replicate the result by converting the RelativeRotation to a Quaternion directly.
        FQuat TargetRotation = this->Mesh->RelativeRotation.Quaternion();

        // Call SetRelativeLocationAndRotation on the mesh.
        // Sets the location to the BaseTranslationOffset while maintaining the current rotation (via the calculated quaternion).
        // Arguments: NewLocation, NewRotation, bSweep, OutSweepHitResult, TeleportType
        this->Mesh->SetRelativeLocationAndRotation(TargetLocation, TargetRotation, false, nullptr, ETeleportType::None);
    }
}

// starts at line 4971
void ADBDPlayer::ResetMeshRelativeRotation()
{
    // Retrieve the Class Default Object (CDO) to access the default properties for this class.
    // The disassembly explicitly checks if the CDO exists and creates it if necessary (GetDefaultObject handles this logic).
    ADBDPlayer* DefaultObject = this->GetClass()->GetDefaultObject<ADBDPlayer>();

    if (DefaultObject != nullptr)
    {
        // Access the Mesh component from the default object (Offset 0x3E0).
        USkeletalMeshComponent* DefaultMesh = DefaultObject->Mesh;

        // Check validity of both the current mesh and the default mesh before accessing them
        if (this->Mesh != nullptr && DefaultMesh != nullptr)
        {
            // Read the default relative rotation from the CDO's mesh.
            // Offsets 0x18C and 0x194 correspond to the RelativeRotation struct members (Pitch, Yaw, Roll).
            FRotator DefaultRotation = DefaultMesh->RelativeRotation;

            // Apply the default rotation to the current mesh.
            // Arguments: NewRotation, bSweep (false), OutSweepHitResult (nullptr), TeleportType (None)
            this->Mesh->SetRelativeRotation(DefaultRotation, false, nullptr, ETeleportType::None);
        }
    }
}

// starts at line 4977
void ADBDPlayer::Server_SetDebugSnapPoint_Implementation(bool enabled)
{
    this->Multicast_SetDebugSnapPoint(enabled);
}

// starts at line 5020
void ADBDPlayer::Server_SetDebugCarry_Implementation(bool enabled)
{
    this->Multicast_SetDebugCarry(enabled);
}

// starts at line 5040
EPlayerRole ADBDPlayer::GetPlayerRole() const
{
    // 1. Get the Player State and Cast
    // Accesses PlayerState at offset 0x3A0.
    // Uses SafeCast (equivalent to Cast<>) to ensure it is an ADBDPlayerState.
    ADBDPlayerState* State = Cast<ADBDPlayerState>(this->GetPlayerState());

    // 2. Validate and Return Role
    if (State)
    {
        // Return the GameRole member.
        // Based on the assembly [rax+750h], this member is located at offset 0x750 in ADBDPlayerState.
        return State->GameRole;
    }

    // 3. Fallback
    // Returns 0 (EPlayerRole::VE_None) if the state is missing or invalid.
    return EPlayerRole::VE_None;
}

// starts at line 5049
UCharacterSnappingComponent* ADBDPlayer::GetCharacterSnappingComponent()
{
    // Return the pointer to the CharacterSnappingComponent member variable
    return this->_characterSnappingComponent;
}

// starts at line 5054
void ADBDPlayer::SnapCharacter(bool snapPosition, FVector position, float stopSnapDistance, bool snapRotation, FRotator rotation, float time, bool useZCoord, bool sweepOnFinalSnap)
{
    // Retrieve the CharacterSnappingComponent (Offset 0xE58).
    UCharacterSnappingComponent* SnappingComponent = this->_characterSnappingComponent;

    // Validate that the component exists and is not pending destruction (IsValid check).
    if (SnappingComponent != nullptr && IsValid(SnappingComponent))
    {
        // Forward the snapping request to the component.
        SnappingComponent->SnapCharacter(
            snapPosition, 
            position, 
            stopSnapDistance, 
            snapRotation, 
            rotation, 
            time, 
            useZCoord, 
            sweepOnFinalSnap
        );
    }
}

// starts at line 5062
void ADBDPlayer::StopSnap(bool snap)
{
    // Retrieve the CharacterSnappingComponent (Offset 0xE58).
    UCharacterSnappingComponent* SnappingComponent = this->_characterSnappingComponent;

    // Validate that the component exists and is not pending destruction (IsValid check).
    if (SnappingComponent != nullptr && IsValid(SnappingComponent))
    {
        // Call the component's Stop function with the provided snap parameter.
        // 'snap' likely determines if the character should immediately snap to the target position
        // or just cancel the snapping process.
        SnappingComponent->Stop(snap);
    }
}

// starts at line 5070
void ADBDPlayer::ServerResetMeshRelativeOffSet_Implementation()
{
    this->Multicast_ServerResetMeshRelativeOffSet();
}

// starts at line 5080
void ADBDPlayer::Multicast_ServerResetMeshRelativeOffSet_Implementation()
{
    this->ResetMeshRelativePosition();
}

// starts at line 5089
bool ADBDPlayer::IsAnyEditorWorld() const
{
    // Retrieve the World pointer
    UWorld* World = this->GetWorld();

    // Check if the World pointer is valid
    if (World != nullptr)
    {
        // Verify the World object is not pending garbage collection
        // Disassembly: Checks GUObjectArray flags (0x20000000 check)
        if (World->IsPendingKill() == false)
        {
            // Check the WorldType property (offset 0x840 in UE4.13 UWorld)
            // 3 = EWorldType::PIE (Play In Editor)
            // 2 = EWorldType::Editor
            // 4 = EWorldType::Preview
            
            if (World->WorldType == EWorldType::PIE || 
                World->WorldType == EWorldType::Editor || 
                World->WorldType == EWorldType::Preview)
            {
                return true;
            }
        }
    }

    return false;
}

// starts at line 5101
bool ADBDPlayer::IsPreviewWorld() const
{
    // Retrieve the World pointer
    UWorld* World = this->GetWorld();

    // Check if the World pointer is valid
    if (World != nullptr)
    {
        // Verify the World object is not pending garbage collection
        // Disassembly: Checks GUObjectArray flags (0x20000000 check)
        if (World->IsPendingKill() == false)
        {
            // Check the WorldType property
            // 2 = EWorldType::Editor
            // 4 = EWorldType::Preview
            // Note: Explicitly excludes EWorldType::PIE (3)
            
            if (World->WorldType == EWorldType::Editor || 
                World->WorldType == EWorldType::Preview)
            {
                return true;
            }
        }
    }

    return false;
}

// starts at line 5112
ECollisionChannel ADBDPlayer::GetPawnCollisionChannel() const
{
    // 1. Check Authoritative Movement Flags
    // Check if any flags are set in the _authoritativeMovementFlags struct.
    // Also check the console variable "EnablePawnCollision".

    // Logic: If flags are set OR Pawn Collision CVar is disabled (0)
    static IConsoleVariable* CVarEnablePawnCollision = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.EnablePawnCollision"));
    if (this->_authoritativeMovementFlags.PackedFlags != 0 || CVarEnablePawnCollision->GetInt() == 0)
    {
        // Return Channel 1 (ECC_WorldDynamic)
        // This effectively turns off specific "Pawn" blocking behavior, treating the actor as a generic dynamic object.
        return ECC_WorldDynamic;
    }

    // 2. Default Behavior
    // Return Channel 2 (ECC_Pawn)
    // This is the standard channel for player characters.
    return ECC_Pawn;
}

// starts at line 5117
void ADBDPlayer::Multicast_SetInteractingPlayer_Implementation(ADBDPlayer* interactingPlayer)
{
    // Update the member variable at offset 0xE18 with the pointer to the interacting player.
    this->_interactingPlayer = interactingPlayer;
}

// starts at line 5122
void ADBDPlayer::SetFirstPersonModelEnabled(bool Enabled, bool Force)
{
    // Check if the mode is actually changing, or if the update is being forced.
    // Logic: If the state is already equal to 'Enabled' AND 'Force' is false, return early.
    if (this->_isInFirstPerson != Enabled || Force)
    {
        // Update the First Person state flag (Offset 0x1165).
        this->_isInFirstPerson = Enabled;

        // Broadcast the OnFirstPersonModeChanged delegate (Offset 0x910).
        // This notifies blueprints or other systems that the camera perspective has changed.
        this->OnFirstPersonModeChanged.Broadcast(Enabled);

        // Handle Head Visibility.
        // In True First Person, the head is often hidden to prevent it from clipping into the camera.
        // Check if the head hidden state needs to be updated (Offset 0x1164).
        if (this->_isHeadHidden != Enabled)
        {
            this->_isHeadHidden = Enabled;
            
            // Call the function responsible for actually hiding/unhiding the head bones/mesh.
            this->OnHeadHidden(Enabled);
        }

        // Update VFX Visibility based on the camera mode.
        // Retrieves the CustomizedSkeletalMesh component.
        UCustomizedSkeletalMesh* CustomizedMesh = this->GetCustomizedSkeletalMesh();

        if (CustomizedMesh != nullptr)
        {
            // Iterate through the EffectCameraTypeSettings array.
            // Based on the assembly loop, the structure size is 16 bytes (0x10).
            for (const FEffectCameraTypeSettings& Setting : this->EffectCameraTypeSettings)
            {
                bool bIsVisible = false;

                if (Enabled)
                {
                    // If in First Person, use the 'VisibleInFirstPerson' flag (Offset 0x8 in struct)
                    bIsVisible = Setting.bVisibleInFirstPerson;
                }
                else
                {
                    // If in Third Person, use the 'VisibleInThirdPerson' flag (Offset 0x9 in struct)
                    bIsVisible = Setting.bVisibleInThirdPerson;
                }

                // Apply the visibility setting to the specific VFX category.
                CustomizedMesh->SetVfxsVisibility(Setting.Category, bIsVisible);
            }
        }
    }
}

// starts at line 5158
bool ADBDPlayer::IsRootMotionAllowed() const
{
    // Check the Console Variable 'CVarEnableRootMotionDuringSnap'.
    // If this CVar is set (value != 0), we bypass the snapping check and defer to the base class.
    // Disassembly: mov rax, cs:CVarEnableRootMotionDuringSnap.Ref; cmp dword ptr [rax], 0
    static IConsoleVariable* CVarEnableRootMotionDuringSnap = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.EnableRootMotionDuringSnap"));
    if (CVarEnableRootMotionDuringSnap->GetInt() == 0)
    {
        // Retrieve the Character Snapping Component
        // Disassembly: Accesses member at offset 0xE58
        UCharacterSnappingComponent* SnappingComponent = this->_characterSnappingComponent;

        // Check if the component exists and is valid (not pending kill)
        if (SnappingComponent != nullptr && SnappingComponent->IsPendingKill() == false)
        {
            // Check if the component is currently in a snapping state
            // Disassembly: Calls UCharacterSnappingComponent::IsSnapping()
            if (SnappingComponent->IsSnapping() == true)
            {
                // If we are snapping (and the override CVar is 0), Root Motion is NOT allowed.
                return false;
            }
        }
    }

    // If we aren't snapping (or the CVar enabled it), defer to the base character implementation.
    // Disassembly: Tail call optimization (jmp) to parent function. 
    // The raw disassembly shows a strange delegate call, but functionally this is Super::IsRootMotionAllowed().
    return Super::IsRootMotionAllowed();
}

// starts at line 5170
void ADBDPlayer::HideHead(bool hidden)
{
    // Check if the current hidden state differs from the requested state.
    // Disassembly: Compares byte at offset 0x1164 with the input boolean.
    if (this->_isHeadHidden != hidden)
    {
        // Update the internal state flag.
        this->_isHeadHidden = hidden;

        // Trigger the callback to handle the actual visibility change logic.
        // Disassembly: Jumps to ADBDPlayer::OnHeadHidden.
        this->OnHeadHidden(hidden);
    }
}

// starts at line 5179
bool ADBDPlayer::IsHeadHidden() const
{
    // Return the boolean value indicating if the head is hidden
    // Disassembly: movzx eax, byte ptr [rcx+1164h] (offset based on HideHead usage)
    return this->_isHeadHidden;
}

// starts at line 5184
void ADBDPlayer::FellOutOfWorld(const UDamageType& dmgType)
{
    // Check if the player is in the "Paradise" state (e.g., sacrifice sequence or end game area)
    // Offset 0x1081 corresponds to _inParadise
    if (this->_inParadise == true)
    {
        return;
    }

    // Log the event using the LogActor category
    UE_LOG(LogActor, Warning, TEXT("ADBDPlayer: Actor falling out of world."));

    // Prepare and record an analytics event
    // The disassembly constructs the string "FellOutOfWorld" and an empty attribute array
    TArray<FAnalyticsEventAttribute> Attributes;
    UBHVRAnalytics::RecordEvent(TEXT("FellOutOfWorld"), Attributes); 

    FVector TargetLocation;

    // Retrieve the current location of the root component for distance calculations
    FVector CurrentLocation = FVector::ZeroVector;
    if (this->GetRootComponent() != nullptr)
    {
        CurrentLocation = this->GetRootComponent()->GetComponentLocation();
    }

    // Determine teleport strategy based on fall frequency and distance
    // Condition 1: Have we fallen recently? (secondsSinceLastFell < 2.0f)
    // Condition 2: Are we extremely far from the last safe position? (Dist > 1000.0f)
    float DistanceToSafePos = FVector::Dist(CurrentLocation, this->_lastSafePosition);

    if (this->_secondsSinceLastFell < 2.0f || DistanceToSafePos > 1000.0f)
    {
        // Strategy: Try to find valid ground nearby, otherwise reset to spawn
        // UE_LOG(LogActor, Warning, TEXT("ADBDPlayer: Actor falling out of world: trying to find collision"));

        FVector NearbyGround;
        if (this->GetNearbyGroundLocation(NearbyGround) == true)
        {
            // Found ground, offset Z by 300.0f to ensure player doesn't clip
            TargetLocation = NearbyGround;
            TargetLocation.Z += 300.0f;
        }
        else
        {
            // No ground found, fallback to original spawn position
            TargetLocation = this->_spawnPosition;
        }
    }
    else
    {
        // Strategy: Return to the last known safe position
        // UE_LOG(LogActor, Warning, TEXT("ADBDPlayer: Actor falling out of world: Using lastSafePosition"));
        TargetLocation = this->_lastSafePosition;
    }

    // Reset the fall timer
    this->_secondsSinceLastFell = 0.0f;

    // Teleport the actor to the determined target location
    // VTable offset 0x450 corresponds to TeleportTo(DestLocation, DestRotation, bIsATest, bNoCheck)
    this->TeleportTo(TargetLocation, FRotator::ZeroRotator, false, false);
}

// starts at line 5231
bool ADBDPlayer::GetNearbyGroundLocation(FVector& GroundLocation)
{
    UWorld* World = this->GetWorld();
    if (!World)
    {
        return false;
    }

    // Initialize Trace Params
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = false;
    QueryParams.bFindInitialOverlaps = true;
    QueryParams.IgnoreMask = 0x100; // Specific Ignore Mask from ASM

    // Setup Response Container to Block All channels
    FCollisionResponseContainer ResponseParams;
    ResponseParams.SetAllChannels(ECR_Block);

    FHitResult OutHit;
    OutHit.Time = 1.0f;

    // Retry Loop: Attempt to find ground 10 times
    for (int32 i = 0; i < 10; i++)
    {
        // 1. Calculate Search Radius
        // Radius increases with each iteration: 100, 200, ..., 1000 units.
        float Radius = (float)(i + 1) * 100.0f;

        // 2. Generate Random 2D Offset
        // The ASM performs manual float randomization (Rand * 1/RAND_MAX * Radius).
        // It appears to add this positive offset to the actor's location.
        float RandomX = (FMath::Rand() / (float)RAND_MAX) * Radius;
        float RandomY = (FMath::Rand() / (float)RAND_MAX) * Radius;

        // 3. Determine Search Column X/Y
        // Start at the actor's current horizontal position
        FVector ActorLoc = this->GetRootComponent()->GetComponentLocation();
        float TargetX = ActorLoc.X + RandomX;
        float TargetY = ActorLoc.Y + RandomY;

        // 4. Define Vertical Trace Segment (Absolute Z)
        // The assembly hardcodes the Z values to -300.0f and 300.0f.
        // This implies the function assumes the valid playable floor is roughly at Z=0.
        // It traces UPWARDS from -300 to 300 to find the floor.
        FVector TraceStart(TargetX, TargetY, -300.0f);
        FVector TraceEnd(TargetX, TargetY, 300.0f);

        // 5. Perform the Trace
        // Uses Trace Channel 31 (0x1F), likely a specific GameTraceChannel for Ground/World.
        if (World->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, (ECollisionChannel)31, QueryParams, ResponseParams))
        {
            // Valid ground found
            GroundLocation = OutHit.Location;
            return true;
        }
    }

    return false;
}

// starts at line 5264
void ADBDPlayer::SaveLastValidLocation(float DeltaSeconds)
{
    // Accumulate the time since the last valid location was saved
    this->_secondsSinceLastSave += DeltaSeconds;

    // Accumulate the time since the player last fell, but stop incrementing once it exceeds the threshold (2.0f).
    // This acts as a stability timer.
    if (this->_secondsSinceLastFell <= 2.0f)
    {
        this->_secondsSinceLastFell += DeltaSeconds;
    }

    // Check if the save interval (5.0 seconds) has passed
    if (this->_secondsSinceLastSave > 5.0f)
    {
        // Ensure the character is not currently falling (must be grounded)
        if (this->CharacterMovement->IsFalling() == false)
        {
            // Ensure the character has been stable (not fallen) for more than 2.0 seconds
            // The disassembly compares 2.0f against _secondsSinceLastFell and jumps if 2.0f >= _secondsSinceLastFell
            if (this->_secondsSinceLastFell > 2.0f)
            {
                // Update the last safe position with the previous tracked position
                this->_lastSafePosition = this->_previousPosition;

                // Reset the save timer
                this->_secondsSinceLastSave = 0.0f;
            }
        }
    }
}

// starts at line 5285
void ADBDPlayer::Authority_UpdateBlinded(float deltaTime)
{
    // Ensure this function only executes on the server (network authority).
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ROLE_Authority)
    {
        // Only process if the player is currently experiencing blindness
        if (this->_blindnessRemainingTime > 0.0f)
        {
            // Decrement the remaining blindness time by deltaTime, clamping it to a minimum of 0.
            this->_blindnessRemainingTime = FMath::Max(this->_blindnessRemainingTime - deltaTime, 0.0f);

            // Call AddCharge with a value of 0.0f and a null instigator. 
            // The disassembly explicitly loads xorps xmm1, xmm1 (0.0) and xor r8d, r8d (nullptr).
            // This is likely triggering an update broadcast or evaluating limits on the ChargeableComponent.
            if (this->BlindingChargeableComponent != nullptr)
            {
                this->BlindingChargeableComponent->AddCharge(0.0f, nullptr);
            }

            // Determine if the player is still blinded after the time decrement
            bool isStillBlinded = (this->_blindnessRemainingTime > 0.0f);

            // Check if the _playerData object is valid using the standard GUObjectArray flags check
            if (IsValid(this->_playerData) == true)
            {
                // Synchronize the boolean state to the player data container
                this->_playerData->Blinded = isStillBlinded;
            }

            // If the blindness has fully worn off, reset the maximum blindness duration to 0
            if (isStillBlinded == false)
            {
                this->_maxBlindnessDuration = 0.0f;
            }
        }
    }
}

// starts at line 5309
void ADBDPlayer::Authority_Blinded(EBlindType blindType, float coolDown, AActor* effector)
{
    // Check if the executing player possesses network authority.
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ROLE_Authority)
    {
        // Set the active blindness duration to the provided cooldown value.
        this->_blindnessRemainingTime = coolDown;

        // Reset the immunity/recently blinded timer using the pre-configured cooldown duration.
        // In Unreal Engine C++, calling a method on a struct often compiles down to 
        // passing the struct's memory address as the first parameter (like FDBDTimer::Reset).
        this->_recentlyBlindedTimer.Reset(this->_recentlyBlindedCooldown);

        // Mark the blinding chargeable component as fully charged/completed.
        // It is standard practice to validate the pointer before invoking methods on it.
        if (this->BlindingChargeableComponent != nullptr)
        {
            this->BlindingChargeableComponent->SetComplete(effector);
        }
    }
}

// starts at line 5320
bool ADBDPlayer::Authority_AddBlindCharge(float charge, float blindnessDuration, AActor* effector)
{
    // Ensure the function only executes on the server (network authority).
    // The value 3 maps to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ROLE_Authority)
    {
        // Standard Unreal Engine pointer validation macro expansion 
        // (checking GUObjectArray and the 0x20000000 pending kill flag).
        if (IsValid(this->BlindingChargeableComponent) == true)
        {
            // Cache whether the player is already blinded before applying new charges
            bool isCurrentlyBlinded = (this->_blindnessRemainingTime > 0.0f);

            float actualCharge = MAX_flt; 
            
            // Limit check: 3.4e38 is the compiler representation of FLT_MAX (MAX_flt in UE).
            // If the incoming charge is not infinite, scale it by the player's susceptibility.
            if (charge < MAX_flt)
            {
                actualCharge = this->GetBlindnessSusceptability() * charge;
            }

            // Apply the calculated charge to the component
            this->BlindingChargeableComponent->AddCharge(actualCharge, effector);

            // Retrieve the Perk Manager via the interface offset to calculate duration modifiers
            UPerkManager* perkManager = this->GetPerkManager();

            // Fetch the modifier. The assembly explicitly passes 0x2D (45) for the modifier type
            // and 4 for the source filter.
            float modifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyBlindnessDuration, EGameplayModifierSource::VE_All);

            // Calculate the total duration by adding the modifier percentage (+1.0f base)
            float calculatedDuration = (modifier + 1.0f) * blindnessDuration;
            
            // Update the max blindness duration if the new duration is larger
            this->_maxBlindnessDuration = FMath::Max(calculatedDuration, this->_maxBlindnessDuration);

            // If the player is NOT currently blinded, check if the charge is complete
            if (isCurrentlyBlinded == false)
            {
                if (this->BlindingChargeableComponent->GetPercentComplete() >= 1.0f)
                {
                    // The disassembly invokes this via a VTable lookup at offset 0xFB0.
                    // The value 1 is passed as the EBlindType enum.
                    this->Authority_Blinded(EBlindType::VE_Flashlight, this->_maxBlindnessDuration, effector);                    
                    return true;
                }
            }
        }
    }

    // Return false if we lacked authority, the component was invalid, or the blind didn't complete
    return false;
}

// starts at line 5350
void ADBDPlayer::Authority_StartBlindCharge(AActor* effector, UFlashLightComponent* flashlight)
{
    // Check if the executing player possesses network authority.
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ROLE_Authority)
    {
        // Validate the effector actor to ensure it is not null and not pending destruction.
        // The underlying disassembly explicitly checks GUObjectArray flags and bAutoDestroyWhenFinished.
        if (IsValid(effector) == true)
        {
            // Validate the flashlight component pointer using the same engine safety checks.
            if (IsValid(flashlight) == true)
            {
                // Add or update the effector-flashlight pair in the tracking map.
                // The pseudo-code exposes the underlying TSet<TPair<K, V>> architecture 
                // that Unreal Engine uses to implement TMap. 
                // Assigning the value to offset +8 maps perfectly to setting the 'Value' of the TPair.
                this->_blindingEffectors.Add(effector, flashlight);
            }
        }
    }
}

// starts at line 5361
void ADBDPlayer::Authority_StopBlindCharge(AActor* effector)
{
    // Check if the executing player possesses network authority.
    // The value 3 corresponds to the ROLE_Authority enum value inside ENetRole.
    if (this->Role == ROLE_Authority)
    {
        // Validate the effector actor to ensure it is not null and not pending destruction.
        // The disassembly checks GUObjectArray flags (0x20000000) and the pending kill byte (0x140).
        if (IsValid(effector) == true)
        {
            // Remove the effector from the internal tracking map.
            // The pseudo-code shows TSet::Remove because Unreal Engine implements TMaps 
            // as TSets of TPairs under the hood. TMap::Remove(Key) compiles directly to this.
            this->_blindingEffectors.Remove(effector);
        }
    }
}

// starts at line 5372
float ADBDPlayer::GetPercentBlinded() const
{
    // Retrieve the percentage complete from the BlindingChargeableComponent.
    // This component tracks the build-up of the "blinded" status effect (e.g., from flashlights).
    return this->BlindingChargeableComponent->GetPercentComplete();
}

// starts at line 5377
bool ADBDPlayer::IsBlinded() const
{
    // Check if the remaining time for the blindness effect is positive.
    // Disassembly: Compares the float at offset 0x1168 with 0.0f.
    if (this->_blindnessRemainingTime > 0.0f)
    {
        return true;
    }

    return false;
}

// starts at line 5382
bool ADBDPlayer::WasRecentlyBlinded()
{
  return this->_blindnessRemainingTime > 0.0 || !this->_recentlyBlindedTimer->IsDone();
}

// starts at line 5387
float ADBDPlayer::GetBlindnessSusceptability() const
{
    // Retrieve the PerkManager via the interface
    // The disassembly calls the function at offset +0x10 of the interface vtable
    // offset 0x7A8 corresponds to the IPerkManagerOwnerInterface implementation
    UPerkManager* PerkMgr = this->GetPerkManager();

    // Check if the PerkManager is valid
    if (PerkMgr != nullptr)
    {
        // Validate the PerkManager object (GUObjectArray check in disassembly)
        if (PerkMgr->IsValidLowLevel() == true)
        {
            // Call GetPerkModifier on the PerkManager
            // Argument 1 (modifierType): 0x2E (46) -> This corresponds to a specific gameplay modifier enum.
            // Based on context/naming, let's assume it's 'BlindnessSusceptibility' or similar.
            // Argument 2 (filter/source): 4 -> This likely corresponds to 'All' or a specific source filter.
            // Pseudo code uses VE_EnablePhantomCollision (likely wrong enum mapping by decompiler) and VE_All.
            // Let's use the raw values or inferred names.
            
            // EGameplayModifierType::BlindnessSusceptibility = 46 (0x2E)
            // EGameplayModifierSource::All = 4
            
            float Modifier = PerkMgr->GetPerkModifier(EGameplayModifierType::VE_ModifyBlindnessSusceptability, EGameplayModifierSource::VE_All);

            // Return the modifier + base value of 1.0f
            return Modifier + 1.0f;
        }
    }

    // Default susceptibility is 1.0f (100%)
    return 1.0f;
}

// starts at line 5410
void ADBDPlayer::Broadcast_LockOnTarget(ADBDPlayer* target)
{
    // Verify that this specific player instance is controlled by the local machine.
    // This prevents simulated proxies (other players on your screen) from firing off 
    // their own lock-on logic on your client.
    if (this->IsLocallyControlled() == true)
    {
        // Update the local weak pointer to track the new target.
        // The pseudo-code explicitly shows FWeakObjectPtr::operator=, which means 
        // _lockOnTarget is a TWeakObjectPtr<ADBDPlayer> under the hood. 
        // Standard C++ assignment handles this abstraction cleanly.
        this->_lockOnTarget = target;

        // Trigger the local notification or Blueprint Implementable Event 
        // so the UI or local animations can react immediately.
        this->OnLockOnTarget(target);

        // Fire the Server RPC to notify the authoritative server of the target change.
        // The server will then validate the lock-on and potentially replicate it to others.
        this->Broadcast_LockOnTarget_Server(target); /* UNREAL AUTO GENERATED FUNCTION */
    }
}

// starts at line 5418
void ADBDPlayer::Broadcast_LockOnTarget_Server_Implementation(ADBDPlayer *target)
{
    this->Broadcast_LockOnTarget_Multicast(target);
}

// starts at line 5426
void ADBDPlayer::Broadcast_LockOnTarget_Multicast_Implementation(ADBDPlayer* target)
{
    // The logic checks if this character is NOT controlled by the local player.
    // If you are the local player, you already updated your target in the initial 
    // Broadcast_LockOnTarget call. This RPC ensures everyone else sees it too.
    if (this->IsLocallyControlled() == false)
    {
        // Update the weak pointer to the new target.
        // In Unreal C++, TWeakObjectPtr supports direct assignment.
        this->_lockOnTarget = target;

        // Trigger the visual/gameplay notification for other players' clients.
        this->OnLockOnTarget(target);
    }
}

// starts at line 5442
void ADBDPlayer::Multicast_LockOnTarget_Implementation(ADBDPlayer* target)
{
    // Assign the provided target to the weak object pointer member located at offset 0x0E48.
    // This utilizes the TWeakObjectPtr assignment operator.
    this->_lockOnTarget = target;

    // Execute the virtual function located at VTable offset 0x0F18.
    // This function handles the specific logic for the lock-on event.
    this->OnLockOnTarget(target);
}

// starts at line 5450
AActor* ADBDPlayer::GetLockOnTarget() const
{
    // Retrieve the raw pointer from the weak pointer member
    // _lockOnTarget is defined as TWeakObjectPtr<AActor> (or FWeakObjectPtr)
    // This safely resolves the pointer or returns nullptr if the object is stale/null
    return this->_lockOnTarget.Get();
}

// starts at line 5456
void ADBDPlayer::Broadcast_DamageTarget(ADBDPlayer* target, EAttackType attackType)
{
    // Ensure the damage initiation logic only runs if this player is locally controlled.
    // This prevents "ghost" inputs from other players' proxies on your client.
    if (this->IsLocallyControlled() == true)
    {
        // Execute the local damage reception logic immediately.
        // This is typically used for local VFX, SFX, or HUD updates so the 
        // player gets instant feedback without waiting for server round-trip.
        this->Receive_DamageTarget(target, attackType);

        // Dispatch a Server RPC to notify the authoritative server of the attack.
        // The server will then validate the hit, apply health changes, and replicate to others.
        this->Broadcast_DamageTarget_Server(target, attackType); /* UNREAL AUTO GENERATED FUNCTION */
    }
}

// starts at line 5464
void ADBDPlayer::Broadcast_DamageTarget_Server_Implementation(ADBDPlayer *target, EAttackType attackType)
{
    this->Broadcast_DamageTarget_Multicast(target, attackType);
}

// starts at line 5472
void ADBDPlayer::Broadcast_DamageTarget_Multicast_Implementation(ADBDPlayer* target, EAttackType attackType)
{
    // Check if this character instance is NOT controlled by the local player.
    // This allows remote clients (observing players) to see the damage effects 
    // that were initiated by the attacker and authorized by the server.
    if (this->IsLocallyControlled() == false)
    {
        // Execute the damage reception logic (VFX/SFX/Animations).
        // On non-controlling clients, this synchronizes the visual state of the hit.
        this->Receive_DamageTarget(target, attackType);
    }
}

// starts at line 5483
void ADBDPlayer::Receive_DamageTarget(ADBDPlayer* Target, EAttackType AttackType)
{
    // Update the weak object pointer to the damage target
    // Corresponds to the assignment at offset 0xE50
    this->_damageTarget = Target;

    // Retrieve the UCustomizedSkeletalMesh component using its static class
    // The disassembly calls GetPrivateStaticClass and GetComponentByClass
    UCustomizedSkeletalMesh* CustomizedSkeletalMesh = Cast<UCustomizedSkeletalMesh>(this->GetComponentByClass(UCustomizedSkeletalMesh::StaticClass()));

    // Check if the component was found and is valid (checks against global object array flags in disassembly)
    if (CustomizedSkeletalMesh != nullptr)
    {
        // Call the OnAttackEnd function on the component
        CustomizedSkeletalMesh->OnAttackEnd();
    }

    // Execute the OnAttackEvent delegate if it is bound
    // Passes the Target player and the AttackType enum
    this->OnAttackEvent.ExecuteIfBound(Target, AttackType);
}

// starts at line 5494
void ADBDPlayer::Multicast_DamageTarget_Implementation(ADBDPlayer* target, EAttackType attackType)
{
    // Delegate to the local handler (which might be BlueprintNativeEvent or internal logic)
    // Pseudocode: this->Receive_DamageTarget(target, attackType);
    this->Receive_DamageTarget(target, attackType);
}

// starts at line 5503
AActor* ADBDPlayer::GetDamageTarget() const
{
    // Retrieve the raw pointer from the weak pointer member variable
    // _damageTarget is likely defined as TWeakObjectPtr<AActor> or FWeakObjectPtr
    return this->_damageTarget.Get();
}

// starts at line 5509
void ADBDPlayer::Broadcast_AttackSubstate(EAttackSubstate attackSubstate)
{
    // Check if the player pawn is locally controlled before initiating the broadcast
    if (this->IsLocallyControlled() == true)
    {
        // Update the internal state to reflect the next attack substate
        this->_nextAttackSubstate = attackSubstate;

        // Call the server RPC to broadcast the attack substate change
        this->Broadcast_AttackSubstate_Server(attackSubstate);
    }
}

// starts at line 5517
void ADBDPlayer::Broadcast_AttackSubstate_Server_Implementation(EAttackSubstate attackSubstate)
{
    this->Broadcast_AttackSubstate_Multicast(attackSubstate);
}

// starts at line 5525
void ADBDPlayer::Broadcast_AttackSubstate_Multicast_Implementation(EAttackSubstate attackSubstate)
{
    if (this->IsLocallyControlled() == false)
    {
        this->_nextAttackSubstate = attackSubstate;
    }
}

// starts at line 5540
void ADBDPlayer::Multicast_AttackSubstate_Implementation(EAttackSubstate attackSubstate)
{
    // Update the local state variable with the server-replicated value.
    // Pseudocode: this->_nextAttackSubstate = attackSubstate;
    this->_nextAttackSubstate = attackSubstate;
}

// starts at line 5550
void ADBDPlayer::Broadcast_PlayMontage(FAnimationMontageDescriptor animMontageID, float playRate)
{
    // Check if the current player pawn is locally controlled
    if (this->IsLocallyControlled() == true)
    {
        // Play the montage locally. 
        // The mangled signature indicates a third boolean parameter, which is passed as false.
        this->PlayMontage(animMontageID, playRate, false);

        // Broadcast the montage data to the server via the VTable RPC function
        this->Broadcast_PlayMontage_Server(animMontageID, playRate);
    }
}

// starts at line 5558
void ADBDPlayer::Broadcast_PlayMontage_Server_Implementation(FAnimationMontageDescriptor animMontageID, float playRate)
{
    // Call the multicast RPC to replicate the montage playback to all other clients.
    // The VTable at offset 0x1068 resolves to Broadcast_PlayMontage_Multicast.
    this->Broadcast_PlayMontage_Multicast(animMontageID, playRate);
}

// starts at line 5566
void ADBDPlayer::Broadcast_PlayMontage_Multicast_Implementation(FAnimationMontageDescriptor animMontageID, float playRate)
{
    // The multicast implementation executes on all clients.
    // We only call PlayMontage if this instance is NOT locally controlled,
    // because the local controller already played the montage before calling the RPC.
    if (this->IsLocallyControlled() == false)
    {
        // Play the montage on simulated proxies.
        // The third parameter (bForceNoIntermediate) is passed as false (0).
        this->PlayMontage(animMontageID, playRate, false);
    }
}

// starts at line 5573
bool ADBDPlayer::Broadcast_PlayMontage_Multicast_Validate(FAnimationMontageDescriptor animMontageID, float playRate)
{
    // In Unreal Engine 4, _Validate functions return a boolean to check for cheat attempts.
    // This implementation simply permits the execution on all clients.
    return true;
}

// starts at line 5581
void ADBDPlayer::Multicast_PlayMontage_Implementation(FAnimationMontageDescriptor animMontageID, float playRate)
{
    // The disassembly shows the creation of local copies of the FAnimationMontageDescriptor 
    // struct (copy constructors) and subsequent destruction (FMemory::Free) at the end. 
    // This is characteristic of passing a complex struct by value in C++.
    
    // Calls the internal PlayMontage function.
    // The third argument is a boolean flag, set to false (0) in the assembly (r9d register).
    this->PlayMontage(animMontageID, playRate, false);
}

// starts at line 5591
void ADBDPlayer::Multicast_AttackBegin_Implementation(EAttackType attackType)
{
    // Retrieve the UCustomizedSkeletalMesh component.
    // This custom component likely handles cosmetic customization and reaction logic.
    // Disassembly: call ?GetPrivateStaticClass@UCustomizedSkeletalMesh...
    // Disassembly: call ?GetComponentByClass@AActor...
    UCustomizedSkeletalMesh* CustomizedMesh = this->GetComponentByClass<UCustomizedSkeletalMesh>();

    // Ensure the component exists and is valid.
    if (CustomizedMesh != nullptr)
    {
        // Validate object internal flags (IsPendingKill check).
        // Disassembly: Checks ClassTreeIndex (Cast verification) and InternalIndex/GUObjectArray flags.
        if (!CustomizedMesh->IsPendingKill())
        {
            // Delegate the event to the component.
            // Note: The 'attackType' parameter seems unused in this specific function call chain, 
            // or OnAttackBegin might use it internally if the register passing matches (which isn't visible here).
            // Disassembly: jmp ?OnAttackBegin@UCustomizedSkeletalMesh@@QEAAXXZ
            CustomizedMesh->OnAttackBegin();
        }
    }
}

// starts at line 5604
FVector ADBDPlayer::GetManualMoveInput() const
{
    // Initialize the result vector to zero
    FVector result = FVector::ZeroVector;

    // Access the shared pointer's underlying object for movement input events
    // MovementInputEvent is at offset 0x848 based on the structure _S
    PollableEvent<FVector>* movementInputObject = this->MovementInputEvent.Get();

    if (movementInputObject != nullptr)
    {
        // The disassembly reveals a loop that iterates through an internal TArray of events
        // and accumulates the FVector values. The IDA pseudo-code shows an unrolled 
        // optimization for groups of 4, but the functional logic is a summation.
        
        int32 eventCount = movementInputObject->_events.Num();

        if (eventCount > 0)
        {
            // Iterate through all recorded movement input vectors in the pollable event
            for (int32 i = 0; i < eventCount; i = i + 1)
            {
                FVector currentInput = movementInputObject->_events[i];
                result += currentInput;
            }
        }
    }

    return result;
}

// starts at line 5622
bool ADBDPlayer::Local_IsLoadoutItemLoaded() const
{
    // 1. Check Local Control
    // Disassembly: call qword ptr [rax+630h] (Virtual IsLocallyControlled)
    if (this->IsLocallyControlled())
    {
        // 2. Editor World Bypass
        // Disassembly: call ?IsAnyEditorWorld@ADBDPlayer@@IEBA_NXZ
        if (this->IsAnyEditorWorld())
        {
            // Log verbose message if enabled
            if (GameFlow->Verbosity >= ELogVerbosity::Log) // 5
            {
                UE_LOG(GameFlow, Log, TEXT("Loadout Item Is Loaded because we are in editor (we do not wait for real)."));
            }
            return true;
        }

        // 3. Retrieve Persistent Data
        // Disassembly: call ?Local_GetPlayerPersistentData@ADBDPlayer@@AEBAPEBUFPlayerPersistentData@@XZ
        const FPlayerPersistentData* PersistentData = this->Local_GetPlayerPersistentData();

        if (PersistentData != nullptr)
        {
            // Retrieve the Loadout Slot value (likely the Item ID name)
            // Disassembly: mov rax, [rax+88h]
            // Note: Offsets suggest this is a member of FPlayerPersistentData, likely 'StartingItemSlot' or similar.
            FName LoadoutItemID = PersistentData->CurrentLoadout.Item;

            // 4. Check for Empty/Locked Slots
            if (LoadoutItemID == LoadoutSlotDefaultValue::LOCKED ||  // "_LOCKED_"
                LoadoutItemID == LoadoutSlotDefaultValue::EMPTY ||  // "_EMPTY_"
                LoadoutItemID == NAME_None)
            {
                // If there is no item to load, we are "loaded".
                if (GameFlow->Verbosity >= ELogVerbosity::Log)
                {
                    UE_LOG(GameFlow, Log, TEXT("Loadout Item Is Loaded because there is no loadout item."));
                }
                return true;
            }

            // 5. Check Inventory Existence
            // Disassembly: cmp [rbx+0C60h], ecx (Check Inventory.Num() > 0)
            // Disassembly: cmp [rax], rcx (Check Inventory[0] != nullptr)
            if (this->Inventory.Num() > 0 && this->Inventory[0] != nullptr)
            {
                // The item exists in the inventory, so it is loaded.
                if (GameFlow->Verbosity >= ELogVerbosity::Log)
                {
                    UE_LOG(GameFlow, Log, TEXT("Loadout Item Is Loaded because the item has been created."));
                }
                return true;
            }
        }
        else
        {
            // Persistent Data was null
            if (GameFlow->Verbosity >= ELogVerbosity::Log)
            {
                UE_LOG(GameFlow, Log, TEXT("Loadout Item has not yet been loaded because the persistent data is null."));
            }
        }
    }

    // Default failure case (Not locally controlled, or item expected but not yet spawned)
    return false;
}

// starts at line 5657
bool ADBDPlayer::IsBeingInterrupted() const
{
    // Return the value of the boolean flag located at offset 0xF40
    // Disassembly: movzx eax, byte ptr [rcx+0F40h]
    return this->_isBeingInterrupted;
}

// starts at line 5662
void ADBDPlayer::SetIsBeingInterrupted(bool value)
{
    // Check if the interruption state is changing (Offset 0xF40)
    if (this->_isBeingInterrupted != value)
    {
        // Update the state
        this->_isBeingInterrupted = value;

        // If the player is now being interrupted (value is true), execute the start logic.
        // Note: There is no corresponding 'OnInterruptedEnd' called here in the assembly for the false case.
        if (value != false)
        {
            this->OnInterruptedStart();
        }
    }
}

// starts at line 5674
FString ADBDPlayer::GetNameDebugString() const
{
    // 1. Get the Actor's unique system name (e.g., "BP_CamperFemale01_C_5")
    // The assembly calls FName::ToString on the NamePrivate member (Offset 0x18).
    FString Result = this->GetName();

    // 2. Retrieve the PlayerState (Offset 0x3A0)
    // The assembly checks if the pointer is null.
    APlayerState* State = this->GetPlayerState();

    // 3. Validate PlayerState
    // The assembly performs a full validity check (GUObjectArray lookup + PendingKill flag check).
    // In UE4 C++, IsValid() handles this safely.
    if (IsValid(State))
    {
        // 4. Append Separator " - "
        // The assembly manually writes the characters for space, dash, space (0x0020, 0x002D, 0x0020).
        Result += TEXT(" - ");

        // 5. Append the Player Name (e.g., "DbD_Fan_123")
        // Accesses the PlayerName string from the PlayerState (Offset 0x388).
        Result += State->GetPlayerName();
    }

    return Result;
}

// starts at line 5685
FString ADBDPlayer::GetMachineTypeString() const
{
    // 1. Determine the Network Role
    // Checks the Role property at offset 0x110.
    // Value 3 corresponds to ROLE_Authority.
    FString Result = (this->Role == ROLE_Authority) ? TEXT("SERVER") : TEXT("CLIENT");

    // 2. Append a space separator
    // The assembly code explicitly writes 0x0020 (L' ') into the string buffer.
    Result += TEXT(" ");

    // 3. Determine Control Status
    // Calls virtual function IsLocallyControlled() (VTable offset 0x630).
    // If true, appends "MASTER", otherwise "SLAVE".
    if (this->IsLocallyControlled())
    {
        Result += TEXT("MASTER");
    }
    else
    {
        Result += TEXT("SLAVE");
    }

    return Result;
}

// starts at line 5708
void ADBDPlayer::DBDAddControllerPitchInput(float Val, bool constant)
{
    // If the input value is zero, do nothing.
    if (Val == 0.0f)
    {
        return;
    }

    // Ensure the Controller exists.
    if (this->Controller == nullptr)
    {
        return;
    }

    // Check a specific bit-flag on the Controller at offset 0x3BC (Bit 1 / Value 2).
    // The pseudo code represents this as: (*((_DWORD *)v5 + 0xEF) & 2) != 0.
    // In the context of UE4 Controllers, this typically checks a flag like 'bIsPlayerController' or similar 
    // to ensure the controller is capable of handling pitch input before making virtual calls.
    if (this->Controller->bIsPlayerController == false)
    {
        return;
    }

    // Check if the controller is local.
    // The disassembly calls the virtual function at offset 0x628 (IsLocalController).
    if (this->Controller->IsLocalController())
    {
        // Cast the generic Controller to the game-specific ADBDPlayerController.
        // The disassembly calls a 'SafeCast' template function. 
        // Note: The code assumes the cast succeeds (no null check after cast) likely because IsLocalController returned true.
        ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(this->Controller);

        // Forward the input to the custom controller method.
        if (DBDController)
        {
            DBDController->DBDAddPitchInput(Val, constant);
        }
    }
}

// starts at line 5717
void ADBDPlayer::DBDAddControllerYawInput(float Val, bool constant)
{
    // If the input value is zero, do nothing to save processing.
    if (Val == 0.0f)
    {
        return;
    }

    // Ensure the Controller exists before accessing it.
    if (this->Controller == nullptr)
    {
        return;
    }

    // Check the specific bit-flag on the Controller at offset 0x3BC (Bit 1 / Value 2).
    // This typically ensures the controller is in a valid state (e.g., IsPlayerController check) before casting.
    if (this->Controller->bIsPlayerController == false)
    {
        return;
    }

    // Check if the controller is local.
    // The disassembly calls the virtual function at offset 0x628 (IsLocalController).
    if (this->Controller->IsLocalController())
    {
        // Cast the generic Controller to the game-specific ADBDPlayerController.
        ADBDPlayerController* DBDController = Cast<ADBDPlayerController>(this->Controller);

        // Forward the input to the custom controller method for Yaw.
        if (DBDController)
        {
            DBDController->DBDAddYawInput(Val, constant);
        }
    }
}

// starts at line 5726
void ADBDPlayer::Server_SetMaxWalkSpeed_Implementation(float maxWalkSpeed)
{
    this->Multicast_SetMaxWalkSpeed(maxWalkSpeed);
}

// starts at line 5736
void ADBDPlayer::Multicast_SetMaxWalkSpeed_Implementation(float maxWalkSpeed)
{
    // Ensure the CharacterMovement component is valid before accessing its members.
    // While the disassembly might assume validity, adding this check ensures functional stability.
    if (this->CharacterMovement != nullptr)
    {
        // Set the new maximum walking speed.
        this->CharacterMovement->MaxWalkSpeed = maxWalkSpeed;
    }
}

// starts at line 5746
void ADBDPlayer::StartStateMachine(bool isDriver)
{
    // Set the "Driving" state of the state machine.
    // This likely controls whether the SM drives animation or root motion.
    this->_SM->SetDrivingStateMachine(isDriver);

    // Begin the state machine execution.
    this->_SM->Start();

    // Establish a Tick Dependency.
    // We want the CharacterMovementComponent to update *after* the State Machine has ticked.
    // This ensures that any movement parameters modified by the State Machine (e.g., max speed, modes)
    // are taken into account during the physics simulation step of the same frame.
    if (this->CharacterMovement != nullptr)
    {
        this->CharacterMovement->PrimaryComponentTick.AddPrerequisite(this->_SM, this->_SM->PrimaryComponentTick);
    }
    
    // Mark the state machine as launched (Offset 0xAC8).
    this->_smLaunched = true;
}

// starts at line 5759
void ADBDPlayer::Server_OnInsaneSkillCheck_Implementation(ADBDPlayer *interruptee)
{
    this->Multicast_OnInsaneSkillCheck(interruptee);
}

// starts at line 5769
void ADBDPlayer::Multicast_OnInsaneSkillCheck_Implementation()
{
    // Call the internal handler for the insane skill check.
    // This function is likely a BlueprintImplementableEvent or a local helper not fully defined in the structure.
    this->OnInsaneSkillCheck();
}
